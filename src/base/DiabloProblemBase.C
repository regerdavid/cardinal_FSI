/********************************************************************/
/*                  SOFTWARE COPYRIGHT NOTIFICATION                 */
/*                             Cardinal                             */
/*                                                                  */
/*                  (c) 2021 UChicago Argonne, LLC                  */
/*                        ALL RIGHTS RESERVED                       */
/*                                                                  */
/*                 Prepared by UChicago Argonne, LLC                */
/*               Under Contract No. DE-AC02-06CH11357               */
/*                With the U. S. Department of Energy               */
/*                                                                  */
/*             Prepared by Battelle Energy Alliance, LLC            */
/*               Under Contract No. DE-AC07-05ID14517               */
/*                With the U. S. Department of Energy               */
/*                                                                  */
/*                 See LICENSE for full restrictions                */
/********************************************************************/

#ifdef ENABLE_DIABLO_COUPLING

#include "DiabloProblemBase.h"
#include "Moose.h"
#include "AuxiliarySystem.h"
#include "TimeStepper.h"
#include "TimedPrint.h"
#include "MooseUtils.h"
#include "CardinalUtils.h"
#include "VariadicTable.h"
#include "UserErrorChecking.h"

InputParameters
DiabloProblemBase::validParams()
{
  InputParameters params = CardinalProblem::validParams();
  params.addRequiredParam<std::string>(
      "casename",
      "Case name for the Diablo input files; "
      "this is <case> in <case>.assembly.");
  params.addRequiredParam<bool>(
      "axeflag",
      "Whether or not to run axe");
  params.addParam<int>("neumann_set",
      "Neumann Set through which Diablo will be coupled to MOOSE");
    params.addParam<int>("num_steps",
      "Number of timesteps to be performed");
  return params;
}

DiabloProblemBase::DiabloProblemBase(const InputParameters & params)
  : CardinalProblem(params),
   _casename(getParam<std::string>("casename")),
   _axeflag(getParam<bool>("axeflag")),
   _neumann_set(getParam<int>("neumann_set")),
   _num_steps(getParam<int>("num_steps"))
{
  _diablo_mesh = dynamic_cast<DiabloMesh *>(&mesh());
}

DiabloProblemBase::~DiabloProblemBase()
{
}

void
DiabloProblemBase::initialSetup()
{
  CardinalProblem::initialSetup();
}

void
DiabloProblemBase::externalSolve()
{

  //I think _time, _dt, _t_step should be accessible from in here and contain the current target time, current dt, and current step number
  //DR 7/5, these will presumable need to get declared outside of the external solve function so they dont get reset all the time.
  int ierr;
  int DiabloStepStart=1; //I'm assuming this always starts at 1, not sure if that is actually the case.
  int kk =_app.getExecutioner()->fixedPointSolve().numFixedPointIts()-1; //Subtracting one between Diablo treats the number as 0-index, while it is 1-index in MOOSE.
  //I have no idea what these do
  char mm_plotfile[ ] = "dblmmplot";
  char sm_plotfile[ ] = "dblsmplot";
  int mm_active = 0;
  int sm_active = 0;

  //DiabloStep = DiabloStep+1 ;
  ierr = Diablo_InitTimeStep( _t_step, DiabloStepStart, _time, _time-_dt, mm_plotfile, mm_active) ;
  ierr += Diablo_Solve( kk+1, _t_step, DiabloStepStart, 0,mm_active, sm_plotfile, sm_active);
  ierr = Diablo_FinalizeTimeStep( _t_step, _time, 0, _dt, mm_active);

}

void
DiabloProblemBase::addExternalVariables()
{
    InputParameters var_params = _factory.getValidParams("MooseVariable");
    var_params.set<MooseEnum>("family") = "LAGRANGE";
    var_params.set<MooseEnum>("order") = "FIRST";

  addAuxVariable("MooseVariable", "disp_x", var_params);
  _external_vars.push_back(_aux->getFieldVariable<Real>(0, "disp_x").number());
  addAuxVariable("MooseVariable", "disp_y", var_params);
  _external_vars.push_back(_aux->getFieldVariable<Real>(0, "disp_y").number());
  addAuxVariable("MooseVariable", "disp_z", var_params);
  _external_vars.push_back(_aux->getFieldVariable<Real>(0, "disp_z").number());
}

void
DiabloProblemBase::extractOutputs()
{
    int ierr;
    int ivar = 2;  //Currently hardcoded for displacement
    _n_vertices_per_elem = 4;
    //int node_index[4] ={0,1,3,2};
    int node_index[4] ={0,1,2,3};
    int components_per_point = 3;

    int _numNodeIds = _diablo_mesh->numNodeIds();
    int _numElems = _diablo_mesh->numElems();
    
    double *diabloVals;
    diabloVals = new double[components_per_point*_numNodeIds];

//    field::DiabloFieldEnum field_enum;
//    field_enum = field::disp_x;

    ierr = Diablo_Transfer_Out(diabloVals,&_neumann_set,&ivar);

    //just for debugging, remember to eventaully remove this for loop
    for (int count =0; count<3*_numNodeIds;count+=3)
    {
      //std::cout<<"Displacement: " << diabloVals[count] << "," << diabloVals[count+1] << "," << diabloVals[count+2]<<std::endl;
    }

    auto & solution = _aux->solution();
    auto sys_number = _aux->number();
    auto pid = _communicator.rank();
    //write the displacement into the MOOSE disp_x, disp_y, and disp_z
    for (int e = 0; e < _numElems; e++)
    {
      for (int build = 0; build < _diablo_mesh->nMoosePerDiablo(); ++build)
      {
        auto elem_ptr = _diablo_mesh->queryElemPtr(e * _diablo_mesh->nMoosePerDiablo() + build);
        // Only work on elements we can find on our local chunk of a
        // distributed mesh
        if (!elem_ptr)
        {
          libmesh_assert(!_nek_mesh->getMesh().is_serial());
          continue;
        }
        for (int n = 0; n < _n_vertices_per_elem; n++)
        {
          auto node_ptr = elem_ptr->node_ptr(n);
          //int node_start = (e * components_per_point*_n_vertices_per_elem/2) + node_index[n]*components_per_point; //DOUBLE CHECK THIS AT SOME POINT
          int node_start = (e*4*components_per_point)+node_index[n]*components_per_point;
          //Should probably make the next 6 lines not so stupidly hard-coded...but for now it works
          auto dof_idx = node_ptr->dof_number(sys_number,_aux->getFieldVariable<Real>(0, "disp_x").number(), 0);
          solution.set(dof_idx,diabloVals[node_start]);
          dof_idx = node_ptr->dof_number(sys_number,_aux->getFieldVariable<Real>(0, "disp_y").number(), 0);
          solution.set(dof_idx,diabloVals[node_start+1]);
          dof_idx = node_ptr->dof_number(sys_number,_aux->getFieldVariable<Real>(0, "disp_z").number(), 0);
          solution.set(dof_idx,diabloVals[node_start+2]);
        }
      }
    }
  freePointer(diabloVals);
}

void
DiabloProblemBase::fillAuxVariable(const unsigned int var_number, const double * value)
{
}

bool
DiabloProblemBase::isDataTransferHappening(ExternalProblem::Direction direction)
{
  if (_axeflag)
    return false;

  switch (direction)
  {
    case ExternalProblem::Direction::TO_EXTERNAL_APP:
      return synchronizeIn();
    case ExternalProblem::Direction::FROM_EXTERNAL_APP:
      return synchronizeOut();
    default:
      mooseError("Unhandled DirectionEnum in DiabloProblemBase!");
  }
}
void
DiabloProblemBase::syncSolutions(ExternalProblem::Direction direction)
{
    if (!isDataTransferHappening(direction))
    return;

  switch (direction)
  {
    case ExternalProblem::Direction::TO_EXTERNAL_APP:
    {

      break;

      return;
    }
    case ExternalProblem::Direction::FROM_EXTERNAL_APP:
    {
      // extract the NekRS solution onto the mesh mirror, if specified
      extractOutputs();

      if (_t_step == _num_steps+1) //I think this needs to be somewhere outside of external solve. Otherwise itll segfault on the last timestep when trying to copy the data from diablo onto the mesh mirror. THIS NEEDS TO BE _num_steps from the executioner, but I admittedly don't know how to get that yet
      {
        int ierr;
        ierr = Diablo_Finalize( _t_step );
      }
      break;
    }
    default:
      mooseError("Unhandled Transfer::DIRECTION enum!");
  }
}
bool
DiabloProblemBase::synchronizeIn()
{
  bool synchronize = true;
  //TODO:Add the logic in here that exists in NekRSProblemBase
  return synchronize;
}
bool
DiabloProblemBase::synchronizeOut()
{
  bool synchronize = true;
  //TODO:Add the logic in here that exists in NekRSProblemBase
  return synchronize;
}
#endif

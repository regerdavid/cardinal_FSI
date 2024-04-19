#ifdef ENABLE_DIABLO_COUPLING

#include "DiabloProblem.h"
#include "Moose.h"
#include "AuxiliarySystem.h"
#include "TimeStepper.h"
#include "TimedPrint.h"
#include "MooseUtils.h"
#include "CardinalUtils.h"
#include "VariadicTable.h"
#include "UserErrorChecking.h"

registerMooseObject("CardinalApp", DiabloProblem);

bool DiabloProblem::_first = true;

InputParameters
DiabloProblem::validParams()
{
  InputParameters params = DiabloProblemBase::validParams();
  return params;
}

DiabloProblem::DiabloProblem(const InputParameters & params)
  : DiabloProblemBase(params),
  _serialized_solution(NumericVector<Number>::build(_communicator).release())
{
    
}

void 
DiabloProblem::addExternalVariables()
{
  DiabloProblemBase::addExternalVariables();
  InputParameters var_params = _factory.getValidParams("MooseVariable");
  var_params.set<MooseEnum>("family") = "LAGRANGE";
  var_params.set<MooseEnum>("order") = "FIRST";

  checkDuplicateVariableName("traction");
  addAuxVariable("MooseVariable","s11",var_params);
  addAuxVariable("MooseVariable","s22",var_params);
  addAuxVariable("MooseVariable","s33",var_params);
  addAuxVariable("MooseVariable","s12",var_params);
  addAuxVariable("MooseVariable","s13",var_params);
  addAuxVariable("MooseVariable","s23",var_params);
  _s11_var = _aux->getFieldVariable<Real>(0, "s11").number();
  _s22_var = _aux->getFieldVariable<Real>(0, "s22").number();
  _s33_var = _aux->getFieldVariable<Real>(0, "s33").number();
  _s12_var = _aux->getFieldVariable<Real>(0, "s12").number();
  _s13_var = _aux->getFieldVariable<Real>(0, "s13").number();
  _s23_var = _aux->getFieldVariable<Real>(0, "s23").number();

}

void 
DiabloProblem::sendTractionsToDiablo()
{
    int ierr;
    int ivar = 2;  //Currently hardcoded for tractions
    int _numElems;
    _n_vertices_per_elem = 4;
    //int node_index[4] ={0,1,3,2};
    int node_index[4] ={0,1,2,3};
    int components_per_point = 6;
    //Can probably make some functions in DiabloMesh.h and call them to get stuff like neumann set, etc (_diablo_mesh->getNeumannSet or something)
    ierr = Diablo_Get_Num_NodeIds(&_numNodeIds,&_neumann_set,&ivar);

    //_numElems = (_numNodeIds-2)/2;
    _numElems = _numNodeIds/4;
    double *diabloVals;
    diabloVals = new double[components_per_point*_numNodeIds];
    
    auto & solution = _aux->solution();
    auto sys_number = _aux->number();
    auto pid = _communicator.rank();
    //Populate the diabloVals array with the stuff from the mesh mirror
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
          auto dof_idx = node_ptr->dof_number(sys_number, _s11_var, 0);
          diabloVals[node_start] = (*_serialized_solution)(dof_idx);
          dof_idx = node_ptr->dof_number(sys_number, _s22_var, 0);
          diabloVals[node_start+1] = (*_serialized_solution)(dof_idx);
          dof_idx = node_ptr->dof_number(sys_number, _s33_var, 0);
          diabloVals[node_start+2] = (*_serialized_solution)(dof_idx);
          dof_idx = node_ptr->dof_number(sys_number, _s12_var, 0);
          diabloVals[node_start+3] = (*_serialized_solution)(dof_idx);
          dof_idx = node_ptr->dof_number(sys_number, _s13_var, 0);
          diabloVals[node_start+4] = (*_serialized_solution)(dof_idx);
          dof_idx = node_ptr->dof_number(sys_number, _s23_var, 0);
          diabloVals[node_start+5] = (*_serialized_solution)(dof_idx);
          /*
          for (int i = 0; i<6; i++)
          {
            if (i==1)
              diabloVals[node_start+i] = -10.0;
            else
            {}
              diabloVals[node_start+i] = 0.0;
          }*/

          //std::cout<<"TEST: " << _traction_var(ndof_idx) << std::endl;
        }
      }
    }

    //just for debugging, remember to eventaully remove this for loop
    for (int count =0; count<6*_numNodeIds;count+=6)
    {
    //  std::cout<<"6 traction components are " << diabloVals[count] << "," << diabloVals[count+1] << "," << diabloVals[count+2]<<"," << diabloVals[count+3]<<"," << diabloVals[count+4] << "," << diabloVals[count+5]<<std::endl;
    }

    ierr = Diablo_Transfer_In(diabloVals,&_neumann_set,&ivar);

}

void
DiabloProblem::syncSolutions(ExternalProblem::Direction direction)
{
    auto & solution = _aux->solution();

    if (!isDataTransferHappening(direction))
    return;

  switch (direction)
  {
    case ExternalProblem::Direction::TO_EXTERNAL_APP:
    {

      if (_first)
      {
        _serialized_solution->init(_aux->sys().n_dofs(), false, SERIAL);
        _first = false;
      }

      solution.localize(*_serialized_solution);
      sendTractionsToDiablo();
      break;

    //  return;
    }
    case ExternalProblem::Direction::FROM_EXTERNAL_APP:
    {
      // extract the NekRS solution onto the mesh mirror, if specified
      extractOutputs();

      if (_t_step == _num_steps+1) //This needs to be at _num_steps, but it segfaults for some reason. I think this needs to be called as the very last thing.
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
#endif
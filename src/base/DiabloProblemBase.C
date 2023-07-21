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
  return params;
}

DiabloProblemBase::DiabloProblemBase(const InputParameters & params)
  : CardinalProblem(params),
   _casename(getParam<std::string>("casename")),
   _axeflag(getParam<bool>("axeflag"))
{

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
  int kk =0; //this should be the current fixed point iteration once we get to that point.
  //I have no idea what these do
  char mm_plotfile[ ] = "dblmmplot";
  char sm_plotfile[ ] = "dblsmplot";
  int mm_active = 0;
  int sm_active = 0;

  //DiabloStep = DiabloStep+1 ;
  ierr = Diablo_InitTimeStep( _t_step, DiabloStepStart, _time, _time-_dt, mm_plotfile, mm_active) ;
  ierr += Diablo_Solve( kk+1, _t_step, DiabloStepStart, 0,mm_active, sm_plotfile, sm_active);
  ierr = Diablo_FinalizeTimeStep( _t_step, _time, 0, _dt, mm_active);


  if (_t_step == 2) //THIS NEEDS TO BE _num_steps from the executioner, but I admittedly don't know how to get that yet
  {
    ierr = Diablo_Finalize( _t_step );
  }
}

void
DiabloProblemBase::syncSolutions(ExternalProblem::Direction direction)
{

}
#endif

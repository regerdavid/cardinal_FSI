#ifdef ENABLE_DIABLO_COUPLING

#include "DiabloStandaloneProblem.h"
#include "UserErrorChecking.h"

registerMooseObject("CardinalApp", DiabloStandaloneProblem);

InputParameters
DiabloStandaloneProblem::validParams()
{
  InputParameters params = DiabloProblemBase::validParams();
  return params;
}

DiabloStandaloneProblem::DiabloStandaloneProblem(const InputParameters & params)
  : DiabloProblemBase(params)
{
    
}
#endif
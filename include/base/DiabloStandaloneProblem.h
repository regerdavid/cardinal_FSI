
#pragma once

#include "DiabloProblemBase.h"

class DiabloStandaloneProblem : public DiabloProblemBase
{
public:
  DiabloStandaloneProblem(const InputParameters & params);

  static InputParameters validParams();
};

#pragma once

#include "DiabloProblemBase.h"

class DiabloProblem : public DiabloProblemBase
{
public:
  DiabloProblem(const InputParameters & params);

  static InputParameters validParams();

  virtual void syncSolutions(ExternalProblem::Direction direction) override;

  virtual void addExternalVariables() override;

  void sendTractionsToDiablo();

  unsigned int _s11_var;
  unsigned int _s22_var;
  unsigned int _s33_var;
  unsigned int _s12_var;
  unsigned int _s13_var;
  unsigned int _s23_var;

  std::unique_ptr<NumericVector<Number>> _serialized_solution;

  /// flag to indicate whether this is the first pass to serialize the solution
  static bool _first;
};
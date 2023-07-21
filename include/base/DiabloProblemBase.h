
#pragma once
extern "C"
{
#include "diablo_sub_cardinal.h"
}
#include "CardinalProblem.h"
#include "Transient.h"

#include <memory>

class DiabloProblemBase : public CardinalProblem
{
public:
  DiabloProblemBase(const InputParameters & params);

  static InputParameters validParams();

  ~DiabloProblemBase();

    virtual void initialSetup() override;

    virtual void externalSolve() override;

    virtual void syncSolutions(ExternalProblem::Direction direction) override;

    virtual bool converged() override { return true; }

    /// Underlying executioner
    Transient * _transient_executioner = nullptr;

    /// Diablo casename
    const std::string & _casename;

    const bool & _axeflag;


};

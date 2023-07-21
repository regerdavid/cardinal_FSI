#pragma once

#include "MooseObjectAction.h"
extern "C"
{
#include "diablo_sub_cardinal.h"
}

/**
 * Initialize Nek application by calling nekrs::setup. This needs to be
 * in an action so that it occurs before the [Mesh] is constructed (which
 * is normally the first MOOSE object constructed) because we build the
 * [Mesh] relying on internal stuff in NekRS, which needs to be initialized
 * at that point.
 */
class DiabloInitAction : public MooseObjectAction
{
public:
  static InputParameters validParams();

  DiabloInitAction(const InputParameters & parameters);

  virtual void act() override;
protected:

    const bool & _axeflag;
    
    const std::string & _casename;
};

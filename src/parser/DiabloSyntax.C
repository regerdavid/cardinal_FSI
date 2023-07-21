#ifdef ENABLE_DIABLO_COUPLING

#include "DiabloSyntax.h"
#include "ActionFactory.h"
#include "Syntax.h"

namespace Diablo
{

void
associateSyntax(Syntax & syntax, ActionFactory & /*action_factory*/)
{
  registerMooseObjectTask("diablo_init", Problem, true);
  registerSyntax("DiabloInitAction", "Problem");
  addTaskDependency("meta_action", "diablo_init");
}

} // namespace Diablo
#endif
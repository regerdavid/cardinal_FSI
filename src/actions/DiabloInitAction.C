#ifdef ENABLE_DIABLO_COUPLING
#include "DiabloInitAction.h"

#include "MooseApp.h"
#include "CommandLine.h"
#include <chrono>

registerMooseAction("CardinalApp", DiabloInitAction, "diablo_init");

//int DiabloInitAction::_n_cases = 0;

InputParameters
DiabloInitAction::validParams()
{
  InputParameters params = MooseObjectAction::validParams();
  params.addParam<std::string>("type", "Problem type");
  params.addParam<std::string>(
      "casename",
      "Case name for the NekRS input files; "
      "this is <case> in <case>.par, <case>.udf, <case>.oudf, and <case>.re2.");
  params.addParam<bool>(
      "axeflag",false,
      "Whether or not to run axe");
      
  return params;
}

DiabloInitAction::DiabloInitAction(const InputParameters & parameters)
  : MooseObjectAction(parameters),
  _axeflag(getParam<bool>("axeflag")),
  _casename(getParam<std::string>("casename"))
{
}

void
DiabloInitAction::act()
{
  bool is_diablo_problem = _type == "DiabloProblem" || _type == "DiabloStandaloneProblem";
  if (!is_diablo_problem)
    return;

  std::string setup_file = getParam<std::string>("casename");

  int FIVSIM_COMM_WORLD;
  FIVSIM_COMM_WORLD = int(MPI_COMM_WORLD);
  int ierr;
  int argcDiablo = 4;
  char *argvDiablo[4];
  char *newstring = NULL;

  std::vector<std::string> input_name(1) ;
  argvDiablo[0] = new char[3];
  std::strcpy(argvDiablo[0], "-i");
  input_name[0] = std::string(setup_file);
  argvDiablo[1] = new char [ input_name[0].length()+1]; 
  std::strcpy(argvDiablo[1],setup_file.c_str());
  argvDiablo[2] = new char [5]; 
  std::strcpy(argvDiablo[2], "-axe");

  //DR 7/5. This section is pretty hard coded compared to what it should be. Refer to FIVSIM for proper implementation.
  int size=1;
  //int nprocs=2;
  int nprocs;
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  if( nprocs > 9 ) { size = 2; }
  if( nprocs > 99 ) { size = 3; }
  if( nprocs > 999 ) { size = 4; }
  if( nprocs > 9999 ) { size = 5; }
  if( nprocs > 99999 ) { size = 6; }
  newstring  =  new char[size+1]; // need space for '\0
  sprintf( newstring, "%u",nprocs); 
  argvDiablo[3] = new char [size+1]; 
  std::strcpy(argvDiablo[3], newstring);



  if (_axeflag)
  {
    ierr = Diablo_SetOptions( argcDiablo, argvDiablo, FIVSIM_COMM_WORLD);
    ierr = Diablo_Initialize( );
    ierr = Diablo_Finalize( -1 );
    mooseError("Finished running Diablo axe. Now set axeflag=false and run the case! Note: This is intended behavior, not an error. I will figure out how to terminate cardinal without an error at a later time.");
  }
  delete [] newstring;

  argcDiablo = 2;
  ierr = Diablo_SetOptions( argcDiablo, argvDiablo, FIVSIM_COMM_WORLD);
  ierr = Diablo_Initialize( );
}
#endif


#ifndef DIABLO_H
#define DIABLO_H


/*****************************************************************
 * TAG( Variable Types )
 *
 * Define types analogous to the F90 side
 */
typedef double fullR;
typedef float  singR;
typedef int    singI;
#ifdef FEI
include "defs_feiBasicTypes.h"
#else
/* typedef long   GlobalID; */  /* FEI identifiers */
#endif

/*****************************************************************
 * TAG( Bool_type )
 *
 * A boolean value.
 */
typedef int Bool_type;

/*****************************************************************
 * TAG( TRUE FALSE )
 *
 * The boolean values of true and false.
 */
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*****************************************************************
 * TAG( Environ )
 * 
 * Make the indices defined in util_run.f90 avaialble to C world
 */
#define ENV_MAX_STRING_LENGTH 40
#define ENV_STR_INDX_VERSION       1
#define ENV_STR_INDX_VERSION_DATE       2
#define ENV_STR_INDX_COMPILE_DATE       3
#define ENV_STR_INDX_EXEC_DATE          4
#define ENV_STR_INDX_USER               5
#define ENV_STR_INDX_HOST               6
#define ENV_STR_INDX_DEBUG_LIST         7
#define ENV_STR_INDX_ASSEMBLY_FILE      8
#define ENV_STR_INDX_SUBASSEMBLY_FILE   9
#define ENV_STR_INDX_ACTIVE_INPUT_FILE 10
#define ENV_STR_INDX_OUTPUT_FILE       11
#define ENV_STR_INDX_PLOT_FILE         12
#define ENV_STR_INDX_INPUT_FORMAT      13
#define ENV_STR_INDX_CFD_INPUT_FILE    14
#define ENV_STR_INDX_CFD_MESH_FORMAT   15
#define ENV_STR_INDX_TECPLOT_FILE      16
#define ENV_STR_INDX_CSPART_FILE       17

/* IRC: Added for restart files */
#define ENV_STR_INDX_RST_FILE_NAME_IN   18
#define ENV_STR_INDX_RESTART_FILE_IN    19
#define ENV_STR_INDX_RST_FILE_PROC_IN   21
#define ENV_STR_INDX_RST_FILE_PREFIX_IN 22
#define ENV_STR_INDX_RST_FILE_PREFIX_OU 23
/* IRC: Added for restart files */

#define ENV_STR_INDX_AXE_PARTITIONER 24
#define ENV_STR_INDX_UGLY_INPUT 25

#define ENV_ACTION_SUCCESS         0
#define ENV_ERR_STRING_LENGTH      1

#endif /* DIABLO_H */

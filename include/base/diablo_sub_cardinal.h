/* $Id: diablo_sub.h,v 1.2 2014/08/28 00:44:11 whitesides1 Exp $ */


/* -------------------------------------------------------------------
 * diablo_sub.h - program entry point when Diablo is a subroutine to another driver
 *
 *      Robert M. Ferencz
 *      Methods Development Group
 *      Lawrence Livermore National Laboratory
 *      March 28, 2000
 *
 * Copyright (c) 2000-14 Regents of the University of California
 * -------------------------------------------------------------------
 */

/* -------------------------------------------------------------------
 *                        L E G A L   N O T I C E                       
 *                                                                      
 *            (C) Copyright 2000-2014 the Regents of the                
 *          University of California. All Rights Reserved.              
 *                                                                      
 *   This work was produced at the University of California,  Lawrence  
 *   Livermore  National  Laboratory  (UC  LLNL) under contract no. W-  
 *   7405-ENG-48 (Contract 48) between the U.S. Department  of  Energy  
 *   (DOE)  and the Regents of the University of California (Universi-  
 *   ty) for the operation of UC LLNL.  Copyright is reserved  to  the  
 *   University  for the purposes of controlled dissemination, commer-  
 *   cialization through formal licensing, or other disposition  under  
 *   terms  of  Contract 48; DOE policies, regulations and orders; and  
 *   U.S. statutes.  The rights of the Federal Government are reserved  
 *   under  Contract 48 subject to the restrictions agreed upon by the  
 *   DOE and University as allowed under DOE Acquisition Letter 88-1.   
 *                                                                     
 *                          D I S C L A I M E R                         
 *                                                                      
 *   This computer code was prepared as an account of  work  sponsored  
 *   by an agency of the United States Government.  Neither the United  
 *   States Government nor the University of  California  nor  any  of  
 *   their  employees  makes  any warranty, express or implied, or as-  
 *   sumes any liability or responsibility for the accuracy, complete-  
 *   ness,  or  usefulness  of  any information, apparatus, product or  
 *   process disclosed or represents that its specific commercial pro-  
 *   ducts, process or service by trade name, trademark, manufacturer,  
 *   or otherwise does not necessarily constitute  or  imply  its  en-  
 *   dorsement,  recommendation,  or  favoring  by  the  United  State  
 *   Government or the University of California.  The views and  opin-  
 *   ions  of authors expressed herein do not necessarily state or re-  
 *   flect those of the United States Government or the University  of  
 *   California,  and shall not be used for advertising or product en-  
 *   dorsement purposes.                                                
 *                                                                    
 */

#ifndef ___diablo_sub___
#define ___diablo_sub___
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <time.h>
#include "defs_ctypes.h"


/************************************************************
 * TAG( Diablo_SetOptions )
 *
 * Get the command line arguments and a little
 * environment data if desired, then pass into Fortran
 */
int Diablo_SetOptions( int argc, char *argv[], int mpi_comm_diablo );

/************************************************************
 * TAG( Diablo_Destroy )
 *
 * Get the command line arguments and a little
 * environment data if desired, then pass into Fortran
 */
int Diablo_Destroy( );

/************************************************************
 * TAG( Diablo_Initialize )
 *
 */
int Diablo_Initialize( );

/************************************************************
 * TAG( Diablo_Solve )
 *
 * Solve a Coupe Iteration
 * i_iter - the multimechanics iteration number
 * i_step - the time step
 * i_stepstart - the first time step (how we keep track of whether we
 *                                    need to do some additional work)
 * zeroguess - whether to zero out the guess (=1) or use the solution at the end
 *             of the last iteration (=0).  Currently zeroguess=1 is not available 
 * mm_active   - =1: plot a state at the end of each multimechanics iteration
 *                  (using mm_plotfile).  Should be consistent with the value chosen
 *                   during Diablo_InitTimeStep
 * mm_plotfile - the root name of the plotfile for the singlemechanics iterations for this
 *               step (should be a different family name for each step, you choose)
 * mm_active   - =1: plot a state at the end of each Diablo Newton iteration
 *                  (using sm_plotfile), useful for debugging problem convergence
 */
int Diablo_Solve( int i_iter, int i_step, int i_stepstart, int zeroguess,
                  int mm_active, char *sm_plotfile, int sm_active);

/************************************************************
 * TAG( Diablo_InitTimeStep )
 *
 * Initialize the plot file names for this time step
 * Advance History variables, etc.
 *
 * i_step - the new time step
 * i_stepstart - the first time step (how we keep track of whether we
 *                                    need to do some additional work)
 * timeNP1 - the time at the end of the step
 * timeN   - the time at the start of the step
 * mm_plotfile - the root name of the plotfile for the multimechanics iterations for this
 *               step (should be a different family name for each step, you choose)
 * mm_active   - =1: plot a state at the end of each multimechanics iteration
 *                  (using mm_plotfile)
 *
 */
int Diablo_InitTimeStep( int i_step, int i_stepstart, double timeNP1, double timeN,
                         char *mm_plotfile, int mm_active );

/************************************************************
 * TAG( Diablo_Predict )
 *
 * Calls the predictor and updates displacements, velocities, 
 * and accelerations (for now, uses Newmark scheme)
 *
 */
int Diablo_Predict( double x0_surf[][9], double y0_surf[][9], double z0_surf[][9],
                    double  x_surf[][9], double  y_surf[][9], double  z_surf[][9],
                    double vx_surf[][9], double vy_surf[][9], double vz_surf[][9],
                    double ax_surf[][9], double ay_surf[][9], double az_surf[][9],
                    int inode[][4][4],
                    int *isetid, const int *n_surf, int *nout,
                    int *iNekFound, int *icheckvels, int *step);

/************************************************************
 * TAG( Diablo_Predict2 )
 *
 * Calls the predictor and updates displacements, velocities, 
 * and accelerations (for now, uses Newmark scheme)
 *
 */
int Diablo_Predict2( double x0_surf[][9], double y0_surf[][9], double z0_surf[][9],
                     double  x_surf[][9], double  y_surf[][9], double  z_surf[][9],
                     double vx_surf[][9], double vy_surf[][9], double vz_surf[][9],
                     double ax_surf[][9], double ay_surf[][9], double az_surf[][9],
                     int iNeknodemap[][9],
                     int *isetid, const int *n_surf, int *nout,
                     int *iNekFound, int *icheckvels, int *step);

/************************************************************
 * TAG( Diablo_Predict3 )
 *
 * Calls the predictor and updates displacements, velocities, 
 * and accelerations (for now, uses Newmark scheme)
 *
 */
int Diablo_Predict3( double x0_surf[][9], double y0_surf[][9], double z0_surf[][9],
                     double  x_surf[][9], double  y_surf[][9], double  z_surf[][9],
                     double vx_surf[][9], double vy_surf[][9], double vz_surf[][9],
                     double ax_surf[][9], double ay_surf[][9], double az_surf[][9],
                     int iNekFacetmap[][9],
                     double NekFacetXi[][9], double NekFacetEta[][9],
                     int *isetid, const int *n_surf, int *nout,
                     int *iNekFound, int *icheckvels, int *step);

/************************************************************
 * TAG( Diablo_SaveSurfKinematics )
 *
 * save velocities, accelerations, and pressures for interface facets for later use
 *
 */
int Diablo_Save_SurfKin(int *isurfid, int* call_index, int* converged);

/************************************************************
 * TAG( Diablo_Update_Params )
 *
 * updated fictitious mass and damping parameters
 *
 */
int Diablo_FSI_UpdateParams(int *isurfid, int* call_index, int* numfacets, int* converged_disp, int* converged_press);

/************************************************************
 * TAG( Diablo_FinalizeTimeStep )
 *
 * Write out the plot files for this time step (1 per time step)
 * Close the multimech plot files for this time step (1 per coupe iterations)
 *
 */
int Diablo_FinalizeTimeStep( int i_step, double time, int i_stepstart, double stepsize, int mm_active);

/************************************************************
 * TAG( Diablo_Finalize )
 *
 * Close the plot file for the time step loop
 * Print out summary information
 *
 */
int Diablo_Finalize( int i_step );

/************************************************************
 * TAG( Diablo_Create_SurfMap )
 *
 * Create the Map for the NEK FIV Coupling
 *
 */
int Diablo_Create_SurfMap( double x_surf[][9], double y_surf[][9], double z_surf[][9],
                           int inode[][4][4], int iface[][4], int ifacenode[][4][4],
                           int *isurfid, const int *n_surf, int *nout, int *iNekFound);

/************************************************************
 * TAG( Diablo_Create_SurfMap3 )
 *
 * Create the Map for the NEK FIV Coupling
 *
 */
int Diablo_Create_SurfMap3( double x_surf[][9], double y_surf[][9], double z_surf[][9],
                           int iNeknodemap[][9],
                           int *isurfid, const int *n_surf, int *nout, int *iNekNodeFound);

/************************************************************
 * TAG( Diablo_Create_SurfMap4 )
 *
 * Create the Map for the NEK FIV Coupling
 *
 */
int Diablo_Create_SurfMap4( double x_surf[][9], double y_surf[][9], double z_surf[][9],
                           int iNeknodemap[][9],
                           double NekFacetXi[][9], double NekFacetEta[][9],
                           int *isurfid, const int *n_surf, int *nout, int *iNekNodeFound);

/************************************************************
 * TAG( Diablo_Load_SurfPres )
 *
 * Load the Surface Pressures from NEK to Diablo
 *
 */
int Diablo_Load_SurfPres( double p_surf[][9],
                          double s11_surf[][9], double s22_surf[][9], double s33_surf[][9],
                          double s12_surf[][9], double s13_surf[][9], double s23_surf[][9],
                          double xsurf[][9], double ysurf[][9],double zsurf[][9],
                          int iface[][4], int ifacenode[][4][4],
                          int *isurfid, const int *n_surf, int *nout, int *iNekFound, int *icheckpressures);

/************************************************************
 * TAG( Diablo_Load_SurfPres2 )
 *
 * Load the Surface Pressures from NEK to Diablo
 *
 */
int Diablo_Load_SurfPres2( double p_surf[][9],
                           double s11_surf[][9], double s22_surf[][9], double s33_surf[][9],
                           double s12_surf[][9], double s13_surf[][9], double s23_surf[][9],
                           double xsurf[][9], double ysurf[][9],double zsurf[][9],
                           int *isurfid, const int *n_surf, int *nout, int *iNekFound, int *icheckpressures);

/************************************************************
 * TAG( Diablo_Load_SurfPres3 )
 *
 * Load the Surface Pressures from NEK to Diablo
 *
 */
int Diablo_Load_SurfPres3( double p_surf[][9],
                           double s11_surf[][9], double s22_surf[][9], double s33_surf[][9],
                           double s12_surf[][9], double s13_surf[][9], double s23_surf[][9],
                           double xsurf[][9], double ysurf[][9],double zsurf[][9],
                           int *isurfid, const int *n_surf, int *nout, int *iNekFound, int *icheckpressures);

/************************************************************
 * TAG( Diablo_Load_SurfKin )
 *
 * Export kinematics at the interface surface from Diablo to <external solver> (NEK)
 *
 */
int Diablo_Load_SurfKin( double x0_surf[][9], double y0_surf[][9], double z0_surf[][9],
                         double  x_surf[][9], double  y_surf[][9], double  z_surf[][9],
                         double vx_surf[][9], double vy_surf[][9], double vz_surf[][9],
                         double ax_surf[][9], double ay_surf[][9], double az_surf[][9],
                         int inode[][4][4],
                         int *isurfid, const int *n_surf, int *nout,
                         int *iNekFound, int *icheckvels);

/************************************************************
 * TAG( Diablo_Load_SurfKin2 )
 *
 * Export kinematics at the interface surface from Diablo to <external solver> (NEK)
 *
 */
int Diablo_Load_SurfKin2( double x0_surf[][9], double y0_surf[][9], double z0_surf[][9],
                          double  x_surf[][9], double  y_surf[][9], double  z_surf[][9],
                          double vx_surf[][9], double vy_surf[][9], double vz_surf[][9],
                          double ax_surf[][9], double ay_surf[][9], double az_surf[][9],
                          int iNeknodemap[][9],
                          int *isurfid, const int *n_surf, int *nout,
                          int *iNekFound, int *icheckvels);

/************************************************************
 * TAG( Diablo_Load_SurfKin3 )
 *
 * Export kinematics at the interface surface from Diablo to <external solver> (NEK)
 *
 */
int Diablo_Load_SurfKin3( double x0_surf[][9], double y0_surf[][9], double z0_surf[][9],
                          double  x_surf[][9], double  y_surf[][9], double  z_surf[][9],
                          double vx_surf[][9], double vy_surf[][9], double vz_surf[][9],
                          double ax_surf[][9], double ay_surf[][9], double az_surf[][9],
                          int iNekFacetmap[][9],
                          double NekFacetXi[][9], double NekFacetEta[][9],
                          int *isurfid, const int *n_surf, int *nout,
                          int *iNekFound, int *icheckvels);

/************************************************************
 * TAG( Diablo_Set_AnalType )
 *
 * Switch back-and-forth between transient and quasi-static analysis
 * Note - currently imech must be set to 2 (MECH_STRESS)
 *
 */
int Diablo_Set_AnalType(int imech, int itype);

/************************************************************
 * TAG( Diablo_Set_MassScaling )
 *
 * Scale the Mass - may be used to run quasi-statics, or slowly run dynamics
 * Note - currently imech must be set to 2 (MECH_STRESS)
 *
 */
int Diablo_Set_MassScaling(int imech, double MassScaling);

/************************************************************
 * TAG( Diablo_FSI_PressTracConv )
 *
 * Evaluates convergence of the relaxed pressure vs. the 'real' fluid pressure
 *
 */
int Diablo_FSI_PressTracConv(const int *isurfid, double *errnorm, double *errtol);

/************************************************************
 * TAG( Diablo_FSI_RelaxPressTrac )
 *
 * Subrelax fluid pressures in a partitioned FSI scheme
 *
 */
int Diablo_FSI_RelaxPressTrac(const int *isurfid, const int* iconvergence_press);

/************************************************************
 * TAG( Diablo_Set_DispRelaxType )
 *
 * set subrelaxation type for displacements at the FSI interface
 *
 */
int Diablo_FSI_SetDispRelaxType(const int* isurfid);


/************************************************************
 * TAG( Diablo_Change_Solver )
 *
 * Change the linear solver used in Diablo
 * for solid mechanics imech should be set to 2
 * isolver = 0 -> PWSMP
 * isolver = 1 -> MUMPS
 * isolver = 2 -> HYPRE
 *
 * set AsymFlag to 0/1 for Symmetric/Assymmetric Storage (MUMPS and PWSMP only)
 *
 */
int Diablo_Change_Solver(int imech, int isolver, int AsymFlag);
int Diablo_Get_Num_NodeIds(  int *numids,       const int *did, const int *ival);
int Diablo_Transfer_In(  double nvalues [], const int *did, const int *ival);
int Diablo_Transfer_Out(  double nvalues [], const int *did, const int *ival);

#endif

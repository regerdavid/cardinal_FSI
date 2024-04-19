[Mesh]
  type = NekRSMesh
  boundary = 2
  displacements = 'disp_x disp_y disp_z'
[]

[Problem]
  type = NekRSProblem
  casename = 'ocyl_nek'
  fixed_point_iterations = true
  output = 'pressure velocity s11 s22 s33 s12 s13 s23'
  n_usrwrk_slots = 10
  # We omit the non-dimensional settings here in order to just extract the
  # non-dimensional solution as-is, without dimensionalizing it.
[]

[Executioner]
  type = Transient

  [TimeStepper]
    type = NekTimeStepper
  []
[]

[Postprocessors]
  [fp_iteration]
    type = Receiver
    execute_on = 'TIMESTEP_BEGIN  TIMESTEP_END'
  []
  [average_disp_in_nek]
    type = ElementAverageValue
    variable = disp_y
  []
  [average_vel_in_nek]
    type = ElementAverageValue
    variable = vel_y
  []
[]

[Outputs]
  gmv = true
  execute_on='FINAL'
#  interval = 100
  hide = 'flux_integral'
[]

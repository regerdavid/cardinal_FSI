[Mesh]
  type = NekRSMesh
  boundary = 2
  displacements = 'disp_x disp_y disp_z'
[]

[Problem]
  type = NekRSStandaloneProblem
  casename = 'ocyl_nek'
  fixed_point_iterations = false
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

[Outputs]
  gmv = true
[]

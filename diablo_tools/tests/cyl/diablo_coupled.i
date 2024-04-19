num_steps = 20000

[Mesh]
 type = DiabloMesh
 neumann_set = 1
[]
[Problem]
  type = DiabloProblem
  casename = 'OcylV0.assembly'
  axeflag = false
  neumann_set = 1
  num_steps = ${num_steps}
[]

[Outputs]
  gmv = false
#  execute_on='FINAL'
  csv = true 
#  interval = 100
[]

[MultiApps]
  [nek]
    type = TransientMultiApp
    app_type = CardinalApp
    input_files = 'nek_coupled.i'
    execute_on = TIMESTEP_BEGIN
    relaxation_factor = 1
    transformed_variables = 'P'
  []
[]

[Transfers]
  [bdisp_x_to_nek]
    type = MultiAppNearestNodeTransfer
    source_variable = disp_x
    direction = to_multiapp
    multi_app = nek
    variable = disp_x
#  source_boundary = 2
  []
  [bdisp_y_to_nek]
    type = MultiAppNearestNodeTransfer
    source_variable = disp_y
    direction = to_multiapp
    multi_app = nek
    variable = disp_y
#  source_boundary = 2
  []
  [bdisp_z_to_nek]
    type = MultiAppNearestNodeTransfer
    source_variable = disp_z
    direction = to_multiapp
    multi_app = nek
    variable = disp_z
#  source_boundary = 2
  []
  [s11_from_nek]
    type = MultiAppGeometricInterpolationTransfer
    source_variable = s11
    from_multi_app = nek
    variable = s11
  []
  [s22_from_nek]
    type = MultiAppGeometricInterpolationTransfer
    source_variable = s22
    from_multi_app = nek
    variable = s22
  []
  [s33_from_nek]
    type = MultiAppGeometricInterpolationTransfer
    source_variable = s33
    from_multi_app = nek
    variable = s33
  []
  [s12_from_nek]
    type = MultiAppGeometricInterpolationTransfer
    source_variable = s12
    from_multi_app = nek
    variable = s12
  []
  [s13_from_nek]
    type = MultiAppGeometricInterpolationTransfer
    source_variable = s13
    from_multi_app = nek
    variable = s13
  []
  [s23_from_nek]
    type = MultiAppGeometricInterpolationTransfer
    source_variable = s23
    from_multi_app = nek
    variable = s23
  []
  [iteration]
    type = MultiAppPostprocessorTransfer
    to_postprocessor = fp_iteration
    from_postprocessor = num_its
    to_multi_app = nek
  []
[]

[Postprocessors]
 [avg_disp_y]
    type = ElementAverageValue
    variable = disp_y
 []
  [num_its]
    type = NumFixedPointIterations
    execute_on = 'CUSTOM'
  []
[]
[Executioner]
  type = Transient
  dt = 5e-3
  num_steps = ${num_steps}
  fixed_point_max_its = 30
  fixed_point_min_its = 3
  custom_pp = avg_disp_y
  custom_rel_tol = 1e-8
  custom_abs_tol = 1e-8
  accept_on_max_fixed_point_iteration = true
[]


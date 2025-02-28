[Mesh]
  # using a finer mesh will more clearly show the symmetry plane,
  # but we use coarser here to have a smaller gold file
  [sphere]
    type = FileMeshGenerator
    file = ../neutronics/meshes/sphere.e
    #file = ../../../tutorials/pebbles/sphere_finer.e
  []
  [solid_ids]
    type = SubdomainIDGenerator
    input = sphere
    subdomain_id = '100'
  []
[]

[AuxVariables]
  [x]
    family = MONOMIAL
    order = CONSTANT
  []
  [y]
    family = MONOMIAL
    order = CONSTANT
  []
  [z]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [x]
    type = PointTransformationAux
    variable = x
    component = x
  []
  [y]
    type = PointTransformationAux
    variable = y
    component = y
  []
  [z]
    type = PointTransformationAux
    variable = z
    component = z
  []
[]

[Problem]
  type = OpenMCCellAverageProblem
  power = 100.0
  solid_blocks = '100'
  solid_cell_level = 0
  tally_type = cell
  normalize_by_global_tally = false
  initial_properties = xml

  symmetry_mapper = sym
[]

[UserObjects]
  [sym]
    type = SymmetryPointGenerator
    normal = '0.0 1.0 0.0'
    rotation_axis = '0.0 0.0 1.0'
    rotation_angle = ${fparse 360.0 / 6.0}
  []
[]

[Executioner]
  type = Transient
  num_steps = 1
[]

[Outputs]
  exodus = true

  # we are only interested in the point transformation aspects here
  hide = 'kappa_fission temp'
[]

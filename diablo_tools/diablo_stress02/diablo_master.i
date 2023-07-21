[Mesh]
 type = DiabloMesh
 neumann_set = 1
[]
[Problem]
  type = DiabloStandaloneProblem
  casename = 'stress02.assembly'
  axeflag = false
[]

[Variables]
  [null]
    initial_condition = 0.0
  []
[]

[Kernels]
  [null]
    type = NullKernel
    variable = null
  []
[]

[Executioner]
  type = Transient
  dt = 1
  num_steps = 2
[]

[Mesh]
 type = DiabloMesh
 neumann_set = 1
[]
[Problem]
  type = DiabloProblem
  casename = 'OcylV0.assembly'
  axeflag = false
  neumann_set = 1
[]

[Variables]
#  [null]
#    initial_condition = 0.0
#  []
[]

[Kernels]
#  [null]
#    type = NullKernel
#    variable = null
#  []
[]

[Functions]
  [test]
    type = ParsedFunction
    expression = -10*z
  []
[]

[ICs]
 [s11]
   type = ConstantIC
   variable = s11
   value = 0.0
 []
 [s22]
   type = FunctionIC
   variable = s22
   function=test
 []
 [s33]
   type = ConstantIC
   variable = s33
   value = 0.0
 []

 [s12]
   type = ConstantIC
   variable = s12
   value = 0.0
 []
 [s13]
   type = ConstantIC
   variable = s13
   value = 0.0
 []
 [s23]
   type = ConstantIC
   variable = s23
   value = 0.0
 []

[]

[Outputs]
  gmv = true
  execute_on=FINAL
[]

[Executioner]
  type = Transient
  dt = 1
  num_steps = 2
[]

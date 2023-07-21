#pragma once

extern "C"
{
#include "diablo_sub_cardinal.h"
}
#include "MooseMesh.h"
#include "MooseApp.h"
#include "CardinalEnums.h"


class DiabloMesh : public MooseMesh
{
public:
  DiabloMesh(const InputParameters & parameters);
  static InputParameters validParams();

  DiabloMesh(const DiabloMesh & /* other_mesh */) = default;

  DiabloMesh & operator=(const DiabloMesh & other_mesh) = delete;
  virtual std::unique_ptr<MooseMesh> safeClone() const override;

  virtual void buildMesh() override;

  virtual void buildDummyMesh();

  //Neumann Set in Diablo to use for coupling
  const int & _neumann_set;

};

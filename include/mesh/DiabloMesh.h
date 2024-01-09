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

  int _numNodeIds;
  int _numElems;
  int _n_vertices_per_elem;
    /// Function pointer to the type of new element to add
  Elem * (DiabloMesh::*_new_elem)() const;

  /// Function returning the processor id which should own each element
  int (DiabloMesh::*_elem_processor_id)(const int elem_id);

  /**
   * Processor id (rank) owning the given boundary element
   * @return processor id
   */
  int boundaryElemProcessorID(const int elem_id);

  /// Create a new element for a boundary mesh
  Elem * boundaryElem() const;

    /// Data structure holding mapping information for boundary coupling
  //DiabloBoundaryCoupling _boundary_coupling;

    /**
   * Get the boundary coupling data structure
   * @return boundary coupling data structure
   */
  //const DiabloBoundaryCoupling & boundaryCoupling() const { return _boundary_coupling; }
  
  /**
   * \brief Get the number of NekRS elements we rebuild in the MOOSE mesh
   *
   * This function is used to perform the data transfer routines in NekRSProblem
   * agnostic of whether we have surface or volume coupling.
   * return number of elements
   */
  const int & numElems() const { return _numElems; }

    /**
   * \brief Get the number of NekRS elements we rebuild in the MOOSE mesh
   *
   * This function is used to perform the data transfer routines in NekRSProblem
   * agnostic of whether we have surface or volume coupling.
   * return number of elements
   */
  const int & numNodeIds() const { return _numNodeIds; }

    /**
   * Get the number of MOOSE elements we build for each Diablo element. Currently just returning 1 as a test.
   * @return MOOSE elements built for each NekRS element
   */
  int nMoosePerDiablo() const { return 1; }
};
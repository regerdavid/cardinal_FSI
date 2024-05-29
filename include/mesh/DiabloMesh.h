#pragma once

extern "C"
{
#include "diablo_sub_cardinal.h"
}
#include "MooseMesh.h"
#include "DiabloBoundaryCoupling.h"
#include "MooseApp.h"
#include "CardinalEnums.h"
#include "libmesh/face_quad4.h"


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
  int _localNodeIds;
  int _numElems;
  int _localElems;
  int _n_vertices_per_elem;

  std::vector<double> _x;
  std::vector<double> _y;
  std::vector<double> _z;

  std::vector<int> _localNodeCounts;
  std::vector<int> _localElemCounts;
  std::vector<int> _globalNodeStart;
  std::vector<int> _globalElemStart;
  std::vector<int> _globalNodeEnd;
  std::vector<int> _globalElemEnd;
  std::vector<int> _processor_id;
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
   * \brief Get the number of Diablo elements we rebuild in the MOOSE mesh
   *
   * This function is used to perform the data transfer routines in DiabloProblem
   * for surface coupling.
   * return number of elements
   */
  const int & numElems() const { return _numElems; }

    /**
   * \brief Get the number of Diablo nodes we rebuild in the MOOSE mesh
   *
   * This function is used to perform the data transfer routines in DiabloProblem
   * for surface coupling.
   * return number of nodes
   */
  const int & numNodeIds() const { return _numNodeIds; }

  /**
   * \brief Get the number of Diablo elements we rebuild in the MOOSE mesh that exist on the current processor
   *
   * This function is used to perform the data transfer routines in DiabloProblem
   * for surface coupling.
   * return number of elements
   */
  const int & localElems() const { return _numElems; }

    /**
   * \brief Get the number of Diablo nodes we rebuild in the MOOSE mesh that exist on the current processor
   *
   * This function is used to perform the data transfer routines in DiabloProblem
   * for surface coupling.
   * return number of nodes
   */
  const int & localNodeIds() const { return _numNodeIds; }

  const std::vector<int> & globalNodeStart() const { return _globalNodeStart; }
  const std::vector<int> & globalElemtart() const { return _globalElemStart; }
  const std::vector<int> & globalNodeEnd() const { return _globalNodeEnd; }
  const std::vector<int> & globalElemEnd() const { return _globalElemEnd; }
  const std::vector<int> & processor_id() const { return _processor_id; }
    /**
   * Get the number of MOOSE elements we build for each Diablo element. Currently just returning 1 as a test.
   * @return MOOSE elements built for each NekRS element
   */
  int nMoosePerDiablo() const { return 1; }

  /// Data structure holding mapping information for boundary coupling
  DiabloBoundaryCoupling _boundary_coupling;

    /**
   * Store the rank-local element and rank ownership for boundary coupling;
   * this loops over the Diablo mesh and fetches relevant information on the boundaries
   */
  void storeBoundaryCoupling();
};

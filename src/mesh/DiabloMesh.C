#ifdef ENABLE_DIABLO_COUPLING

#include "DiabloMesh.h"
#include "libmesh/face_quad4.h"
#include "libmesh/face_quad9.h"
#include "libmesh/cell_hex8.h"
#include "libmesh/cell_hex27.h"
#include "CardinalUtils.h"
#include "VariadicTable.h"

registerMooseObject("CardinalApp", DiabloMesh);

InputParameters
DiabloMesh::validParams()
{
    InputParameters params = MooseMesh::validParams();
    params.addParam<int>("neumann_set",
                                    "Neumann Set through which Diablo will be coupled to MOOSE");
    return params;
}

DiabloMesh::DiabloMesh(const InputParameters & parameters)
  : MooseMesh(parameters),
  _neumann_set(getParam<int>("neumann_set"))
  {
  }

void
DiabloMesh::buildDummyMesh()
{
  int e = 1;

  auto elem = new Quad4;
  elem->set_id() = e;
  elem->processor_id() = 0;
  _mesh->add_elem(elem);

  Point pt1(0.0, 0.0, 0.0);
  Point pt2(1.0, 0.0, 0.0);
  Point pt3(1.0, 1.0, 0.0);
  Point pt4(0.0, 1.0, 0.0);

  elem->set_node(0) = _mesh->add_point(pt1);
  elem->set_node(1) = _mesh->add_point(pt2);
  elem->set_node(2) = _mesh->add_point(pt3);
  elem->set_node(3) = _mesh->add_point(pt4);

  _mesh->prepare_for_use();


}

void
DiabloMesh::buildMesh()
{
    int ivar = 1; //1 will take mesh coords from diablo, 2 will take displacements, 3 will take velocities
    int ierr;
    _n_vertices_per_elem = 4; //Hard coded for quad element currently
    _new_elem = &DiabloMesh::boundaryElem;
    int node_index[4] ={0,1,3,2};
    //_elem_processor_id = &DiabloMesh::boundaryElemProcessorID;

    //DiabloMesh::buildDummyMesh(); //This can be commented out once I get an actual working mesh mirror
    ierr = Diablo_Get_Num_NodeIds(&_numNodeIds,&_neumann_set,&ivar); //Retrieve the number of vertexes in the specified neumann set
    std::cout<<"NumNodeIds is " << _numNodeIds <<std::endl;
    _numElems = (_numNodeIds-2)/2; //THIS MAY NOT BE TRUE FOR ALL ELEMENT TYPES (OR CORRECT AT ALL)

    double *nodeCoords;
    nodeCoords = new double[3*_numNodeIds];

    ierr = Diablo_Transfer_Out(nodeCoords,&_neumann_set,&ivar); //Retrieve the coordinates of the vertexes in the neumann set

/*    for (int count =0; count<3*_numNodeIds;count+=3)
    {
      std::cout<<"Coords: " << nodeCoords[count] << "," << nodeCoords[count+1] << "," << nodeCoords[count+2]<<std::endl;
    } */

    for (int e = 0; e < _numElems; e++)
    {
      auto elem = (this->*_new_elem)();
      elem->set_id() = e;
      //elem->processor_id() = (this->*_elem_processor_id)(e); //TODO: Add this back in
      _mesh->add_elem(elem);

      for (int n = 0; n < _n_vertices_per_elem; n++)
      {
        int node = (e * 6) + node_index[n]*3;
        Point p(nodeCoords[node], nodeCoords[node+1], nodeCoords[node+2]);
       // p *= _scaling;

        auto node_ptr = _mesh->add_point(p);
        elem->set_node(n) = node_ptr;
      }
    }
    // We're looking up the elements by id, so we can't let the ids get
  // renumbered.
  _mesh->allow_renumbering(false);

  // If we have a DistributedMesh then:
  if (!_mesh->is_replicated())
  {
    // we've already partitioned the elements to match the nekrs
    // mesh, and libMesh shouldn't try to improve on that.  We won't
    // ever be doing any element deletion or coarsening, so we don't
    // even need libMesh's "critical" partitioning.
    _mesh->skip_partitioning(true);

    // But that means we have to update the partitioning metadata
    // ourselves
    _mesh->recalculate_n_partitions();

    // But, we haven't yet partitioned nodes, and if we tell libMesh
    // not to do that automatically then we need to do it manually
    libMesh::Partitioner::set_node_processor_ids(*_mesh);
  }

  _mesh->prepare_for_use();
  //
    return;
}

int
DiabloMesh::boundaryElemProcessorID(const int elem_id)
{
  //return _boundary_coupling.processor_id(elem_id);
  return 0;
}

Elem *
DiabloMesh::boundaryElem() const
{
    return new Quad4;
}

std::unique_ptr<MooseMesh>
DiabloMesh::safeClone() const
{
  return libmesh_make_unique<DiabloMesh>(*this);
}
#endif
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

    int numNodeIds;
    int ierr;

    DiabloMesh::buildDummyMesh(); //This can be commented out once I get an actual working mesh mirror

    ierr = Diablo_Get_Num_NodeIds(&numNodeIds,&_neumann_set,&ivar); //Retrieve the number of vertexes in the specified neumann set
    std::cout<<"NumNodeIds is " << numNodeIds <<std::endl;

    double *nodeCoords;
    nodeCoords = new double[3*numNodeIds];

    ierr = Diablo_Transfer_Out(nodeCoords,&_neumann_set,&ivar); //Retrieve the coordinates of the vertexes in the neumann set

    for (int count =0; count<3*numNodeIds;count+=3)
    {
      std::cout<<"Coords: " << nodeCoords[count] << "," << nodeCoords[count+1] << "," << nodeCoords[count+2]<<std::endl;
    }

    return;
}

std::unique_ptr<MooseMesh>
DiabloMesh::safeClone() const
{
  return libmesh_make_unique<DiabloMesh>(*this);
}
#endif
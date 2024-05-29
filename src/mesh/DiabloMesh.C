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

    int node_index[4] ={0,1,2,3}; //If needing to translate between how libmesh orders quad nodes and how Diablo orders quad nodes. Forutnately, they order them the same way.
    int components_per_point = 3; //Transferring 3 components per node (x, y, and z)
    _elem_processor_id = &DiabloMesh::boundaryElemProcessorID;
    //DiabloMesh::buildDummyMesh(); //PREVIOUSLY FOR TESTING
    ierr = Diablo_Get_Num_NodeIds(&_numNodeIds,&_neumann_set,&ivar); //Retrieve the number of vertexes in the specified neumann set
    _localNodeIds = _numNodeIds; //Store the number of node IDs that live on the current processor
    _communicator.sum(_numNodeIds);

    //std::cout<<"NumNodeIds is " << _numNodeIds <<std::endl;
    //_numElems = (_numNodeIds-2)/2; //This was for the stress02 example that ordered everything by single points intead of (point[1:4],face)
    _numElems = _numNodeIds/4;
    _localElems = _localNodeIds/4;
    double *nodeCoords;
    nodeCoords = new double[components_per_point*_localNodeIds];

    ierr = Diablo_Transfer_Out(nodeCoords,&_neumann_set,&ivar); //Retrieve the coordinates of the vertexes in the neumann set

    
    //THIS SETS UP THE MAPPING FOR MULTIPLE PROCESSORS
    _localNodeCounts.resize(_communicator.size());
    _localElemCounts.resize(_communicator.size());
    _globalNodeStart.resize(_communicator.size());
    _globalElemStart.resize(_communicator.size());
    _globalNodeEnd.resize(_communicator.size());
    _globalElemEnd.resize(_communicator.size());
    _communicator.allgather(_localNodeIds,_localNodeCounts);
    _communicator.allgather(_localElems,_localElemCounts);
    for (int i = 0; i<_globalNodeStart.size();i++)
    {
      if (i==0)
      {
        _globalNodeStart.at(i)=0;
        _globalElemStart.at(i)=0;
      }  
      else
      {
        for (int j=0; j<i; j++)
        {
          _globalNodeStart.at(i)+=_localNodeCounts.at(j);
          _globalElemStart.at(i)+=_localElemCounts.at(j);

        }
      }

      if (_localNodeCounts.at(i)==0)
      {
        //My logic for making the map gets a bit messed up if a rank doesn't own any nodes.
        //If that is the case, just make the start and stop outside of the range of elements.
        _globalNodeStart.at(i)=_numElems+1; 
        _globalElemStart.at(i)=_numElems+1;
        _globalNodeEnd.at(i)=_numElems+1; 
        _globalElemEnd.at(i)=_numElems+1;
      }
      else
      {
      _globalNodeEnd.at(i)=_globalNodeStart.at(i)+_localNodeCounts.at(i)-1;
      _globalElemEnd.at(i)=_globalElemStart.at(i)+_localElemCounts.at(i)-1;
      }
      //UNCOMMENT TO DEBUG THE BINNING OF ELEMENTS TO EACH PROCESSOR
      //std::cout<<"Global elem start at " << i << " is " << _globalElemStart.at(i)<<std::endl;
      //std::cout<<"Global elem end at " << i << " is " << _globalElemEnd.at(i)<<std::endl;
    }

    for (int i = 0; i<_localNodeIds*components_per_point; i+=3)
    {
	    _x.push_back(nodeCoords[i]);
      _y.push_back(nodeCoords[i+1]);
      _z.push_back(nodeCoords[i+2]);

    }
    _communicator.allgather(_x);
    _communicator.allgather(_y);
    _communicator.allgather(_z);

    _processor_id.resize(_numElems);

    for (int e = 0; e < _numElems; e++)
    {
      auto elem = (this->*_new_elem)();
      elem->set_id() = e;
      for (int i = 0; i < _communicator.size(); i++)
      {
        if (e>=_globalElemStart.at(i) && e<=_globalElemEnd.at(i))
          _processor_id.at(e) = i;
      }
      elem->processor_id() = _processor_id.at(e);
      _mesh->add_elem(elem);
      //std::printf("Processor assigned to element %d on rank %d is %d\n",e,_communicator.rank(),elem->processor_id());
      for (int n = 0; n < _n_vertices_per_elem; n++)
      {
	    int node_start = (e*4)+node_index[n];
        
      //Set the current point x,y,z
	    Point p(_x[node_start], _y[node_start], _z[node_start]);
        
       // p *= _scaling;
        //Add the point to the current mesh mirror element
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
  
    return;
}

int
DiabloMesh::boundaryElemProcessorID(const int elem_id)
{
  //THIS DOESNT CURRENTLY GET CALLED ANYWHERE!
  //return _boundary_coupling.processor_id(elem_id);
  //return _communicator.rank();
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

void
DiabloMesh::storeBoundaryCoupling()
{
}
#endif


#pragma once
extern "C"
{
#include "diablo_sub_cardinal.h"
}
#include "CardinalProblem.h"
#include "Transient.h"
#include "DiabloMesh.h"

#include <memory>

class DiabloProblemBase : public CardinalProblem
{
public:
  DiabloProblemBase(const InputParameters & params);

  static InputParameters validParams();

  ~DiabloProblemBase();
    /**
   * Whether a data transfer to/from Nek is occurring
   * @param[in] direction direction of data transfer
   * @return whether a data transfer to Nek is about to occur
   */
    bool isDataTransferHappening(ExternalProblem::Direction direction);

    virtual void initialSetup() override;

    virtual void externalSolve() override;

    virtual void syncSolutions(ExternalProblem::Direction direction) override;

    virtual bool converged() override { return true; }

    virtual void addExternalVariables() override;

  /**
   * Whether data should be synchronized in to Diablo
   * \return whether inward data synchronization should occur
   */
  virtual bool synchronizeIn();

  /**
   * Whether data should be synchronized out of Diablo
   * \return whether outward data synchronization should occur
   */
  virtual bool synchronizeOut();

    /// Underlying executioner
    Transient * _transient_executioner = nullptr;

    /// Diablo casename
    const std::string & _casename;

    const bool & _axeflag;

    int _numNodeIds;

    int _numElems;

    int _n_vertices_per_elem;

    DiabloMesh * _diablo_mesh;

    /// Numeric identifiers for the external variables
    std::vector<unsigned int> _external_vars;

    //Neumann Set in Diablo to use for coupling
    const int & _neumann_set;

    const int & _num_steps;
    
    /**
   * Extract user-specified parts of the Diablo solution onto the mesh mirror
   */
  virtual void extractOutputs();

  /**
  Fill an outgoing auxiliary variable field with nekRS solution data
   * \param[in] var_number auxiliary variable number
   * \param[in] value nekRS solution data to fill the variable with
   */
  virtual void fillAuxVariable(const unsigned int var_number, const double * value);

};

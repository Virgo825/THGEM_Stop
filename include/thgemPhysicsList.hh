#ifndef thgemPhysicsList_h
#define thgemPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class thgemPhysicsList : public G4VModularPhysicsList
{

public:
    thgemPhysicsList();
    virtual ~thgemPhysicsList();

    virtual void SetCuts();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    
protected:
    // these methods Construct physics processes and register them
    void AddParameterisation();
    
};
#endif

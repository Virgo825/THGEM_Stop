
#ifndef thgemActionInitialization_h
#define thgemActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class thgemDetectorConstruction;
class thgemPrimaryGeneratorAction;

// Action Initialization class
class thgemActionInitialization : public G4VUserActionInitialization
{
public:
	thgemActionInitialization(thgemDetectorConstruction*);
	virtual ~thgemActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;

private:
	thgemDetectorConstruction* fDetConstruction;
};

#endif
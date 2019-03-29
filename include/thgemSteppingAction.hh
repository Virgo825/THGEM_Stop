
#ifndef thgemSteppingAction_h
#define thgemSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class thgemDetectorConstruction;
class thgemEventAction;

class thgemSteppingAction : public G4UserSteppingAction
{
public:
	thgemSteppingAction(const thgemDetectorConstruction* detectorConstruction,
						thgemEventAction* eventAction);
	virtual ~thgemSteppingAction();

	virtual void UserSteppingAction(const G4Step* step);

private:
	const thgemDetectorConstruction* fDetConstruction;
	thgemEventAction* fEventAction;
};
#endif
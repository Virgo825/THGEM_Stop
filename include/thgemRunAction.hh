
#ifndef thgemRunAction_h
#define thgemRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class thgemPrimaryGeneratorAction;
class thgemDetectorConstruction;

class thgemRunAction : public G4UserRunAction
{
public:
	thgemRunAction(thgemDetectorConstruction*, thgemPrimaryGeneratorAction*);
	virtual ~thgemRunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);

private:
	thgemDetectorConstruction* fDetector;
	thgemPrimaryGeneratorAction* fPrimary;
};
#endif
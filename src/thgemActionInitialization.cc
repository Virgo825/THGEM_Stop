
#include "thgemActionInitialization.hh"
#include "thgemPrimaryGeneratorAction.hh"
#include "thgemPrimaryGeneratorAction.hh"
#include "thgemRunAction.hh"
#include "thgemEventAction.hh"
#include "thgemSteppingAction.hh"
#include "thgemDetectorConstruction.hh"

thgemActionInitialization::thgemActionInitialization(thgemDetectorConstruction* detConstruction)
	:G4VUserActionInitialization(),
	fDetConstruction(detConstruction)
{}

thgemActionInitialization::~thgemActionInitialization()
{}

void thgemActionInitialization::BuildForMaster() const
{
	thgemPrimaryGeneratorAction* generator = new thgemPrimaryGeneratorAction(fDetConstruction);
	SetUserAction(new thgemRunAction(fDetConstruction, generator));
}
void thgemActionInitialization::Build() const
{
	thgemPrimaryGeneratorAction* generator = new thgemPrimaryGeneratorAction(fDetConstruction);
	SetUserAction(generator);
	SetUserAction(new thgemRunAction(fDetConstruction, generator));
	thgemEventAction* eventAction = new thgemEventAction(generator);
	SetUserAction(eventAction);
	SetUserAction(new thgemSteppingAction(fDetConstruction,eventAction));
}
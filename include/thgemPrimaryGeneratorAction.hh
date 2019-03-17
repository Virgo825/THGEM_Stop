
#ifndef thgemPrimaryGeneratorAction_h
#define thgemPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;

class thgemDetectorConstruction;
class thgemPrimaryGeneratorMessenger;

class thgemPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	thgemPrimaryGeneratorAction(thgemDetectorConstruction* );
	virtual ~thgemPrimaryGeneratorAction();

	void SetDefaultKinematic();	
	virtual void GeneratePrimaries(G4Event* anEvent);
	
	//
	void PrintParticleParameter() const;

	// set methodes
	void SetNeutronWavelength(G4double val) {fNeutronWavelength = val;}
	
	// get methodes
	const G4ParticleGun* GetParticleGun() const {return fParticleGun;}
	G4double GetNeutronEnergy() {return 0.0819154132 / (fNeutronWavelength * fNeutronWavelength) * eV;}
	G4double GetNeutronWavelength() {return fNeutronWavelength;}
	
private:
	G4ParticleGun* fParticleGun; // G4 particle gun
	thgemDetectorConstruction* fDetector;
	thgemPrimaryGeneratorMessenger* fGunMessenger;

	G4double fNeutronWavelength;
};
#endif
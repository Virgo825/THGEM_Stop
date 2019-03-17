
#ifndef thgemEventAction_h
#define thgemEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include <iostream>
#include <fstream>

using namespace std;

class thgemPrimaryGeneratorAction;

class thgemEventAction : public G4UserEventAction
{
private:
	G4int eventID;
	G4String fparticleName;
	G4double fKinEnergyStart;
	G4double fKinEnergy;
	G4double fEnergyGas;
	G4double fGlobalTime;
	G4ThreeVector fPosition;
	// G4ThreeVector fMomentumStartDir;
	G4ThreeVector fMomentumDir;
	G4ThreeVector fElectionPositionSum;
	G4int fElectronNumber;

	thgemPrimaryGeneratorAction* fPrimary;

public:
	thgemEventAction(thgemPrimaryGeneratorAction*);
	virtual ~thgemEventAction();

	virtual void BeginOfEventAction(const G4Event* event);
	virtual void EndOfEventAction(const G4Event* event);

	inline G4int GetEventID() {return eventID;}
	inline void AddGas(G4double de){fEnergyGas += de;}
	inline void AddElectronPosition(G4ThreeVector pos) {fElectionPositionSum += pos;}
	inline void AddElectronNumber() {fElectronNumber++;}

	inline void SaveParticleName(G4String name){fparticleName = name;}
	inline void SaveKinEnergyStart(G4double ne){fKinEnergyStart = ne;}
	inline void SaveKinEnergy(G4double ne){fKinEnergy = ne;}
	inline void SaveGlobalTime(G4double t){fGlobalTime = t;}
	inline void SavePosition(G4ThreeVector pos){fPosition = pos;}
	// inline void SaveMomentumStartDir(G4ThreeVector dir){fMomentumStartDir = dir;}
	inline void SaveMomentumDir(G4ThreeVector dir){fMomentumDir = dir;}	
	
};
#endif
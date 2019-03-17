
#ifndef thgemSteppingAction_h
#define thgemSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "thgemDetectorConstruction.hh"

class G4VPhysicalVolum;
class G4VLogicalVolume;
class G4Material;

class thgemDetectorConstruction;
class thgemEventAction;

class thgemSteppingAction : public G4UserSteppingAction
{
private:
	G4int eventID, trackID, parentID, stepNumber;
	G4String particleName;
	G4ThreeVector momentumStartDir, momentumDir, momentumSize; // start momentum direction, every step momentum direction, every step momentum size
	G4ThreeVector position;
	G4double kinEnergyStart, kinEnergy, totalEnergy; // start kinetic energy, every step particle kinetic energy, total energy
	G4double globalTime, localTime, properTime; // 

	G4double energyDeposit, stepLength, stepTOF; // every step enery deposit, step length, 
	G4StepPoint* pointPre, *pointPost; // step consist of two points
	G4ThreeVector positionPre, positionPost; // position in the global coordinate system
	G4double kinEnergyPre, kinEnergyPost; // kinetic Energy
	G4TouchableHandle touchPre, touchPost; // geometrical information
	G4VPhysicalVolume* volumePre, *volumePost; //current volume
	G4String namePre, namePost; // current volume name
	// G4LogicalVolume* logical_volumePre, *logical_volumePost; // current logical volume 
	// G4String logical_volumePre, logical_volumePost; // current logical volume name
	G4String materialPre, materialPost; // current volume material name
	G4VPhysicalVolume* mother_volumePre, *mother_volumePost; //current volume's mother volume

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
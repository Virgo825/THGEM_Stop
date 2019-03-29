
#include "thgemSteppingAction.hh"
#include "thgemEventAction.hh"
#include "thgemDetectorConstruction.hh"
#include "thgemAnalysis.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

thgemSteppingAction::thgemSteppingAction(
	const thgemDetectorConstruction* detectorConstruction, thgemEventAction* eventAction)
	:G4UserSteppingAction(), fDetConstruction(detectorConstruction), fEventAction(eventAction)
{}
thgemSteppingAction::~thgemSteppingAction()
{}
void thgemSteppingAction::UserSteppingAction(const G4Step* step)
{	
	G4Track* track = step->GetTrack();

	// G4int eventID = fEventAction->GetEventID();
	// G4int trackID = track->GetTrackID();
	// G4int parentID = track->GetParentID();	
	// G4int stepNumber = track->GetCurrentStepNumber();
	G4String particleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();
	G4double globalTime = track->GetGlobalTime();
	// G4double localTime = track->GetLocalTime();
	// G4double properTime = track->GetProperTime();
	G4double kinEnergyStart = track->GetVertexKineticEnergy();
	G4double kinEnergy = track->GetKineticEnergy();
	G4ThreeVector position = track->GetPosition();
	// G4ThreeVector momentumStartDir = track->GetVertexMomentumDirection();
	G4ThreeVector momentumDir = track->GetMomentumDirection();
	// G4ThreeVector momentumSize = track->GetMomentum();

	G4double energyDeposit = step->GetTotalEnergyDeposit();
	// G4double stepLength = step->GetStepLength();
	// G4double stepTOF = step->GetDeltaTime();

	G4VPhysicalVolume *volumePre = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
	G4VPhysicalVolume *volumePost = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

	// positionPre = pointPre->GetPosition(); // get position in the global coordinate system;
	// positionPost = pointPost->GetPosition();

	// G4String namePre, namePost;

	// if(!volumePost){
	// 	namePre = volumePre->GetName();
	// 	namePost = "OutOfWorld";
	// 	// logical_volumePre = volumePre->GetLogicalVolume()->GetName();
	// 	// logical_volumePost = "LogicalNull";
	// 	// materialPre = pointPre->GetMaterial()->GetName();
	// 	// materialPost = "MaterialNull";
	// }else{
	// 	namePre = volumePre->GetName();
	// 	namePost = volumePost->GetName();
	// 	// logical_volumePre = volumePre->GetLogicalVolume();
	// 	// logical_volumePost = volumePost->GetLogicalVolume();
	// 	// materialPre = pointPre->GetMaterial()->GetName();
	// 	// materialPost = pointPost->GetMaterial()->GetName();
	// }
	// if(positionPre->GetStepStatus() == fGeomBoundary) // the preStepPoint is at the boundary
	// if(positionPost->GetStepStatus() == fGeomBoundary) // the postStepPoint is at the boundary
	
	if(volumePre == fDetConstruction->GetPhysiGas()) fEventAction->AddGas(energyDeposit);

	// Debug
	// if(particleName != "neutron")
	// G4cout << eventID << "    " << parentID << "    " << trackID << "    " << stepNumber << "    " << particleName << "    "  << G4BestUnit(globalTime, "Time") << "    " << G4BestUnit(kinEnergyStart, "Energy") << "    " << G4BestUnit(kinEnergy, "Energy") << "    " << G4BestUnit(energyDeposit, "Energy") << "	" << namePre << "	" << namePost << G4endl;
	
	if(fDetConstruction->GetPhysiStop())
	{
		if(((particleName == "alpha")||(particleName == "Li7"))&&((volumePre == fDetConstruction->GetPhysiStop())&&(volumePost == fDetConstruction->GetPhysiGas())))
		{
			fEventAction->SaveParticleName(particleName);
			fEventAction->SavePosition(position);		
			fEventAction->SaveKinEnergyStart(kinEnergyStart);
			fEventAction->SaveKinEnergy(kinEnergy);
			fEventAction->SaveMomentumDir(momentumDir);
			fEventAction->SaveGlobalTime(globalTime);
		}		
	}
	else
	{
		if(((particleName == "alpha")||(particleName == "Li7"))&&((volumePre == fDetConstruction->GetPhysiTransform())&&(volumePost == fDetConstruction->GetPhysiGas())))
		{
			fEventAction->SaveParticleName(particleName);
			fEventAction->SavePosition(position);	
			fEventAction->SaveKinEnergyStart(kinEnergyStart);
			fEventAction->SaveKinEnergy(kinEnergy);
			fEventAction->SaveMomentumDir(momentumDir);
			fEventAction->SaveGlobalTime(globalTime);
		}
	}
}

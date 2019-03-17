
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
	eventID = fEventAction->GetEventID();

	G4Track* track = step->GetTrack();
	trackID = track->GetTrackID();
	parentID = track->GetParentID();	
	particleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();
	stepNumber = track->GetCurrentStepNumber();
	globalTime = track->GetGlobalTime();
	// localTime = track->GetLocalTime();
	// properTime = track->GetProperTime();
	position = track->GetPosition();

	kinEnergyStart = track->GetVertexKineticEnergy();
	kinEnergy = track->GetKineticEnergy();
	// totalEnergy = track->GetTotalEnergy();
	momentumStartDir = track->GetVertexMomentumDirection();
	momentumDir = track->GetMomentumDirection();
	// momentumSize = track->GetMomentum();

	energyDeposit = step->GetTotalEnergyDeposit();
	// stepLength = step->GetStepLength();
	// stepTOF = step->GetDeltaTime();

	pointPre = step->GetPreStepPoint();
	pointPost = step->GetPostStepPoint();

	// positionPre = pointPre->GetPosition(); // get position in the global coordinate system;
	// positionPost = pointPost->GetPosition();

	touchPre = pointPre->GetTouchableHandle(); // 
	touchPost = pointPost->GetTouchableHandle();

	volumePre = touchPre->GetVolume(); // get the current volume
	volumePost = touchPost->GetVolume();

	if(volumePost == 0){
		namePre = volumePre->GetName();
		namePost = "OutOfWorld";
		// logical_volumePre = volumePre->GetLogicalVolume()->GetName();
		// logical_volumePost = "LogicalNull";
		materialPre = pointPre->GetMaterial()->GetName();
		materialPost = "MaterialNull";
	}else{
		namePre = volumePre->GetName();
		namePost = volumePost->GetName();
		// logical_volumePre = volumePre->GetLogicalVolume();
		// logical_volumePost = volumePost->GetLogicalVolume();
		materialPre = pointPre->GetMaterial()->GetName();
		materialPost = pointPost->GetMaterial()->GetName();
	}
	// if(positionPre->GetStepStatus() == fGeomBoundary) // the preStepPoint is at the boundary
	// if(positionPost->GetStepStatus() == fGeomBoundary) // the postStepPoint is at the boundary
	if(volumePre == fDetConstruction->GetPhysiGas())
		fEventAction->AddGas(energyDeposit);

	// Debug
	// if(particleName != "neutron")
	// G4cout << eventID << "    " << parentID << "    " << trackID << "    " << stepNumber << "    " << particleName << "    "  << G4BestUnit(globalTime, "Time") << "    " << G4BestUnit(kinEnergyStart, "Energy") << "    " << G4BestUnit(kinEnergy, "Energy") << "    " << G4BestUnit(energyDeposit, "Energy") << "    " << G4BestUnit(position.z(), "Length") << "    " << G4BestUnit(positionPost.z(), "Length") << "    " << namePre << "    " << namePost << G4endl;
	
	// if(((particleName == "alpha")||(particleName == "Li7"))&&((namePre == "Stop")&&(namePost == "Gas")))
	if(((particleName == "alpha")||(particleName == "Li7"))&&((namePre == "Transform")&&(namePost == "Gas")))
	{
		fEventAction->SaveParticleName(particleName);
		fEventAction->SaveKinEnergyStart(kinEnergyStart);
		// fEventAction->SaveMomentumStartDir(momentumStartDir);
		fEventAction->SaveKinEnergy(kinEnergy);
		fEventAction->SaveGlobalTime(globalTime);
		fEventAction->SavePosition(position);		
		fEventAction->SaveMomentumDir(momentumDir);
	}
	if((particleName == "e-") && (stepNumber == 1) && (namePre == "Gas") && (namePost == "Gas"))
	{
		fEventAction->AddElectronNumber();
		fEventAction->AddElectronPosition(position);

		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillH3(1, position.x(), position.y(), position.z());
		analysisManager->FillNtupleIColumn(1, 0, eventID);
		analysisManager->FillNtupleIColumn(1, 1, parentID);
		analysisManager->FillNtupleDColumn(1, 2, kinEnergyStart/eV);
		analysisManager->FillNtupleDColumn(1, 3, globalTime/ns);
		analysisManager->FillNtupleDColumn(1, 4, position.x());
		analysisManager->FillNtupleDColumn(1, 5, position.y());
		analysisManager->FillNtupleDColumn(1, 6, position.z());
		analysisManager->FillNtupleDColumn(1, 7, momentumDir.x());
		analysisManager->FillNtupleDColumn(1, 8, momentumDir.y());
		analysisManager->FillNtupleDColumn(1, 9, momentumDir.z());
		analysisManager->AddNtupleRow(1);
	}
	
}

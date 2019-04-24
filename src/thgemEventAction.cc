#include "thgemPhysics.hh"
#include "thgemEventAction.hh"
#include "thgemRunAction.hh"
#include "thgemAnalysis.hh"
#include "thgemPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

thgemEventAction::thgemEventAction(thgemPrimaryGeneratorAction* prim)
	: G4UserEventAction(), fPrimary(prim)
{}
thgemEventAction::thgemEventAction()
	: G4UserEventAction()
{}
thgemEventAction::~thgemEventAction()
{}
void thgemEventAction::BeginOfEventAction(const G4Event* event)
{
	fEventID = event->GetEventID();
	fParticleName = "neutron";
	fKinEnergyStart = 0.;
	fKinEnergy = 0.;
	fEnergyGas = 0.;
	fGlobalTime = 0.;
	fPosition = G4ThreeVector(0.,0.,0.);
	fMomentumDir = G4ThreeVector(0.,0.,0.);

	thgemPhysics *physics = thgemPhysics::GetInstance();
	physics->Clear();
}
void thgemEventAction::EndOfEventAction(const G4Event* /*event*/)
{
	if(fParticleName != "neutron")
	{
		thgemPhysics *physics = thgemPhysics::GetInstance();

		G4cout << " ---> EventID: " << fEventID << "   " << fParticleName << "	" << G4BestUnit(fEnergyGas, "Energy") << "	" << physics->GetElectronNumber() << G4endl;

		G4double angle = (atan(fabs(sqrt((fMomentumDir.x())*(fMomentumDir.x())+(fMomentumDir.z())*(fMomentumDir.z()))/fMomentumDir.y())))*180/3.14;

		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleIColumn(0, 0, fEventID);
		analysisManager->FillNtupleSColumn(0, 1, fParticleName);
		analysisManager->FillNtupleDColumn(0, 2, fKinEnergyStart);
		analysisManager->FillNtupleDColumn(0, 3, fKinEnergy);
		analysisManager->FillNtupleDColumn(0, 4, fEnergyGas);
		analysisManager->FillNtupleDColumn(0, 5, fGlobalTime);
		analysisManager->FillNtupleDColumn(0, 6, fPosition.x());
		analysisManager->FillNtupleDColumn(0, 7, fPosition.y());
		analysisManager->FillNtupleDColumn(0, 8, fPosition.z());
		analysisManager->FillNtupleDColumn(0, 9, fMomentumDir.x());
		analysisManager->FillNtupleDColumn(0, 10, fMomentumDir.y());
		analysisManager->FillNtupleDColumn(0, 11, fMomentumDir.z());
		analysisManager->FillNtupleDColumn(0, 12, angle);
		analysisManager->FillNtupleIColumn(0, 13, physics->GetElectronNumber());
		analysisManager->FillNtupleDColumn(0, 14, physics->GetElectronCenterX());
		analysisManager->FillNtupleDColumn(0, 15, physics->GetElectronCenterY());
		analysisManager->FillNtupleDColumn(0, 16, physics->GetElectronCenterZ());
		analysisManager->AddNtupleRow(0);
	}
}

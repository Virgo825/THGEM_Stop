
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
	: G4UserEventAction(), 
	  fPrimary(prim)
{}
thgemEventAction::~thgemEventAction()
{}
void thgemEventAction::BeginOfEventAction(const G4Event* event)
{
	eventID = event->GetEventID();
	fparticleName = "neutron";
	fKinEnergyStart = 0.;
	fKinEnergy = 0.;
	fEnergyGas = 0.;
	fGlobalTime = 0.;
	fPosition = G4ThreeVector(0.,0.,0.);
	// fMomentumStartDir = G4ThreeVector(0.,0.,0.);
	fMomentumDir = G4ThreeVector(0.,0.,0.);
	fElectionPositionSum = G4ThreeVector(0.,0.,0.);
	fElectronNumber = 0;	
}
void thgemEventAction::EndOfEventAction(const G4Event* /*event*/)
{
	if(fparticleName != "neutron")
	{
		G4cout << " ---> EventID: " << eventID << "   " << fparticleName << G4endl;

		G4double angle = (atan(fabs(sqrt((fMomentumDir.x())*(fMomentumDir.x())+(fMomentumDir.y())*(fMomentumDir.y()))/fMomentumDir.z())))*180/3.14;

		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleIColumn(0, 0, eventID);
		analysisManager->FillNtupleSColumn(0, 1, fparticleName);
		analysisManager->FillNtupleDColumn(0, 2, fKinEnergyStart);
		analysisManager->FillNtupleDColumn(0, 3, fKinEnergy);
		analysisManager->FillNtupleDColumn(0, 4, fGlobalTime);
		analysisManager->FillNtupleDColumn(0, 5, fPosition.x());
		analysisManager->FillNtupleDColumn(0, 6, fPosition.y());
		analysisManager->FillNtupleDColumn(0, 7, fPosition.z());
		analysisManager->FillNtupleDColumn(0, 8, fMomentumDir.x());
		analysisManager->FillNtupleDColumn(0, 9, fMomentumDir.y());
		analysisManager->FillNtupleDColumn(0, 10, fMomentumDir.z());
		analysisManager->FillNtupleDColumn(0, 11, angle);
		analysisManager->FillNtupleDColumn(0, 12, fEnergyGas);
		analysisManager->FillNtupleIColumn(0, 13, fElectronNumber);
		analysisManager->FillNtupleDColumn(0, 14, fElectionPositionSum.x()/fElectronNumber);
		analysisManager->FillNtupleDColumn(0, 15, fElectionPositionSum.y()/fElectronNumber);
		analysisManager->FillNtupleDColumn(0, 16, fElectionPositionSum.z()/fElectronNumber);
		analysisManager->AddNtupleRow(0);
	}
}

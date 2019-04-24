
#include "thgemRunAction.hh"
#include "thgemDetectorConstruction.hh"
#include "thgemPrimaryGeneratorAction.hh"
#include "thgemAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

thgemRunAction::thgemRunAction(thgemDetectorConstruction* det, thgemPrimaryGeneratorAction* prim)
	: G4UserRunAction(), fDetector(det), fPrimary(prim)
{
	G4RunManager::GetRunManager()->SetPrintProgress(0); // set printing event number per each event

	// Create analysis manager
	// The choice of analysis technology is done via selection of a namespace in thgemAnalysis.hh
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	G4cout << "Using " << analysisManager->GetType() << G4endl;

	// Default settings
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFirstHistoId(1);
	// analysisManager->SetFirstNtupleId(1);
	// analysisManager->SetFirstNtupleColumnId(1);

	analysisManager->SetFileName("NeutronWavelength_1.8A.root");

	// Creating histograms
	// analysisManager->CreateH1("1", "Edep in Gas", 100, 0., 2*MeV);
	// analysisManager->CreateH2("1", "Track position in XY", 200, -10*cm, 10*cm, 200, -10*cm, 10*cm);
	// analysisManager->CreateH3("1", "Track position", 100, -5*mm, 5*mm, 100, -5*mm, 5*mm, 100, -1*mm, 9*mm);

	// Creating 1st ntuple (id = 0)
	analysisManager->CreateNtuple("ion", "The information of primary ions");
	analysisManager->CreateNtupleIColumn("EventID");
	analysisManager->CreateNtupleSColumn("ParticleName");
	analysisManager->CreateNtupleDColumn("KinEnergyStart");
	analysisManager->CreateNtupleDColumn("KinEnergy");
	analysisManager->CreateNtupleDColumn("EnergyGas");
	analysisManager->CreateNtupleDColumn("GlobalTime");
	analysisManager->CreateNtupleDColumn("Position_x");
	analysisManager->CreateNtupleDColumn("Position_y");
	analysisManager->CreateNtupleDColumn("Position_z");
	analysisManager->CreateNtupleDColumn("Momentum_x");
	analysisManager->CreateNtupleDColumn("Momentum_y");
	analysisManager->CreateNtupleDColumn("Momentum_z");
	analysisManager->CreateNtupleDColumn("Angle");
	analysisManager->CreateNtupleIColumn("ElectronNumber");
	analysisManager->CreateNtupleDColumn("ElectronCenter_x");
	analysisManager->CreateNtupleDColumn("ElectronCenter_y");
	analysisManager->CreateNtupleDColumn("ElectronCenter_z");
	analysisManager->FinishNtuple();

	// Creating 2nd ntuple (id = 1)
	analysisManager->CreateNtuple("ele", "The information of electron");
	analysisManager->CreateNtupleDColumn("Position_x");
	analysisManager->CreateNtupleDColumn("Position_y");
	analysisManager->CreateNtupleDColumn("Position_z");
	analysisManager->CreateNtupleDColumn("GlobalTime");
	analysisManager->FinishNtuple();
}
thgemRunAction::~thgemRunAction()
{
	delete G4AnalysisManager::Instance();
}
void thgemRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
	// inform the runManager to save random number seed
	// G4RunManager::GetRunManager()->SetRandomNumberStore(true);

	G4double wavelength = fPrimary->GetNeutronWavelength();
	G4double transformThick = fDetector->GetTransformThick()/um;
	G4double stopThick = fDetector->GetStopThick()/um;
	G4double gasThick = fDetector->GetGasThick()/mm;
	G4String transformMaterial = fDetector->GetTransformMaterial();
	G4String stopMaterial = fDetector->GetStopMaterial();
	G4String gasMaterial = fDetector->GetGasMaterial();

	char rootName[200];
	sprintf(rootName, "%.1lfA_neutron_%.2lfum_%s_%.1lfum_%s_%.0lfmm_%s.root", wavelength, transformThick, transformMaterial.c_str(), stopThick, stopMaterial.c_str(), gasThick, gasMaterial.c_str());

	// Get analysis manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	// Open an output file
	analysisManager->OpenFile(G4String(rootName));
}
void thgemRunAction::EndOfRunAction(const G4Run* /*run*/)
{
	// print parameter
	fDetector->PrintDetectorParameter();
	fPrimary->PrintParticleParameter();

	// Save histogram & ntuple
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();
}

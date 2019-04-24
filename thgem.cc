
#include "thgemDetectorConstruction.hh"
#include "thgemActionInitialization.hh"
#include "thgemPhysicsList.hh"
#include "thgemPhysics.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"
#include "FTFP_BERT_HP.hh"
#include "QGSP_BERT_HP.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Random.hh"

namespace {
	void PrintUsage(){
		G4cerr << "Usage: " << G4endl;
		G4cerr << "thgem [-m macro] [-u UIsession]" << G4endl;
	}
}
int main(int argc, char** argv)
{
	// Evaluate arguments
	if(argc > 7){
		PrintUsage();
		return 1;
	}

	G4String macro, session;

	for(G4int i = 1; i < argc; i=i+2){
		if	   (G4String(argv[i]) == "-m") macro = argv[i+1];
		else if(G4String(argv[i]) == "-u") session = argv[i+1];
		else{
			PrintUsage();
			return 1;
		}
	}

	// Detector interactive mode (if no macro provided) and define UI session.
	G4UIExecutive* ui = 0;
	if(!macro.size()) ui = new G4UIExecutive(argc, argv, session);

	// Choose the Random engine.
	// G4Random::setTheEngine(new CLHEP::RanecuEngine);
	CLHEP::HepRandom::setTheSeed((G4long)time(0));
	Garfield::randomEngine.Seed((G4long)time(0));

	G4RunManager * runManager = new G4RunManager;

	// Set mandatory initialization classes.
	thgemDetectorConstruction* detConstruction = new thgemDetectorConstruction();
	runManager->SetUserInitialization(detConstruction);

	// G4VModularPhysicsList* physics_list = new FTFP_BERT_HP;
	// runManager->SetUserInitialization(physics_list);
	runManager->SetUserInitialization(new thgemPhysicsList());

	thgemActionInitialization* actionInitialization = new thgemActionInitialization(detConstruction);
	runManager->SetUserInitialization(actionInitialization);

	// Initialize visualization
	G4VisExecutive* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Get the pointer to the User Interface manager.
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session.
	if(macro.size()){
		// batch mode
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command + macro);
	}else{
		// interactive mode: define UI session.
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		if(ui->IsGUI()) UImanager->ApplyCommand("/control/execute gui.mac");
		ui->SessionStart();
		delete ui;
	}
	// Job termination
	// Free the store: user actions, physics_list and detector_description are owned and deleted by the run manager,
	// so they should not be deleted in the main() program!
	 
	delete visManager;
	delete runManager;
	thgemPhysics::Dispose();
}

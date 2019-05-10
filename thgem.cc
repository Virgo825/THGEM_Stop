
#include "thgemDetectorConstruction.hh"
#include "thgemActionInitialization.hh"
#include "thgemPhysicsList.hh"
#include "thgemPhysics.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

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
		if(G4String(argv[i]) == "-m") 
			macro = argv[i+1];
		else if(G4String(argv[i]) == "-u") 
			session = argv[i+1];
		else{
			PrintUsage();
			return 1;
		}
	}

	// Detector interactive mode (if no macro provided) and define UI session
	G4UIExecutive* ui = 0;
	if(!macro.size())
		ui = new G4UIExecutive(argc, argv, session);

	// Choose the Random engine.
	// G4Random::setTheEngine(new CLHEP::RanecuEngine);
	CLHEP::HepRandom::setTheSeed((G4long)time(NULL));
	Garfield::randomEngine.Seed((G4long)time(NULL));

	G4RunManager * runManager = new G4RunManager;

	// Set mandatory initialization classes.
	thgemDetectorConstruction* detConstruction = new thgemDetectorConstruction();
	runManager->SetUserInitialization(detConstruction);

	// G4VModularPhysicsList* physics_list = new FTFP_BERT_HP;
	// runManager->SetUserInitialization(physics_list);
	runManager->SetUserInitialization(new thgemPhysicsList());

	thgemActionInitialization* actionInitialization = new thgemActionInitialization(detConstruction);
	runManager->SetUserInitialization(actionInitialization);

#ifdef G4VIS_USE
	// Initialize visualization
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();
#endif

	// Get the pointer to the User Interface manager.
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session.
	if(macro.size()){
		// batch mode
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command + macro);
	}else{
#ifdef G4UI_USE
#ifdef G4VIS_USE
		// interactive mode: define UI session.
		UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
		UImanager->ApplyCommand("/control/execute init.mac");
#endif
		if(ui->IsGUI()) UImanager->ApplyCommand("/control/execute gui.mac");
		// start interactive session
		ui->SessionStart();
		delete ui;
#endif
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are owned and deleted by the run manager,
	// so they should not be deleted in the main() program!

#ifdef G4VIS_USE	 
	delete visManager;
#endif

	delete runManager;
	thgemPhysics::Dispose();
}

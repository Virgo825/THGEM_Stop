// #include "thgemPhysicsMessenger.hh"
// #include "thgemPhysics.hh"

// #include "G4UIcommand.hh"
// #include "G4UIdirectory.hh"
// #include "G4UIparameter.hh"

// thgemPhysicsMessenger::thgemPhysicsMessenger()
//     : G4UImessenger()
// {
//     fPhysicsDir = new G4UIdirectory("/thgem/physics/");
//     fPhysicsDir->SetGuidance("Particle and energy ranges for Garfield++ physics model.");

//     fIonizationModelCmd = new G4UIcommand("/thgem/physics/setIonizationModel", this);
// 	fIonizationModelCmd->SetGuidance("Select ionization model for Garfield++");
// 	fIonizationModelCmd->SetGuidance(" and choose whether to use default particles");
// 	fIonizationModelCmd->SetGuidance(" and energy ranges for the chosen model");

// 	G4UIparameter *ionizationModelPrm = new G4UIparameter("ionizationModel", 's', false);
// 	ionizationModelPrm->SetGuidance("ionization model (1. PAIPhot, 2. PAI, 3. Heed)");
// 	ionizationModelPrm->SetGuidance(" 1. PAIPhot model in Geant4, delta electrons transported by Heed");
// 	ionizationModelPrm->SetGuidance(" 2, PAI model in Geant4, delta electrons transported by Heed");
// 	ionizationModelPrm->SetGuidance(" 3. Use directly Heed");
// 	fIonizationModelCmd->SetParameter(ionizationModelPrm);

// 	G4UIparameter *useDefaultPrm = new G4UIparameter("useDefaults", 'b', false);
// 	useDefaultPrm->SetGuidance("true to use default, false to manully choose particles and energies");
// 	fIonizationModelCmd->SetParameter(useDefaultPrm);

// 	fIonizationModelCmd->AvailableForStates(G4State_PreInit);

// 	fGeant4ParticleCmd = new G4UIcommand("/thgem/physics/setGeant4ParticleTypeAndEnergy", this);
// 	fGeant4ParticleCmd->SetGuidance("Select particle types and energies for PAI and PAIPhot model");
// 	fGeant4ParticleCmd->SetGuidance(" in Geant4");

// 	G4UIparameter *particleGeant4Prm = new G4UIparameter("particleName", 's', false);
// 	particleGeant4Prm->SetGuidance("Particle name (e-, e+, mu-, mu+, proton, pi-, pi+, alpha, Li7, He3, GenericIon)");
// 	fGeant4ParticleCmd->SetParameter(particleGeant4Prm);

// 	G4UIparameter *minEnergyGeant4Prm = new G4UIparameter("minimumEnergyGeant4", 'd', false);
// 	minEnergyGeant4Prm->SetGuidance("minimum energy");
// 	minEnergyGeant4Prm->SetParameterRange("minimumEnergyGeant4>=0");
// 	fGeant4ParticleCmd->SetParameter(minEnergyGeant4Prm);

// 	G4UIparameter *maxEnergyGeant4Prm = new G4UIparameter("maximumEnergyGeant4", 'd', false);
// 	maxEnergyGeant4Prm->SetGuidance("maximum energy");
// 	maxEnergyGeant4Prm->SetParameterRange("maximumEnergyGeant4>=0");
// 	fGeant4ParticleCmd->SetParameter(maxEnergyGeant4Prm);

// 	G4UIparameter *unitGeant4Prm = new G4UIparameter("unit", 's', false);
// 	unitGeant4Prm->SetGuidance("unit of energy");
// 	G4String unitListGeant4 = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("MeV"));
// 	unitGeant4Prm->SetParameterCandidates(unitListGeant4);
// 	fGeant4ParticleCmd->SetParameter(unitGeant4Prm);

// 	fGeant4ParticleCmd->AvailableForStates(G4State_PreInit);

// 	fGarfieldParticleCmd = new G4UIcommand("/thgem/physics/setGarfieldParticleTypeAndEnergy", this);
// 	fGarfieldParticleCmd->SetGuidance("Select particle types and energies for Heed model");
// 	fGarfieldParticleCmd->SetGuidance(" For PAI and PAIPhot model choose at which energy electrons are");
// 	fGarfieldParticleCmd->SetGuidance(" transport as delta electrons by Heed, and treatment of gammas");

// 	G4UIparameter *particleGarfieldPrm = new G4UIparameter("particleName", 's', false);
// 	particleGarfieldPrm->SetGuidance("Particle name (gamma, e-, e+, mu-, mu+, proton, anti_proton, pi-, pi+, kaon, kaon+, alpha, Li7, deuteron)");
// 	fGarfieldParticleCmd->SetParameter(particleGarfieldPrm);

// 	G4UIparameter *minEnergyGarfieldPrm = new G4UIparameter("minimumEnergyGarfield", 'd', false);
// 	minEnergyGarfieldPrm->SetGuidance("minimum energy");
// 	minEnergyGarfieldPrm->SetParameterRange("minimumEnergyGarfield>=0");
// 	fGarfieldParticleCmd->SetParameter(minEnergyGarfieldPrm);

// 	G4UIparameter *maxEnergyGarfieldPrm = new G4UIparameter("maximumEnergyGarfield", 'd', false);
// 	maxEnergyGarfieldPrm->SetGuidance("maximum energy");
// 	maxEnergyGarfieldPrm->SetParameterRange("maximumEnergyGarfield>=0");
// 	fGarfieldParticleCmd->SetParameter(maxEnergyGarfieldPrm);

// 	G4UIparameter *unitGarfieldPrm = new G4UIparameter("unit", 's', false);
// 	unitGarfieldPrm->SetGuidance("unit of energy");
// 	G4String unitListGarfield = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("MeV"));
// 	unitGarfieldPrm->SetParameterCandidates(unitListGarfield);
// 	fGarfieldParticleCmd->SetParameter(unitGarfieldPrm);

// 	fGarfieldParticleCmd->AvailableForStates(G4State_PreInit);
// }
// thgemPhysicsMessenger::~thgemPhysicsMessenger()
// {
//     delete fPhysicsDir;
//     delete fIonizationModelCmd;
//     delete fGeant4ParticleCmd;
//     delete fGarfieldParticleCmd;
// }
// void thgemPhysicsMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
// {
//     if(command == fIonizationModelCmd)
//     {
//         thgemPhysics *physics = thgemPhysics::GetInstance();
//         G4String modelName;
//         G4bool useDefaults;
//         std::istringstream is(newValue);
//         std::cout << newValue << std::endl;
//         is >> modelName >> std::boolalpha >> useDefaults;
// 		G4cout << "zhoujianjinIon" << G4endl;
//         physics->SetIonizationModel(modelName, useDefaults);
//     }
//     if(command == fGeant4ParticleCmd)
//     {
// 		thgemPhysics *physics = thgemPhysics::GetInstance();
// 		G4String particleName, unit, programName;
// 		G4double minEnergy;
// 		G4double maxEnergy;
// 		std::istringstream is(newValue);
// 		is >> particleName >> minEnergy >> maxEnergy >> unit;
// 		minEnergy *= G4UIcommand::ValueOf(unit);
// 		maxEnergy *= G4UIcommand::ValueOf(unit);
// 		physics->AddParticleName(particleName, minEnergy, maxEnergy, "geant4");
//     }
// 	if (command == fGarfieldParticleCmd)
// 	{
// 		thgemPhysics *physics = thgemPhysics::GetInstance();
// 		G4String particleName, unit, programName;
// 		G4double minEnergy;
// 		G4double maxEnergy;
// 		std::istringstream is(newValue);
// 		is >> particleName >> minEnergy >> maxEnergy >> unit;
// 		minEnergy *= G4UIcommand::ValueOf(unit);
// 		maxEnergy *= G4UIcommand::ValueOf(unit);
// 		physics->AddParticleName(particleName, minEnergy, maxEnergy, "garfield");
// 	}
// }
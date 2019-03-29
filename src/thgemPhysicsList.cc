
#include "thgemPhysicsList.hh"
#include "thgemPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "FTFP_BERT_HP.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmParameters.hh"
#include "G4EmConfigurator.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4PAIPhotModel.hh"
#include "G4PAIModel.hh"
#include "G4LossTableManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

thgemPhysicsList::thgemPhysicsList()
    : G4VModularPhysicsList()
{
    G4int verb = 0;
    SetVerboseLevel(verb);
    defaultCutValue = 1 * CLHEP::mm;
    FTFP_BERT_HP *physicsList = new FTFP_BERT_HP;
    for(G4int i = 0; ; i++)
    {
        G4VPhysicsConstructor* elem = const_cast<G4VPhysicsConstructor*>(physicsList->GetPhysics(i));
        if(elem == NULL) break;
        G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
        RegisterPhysics(elem);
    } 
}
thgemPhysicsList::~thgemPhysicsList()
{}
void thgemPhysicsList::AddParameterisation()
{
	thgemPhysics *physics = thgemPhysics::GetInstance();

	G4String ionizationModel = physics->GetIonizationModel();

	G4FastSimulationManagerProcess *fastSimProcess_thgem = new G4FastSimulationManagerProcess("G4FSMP_garfield");

	theParticleIterator->reset();
	while ((*theParticleIterator)())
	{
		G4ParticleDefinition *particle = theParticleIterator->value();
		G4ProcessManager *pmanager = particle->GetProcessManager();
		G4EmConfigurator *config = G4LossTableManager::Instance()->EmConfigurator();
		G4LossTableManager::Instance()->SetVerbose(1);

		if (physics->FindParticleName(particle->GetParticleName(), "garfield"))
			pmanager->AddDiscreteProcess(fastSimProcess_thgem);    
		if (physics->FindParticleName(particle->GetParticleName(), "geant4"))
		{
			G4double minEnergy_MeV = physics->GetMinEnergyMeVParticle(particle->GetParticleName(), "geant4");
			G4double maxEnergy_MeV = physics->GetMaxEnergyMeVParticle(particle->GetParticleName(), "geant4");
			if (ionizationModel == "PAI")
			{
				G4PAIModel *pai = new G4PAIModel(particle, "G4PAIModel");
				if (particle->GetParticleName() == "e-" || particle->GetParticleName() == "e+")
					config->SetExtraEmModel(particle->GetParticleName(), "eIoni", pai, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, pai);
				else if (particle->GetParticleName() == "mu-" || particle->GetParticleName() == "mu+")
					config->SetExtraEmModel(particle->GetParticleName(), "muIoni", pai, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, pai);
				else if (particle->GetParticleName() == "proton" || particle->GetParticleName() == "pi+" || particle->GetParticleName() == "pi-")
					config->SetExtraEmModel(particle->GetParticleName(), "hIoni", pai, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, pai);
				else if (particle->GetParticleName() == "alpha" || particle->GetParticleName() == "He3" || particle->GetParticleName() == "GenericIon")
					config->SetExtraEmModel(particle->GetParticleName(), "ionIoni", pai, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, pai);
			}
			else if (ionizationModel == "PAIPhot")
			{
				G4PAIPhotModel *paiPhot = new G4PAIPhotModel(particle, "G4PAIPhotModel");
				if (particle->GetParticleName() == "e-" || particle->GetParticleName() == "e+")
					config->SetExtraEmModel(particle->GetParticleName(), "eIoni", paiPhot, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, paiPhot);
				else if (particle->GetParticleName() == "mu-" || particle->GetParticleName() == "mu+")
					config->SetExtraEmModel(particle->GetParticleName(), "muIoni", paiPhot, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, paiPhot);
				else if (particle->GetParticleName() == "proton" || particle->GetParticleName() == "pi+" || particle->GetParticleName() == "pi-")
					config->SetExtraEmModel(particle->GetParticleName(), "hIoni", paiPhot, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, paiPhot);
				else if (particle->GetParticleName() == "alpha" || particle->GetParticleName() == "He3" || particle->GetParticleName() == "GenericIon")
					config->SetExtraEmModel(particle->GetParticleName(), "ionIoni", paiPhot, "RegionGas", minEnergy_MeV * MeV, maxEnergy_MeV * MeV, paiPhot);
			}
		}
	}
}
void thgemPhysicsList::SetCuts()
{
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(21.5 * eV, 100 * TeV);
    SetCutsWithDefault();

    G4Region* region = G4RegionStore::GetInstance()->GetRegion("RegionGas");
    G4ProductionCuts* cuts = new G4ProductionCuts();
    cuts->SetProductionCut(1. * um, G4ProductionCuts::GetIndex("gamma"));
    cuts->SetProductionCut(1. * um, G4ProductionCuts::GetIndex("e+"));
    cuts->SetProductionCut(1. * um, G4ProductionCuts::GetIndex("e-"));
    cuts->SetProductionCut(1. * um, G4ProductionCuts::GetIndex("proton"));
    // cuts->SetProductionCut(1. * um, G4ProductionCuts::GetIndex("alpha"));
    // cuts->SetProductionCut(1. * um, G4ProductionCuts::GetIndex("Li7"));
    // cuts->SetProductionCut(1. * um, G4ProductionCuts::GetIndex("GenericIon"));

    if(region) region->SetProductionCuts(cuts);

    DumpCutValuesTable();
    
    G4cout << "zhou  " << G4BestUnit(G4ProductionCutsTable::GetProductionCutsTable()->GetLowEdgeEnergy(),"Energy") << G4endl;
    G4cout << "zhou  " << G4BestUnit(cuts->GetProductionCut("alpha"),"Length") << G4endl;
    G4cout << "zhou  " << G4BestUnit(cuts->GetProductionCut("Li7"),"Length") << G4endl;
}
void thgemPhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();
}
void thgemPhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
    AddParameterisation();
}
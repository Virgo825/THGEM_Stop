#include "thgemFastSimulationModel.hh"

#include "G4VPhysicalVolume.hh"
#include "G4GDMLParser.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

thgemFastSimulationModel::thgemFastSimulationModel(G4String modelName, G4Region *envelope)
    : G4VFastSimulationModel(modelName, envelope)
{
    fPhysics = thgemPhysics::GetInstance();
    fPhysics->InitializePhysics();
}
thgemFastSimulationModel::thgemFastSimulationModel(G4String modelName)
    : G4VFastSimulationModel(modelName)
{
    fPhysics = thgemPhysics::GetInstance();
    fPhysics->InitializePhysics();
}
thgemFastSimulationModel::~thgemFastSimulationModel()
{}
void thgemFastSimulationModel::WriteGeometryToGDML(G4VPhysicalVolume *physicalVolume)
{
	G4GDMLParser* parser = new G4GDMLParser();
	remove("thgemGeometry.gdml");
	parser->Write("thgemGeometry.gdml", physicalVolume, false);
	delete parser;
}
G4bool thgemFastSimulationModel::IsApplicable(const G4ParticleDefinition& particleType)
{
	G4String particleName = particleType.GetParticleName();

	if(fPhysics->FindParticleName(particleName, "garfield")) return true;

	return false;
}
G4bool thgemFastSimulationModel::ModelTrigger(const G4FastTrack& fastTrack)
{
	G4String particleName = fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();
	G4double ekin_MeV = fastTrack.GetPrimaryTrack()->GetKineticEnergy() / MeV;

	if(fPhysics->FindParticleNameEnergy(particleName, ekin_MeV, "garfield")) return true;

	return false;
}
void thgemFastSimulationModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep)
{
    G4TouchableHandle theTouchable = fastTrack.GetPrimaryTrack()->GetTouchableHandle();
    G4String name = theTouchable->GetVolume()->GetName();

    G4ThreeVector worldDir = fastTrack.GetPrimaryTrack()->GetMomentumDirection();
    G4ThreeVector localDir = fastTrack.GetPrimaryTrackLocalDirection();

    G4ThreeVector worldPosition = fastTrack.GetPrimaryTrack()->GetPosition();
    G4ThreeVector localPosition = fastTrack.GetPrimaryTrackLocalPosition();

    G4double ekin_MeV = fastTrack.GetPrimaryTrack()->GetKineticEnergy() / MeV;
    G4double globalTime = fastTrack.GetPrimaryTrack()->GetGlobalTime();

    G4String particleName = fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();

    fastStep.KillPrimaryTrack();
    fastStep.SetPrimaryTrackPathLength(0.0);

    if (particleName == "kano+") particleName = "K+";
    else if (particleName == "kano-") particleName = "K-";
    else if (particleName == "anti_proton") particleName = "anti-proton";

    fPhysics->DoIt(particleName, ekin_MeV, globalTime,
        worldPosition.x() / CLHEP::cm, worldPosition.y() / CLHEP::cm, worldPosition.z() / CLHEP::cm,
        worldDir.x(), worldDir.y(), worldDir.z());

    fastStep.SetTotalEnergyDeposited(fPhysics->GetEnergyDeposit_MeV());

    if (fPhysics->GetCreateSecondariesInGeant4())
    {
        std::vector<GarfieldParticle *> *secondaryParticles = fPhysics->GetSecondaryParticles();
        if (secondaryParticles->size() > 0)
        {
            fastStep.SetNumberOfSecondaryTracks(secondaryParticles->size());

            G4double totalEnergySecondaries_MeV = 0.;

            for (std::vector<GarfieldParticle *>::iterator it = secondaryParticles->begin(); it != secondaryParticles->end(); ++it)
            {
                G4double x = (*it)->GetX_mm();
                G4double y = (*it)->GetY_mm();
                G4double z = (*it)->GetZ_mm();
                G4double eKin_MeV = (*it)->GetEkin_MeV();
                G4double dx = (*it)->GetDX();
                G4double dy = (*it)->GetDY();
                G4double dz = (*it)->GetDZ();
                G4double time = (*it)->GetTime();
                G4ThreeVector momentumDirection(dx, dy, dz);
                G4ThreeVector position(x, y, z);
                if ((*it)->GetParticleName() == "e-")
                {
                    G4DynamicParticle particle(G4Electron::ElectronDefinition(), momentumDirection, eKin_MeV);
                    fastStep.CreateSecondaryTrack(particle, position, time, true);
                    totalEnergySecondaries_MeV += eKin_MeV;
                }
                else if ((*it)->GetParticleName() == "gamma")
                {
                    G4DynamicParticle particle(G4Gamma::GammaDefinition(), momentumDirection, eKin_MeV);
                    fastStep.CreateSecondaryTrack(particle, position, time, true);
                    totalEnergySecondaries_MeV += eKin_MeV;
                }
            }
        }
    }
}

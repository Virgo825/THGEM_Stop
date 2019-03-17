
#include "thgemPrimaryGeneratorAction.hh"
#include "thgemPrimaryGeneratorMessenger.hh"
#include "thgemDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4GeneralParticleSource.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "Randomize.hh"

thgemPrimaryGeneratorAction::thgemPrimaryGeneratorAction(thgemDetectorConstruction* det)
	:G4VUserPrimaryGeneratorAction(),
	fParticleGun(NULL), fDetector(det), fGunMessenger(NULL),
	fNeutronWavelength(1.8)
{
	G4int nofParficle = 1;
	fParticleGun = new G4ParticleGun(nofParficle);

	SetDefaultKinematic();

	fGunMessenger = new thgemPrimaryGeneratorMessenger(this);
}
thgemPrimaryGeneratorAction::~thgemPrimaryGeneratorAction()
{
	delete fParticleGun;
	delete fGunMessenger;
}
void thgemPrimaryGeneratorAction::SetDefaultKinematic()
{
	G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
	fParticleGun->SetParticleEnergy(GetNeutronEnergy());
}
void thgemPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	G4double startPositionX = G4UniformRand()-0.5;
	G4double startPositionY = G4UniformRand()-0.5;
	while(sqrt(startPositionX*startPositionX+startPositionY*startPositionY) > 0.5)
	{
		startPositionX = G4UniformRand()-0.5;
		startPositionY = G4UniformRand()-0.5;
	}
	G4double startPositionZ = (fDetector->GetDetectorSizeZ()) / 2. + 3.;
	fParticleGun->SetParticlePosition(G4ThreeVector(startPositionX, startPositionY, startPositionZ));
	if(fNeutronWavelength != 1.8)
		fParticleGun->SetParticleEnergy(GetNeutronEnergy());

	fParticleGun->GeneratePrimaryVertex(anEvent);
}
void thgemPrimaryGeneratorAction::PrintParticleParameter() const
{
	G4cout << "-------------------------------------------------" << G4endl
		   << " ---> The particle property: " << G4endl
		   << " ---> Name:      " << fParticleGun->GetParticleDefinition()->GetParticleName() << G4endl
		   << " ---> Energy:    " << G4BestUnit(fParticleGun->GetParticleEnergy(), "Energy") << G4endl
		   << " ---> PositionZ: " << G4BestUnit((fParticleGun->GetParticlePosition()).z(), "Length") << G4endl
		   << "-------------------------------------------------" << G4endl;
}

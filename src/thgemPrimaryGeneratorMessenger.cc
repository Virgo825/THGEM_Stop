
#include "thgemPrimaryGeneratorMessenger.hh"
#include "thgemPrimaryGeneratorAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

thgemPrimaryGeneratorMessenger::thgemPrimaryGeneratorMessenger(thgemPrimaryGeneratorAction* Gun)
    : G4UImessenger(), fGenerator(Gun)
{
    fGunDir = new G4UIdirectory("/thgem/generator/");
    fGunDir->SetGuidance("Primary generator control");

    fDefaultCmd = new G4UIcmdWithoutParameter("/thgem/generator/setDefault", this);
    fDefaultCmd->SetGuidance("Set/Reset kinematic defined in PrimaryGenerator.");
    fDefaultCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fNeutronWavelengthCmd = new G4UIcmdWithADouble("/thgem/generator/setWavelength", this);
    fNeutronWavelengthCmd->SetGuidance("Select wavelength of neutron");
    fNeutronWavelengthCmd->SetParameterName("Wavelength", false);
    fNeutronWavelengthCmd->SetRange("Wavelength>0");
    fNeutronWavelengthCmd->AvailableForStates(G4State_Idle);
}
thgemPrimaryGeneratorMessenger::~thgemPrimaryGeneratorMessenger()
{
    delete fGunDir;
    delete fDefaultCmd;
    delete fNeutronWavelengthCmd;
}
void thgemPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == fDefaultCmd)
        fGenerator->SetDefaultKinematic();
    if(command == fNeutronWavelengthCmd)
        fGenerator->SetNeutronWavelength(fNeutronWavelengthCmd->GetNewDoubleValue(newValue));
}
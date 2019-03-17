
#ifndef thgemPrimaryGeneratorMessenger_h
#define thgemPrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class thgemPrimaryGeneratorAction;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

class thgemPrimaryGeneratorMessenger: public G4UImessenger
{
public:
    thgemPrimaryGeneratorMessenger(thgemPrimaryGeneratorAction*);
    ~thgemPrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:
    thgemPrimaryGeneratorAction* fGenerator;

    G4UIdirectory* fGunDir;
    G4UIcmdWithoutParameter* fDefaultCmd;
    G4UIcmdWithADouble* fNeutronWavelengthCmd;
};
#endif





    
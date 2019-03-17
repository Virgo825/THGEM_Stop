

#ifndef thgemDetectorMessenger_h
#define thgemDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class thgemDetectorConstruction;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class thgemDetectorMessenger: public G4UImessenger
{
public:
    thgemDetectorMessenger(thgemDetectorConstruction* );
    virtual ~thgemDetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

private:
    thgemDetectorConstruction* fDetectorConstruction;

    G4UIdirectory* fTHGEMDirectory;
    G4UIdirectory* fDetDirectory;

    G4UIcmdWithAString* fCathodeMatCmd;
    G4UIcmdWithAString* fTransformMatCmd;
    G4UIcmdWithAString* fGasMatCmd;

    G4UIcmdWithAnInteger* fNbOfLayerCmd;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
    G4UIcmdWithADoubleAndUnit* fCathodeThickCmd;
    G4UIcmdWithADoubleAndUnit* fTransformThickCmd;
    G4UIcmdWithADoubleAndUnit* fGasThickCmd;
};
#endif
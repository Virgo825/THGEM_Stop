
#include "thgemDetectorMessenger.hh"
#include "thgemDetectorConstruction.hh"

#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

thgemDetectorMessenger::thgemDetectorMessenger(thgemDetectorConstruction* det)
    : G4UImessenger(),
      fDetectorConstruction(det)
{
    fTHGEMDirectory = new G4UIdirectory("/thgem/");
    fTHGEMDirectory->SetGuidance("UI commands specific to this example.");

    fDetDirectory = new G4UIdirectory("/thgem/detector/");
    fDetDirectory->SetGuidance("Detector construction control.");

    fCathodeMatCmd = new G4UIcmdWithAString("/thgem/detector/setCathodeMaterial", this);
    fCathodeMatCmd->SetGuidance("Select Material of the Cathode.");
    fCathodeMatCmd->SetParameterName("choice", false);
    fCathodeMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fTransformMatCmd = new G4UIcmdWithAString("/thgem/detector/setTransformMaterial", this);
    fTransformMatCmd->SetGuidance("Select Material of the Transform.");
    fTransformMatCmd->SetParameterName("choice", false);
    fTransformMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);    

    fGasMatCmd = new G4UIcmdWithAString("/thgem/detector/setGasMaterial", this);
    fGasMatCmd->SetGuidance("Select Material of the Gas.");
    fGasMatCmd->SetParameterName("choice", false);
    fGasMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fNbOfLayerCmd = new G4UIcmdWithAnInteger("/thgem/detector/setNbOfLayer", this);
    fNbOfLayerCmd->SetGuidance("Set number of layers.");
    fNbOfLayerCmd->SetParameterName("NbLayers", false);
    fNbOfLayerCmd->SetRange("NbLayers>0");
    fNbOfLayerCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/thgem/detector/stepMax", this);
    fStepMaxCmd->SetGuidance("Define a step max.");
    fStepMaxCmd->SetParameterName("stepMax", false);
    fStepMaxCmd->SetUnitCategory("Length");
    fStepMaxCmd->AvailableForStates(G4State_Idle);

    fCathodeThickCmd = new G4UIcmdWithADoubleAndUnit("/thgem/detector/setCathodeThick", this);
    fCathodeThickCmd->SetGuidance("Select thick of the Cathode.");
    fCathodeThickCmd->SetParameterName("Thick", false);
    fCathodeThickCmd->SetUnitCategory("Length");
    fCathodeThickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fTransformThickCmd = new G4UIcmdWithADoubleAndUnit("/thgem/detector/setTransformThick", this);
    fTransformThickCmd->SetGuidance("Select thick of the Transform.");
    fTransformThickCmd->SetParameterName("Thick", false);
    fTransformThickCmd->SetUnitCategory("Length");
    fTransformThickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fGasThickCmd = new G4UIcmdWithADoubleAndUnit("/thgem/detector/setGasThick", this);
    fGasThickCmd->SetGuidance("Select thick of the Gas.");
    fGasThickCmd->SetParameterName("Thick", false);
    fGasThickCmd->SetUnitCategory("Length");
    fGasThickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}
thgemDetectorMessenger::~thgemDetectorMessenger()
{
    delete fTHGEMDirectory;
    delete fDetDirectory;
    delete fCathodeMatCmd;
    delete fTransformMatCmd;
    delete fGasMatCmd;
    delete fNbOfLayerCmd;
    delete fStepMaxCmd;
    delete fCathodeThickCmd;
    delete fTransformThickCmd;
    delete fGasThickCmd;
}
void thgemDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == fCathodeMatCmd) 
        fDetectorConstruction->SetCathodeMaterial(newValue);
    if(command == fTransformMatCmd)
        fDetectorConstruction->SetTransformMaterial(newValue);
    if(command == fGasMatCmd)
        fDetectorConstruction->SetGasMaterial(newValue);
    if(command == fNbOfLayerCmd)
        fDetectorConstruction->SetNbOfLayer(fNbOfLayerCmd->GetNewIntValue(newValue));
    if(command == fStepMaxCmd)
        fDetectorConstruction->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
    if(command == fCathodeThickCmd)
        fDetectorConstruction->SetCathodeThick(fCathodeThickCmd->GetNewDoubleValue(newValue));
    if(command == fTransformThickCmd)
        fDetectorConstruction->SetTransformThick(fTransformThickCmd->GetNewDoubleValue(newValue));
    if(command == fGasThickCmd)
        fDetectorConstruction->SetGasThick(fGasThickCmd->GetNewDoubleValue(newValue));
}
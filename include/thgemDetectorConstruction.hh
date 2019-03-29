
#ifndef thgemDetectorConstruction_h
#define thgemDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4Material;
class G4UserLimits;

class thgemFastSimulationModel;
class thgemDetectorMessenger;

class thgemDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
	thgemDetectorConstruction();
	virtual ~thgemDetectorConstruction();

	virtual G4VPhysicalVolume *Construct();
	virtual void ConstructSDandField();

	// print parameter
	void PrintDetectorParameter() const;

	// set methods
	void SetCathodeMaterial(G4String);
	void SetTransformMaterial(G4String);
	void SetStopMaterial(G4String);
	void SetGasMaterial(G4String);
	void SetMaxStep(G4double);
	void SetCathodeThick(G4double);
	void SetTransformThick(G4double);
	void SetStopThick(G4double);
	void SetGasThick(G4double);
	void SetCheckOverlaps(G4bool);

	// get methods
	G4double GetDetectorSizeZ() { return (fCathodeThick + fTransformThick + fStopThick + fGasThick); }
	G4double GetTransformThick() { return fTransformThick; }
	G4double GetStopThick() { return fStopThick; }
	G4double GetGasThick() { return fGasThick; }
	G4String GetTransformMaterial() { return fTransformMaterial->GetName(); }
	G4String GetStopMaterial() { return fStopMaterial->GetName(); }
	G4String GetGasMaterial() { return fGasMaterial->GetName(); }

	inline const G4VPhysicalVolume *GetPhysiCathode() const { return fPhysiCathode; }
	inline const G4VPhysicalVolume *GetPhysiTransform() const { return fPhysiTransform; }
	inline const G4VPhysicalVolume *GetPhysiStop() const { return fPhysiStop; }
	inline const G4VPhysicalVolume *GetPhysiGas() const { return fPhysiGas; }

  private:
	// methods
	void DefineMaterials();
	G4VPhysicalVolume *DefineVolumes();

	// data members
	G4Material *fCathodeMaterial;
	G4Material *fTransformMaterial;
	G4Material *fStopMaterial;
	G4Material *fGasMaterial;

	G4VPhysicalVolume *fPhysiCathode;
	G4VPhysicalVolume *fPhysiTransform;
	G4VPhysicalVolume *fPhysiStop;
	G4VPhysicalVolume *fPhysiGas;

	G4UserLimits *fStepLimit;

	thgemFastSimulationModel *fFastSimulationModel;
	thgemDetectorMessenger *fMessenger;

	G4double fCathodeThick;
	G4double fTransformThick;
	G4double fStopThick;
	G4double fGasThick;
	G4double fB10Abundance; 
	G4bool fCheckOverlaps; // option to acticate checking of volumes overlaps
};
#endif
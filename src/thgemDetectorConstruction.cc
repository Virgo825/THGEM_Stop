
#include "thgemDetectorConstruction.hh"
#include "thgemDetectorMessenger.hh"
#include "thgemFastSimulationModel.hh"
#include "thgemPhysics.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"
#include "G4Region.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4GlobalMagFieldMessenger.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RegionStore.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

thgemDetectorConstruction::thgemDetectorConstruction()
	: G4VUserDetectorConstruction(),
	  fCathodeMaterial(NULL), fTransformMaterial(NULL), fGasMaterial(NULL),
	  fPhysiCathode(NULL), fPhysiTransform(NULL), fPhysiStop(NULL), fPhysiGas(NULL),
	  fStepLimit(NULL), fFastSimulationModel(NULL), 
	  fCathodeThick(100*um), fTransformThick(2.0*um), fStopThick(1.0*um), fGasThick(2.*mm),
	  fNOfLayers(8), fB10Abundance(96.), fCheckOverlaps(true)
{
	// Define materials
	DefineMaterials();

	fMessenger = new thgemDetectorMessenger(this);
}
thgemDetectorConstruction::~thgemDetectorConstruction()
{
	delete fStepLimit;
	delete fMessenger;
}
G4VPhysicalVolume *thgemDetectorConstruction::Construct()
{
	// Define volumes
	return DefineVolumes();
}
void thgemDetectorConstruction::DefineMaterials()
{
	// G4bool isotopes = false;
	G4String name, symbol;
	G4double a,  density; // a: mass of module; z: mean number of protons.
	G4int iz, n;			// iz: nb of protons in an isotope; n: nb of nucleons in an isotope.

	G4int ncomponents, natoms;
	G4double abundance, fractionmass;
	// G4double temperature, pressure;

	G4Isotope *B10 = new G4Isotope(name = "B10", iz = 5, n = 10, a = 10. * g / mole);
	G4Isotope *B11 = new G4Isotope(name = "B11", iz = 5, n = 11, a = 11. * g / mole);

	G4Element *enrichB_ele = new G4Element(name = "enriched_B_element", symbol = "enrichB_ele", ncomponents = 2);
	enrichB_ele->AddIsotope(B10, abundance = fB10Abundance * perCent);
	enrichB_ele->AddIsotope(B11, abundance = (100. - fB10Abundance) * perCent);

	// material defined using NIST Manager
	G4NistManager *nistManager = G4NistManager::Instance();
	nistManager->FindOrBuildMaterial("G4_AIR");
	nistManager->FindOrBuildMaterial("G4_Galactic");
	nistManager->FindOrBuildMaterial("G4_Al");
	nistManager->FindOrBuildMaterial("G4_Cu");
	nistManager->FindOrBuildMaterial("G4_Ti");
	G4Material *CO2 = nistManager->FindOrBuildMaterial("G4_CARBON_DIOXIDE"); // co2
	G4Material *C4H10 = nistManager->FindOrBuildMaterial("G4_BUTANE");		// c4h10 not ic4h10
	G4Element *H = nistManager->FindOrBuildElement("H");
	G4Element *B = nistManager->FindOrBuildElement("B");
	G4Element *C = nistManager->FindOrBuildElement("C");
	G4Element *N = nistManager->FindOrBuildElement("N");
	G4Element *O = nistManager->FindOrBuildElement("O");
	G4Element *Ne = nistManager->FindOrBuildElement("Ne");
	G4Element *Ar = nistManager->FindOrBuildElement("Ar");

	// enriched B
	G4Material *enrichB = new G4Material("enriched_B", density = 2.34 * CLHEP::g / CLHEP::cm3, ncomponents = 1);
	enrichB->AddElement(enrichB_ele, 1);

	// natural B4C
	G4Material *naturalB4C = new G4Material("natural_B4C", density = 2.52 * CLHEP::g / CLHEP::cm3, ncomponents = 2);
	naturalB4C->AddElement(B, 4);
	naturalB4C->AddElement(C, 1);

	// enriched B4C
	G4Material *enrichB4C = new G4Material("enriched_B4C", density = 2.52 * CLHEP::g / CLHEP::cm3, ncomponents = 2);
	enrichB4C->AddElement(enrichB_ele, natoms = 4);
	enrichB4C->AddElement(C, natoms = 1);

	// 90%Ar+10%CO2
	G4Material *ArCO2_90_10 = new G4Material("ArCO2_90_10", density = 1.8014866 * CLHEP::mg / CLHEP::cm3, ncomponents = 2);
	ArCO2_90_10->AddElement(Ar, fractionmass = 0.90);
	ArCO2_90_10->AddMaterial(CO2, fractionmass = 0.10);

	// 97%Ne+3%C4H10
	G4Material *NeC4H10_97_3 = new G4Material("NeC4H10_97_3", density = 0.95455357 * CLHEP::mg / CLHEP::cm3, ncomponents = 2);
	NeC4H10_97_3->AddElement(Ne, fractionmass = 0.97);
	NeC4H10_97_3->AddMaterial(C4H10, fractionmass = 0.03);

	// Kapton
	G4Material *Kapton = new G4Material("Kapton", density = 1.413 * CLHEP::g / CLHEP::cm3, ncomponents = 4);
	Kapton->AddElement(O, natoms = 5);
	Kapton->AddElement(C, natoms = 22);
	Kapton->AddElement(N, natoms = 2);
	Kapton->AddElement(H, natoms = 10);

	// Print materials
	// G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
G4VPhysicalVolume *thgemDetectorConstruction::DefineVolumes()
{
	// Geometry parameters
	const G4double SizeXY = 20 * mm;
	G4double cathodeSizeZ = fCathodeThick;
	G4double transformSizeZ = fTransformThick;
	G4double stopSizeZ = fStopThick;
	G4double gasSizeZ = fGasThick;
	G4double layerSizeZ = cathodeSizeZ + transformSizeZ + stopSizeZ + gasSizeZ;
	G4double detectorSizeZ = layerSizeZ * fNOfLayers;
	G4double worldSizeXZ = 1.2 * SizeXY;
	G4double worldSizeY = 6 * detectorSizeZ;

	// Set drift thick in garfield++.
	thgemPhysics *physics = thgemPhysics::GetInstance();
	physics->SetDriftThick(detectorSizeZ);

	// Get materials
	G4Material *defaultMaterial = G4Material::GetMaterial("G4_AIR"); // Neutron react with nitrogen and oxygen.
	fCathodeMaterial = G4Material::GetMaterial("G4_Al");
	fTransformMaterial = G4Material::GetMaterial("enriched_B4C");
	fStopMaterial = G4Material::GetMaterial("G4_Al");
	fGasMaterial = G4Material::GetMaterial("ArCO2_90_10");

	// Cleanup old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	// World
	G4VSolid *solidWorld = new G4Box("World",												  // its name
									 0.5 * worldSizeXZ, 0.5 * worldSizeY, 0.5 * worldSizeXZ); // its size
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,							  // its solid
													  defaultMaterial,						  // its material
													  "World");								  //its name
	G4VPhysicalVolume *physiWorld = new G4PVPlacement(0,									  // no rotation
													  G4ThreeVector(),						  // at (0,0,0)
													  logicWorld,							  // its logical volume
													  "World",								  // its name
													  0,									  // its mother name
													  false,								  // no boolean operation
													  0,									  // copy number
													  fCheckOverlaps);						  // checking overlaps
	// Detector
	G4ThreeVector positionDetector = G4ThreeVector(0., detectorSizeZ/2., 0.);
	G4RotationMatrix *rotX = new G4RotationMatrix();
	rotX->rotateX(90. * CLHEP::degree);
	G4VSolid *solidDetector = new G4Box("Detector", 0.5 * SizeXY, 0.5 * SizeXY, 0.5 * detectorSizeZ);
	G4LogicalVolume *logicDetector = new G4LogicalVolume(solidDetector, defaultMaterial, "Detector");
	new G4PVPlacement(rotX, positionDetector, logicDetector, "Detector", logicWorld, false, 0, fCheckOverlaps);

	// Layer
	G4ThreeVector positionLayer = G4ThreeVector(0., 0., 0.);
	G4VSolid *solidLayer = new G4Box("Layer", 0.5 * SizeXY, 0.5 * SizeXY, 0.5 * layerSizeZ);
	G4LogicalVolume *logicLayer = new G4LogicalVolume(solidLayer, defaultMaterial, "Layer");
	if(1 == fNOfLayers)
		new G4PVPlacement(0, positionLayer, logicLayer, "Layer", logicDetector, false, 0, fCheckOverlaps);
	else
		new G4PVReplica("Layer", logicLayer, logicDetector, kZAxis, fNOfLayers, layerSizeZ);

	// Cathode
	G4ThreeVector positionCathode = G4ThreeVector(0., 0., layerSizeZ/2.-cathodeSizeZ/2.);
	G4VSolid *solidCathode = new G4Box("Cathode", 0.5 * SizeXY, 0.5 * SizeXY, 0.5 * cathodeSizeZ);
	G4LogicalVolume *logicCathode = new G4LogicalVolume(solidCathode, fCathodeMaterial, "Cathode");
	fPhysiCathode = new G4PVPlacement(0, positionCathode, logicCathode, "Cathode", logicLayer, false, 0, fCheckOverlaps);

	// Transform
	G4ThreeVector positionTransform = G4ThreeVector(0., 0., layerSizeZ/2.-cathodeSizeZ-transformSizeZ/2.);
	G4VSolid *solidTransform = new G4Box("Transform", 0.5 * SizeXY, 0.5 * SizeXY, 0.5 * transformSizeZ);
	G4LogicalVolume *logicTransform = new G4LogicalVolume(solidTransform, fTransformMaterial, "Transform");
	fPhysiTransform= new G4PVPlacement(0, positionTransform, logicTransform, "Transform", logicLayer, false, 0, fCheckOverlaps);

	// Stop
	if(fStopThick != 0)
	{
		G4ThreeVector positionStop = G4ThreeVector(0., 0., layerSizeZ/2.-cathodeSizeZ-transformSizeZ-stopSizeZ/2.);
		G4VSolid *solidStop = new G4Box("Stop", 0.5 * SizeXY, 0.5 * SizeXY, 0.5 * stopSizeZ);
		G4LogicalVolume *logicStop = new G4LogicalVolume(solidStop, fStopMaterial, "Stop");
		fPhysiStop= new G4PVPlacement(0, positionStop, logicStop, "Stop", logicLayer, false, 0, fCheckOverlaps);
		
		G4VisAttributes *VisAttYellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
		logicStop->SetVisAttributes(VisAttYellow);
	}

	// Gas
	G4ThreeVector positionGas = G4ThreeVector(0., 0., -layerSizeZ/2.+gasSizeZ/2.);
	G4VSolid *solidGas = new G4Box("Gas", 0.5 * SizeXY, 0.5 * SizeXY, 0.5 * gasSizeZ);
	G4LogicalVolume *logicGas = new G4LogicalVolume(solidGas, fGasMaterial, "Gas");
	fPhysiGas = new G4PVPlacement(0, positionGas, logicGas, "Gas", logicLayer, false, 0, fCheckOverlaps);

	// Visualization attributes
	G4VisAttributes *VisAttBlue = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	G4VisAttributes *VisAttGreen = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
	G4VisAttributes *VisAttRed = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
	G4VisAttributes *VisAttWhite = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));

	logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
	logicDetector->SetVisAttributes(VisAttWhite);
	logicCathode->SetVisAttributes(VisAttRed);
	logicTransform->SetVisAttributes(VisAttGreen);
	logicGas->SetVisAttributes(VisAttBlue);

	G4Region* regionGas = new G4Region("RegionGas");
	// logicGas->SetRegion(regionGas);
	regionGas->AddRootLogicalVolume(logicGas);

	fFastSimulationModel = new thgemFastSimulationModel("thgemG4FastSimulationModel", regionGas);
	fFastSimulationModel->WriteGeometryToGDML(fPhysiGas);

	// Always return the physical World
	return physiWorld;
}
void thgemDetectorConstruction::ConstructSDandField()
{
}
void thgemDetectorConstruction::PrintDetectorParameter() const
{
	G4cout << "------------------------------------------" << G4endl
		   << " ---> The detector component: " << G4endl
		   << " ---> " << fCathodeMaterial->GetName() << " : " << G4BestUnit(fCathodeThick, "Length") << G4endl
		   << " ---> " << fTransformMaterial->GetName() << " : " << G4BestUnit(fTransformThick, "Length") << G4endl
		   << " ---> " << fStopMaterial->GetName() << " : " << G4BestUnit(fStopThick, "Length") << G4endl 
		   << " ---> " << fGasMaterial->GetName() << " : " << G4BestUnit(fGasThick, "Length") << G4endl 
		   << "------------------------------------------" << G4endl;
}
void thgemDetectorConstruction::SetCathodeMaterial(G4String materialName)
{
	G4Material* newMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialName);
	if(newMaterial)
	{
		fCathodeMaterial = newMaterial;
		fPhysiCathode->GetLogicalVolume()->SetMaterial(fCathodeMaterial);
		G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	}else
		G4cout << "\n---> WARNING from SetCathodeMaterial :" << materialName << " not found." << G4endl;
}
void thgemDetectorConstruction::SetTransformMaterial(G4String materialName)
{
	G4Material* newMaterial = G4Material::GetMaterial(materialName);
	if(fTransformMaterial != newMaterial)
	{
		if(newMaterial)
		{
			fTransformMaterial = newMaterial;
			fPhysiTransform->GetLogicalVolume()->SetMaterial(fTransformMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
		}else
			G4cout << "\n---> WARNING from SetTransformMaterial :" << materialName << " not found." << G4endl;
	}
}
void thgemDetectorConstruction::SetStopMaterial(G4String materialName)
{
	G4Material* newMaterial = G4Material::GetMaterial(materialName);
	if(fStopMaterial != newMaterial)
	{
		if(newMaterial)
		{
			fStopMaterial = newMaterial;
			fPhysiStop->GetLogicalVolume()->SetMaterial(fStopMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
		}else
			G4cout << "\n---> WARNING from SetStopMaterial :" << materialName << " not found." << G4endl;
	}
}
void thgemDetectorConstruction::SetGasMaterial(G4String materialName)
{
	G4Material* newMaterial = G4Material::GetMaterial(materialName);
	if(fGasMaterial != newMaterial)
	{
		if(newMaterial)
		{
			fGasMaterial = newMaterial;
			fPhysiGas->GetLogicalVolume()->SetMaterial(fGasMaterial);
			G4RunManager::GetRunManager()->PhysicsHasBeenModified();
		}else
			G4cout << "\n---> WARNING from SetGasMaterial :" << materialName << " not found." << G4endl;
	}
}
void thgemDetectorConstruction::SetMaxStep(G4double maxStep)
{
	if((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}
// void thgemDetectorConstruction::SetCathodeThick(G4double val)
// {
// 	G4Box* newBox = dynamic_cast<G4Box*>(fPhysiCathode->GetLogicalVolume()->GetSolid());
// 	if(newBox)
// 	{
// 		fCathodeThick = val;
// 		newBox->SetZHalfLength(fCathodeThick/2.);
// 		G4RunManager::GetRunManager()->GeometryHasBeenModified();
// 	}		
// 	else
// 		G4cout << "Cathode volume of box not found." << G4endl;
// }
// void thgemDetectorConstruction::SetTransformThick(G4double val)
// {
// 	G4Box* newBox = dynamic_cast<G4Box*>(fPhysiTransform->GetLogicalVolume()->GetSolid());
// 	if(newBox)
// 	{
// 		fTransformThick = val;
// 		newBox->SetZHalfLength(fTransformThick/2.);
// 		G4RunManager::GetRunManager()->GeometryHasBeenModified();
// 	}		
// 	else
// 		G4cout << "Transform volume of box not found." << G4endl;
// }
// void thgemDetectorConstruction::SetGasThick(G4double val)
// {
// 	G4Box* newBox = dynamic_cast<G4Box*>(fPhysiGas->GetLogicalVolume()->GetSolid());
// 	if(newBox)
// 	{
// 		fGasThick = val;
// 		newBox->SetZHalfLength(fGasThick/2.);
// 		G4RunManager::GetRunManager()->GeometryHasBeenModified();
// 	}		
// 	else
// 		G4cout << "Transform volume of box not found." << G4endl;
// }
void thgemDetectorConstruction::SetCathodeThick(G4double val)
{
	fCathodeThick = val;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void thgemDetectorConstruction::SetTransformThick(G4double val)
{
	fTransformThick = val;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void thgemDetectorConstruction::SetStopThick(G4double val)
{
	fStopThick = val;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void thgemDetectorConstruction::SetGasThick(G4double val)
{
	fGasThick = val;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}
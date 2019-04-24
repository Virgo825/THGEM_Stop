
#include "thgemPhysics.hh"
#include "thgemPhysicsMessenger.hh"
#include "thgemAnalysis.hh"
#include "thgemDetectorConstruction.hh"

#include "TGeoManager.h"
#include "TGeoBBox.h"

thgemPhysics *thgemPhysics::fPhysics = 0;
thgemPhysics *thgemPhysics::GetInstance()
{
    if(!fPhysics) fPhysics = new thgemPhysics();
    
    return fPhysics;
}
void thgemPhysics::Dispose()
{
    delete fPhysics;
    fPhysics = 0;
}
thgemPhysics::thgemPhysics()
{

	fMapParticlesEnergyGeant4 = new MapParticlesEnergy();
	fMapParticlesEnergyGarfield = new MapParticlesEnergy();
	fGeoManager = 0;

    fMediumMagboltz = 0;
	fTHGEM = 0;
	fSensor = 0;
	fTrackHeed = 0;
	fDrift = 0;
	fAvalanche = 0;

	fSecondaryParticles = new std::vector<GarfieldParticle *>();

	createSecondariesInGeant4 = false;
	// fIonizationModel = "PAIPhot";
	fIonizationModel = "PAI";
}
thgemPhysics::~thgemPhysics()
{
	delete fMapParticlesEnergyGeant4;
	delete fMapParticlesEnergyGarfield;
	delete fMediumMagboltz;
	delete fTHGEM;
	delete fSensor;
	delete fTrackHeed;
	delete fDrift;
	delete fAvalanche;

	DeleteSecondaryParticles();

	std::cout << "Deconstructor thgemPhysics" << std::endl;
}
std::string thgemPhysics::GetIonizationModel()
{
    return fIonizationModel;
}
void thgemPhysics::SetIonizationModel(std::string model, bool useDefaults)
{
	if (model != "PAIPhot" && model != "PAI" && model != "Heed")
	{
		std::cout << "Unknown ionization model" << model << std::endl;
		std::cout << "Using PAIPhot as default model!" << std::endl;
		model = "PAI";
	}
	fIonizationModel = model;
	if (fIonizationModel == "PAIPhot" || fIonizationModel == "PAI")
	{
		if (useDefaults == true)
		{
			// Particle types and energies for which the G4FastSimulationModel with Garfield++ is valid.
			this->AddParticleName("e-", 1e-6, 1e-3, "garfield");
			this->AddParticleName("gamma", 1e-6, 1e+8, "garfield");
			// Particle types and energies for which the PAI or PAIPhot model is valid.
			this->AddParticleName("e-", 0, 1e+8, "geant4");
			this->AddParticleName("e+", 0, 1e+8, "geant4");
			this->AddParticleName("mu-", 0, 1e+8, "geant4");
			this->AddParticleName("mu+", 0, 1e+8, "geant4");
			this->AddParticleName("proton", 0, 1e+8, "geant4");
			this->AddParticleName("pi+", 0, 1e+8, "geant4");
			this->AddParticleName("pi-", 0, 1e+8, "geant4");
			this->AddParticleName("alpha", 0, 1e+8, "geant4");
			this->AddParticleName("Li7", 0, 1e+8, "geant4");
			this->AddParticleName("He3", 0, 1e+8, "geant4");
			this->AddParticleName("GenericIon", 0, 1e+8, "geant4");
		}
	}
	else if (fIonizationModel == "Heed")
	{
		if (useDefaults == true)
		{
			// Particle types and energies for which the G4FastSimulationModel with Garfield++ is valid.
			this->AddParticleName("gamma", 1e-6, 1e+8, "garfield");
			this->AddParticleName("e-", 6e-2, 1e+7, "garfield");
			this->AddParticleName("e+", 6e-2, 1e+7, "garfield");
			this->AddParticleName("mu-", 1e+1, 1e+8, "garfield");
			this->AddParticleName("mu+", 1e+1, 1e+8, "garfield");
			this->AddParticleName("pi+", 2e+1, 1e+8, "garfield");
			this->AddParticleName("pi-", 2e+1, 1e+8, "garfield");
			this->AddParticleName("kano-", 1e+1, 1e+8, "garfield");
			this->AddParticleName("kano+", 1e+1, 1e+8, "garfield");
			this->AddParticleName("proton", 9.e+1, 1e+8, "garfield");
			this->AddParticleName("anti_proton", 9.e+1, 1e+8, "garfield");
			this->AddParticleName("deuteron", 2.e+2, 1e+8, "garfield");
			this->AddParticleName("alpha", 4.e+2, 1e+8, "garfield");
			this->AddParticleName("Li7", 4.e+2, 1e+8, "garfield");
		}
	}
}
void thgemPhysics::AddParticleName(const std::string particleName, double ekin_min_MeV, double ekin_max_MeV, std::string program)
{
	if (ekin_min_MeV >= ekin_max_MeV)
	{
		std::cout << "Ekin_min=" << ekin_min_MeV << " keV is larger than Ekin_max=" << ekin_max_MeV << " keV" << std::endl;
		return;
	}
	if (program == "garfield")
	{
		std::cout << "Garfield model (Heed) is applicable for G4Particle " << particleName << " between " << ekin_min_MeV << " MeV and " << ekin_max_MeV << " MeV" << std::endl;
		fMapParticlesEnergyGarfield->insert(std::make_pair(particleName, std::make_pair(ekin_min_MeV, ekin_max_MeV)));
	}
	else
	{
		std::cout << fIonizationModel << " is applicable for G4Particle " << particleName << " between " << ekin_min_MeV << " MeV and " << ekin_max_MeV << " MeV" << std::endl;
		fMapParticlesEnergyGeant4->insert(std::make_pair(particleName, std::make_pair(ekin_min_MeV, ekin_max_MeV)));
	}
}
bool thgemPhysics::FindParticleName(std::string name, std::string program)
{
	MapParticlesEnergy::iterator it;
	if (program == "garfield")
	{
		it = fMapParticlesEnergyGarfield->find(name);
		if (it != fMapParticlesEnergyGarfield->end()) return true;

		return false;
	}
	else
	{
		it = fMapParticlesEnergyGeant4->find(name);
		if (it != fMapParticlesEnergyGeant4->end()) return true;

		return false;
	}
}
bool thgemPhysics::FindParticleNameEnergy(std::string name, double ekin_MeV, std::string program)
{
	MapParticlesEnergy::iterator it;
	if (program == "garfield")
	{
		it = fMapParticlesEnergyGarfield->find(name);
		if (it != fMapParticlesEnergyGarfield->end())
		{
			EnergyRange_MeV range = it->second;
			if (range.first <= ekin_MeV && range.second >= ekin_MeV) return true;
		}
		return false;
	}
	else
	{
		it = fMapParticlesEnergyGeant4->find(name);
		if (it != fMapParticlesEnergyGeant4->end())
		{
			EnergyRange_MeV range = it->second;
			if (range.first <= ekin_MeV && range.second >= ekin_MeV) return true;
		}
		return false;
	}
}
double thgemPhysics::GetMinEnergyMeVParticle(std::string name, std::string program)
{
	MapParticlesEnergy::iterator it;
	if (program == "garfield")
	{
		it = fMapParticlesEnergyGarfield->find(name);
		if (it != fMapParticlesEnergyGarfield->end())
		{
			EnergyRange_MeV range = it->second;
			return range.first;
		}
		return false;
	}
	else
	{
		it = fMapParticlesEnergyGeant4->find(name);
		if (it != fMapParticlesEnergyGeant4->end())
		{
			EnergyRange_MeV range = it->second;
			return range.first;
		}
	}
	return -1;
}
double thgemPhysics::GetMaxEnergyMeVParticle(std::string name, std::string program)
{
	MapParticlesEnergy::iterator it;
	if (program == "garfield")
	{
		it = fMapParticlesEnergyGarfield->find(name);
		if (it != fMapParticlesEnergyGarfield->end())
		{
			EnergyRange_MeV range = it->second;
			return range.second;
		}
	}
	else
	{
		it = fMapParticlesEnergyGeant4->find(name);
		if (it != fMapParticlesEnergyGeant4->end())
		{
			EnergyRange_MeV range = it->second;
			return range.second;
		}
	}
	return -1;   
}
void thgemPhysics::InitializePhysics()
{
	fMediumMagboltz = new Garfield::MediumMagboltz();
	fMediumMagboltz->SetComposition("ar", 90., "co2", 10.);
	fMediumMagboltz->SetTemperature(293.15);
	fMediumMagboltz->SetPressure(760.);
	fMediumMagboltz->EnableDebugging();
	fMediumMagboltz->Initialise(true);
	fMediumMagboltz->DisableDebugging();
	const double rPenning = 0.51;
	const double lambdaPenning = 0.;
	fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
	fMediumMagboltz->LoadGasFile("ar_90_co2_10_1000mbar.gas");

    const double gasSizeXZ = 10.;
    const double VGemUp = -1400.;
    const double Vdrift = VGemUp - fDriftThick * 1000.;

    fGeometrySimple = new Garfield::GeometrySimple();
    fSolidBox = new Garfield::SolidBox(0., fDriftThick/2., 0., gasSizeXZ/2., fDriftThick/2., gasSizeXZ/2.);
    fGeometrySimple->AddSolid(fSolidBox, fMediumMagboltz);

    fComponentAnalyticField = new Garfield::ComponentAnalyticField();
    fComponentAnalyticField->SetGeometry(fGeometrySimple);
    fComponentAnalyticField->AddPlaneY(fDriftThick, Vdrift, "Cathode");
    fComponentAnalyticField->AddPlaneY(0., VGemUp, "GemUp");

    // const double pitch = 0.06;                       // Distance between holes, in cm
    // const double metal = 0.0018;                     // Thickness of the metal layer, in cm
    // const double ceramics = 0.0168;                  // Thickness of the ceramics layer, in cm
    // const double dia = 0.02;                         // Outer diameter of hole, in cm
    // const double rim = 0.008;                        // Rim diameter, in cm
    // const double fDriftThick = 0.4;                        // Thickness of drift region
    // const double induct = 0.2;                       // Thickness of induct region
    // const double thgem_thick = metal * 2 + ceramics; // Thickness of THGEM, in cm

    // fTHGEM = new Garfield::ComponentAnsys123();
	// fTHGEM->Initialise("../ansys/ELIST.lis", "../ansys/NLIST.lis", "../ansys/MPLIST.lis", "../ansys/PRNSOL.lis", "mm");
	// fTHGEM->EnableMirrorPeriodicityX();
	// fTHGEM->EnableMirrorPeriodicityY();
	// fTHGEM->PrintRange();

	// const unsigned int nMaterials = fTHGEM->GetNumberOfMaterials();
	// for (unsigned int i = 0; i < nMaterials; ++i)
	// {
	// 	const double eps = fTHGEM->GetPermittivity(i);
	// 	if (eps == 1.)
	// 		fTHGEM->SetMedium(i, fMediumMagboltz);
	// }
	// fTHGEM->PrintMaterials();

	fSensor = new Garfield::Sensor();
	fDrift = new Garfield::AvalancheMC();
	fAvalanche = new Garfield::AvalancheMicroscopic();

	// fSensor->AddComponent(fTHGEM);
	// fSensor->SetArea(-84 * pitch, -84 * pitch, -(induct + thgem_thick / 2.),
	// 				  84 * pitch,  84 * pitch,   drift + thgem_thick / 2.);
    
    fSensor->AddComponent(fComponentAnalyticField);

	fDrift->SetSensor(fSensor);
	fAvalanche->SetSensor(fSensor);

	fTrackHeed = new Garfield::TrackHeed();
	fTrackHeed->EnableDebugging();
	fTrackHeed->SetSensor(fSensor);
	fTrackHeed->EnableDeltaElectronTransport();
}
void thgemPhysics::DoIt(std::string particleName, double ekin_MeV, double time, double x_cm, double y_cm, double z_cm, double dx, double dy, double dz)
{
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	fEnergyDeposit = 0;
	DeleteSecondaryParticles();

	// const double metal = 0.0018;					 // Thickness of the metal layer, in cm
	// const double ceramics = 0.0168;					 // Thickness of the ceramics layer, in cm
	// const double fDriftThick = 0.4;						 // Thickness of drift region
	// const double induct = 0.2;						 // Thickness of induct region
	// const double thgem_thick = metal * 2 + ceramics; // Thickness of THGEM, in cm

	double eKin_eV = ekin_MeV * 1.e+6;

	double xc = 0., yc = 0., zc = 0., tc = 0.;

    int nc = 0;        // Number of electrons produced in a collision
    double ec = 0.;    // Energy loss in a collision
    double extra = 0.; // Dummy variable (not used at present)

    if (fIonizationModel != "Heed" || particleName == "gamma")
	{
		if (particleName == "gamma")
		{
			if(y_cm >= 0. && y_cm <= fDriftThick)
				fTrackHeed->TransportPhoton(x_cm, y_cm, z_cm, time, eKin_eV, dx, dy, dz, nc);
		}
		else
		{
            fTrackHeed->TransportDeltaElectron(x_cm, y_cm, z_cm, time, eKin_eV, dx, dy, dz, nc);
            fEnergyDeposit = eKin_eV;
		}        
        // std::cout << particleName << std::setw(15) << x_cm << std::setw(15) << y_cm << std::setw(15) << z_cm << std::setw(15) << eKin_eV << std::setw(10) << nc << std::endl;
        for (int cl = 0; cl < nc; cl++)
		{
			double xe, ye, ze, te, ee, dxe, dye, dze;
			fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
			if (ye >= 0. && ye <= fDriftThick)
			{
                fElectronNumber++;
                fElectronPosSumX += xe;
                fElectronPosSumY += ye;
                fElectronPosSumZ += ze;

                // analysisManager->FillH3(1, xe*10., ze*10., ye*10.);
                analysisManager->FillNtupleDColumn(1, 0, xe);
                analysisManager->FillNtupleDColumn(1, 1, ye);
                analysisManager->FillNtupleDColumn(1, 2, ze);
                analysisManager->FillNtupleDColumn(1, 3, te);
                analysisManager->AddNtupleRow(1);

				if (particleName == "gamma")
				{
					fEnergyDeposit += fTrackHeed->GetW();
				}
				if (createSecondariesInGeant4)
				{
					double newTime = te;
					if (newTime < time)	newTime += time;
					fSecondaryParticles->push_back(new GarfieldParticle("e-", ee, newTime, xe, ye, ze, dxe, dye, dze));
				}
			}
		}
	}
	else
	{
		fTrackHeed->SetParticle(particleName);
		fTrackHeed->SetKineticEnergy(eKin_eV);
		fTrackHeed->NewTrack(x_cm, y_cm, z_cm, time, dx, dy, dz);
		while (fTrackHeed->GetCluster(xc, yc, zc, tc, nc, ec, extra))
		{
            if (yc >= 0. && yc <= fDriftThick)
            {
                fEnergyDeposit += ec;
                for (int cl = 0; cl < nc; cl++)
                {
                    double xe, ye, ze, te, ee, dxe, dye, dze;
                    fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
                    if (createSecondariesInGeant4)
                    {
                        double newTime = te;
                        if (newTime < time) newTime += time;
                        fSecondaryParticles->push_back(new GarfieldParticle("e-", ee, newTime, xe, ye, ze, dxe, dye, dze));
                    }                
                }
            }
		}
	}
}
std::vector<GarfieldParticle *> *thgemPhysics::GetSecondaryParticles()
{
	return fSecondaryParticles;
}
void thgemPhysics::DeleteSecondaryParticles()
{
	if (!fSecondaryParticles->empty())
		fSecondaryParticles->erase(fSecondaryParticles->begin(), fSecondaryParticles->end());
}
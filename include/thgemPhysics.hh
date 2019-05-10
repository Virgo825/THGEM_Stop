#ifndef thgemPhysics_h
#define thgemPhysics_h 1

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "Garfield/SolidBox.hh"
#include "Garfield/GeometrySimple.hh"
#include "Garfield/GeometryRoot.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/ComponentAnsys123.hh"
#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/TrackHeed.hh"
#include "Garfield/TrackSimple.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/AvalancheMicroscopic.hh"

typedef std::pair<double, double> EnergyRange_MeV;
typedef std::map<const std::string, EnergyRange_MeV> MapParticlesEnergy;

class GarfieldParticle
{
public:
    GarfieldParticle(std::string particleName, double ekin_eV, double time, double x_cm, double y_cm, double z_cm, double dx, double dy, double dz)
        : fParticleName(particleName), fEkin_MeV(ekin_eV / 1000000.), fTime(time), fx_mm(x_cm*10), fy_mm(y_cm*10), fz_mm(z_cm*10), fdx(dx), fdy(dy), fdz(dz)
    {}
    ~GarfieldParticle()
    {}
    std::string GetParticleName() {return fParticleName;}
    double GetX_mm() {return fx_mm;}
    double GetY_mm() {return fy_mm;}
    double GetZ_mm() {return fz_mm;}
    double GetEkin_MeV() {return fEkin_MeV;}
    double GetTime() {return fTime;}
    double GetDX() {return fdx;}
    double GetDY() {return fdy;}
    double GetDZ() {return fdz;}

private:
    std::string fParticleName;
    double fEkin_MeV, fTime, fx_mm, fy_mm, fz_mm, fdx, fdy, fdz;
};

class thgemPhysics
{
public:
    static thgemPhysics  *GetInstance();
    static void Dispose();

    std::string GetIonizationModel();
    void SetIonizationModel(std::string, bool useDefaults = true);
    void AddParticleName(const std::string, double, double, std::string);
    bool FindParticleName(std::string, std::string program = "garfield");
    bool FindParticleNameEnergy(std::string, double, std::string program = "garfield");
    double GetMinEnergyMeVParticle(std::string, std::string program = "garfield");
    double GetMaxEnergyMeVParticle(std::string, std::string program = "garfield");
    void InitializePhysics();
    void DoIt(std::string, double, double, double, double, double, double, double, double);
    std::vector<GarfieldParticle *> *GetSecondaryParticles();
    void DeleteSecondaryParticles();
    void SetDriftThick(double l) {fDriftThick = l/10.;}

    inline void EnableCreateSecondariesInGeant4(bool flag) {createSecondariesInGeant4 = flag;}
    inline bool GetCreateSecondariesInGeant4() {return createSecondariesInGeant4;}
    inline double GetEnergyDeposit_MeV() {return fEnergyDeposit / 1000000.;}
    inline int GetElectronNumber() {return fElectronNumber;}
    inline double GetElectronCenterX() {return fElectronPosSumX/fElectronNumber;}
    inline double GetElectronCenterY() {return fElectronPosSumY/fElectronNumber;}
    inline double GetElectronCenterZ() {return fElectronPosSumZ/fElectronNumber;}
    inline void Clear() 
    {
        fEnergyDeposit = 0.;
        fElectronPosSumX = 0.;
        fElectronPosSumY = 0.;
        fElectronPosSumZ = 0.;
        fElectronNumber = 0;
    }

private:
    thgemPhysics();
    ~thgemPhysics();

    std::string fIonizationModel;
    static thgemPhysics *fPhysics;
    

    MapParticlesEnergy *fMapParticlesEnergyGeant4;
    MapParticlesEnergy *fMapParticlesEnergyGarfield;
    
    TGeoManager *fGeoManager;

    Garfield::MediumMagboltz *fMediumMagboltz;
    Garfield::GeometrySimple *fGeometrySimple;
    Garfield::SolidBox *fSolidBox;
    Garfield::ComponentAnalyticField *fComponentAnalyticField;
    Garfield::ComponentAnsys123 *fTHGEM;
    Garfield::Sensor *fSensor;
    Garfield::TrackHeed *fTrackHeed;
    Garfield::AvalancheMC *fDrift;
    Garfield::AvalancheMicroscopic *fAvalanche;

    std::vector<GarfieldParticle *> *fSecondaryParticles;

    bool createSecondariesInGeant4;
    double fDriftThick;
    double fEnergyDeposit;
    double fElectronPosSumX;
    double fElectronPosSumY;
    double fElectronPosSumZ;
    int fElectronNumber;
};
#endif

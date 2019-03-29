#ifndef thgemFastSimulationModel_h
#define thgemFastSimulationModel_h 1

#include "G4VFastSimulationModel.hh"
#include "thgemPhysics.hh"

class G4VPhysicalVolum;

class thgemFastSimulationModel : public G4VFastSimulationModel
{
public:
    thgemFastSimulationModel(G4String, G4Region*);
    thgemFastSimulationModel(G4String);
    ~thgemFastSimulationModel();

    // void SetPhysics(thgemPhysics *);
    void WriteGeometryToGDML(G4VPhysicalVolume *);

    virtual G4bool IsApplicable(const G4ParticleDefinition&);
    virtual G4bool ModelTrigger(const G4FastTrack&);
    virtual void DoIt(const G4FastTrack&, G4FastStep&);

private:
    thgemPhysics *fPhysics;
};
#endif

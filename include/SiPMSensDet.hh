#include "G4VSensitiveDetector.hh"

class SiPMSensitiveDetector : public G4VSensitiveDetector {

public:

  SiPMSensitiveDetector(const G4String& name);
  virtual ~SiPMSensitiveDetector();


  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);

};

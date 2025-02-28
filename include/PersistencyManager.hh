#ifndef PERSISTENCY_MANAGER_H
#define PERSISTENCY_MANAGER_H

#include <G4VPersistencyManager.hh>
#include "G4ThreeVector.hh"
#include <vector>

#ifdef With_Opticks
#include "SEvt.hh"
#include "sphoton.h"
#endif

struct OpticalHit{
  G4String name;
  G4double time;
  G4ThreeVector position;
  G4ThreeVector mom;
  G4ThreeVector polarization;
  G4double wavelength;
};


class PersistencyManager: public G4VPersistencyManager
{
public:
  PersistencyManager();
  ~PersistencyManager();

  void CollectOpticksHits();
  
#ifdef With_Opticks  
  std::vector<sphoton> AllOpticksHits;
  G4int OpticksHitCollectCount;
#endif
};


#endif

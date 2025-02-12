#include <G4VPersistencyManager.hh>
#include <G4RunManager.hh>
#include <G4Run.hh>
#include "PersistencyManager.hh"

#ifdef With_Opticks
#include "SEvt.hh"
#include "G4CXOpticks.hh"
#include "OpticksPhoton.hh"
#include "OpticksGenstep.h"
#endif


PersistencyManager::PersistencyManager()
{

#ifdef With_Opticks
  AllOpticksHits=std::vector<sphoton>();
  OpticksHitCollectCount=0;
#endif
}

PersistencyManager::~PersistencyManager()
{
 
}


void PersistencyManager::CollectOpticksHits(){
  
#ifdef With_Opticks
    SEvt* sev             = SEvt::Get_EGPU();
    auto run= G4RunManager::GetRunManager();
    G4int eventID=run->GetCurrentEvent()->GetEventID();

    //First Lets Collect Quickly whatever we get from GPU

    std::cout<< "Collecting Opticks Hits "<<std::endl;
    
    G4int ngenstep=SEvt::GetNumGenstepFromGenstep(0);                                         
    G4int nphotons=SEvt::GetNumPhotonCollected(0);
    G4int num_hits=SEvt::GetNumHit(0);
    std::cout << "Saving the hits for EventID "<<eventID  <<std::endl;
    G4cout << "Number of Steps Generated " <<ngenstep << G4endl;
    G4cout << "Number of Photons Generated " <<nphotons << G4endl;
    G4cout << "Number of Hits Opticks  " <<num_hits<< G4endl;                                       

    std::vector<sphoton> hits;
    sphoton::Get(hits, sev->getHit());

    // Assign the event quickly or just keep copy it

    if(OpticksHitCollectCount==0) AllOpticksHits=hits;
    else std::copy(hits.begin(),hits.end(),std::back_inserter(AllOpticksHits));
    // Dnt care about the speed during debuging mode, we want to improve memory

    //if(store_steps_) StoreOpticksSteps();

    //Count how many times we collected

    OpticksHitCollectCount++;
    // clear the hits
    G4CXOpticks::Get()->reset(eventID);
    
#endif

}

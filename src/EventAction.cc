//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
#include "PersistencyManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4TrajectoryContainer.hh"
#include <G4Trajectory.hh>

#ifdef With_Opticks
    #  include "SEvt.hh"
    #  include "G4CXOpticks.hh"
namespace {G4Mutex opticks_mt =G4MUTEX_INITIALIZER;}
#endif

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
  fEdep = 0.;
  
  PersistencyManager* pm = dynamic_cast<PersistencyManager*>
    (G4VPersistencyManager::GetPersistencyManager());
    
  G4PrimaryVertex* pVtx;
  pVtx = event->GetPrimaryVertex();

  G4cout << "_____________Begin of event_____________" << G4endl;
  G4cout << "______________" << event->GetEventID() << "______________" << G4endl; 
  G4cout << "______________________________________" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
void EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  //  fRunAction->AddEdep(fEdep);

  PersistencyManager* pm = dynamic_cast<PersistencyManager*>
    (G4VPersistencyManager::GetPersistencyManager());
  
#ifdef With_Opticks
  G4AutoLock lock(&opticks_mt);
  G4CXOpticks * g4cx=G4CXOpticks::Get();
  
  G4int eventID=event->GetEventID();
  G4int ngenstep=SEvt::GetNumGenstepFromGenstep(0);
  G4int nphotons=SEvt::GetNumPhotonCollected(0);                                                                                                                                                                                              
  G4int hits;
  
  // Simulate the photons                                                                                                                                                                                                                       
  if(ngenstep>0){
    std::cout<<g4cx->desc()<<std::endl;
    std::cout<<"--- G4Optickx ---" << g4cx->descSimulate() <<std::endl;
    g4cx->simulate(eventID,0); // For Simulation
    cudaDeviceSynchronize();
    hits=SEvt::GetNumHit(0);
    
    std::cout << "EventAction Hits" << hits<<std::endl;
    if(hits>0) pm->CollectOpticksHits();
    
    std::cout<<"Event " <<eventID <<" Simulating with Opticks nphotons "<< nphotons << " nsteps " << ngenstep << " Hits " <<SEvt::GetNumHit(0) << std::endl;
  }
  
  G4CXOpticks::Get()->reset(eventID);
  
  G4cout<<" Opticks End of Event Action" <<G4endl;
  
#endif

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

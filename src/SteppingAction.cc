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
/// \file B1/src/SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "PersistencyManager.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Scintillation.hh"
#include <G4OpticalPhoton.hh>
#include <G4ProcessManager.hh>

#ifdef With_Opticks
    #include "G4CXOpticks.hh"
    #include "SEvt.hh"
    #include "U4.hh"
    #include "omp.h"
#endif



namespace B1
{
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction):G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  G4ParticleDefinition *pdef = step->GetTrack()->GetDefinition();
  G4Track *track = step->GetTrack();

  PersistencyManager *pManger= dynamic_cast<PersistencyManager *>(PersistencyManager::GetPersistencyManager());
  
  if (pdef != G4OpticalPhoton::Definition()) {
    G4SteppingManager *sMg = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
    G4StepStatus stepStatus = sMg->GetfStepStatus();
    G4double t1, t2 = 0;
    G4int singlets=0, triplets=0,TotalPhotns = 0;
    G4MaterialPropertiesTable *MPT = track->GetMaterial()->GetMaterialPropertiesTable();
    
    if (stepStatus != fAtRestDoItProc) {
      G4ProcessVector *PostStepProc = sMg->GetfPostStepDoItVector();
      size_t MaxSteps = sMg->GetMAXofPostStepLoops();
      //#pragma omp parallel for num_threads(15)                                                                                                                                                                                                    
      for (int stp = 0; stp < MaxSteps; stp++) {
	if ((*PostStepProc)[stp]->GetProcessName() == "Scintillation") {
	  G4Scintillation *ScintProc = (G4Scintillation *) (*PostStepProc)[stp];
	  G4int num_photons = ScintProc->GetNumPhotons();
	  std::cout << "Scintilation "<< num_photons <<std::endl;
	  
	  if (num_photons > 0) {
	    TotalPhotns+=num_photons;
	    std::cout << "Scintilation "<< num_photons <<" Amount of Singlets " <<singlets <<" Triplets " << triplets <<std::endl;                                                                                                          
	  }	  
	}
      }
      
      
#ifdef With_Opticks
      t1 = MPT->GetConstProperty(kSCINTILLATIONTIMECONSTANT1);
      t2 = MPT->GetConstProperty(kSCINTILLATIONTIMECONSTANT2);
      singlets = floor(MPT->GetConstProperty(kSCINTILLATIONYIELD1) * TotalPhotns);
      triplets = ceil(MPT->GetConstProperty(kSCINTILLATIONYIELD2) * TotalPhotns);
      std::cout << "Singlets + triplets =" << (singlets+triplets) << std::endl;; // S1

      if (singlets > 0)
	U4::CollectGenstep_DsG4Scintillation_r4695(track, step, singlets, 0, t1);
      if (triplets > 0)
	U4::CollectGenstep_DsG4Scintillation_r4695(track, step, triplets, 1, t2);
      
      std::cout << "HERE" << std::endl;      
      /*#ifdef With_G4OpticksTest
      //pManger->fOpticksPhotonCounter+=(singlets+triplets);                                                                                                                                                                                             
      pManger->fOpticksPhotonCounter+=(num_photons);
      #endif*/

      int CollectedPhotons;
      int maxPhoton=16500;
      
      CollectedPhotons=SEvt::GetNumPhotonCollected(0);

      std::cout << "CollectedPhotons: " <<CollectedPhotons << std::endl;
      
      std::cout << "HERE2" << std::endl;

      G4RunManager * runmng=G4RunManager::GetRunManager();      
      const int eventID=runmng->GetCurrentEvent()->GetEventID();
      
      if(CollectedPhotons<(maxPhoton*0.97)){
        // Get Persistance Manager                                                                                                                                                               
        std::cout<<"Event " << eventID<<" Simulating Photons in GPU and Saving the hits to file .." <<std::endl;
        G4CXOpticks * g4xc=G4CXOpticks::Get();
        //auto startTime=std::chrono::high_resolution_clock::now();
        g4xc->simulate(eventID,0);
        cudaDeviceSynchronize();
        //auto endTime=std::chrono::high_resolution_clock::now();                                                                                                                                                                                                               
        //std::chrono::duration<double> duration = endTime - startTime;                                                                                                                                                                                                         
        //std::cout << "SimTime " << duration.count() << std::endl;                                                                                                                                                                                     
	std::cout << "SEvt::GetNumHit(0)" << SEvt::GetNumHit(0) << std::endl;
      }
#endif
      
    }
  }else{
    
    std::cout << "HERE4" << std::endl;
    /*#ifdef With_G4OpticksTest
      if( G4OpticalPhoton::Definition() and track->GetTrackStatus()==fStopAndKill) pManger->fG4PhotonCounter+=1;
      #endif*/
    
#if defined(With_Opticks) and not defined(With_G4OpticksTest)
    std::cout << "HERE4.2" << std::endl;
    //if(step->GetTrack()->GetDefinition()==G4OpticalPhoton::Definition()) step->GetTrack()->SetTrackStatus(fStopAndKill);
#endif
  }
#if not defined(With_Opticks) || defined(With_G4OpticksTest)
  static G4OpBoundaryProcess* boundary = 0;
  std::cout << "HERE4.5" << std::endl;
  if (!boundary) { // the pointer is not defined yet                                                                                                                                                                                                    
    // Get the list of processes defined for the optical photon                                                                                                                                                                                       
    // and loop through it to find the optical boundary process.                                                                                                                                                                                      
    G4ProcessVector* pv = pdef->GetProcessManager()->GetProcessList();
    for (size_t i=0; i<pv->size(); i++) {
      std::cout << "HERE5" << std::endl;
      if ((*pv)[i]->GetProcessName() == "OpBoundary") {
	boundary = (G4OpBoundaryProcess*) (*pv)[i];
	std::cout << "HERE5.5" << std::endl;
	break;
      }
    }
  }
  std::cout << "HERE6" << std::endl;
  if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
    if (boundary and boundary->GetStatus() == Detection ){
      G4String detector_name = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
      auto PostStep=step->GetPostStepPoint();
      G4double wavelength=1239.8/PostStep->GetTotalEnergy()*CLHEP::eV; //nm                                                                                                                                                                         
      std::cot << "Optical Hits" << detector_name << " " << PostStep->GetPosition() << " " << PostStep->GetGlobalTime() << " " PostStep->GetMomentum() << " " << PostStep->GetPolarization() << " " << wavelength << std::endl;
    }
    std::cout << "HERE7" << std::endl;
  }
  
  
#endif
  std::cout << "HERE8" << std::endl;
  return;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

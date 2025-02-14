#include "SiPMSensDet.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4TouchableHistory.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"

SiPMSensitiveDetector::SiPMSensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {}

SiPMSensitiveDetector::~SiPMSensitiveDetector() {}

G4bool SiPMSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {

  //G4cout << "______PASSED____" << G4endl;

  // Get the track of the particle
  G4Track* track = step->GetTrack();
  
  // Check if the particle is an optical photon
  if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
    // Get the sensor name (from the touchable history)
    G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
    G4String sensorName = touchable->GetVolume()->GetName();
    
    // Get the wavelength of the optical photon
    G4double energy = track->GetKineticEnergy();
    G4double wavelength = (CLHEP::h_Planck * CLHEP::c_light / energy) / nanometer; // Convert to nm
    
    // Get the process that generated the optical photon
    G4String creatorProcessName = "Unknown";
    if (track->GetCreatorProcess()) {
      creatorProcessName = track->GetCreatorProcess()->GetProcessName();
    }
    
    // Get the current process (if any)
    G4String currentProcessName = "None";
    if (step->GetPostStepPoint()->GetProcessDefinedStep()) {
      currentProcessName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    }


    /*
    // Print the information
    G4cout << "Optical photon detected in sensor: " << sensorName << G4endl;
    G4cout << "  Wavelength: " << wavelength << " nm" << G4endl;
    G4cout << "  Creator Process: " << creatorProcessName << G4endl;
    G4cout << "  Current Process: " << currentProcessName << G4endl;
    G4cout << "  Position: " << G4BestUnit(step->GetPostStepPoint()->GetPosition(), "Length") << G4endl;
    G4cout << "  Time: " << G4BestUnit(step->GetPostStepPoint()->GetGlobalTime(), "Time") << G4endl;
    G4cout << "  Track ID: " << track->GetTrackID() << G4endl;
    G4cout << "  Parent ID: " << track->GetParentID() << G4endl;
    G4cout << G4endl;
    */
    
  }

  track->SetTrackStatus(fStopAndKill);
  
  return true;
}

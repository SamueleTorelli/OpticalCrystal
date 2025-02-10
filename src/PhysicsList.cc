#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysListFactory.hh"
#include "G4DecayPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmStandardPhysics.hh"

PhysicsList::PhysicsList() {
    // Decay physics for handling particle decays
    RegisterPhysics(new G4DecayPhysics());

    RegisterPhysics(new G4EmStandardPhysics());
    
    // Optical physics for Cherenkov, scintillation, etc.
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();    
    RegisterPhysics(opticalPhysics);

}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts() {

  SetDefaultCutValue(1 * mm);
  G4VUserPhysicsList::SetCuts();
}

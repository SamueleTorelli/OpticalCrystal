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
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"


#include "SiPMSensDet.hh"
#include "OpticalProperties.hh"

#ifdef With_Opticks
#include "G4CXOpticks.hh"
#include <cuda_runtime.h>
#include "SEventConfig.hh"
#endif

namespace B1

{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4bool checkOverlaps = true;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //
  // World
  //
  G4double world_sizeXY = 20*cm;
  G4double world_sizeZ  = 20*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  world_mat->SetMaterialPropertiesTable(opticalprops::Air());
  
  auto solidWorld = new G4Box("World",       // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                   // its material
    "World");// its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),       // at (0,0,0)
    logicWorld,            // its logical volume
    "World",               // its name
    nullptr,               // its mother  volume
    false,                 // no boolean operation
    0, // copy number
    checkOverlaps);        // overlaps checking

  // Define visualization attributes for the world
  auto worldVisAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0)); // Light blue with alpha = 0.8 (20% transparent)
  worldVisAttr->SetVisibility(true);                 // Make it visible
  worldVisAttr->SetForceWireframe(true);             // Only lines
  logicWorld->SetVisAttributes(worldVisAttr);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //
  //CsI crystal
  //


  // Dimensions of the CsI crystal
  G4double crystal_size = 5 * cm;
  
  // Material for the crystal
  G4Material* crystal_mat = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");

  crystal_mat->SetMaterialPropertiesTable(opticalprops::CsI());
  
  // Define the solid volume for the CsI crystal
  auto solidCrystal = new G4Box("Crystal", 
				0.5 * crystal_size, 0.5 * crystal_size, 0.5 * crystal_size); // Half-dimensions
  
  // Define the logical volume for the CsI crystal
  auto logicCrystal = new G4LogicalVolume(solidCrystal, 
					  crystal_mat, "Crystal");

  // Place the CsI crystal at the center of the world volume
  auto physicCrystal = new G4PVPlacement(nullptr,               // No rotation
		    G4ThreeVector(0, 0, 0),              // Position at the center of the world
		    logicCrystal,    // Logical volume
		    "Crystal",       // Name
		    logicWorld,      // Mother volume (the world)
		    false,           // No boolean operations
		    0,               // Copy number
		    checkOverlaps);  // Overlaps checking
  
  // Set visualization attributes for the CsI crystal
  auto crystalVisAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0, 0.2)); // Yellow color
  crystalVisAttr->SetVisibility(true);
  crystalVisAttr->SetForceSolid(true); // Render as a solid
  logicCrystal->SetVisAttributes(crystalVisAttr);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  //
  //Teflon box
  //

  // Create Teflon material (you can use a standard material or custom-defined one)
  G4Material* teflon = new G4Material("Teflon", 2.200*g/cm3, 2);
  teflon->AddElement(nist->FindOrBuildElement("C"), 0.8);  // Carbon (C)
  teflon->AddElement(nist->FindOrBuildElement("F"), 0.2);  // Fluorine (F)

  teflon->SetMaterialPropertiesTable(opticalprops::PTFE());
  
  // Define the large box (5x6x6 cm³)
  G4double largeBox_x = 6 * cm;
  G4double largeBox_y = 6 * cm;
  G4double largeBox_z = 6 * cm;
  G4Box* largeBox = new G4Box("LargeBox", 0.5 * largeBox_x, 0.5 * largeBox_y, 0.5 * largeBox_z);
  
  // Define the smaller box (5x5x5 cm³)
  G4double smallBox_x = crystal_size;
  G4double smallBox_y = crystal_size;
  G4double smallBox_z = crystal_size;
  G4Box* smallBox = new G4Box("SmallBox", 0.5 * smallBox_x, 0.5 * smallBox_y, 0.5 * smallBox_z);
  
  // Subtract the small box from the large box
  G4SubtractionSolid* teflonBox = new G4SubtractionSolid(
							 "TeflonBox",                // name of the new solid
							 largeBox,    // original solid
							 smallBox,    // solid to subtract
							 0,           // no rotation
							 G4ThreeVector(-0.5*cm, 0, 0)      // position of the smaller box relative to the large box
							 );
  
  // Create the logical volume for the subtracted box
  G4LogicalVolume* logicTeflonBox = new G4LogicalVolume(teflonBox, teflon, "TeflonBoxLV");
  
  // Place the subtracted box in the world (or in a mother volume)
  auto physicTeflonBox = new G4PVPlacement(
		    nullptr,    // no rotation
		    G4ThreeVector(0.5*cm, 0, 0),         // position at (0,0,0)
		    logicTeflonBox,             // logical volume
		    "TeflonBox",                // name of the physical volume
		    logicWorld,    // no mother volume
		    false,      // no boolean operation
		    0            // copy number
		    );
  

  
  
  // Set the visualization attributes (white with transparency of 0.2)
  G4VisAttributes* visAttributes_tefl = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.2));  // White color (RGB)
  visAttributes_tefl->SetVisibility(true);
  visAttributes_tefl->SetForceSolid(true);
  logicTeflonBox->SetVisAttributes(visAttributes_tefl);


  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  //
  //SiPM Detector
  //Window

  G4String name = "OpticalSilicone";
  
  G4Material* optical_silicone = G4Material::GetMaterial(name, false);
  
  G4Element* H  = nist->FindOrBuildElement("H");
  G4Element* C  = nist->FindOrBuildElement("C");
  G4Element* O  = nist->FindOrBuildElement("O");
  G4Element* Si = nist->FindOrBuildElement("Si");
  
  optical_silicone = new G4Material(name, 1.05*g/cm3, 4, kStateSolid);
  optical_silicone->AddElement(H,  3);
  optical_silicone->AddElement(C,  1);
  optical_silicone->AddElement(Si, 1);
  optical_silicone->AddElement(O,  1);
  
  G4String sipm_name = "SIPM_S13372";

  G4String window_name = sipm_name + "_WINDOW";

  G4double window_X  = 0.2 * mm;
  G4double window_Y = 6.3 * mm;
  G4double window_Z = 6.3 * mm;
  G4double window_xpos   = - crystal_size/2. - window_X/2;

  optical_silicone->SetMaterialPropertiesTable(opticalprops::OptCoupler());
  
  G4Box* window_solid_vol =
    new G4Box(window_name, window_X/2., window_Y/2., window_Z/2.);
  
  G4LogicalVolume* window_logic_vol =
    new G4LogicalVolume(window_solid_vol, optical_silicone, window_name);
  
  /*new G4PVPlacement(nullptr, G4ThreeVector(window_xpos, 0., 0.),
    window_logic_vol, window_name, logicWorld, false, 0, false);*/

  G4int numWindows = 8;
  G4double spacingY = window_Y; // Spacing along Y axis
  G4double spacingZ = window_Z; // Spacing along Z axis

  G4VPhysicalVolume* window_phys_vol[numWindows*numWindows];
  
  for (G4int i = 0; i < numWindows; ++i) {
    for (G4int j = 0; j < numWindows; ++j) {
      G4double ypos = (i - (numWindows - 1) / 2.0) * spacingY;
      G4double zpos = (j - (numWindows - 1) / 2.0) * spacingZ;
      
      G4String uniqueName = window_name + "_" + std::to_string(i) + "_" + std::to_string(j);
      
      window_phys_vol[i*numWindows+j] = new G4PVPlacement(nullptr, G4ThreeVector(window_xpos, ypos, zpos),
			window_logic_vol, uniqueName, logicWorld, false, i * numWindows + j, true);
    }
  }
  
  
  // Define the visual attributes for light blue with 0.2 transparency
  G4VisAttributes* windowVisAttr = new G4VisAttributes(G4Colour(0.68, 0.85, 0.9, 0.4)); // RGBA: Light Blue
  
  // Set the attributes for the logical volume
  windowVisAttr->SetForceSolid(true); // Optional: Force solid appearance in visualization
  window_logic_vol->SetVisAttributes(windowVisAttr);
  
  //
  //SiPM Detector
  //SensDet

  G4Material* SensitiveSi = nist->FindOrBuildMaterial("G4_Fe");
  SensitiveSi->SetMaterialPropertiesTable(opticalprops::SipmHamamatsu());
    
  G4String sens_name = sipm_name + "_SENSAREA";

  G4double sens_X  = 0.4 * mm;
  G4double sens_Y = 6.3 * mm;
  G4double sens_Z = 6.3 * mm;
  G4double sens_xpos = - crystal_size/2. - window_X - sens_X/2;
  
  G4Box* sens_solid_vol =
    new G4Box(sens_name, sens_X/2., sens_Y/2., sens_Z/2.);
 
  fsens_logic_vol =
    new G4LogicalVolume(sens_solid_vol,
			SensitiveSi,
			sens_name);

  /*new G4PVPlacement(nullptr, G4ThreeVector(sens_xpos, 0., 0.),
    fsens_logic_vol, sens_name, logicWorld, false, 0, false);*/
		    
  G4int numSensors = 8;

  G4VPhysicalVolume* sens_phys_vol[numSensors*numSensors];   
  
  for (G4int i = 0; i < numSensors; ++i) {
    for (G4int j = 0; j < numSensors; ++j) {
      G4double ypos = (i - (numSensors - 1) / 2.0) * spacingY;
      G4double zpos = (j - (numSensors - 1) / 2.0) * spacingZ;
      
      G4String uniqueName = sens_name + "_" + std::to_string(i) + "_" + std::to_string(j);
      
      sens_phys_vol[i*numSensors+j] = new G4PVPlacement(nullptr, G4ThreeVector(sens_xpos, ypos, zpos),
			fsens_logic_vol, uniqueName, logicWorld, false, i * numSensors + j, true);
    }
  }
  
  // Define the visual attributes for brown
  G4VisAttributes* sensVisAttr = new G4VisAttributes(G4Colour(0.59, 0.29, 0.0, 0.4)); // RGBA: Brown
  sensVisAttr->SetForceSolid(true); // Optional: Solid rendering
  
  // Assign the visual attributes to the logical volume
  fsens_logic_vol->SetVisAttributes(sensVisAttr);
  

  //
  //
  //Defining the surfaces
  //
  //

  /*
  //TeflonSurface
  G4OpticalSurface* opTeflonSurface = new G4OpticalSurface("TeflonSurface");
  opTeflonSurface->SetType(dielectric_LUT);
  opTeflonSurface->SetFinish(polished);
  opTeflonSurface->SetModel(LUT);
  opTeflonSurface->SetMaterialPropertiesTable(opticalprops::PTFE());
  
  G4LogicalSkinSurface* LTeflonSurface  =
    new G4LogicalSkinSurface( "LTeflonSurface", logicTeflonBox, opTeflonSurface);

  
  //CrysSurface
  G4OpticalSurface* opCrysSurface = new G4OpticalSurface("CrysSurface");
  opCrysSurface->SetType(dielectric_LUT);
  opCrysSurface->SetFinish(polished);
  opCrysSurface->SetModel(LUT);
  opCrysSurface->SetMaterialPropertiesTable(opticalprops::PTFE());
  
  G4LogicalSkinSurface* LCrysSurface  =
    new G4LogicalSkinSurface( "LCrysSurface", logicCrystal, opCrysSurface);
  */

  //WindowSkin (Use the skin due to contact with multiple elements)
  G4OpticalSurface* opWindowSurface = new G4OpticalSurface("WindowSkin");
  opWindowSurface->SetType(dielectric_dielectric);
  opWindowSurface->SetFinish(polished);
  opWindowSurface->SetModel(glisur);
  opWindowSurface->SetMaterialPropertiesTable(opticalprops::OptCoupler());
  
  G4LogicalSkinSurface* CrysWindowSurface =
    new G4LogicalSkinSurface("LWindowSkin", window_logic_vol, opWindowSurface);


  //SiPMSkin (Use the skin due to contact with multiple elements) 
  G4OpticalSurface* opSiPMSurface = new G4OpticalSurface("SiPMSkin");
  opSiPMSurface->SetType(dielectric_LUT);
  opSiPMSurface->SetFinish(polished);
  opSiPMSurface->SetModel(glisur);
  opSiPMSurface->SetMaterialPropertiesTable(opticalprops::SipmHamamatsu());
  
  
  G4LogicalSkinSurface* SiPMSurface =
    new G4LogicalSkinSurface("LSiPMSkin", window_logic_vol, opWindowSurface);
  

#ifdef With_Opticks
  std::cout <<"Setting our detector geometry with opticks" <<std::endl;
  G4CXOpticks::SetGeometry(physWorld);
  std::cout << SEventConfig::Desc() <<std::endl;
#endif
  
  //
  //always return the physical World
  //
  
  return physWorld;

  }
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



  void DetectorConstruction::ConstructSDandField() {
    
    G4VSensitiveDetector* sensitiveDetector = new SiPMSensitiveDetector("SiPMSensitiveDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);
    
    // Assign the sensitive detector to the logical volume
    fsens_logic_vol->SetSensitiveDetector(sensitiveDetector);
  }


}

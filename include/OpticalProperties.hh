#ifndef Optical_Material_Properties_h
#define Optical_Material_Properties_h

#include <globals.hh>
#include "G4MaterialPropertiesTable.hh"

#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Units/SystemOfUnits.h>

class G4MaterialPropertiesTable;

namespace opticalprops {

  using namespace CLHEP;
  
  //PTFE is TEFLON
  G4MaterialPropertiesTable* PTFE();

  G4MaterialPropertiesTable* Air();

  G4MaterialPropertiesTable* CsI();

  G4MaterialPropertiesTable* OptCoupler();

  G4MaterialPropertiesTable* GlassEpoxy();     

  G4MaterialPropertiesTable* SipmHamamatsu();

  
  constexpr G4double optPhotMinE_ =  0.2  * eV;
  constexpr G4double optPhotMaxE_ = 11.5  * eV;
  constexpr G4double noAbsLength_ = 1.e8  * m;

  inline G4double GetOptPhotMinE() {return optPhotMinE_;}
  inline G4double GetOptPhotMaxE() {return optPhotMaxE_;}
  
  
  // Constant that allows to convert nm to eV:
  // nm_to_eV_ / wavelength (nm) = energy (eV)
  constexpr G4double nm_to_eV_ = h_Planck * c_light * 1.e6;
  
}


#endif // Optical_Material_Properties_h

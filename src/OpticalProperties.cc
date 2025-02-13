#include <G4MaterialPropertiesTable.hh>
#include "OpticalProperties.hh"

#include <CLHEP/Units/SystemOfUnits.h>  

namespace opticalprops {
  
  using namespace CLHEP; 
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  G4MaterialPropertiesTable *PTFE() {
    
    G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
    
    // REFLECTIVITY                           
    std::vector<G4double> ENERGIES = {
      optPhotMinE_, 2.8 * eV, 3.5 * eV, 4. * eV,
      6. * eV, 7.2 * eV, optPhotMaxE_
    };
    std::vector<G4double> REFLECTIVITY = {
      .98, .98, .98, .98,
      .72, .72, .72
    };
    mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY);
    
    // REFLEXION BEHAVIOR                     
    std::vector<G4double> ENERGIES_2 = {optPhotMinE_, optPhotMaxE_};
    // Specular reflection about the normal to a microfacet.               
    // Such a vector is chosen according to a gaussian distribution with   
    // sigma = SigmaAlhpa (in rad) and centered in the average normal.     
    std::vector<G4double> specularlobe = {0., 0.};
    // specular reflection about the average normal                        
    std::vector<G4double> specularspike = {0., 0.};
    // 180 degrees reflection.                
    std::vector<G4double> backscatter = {0., 0.};
    // 1 - the sum of these three last parameters is the percentage of Lambertian reflection           
    
    mpt->AddProperty("SPECULARLOBECONSTANT", ENERGIES_2, specularlobe);
    mpt->AddProperty("SPECULARSPIKECONSTANT", ENERGIES_2, specularspike);
    mpt->AddProperty("BACKSCATTERCONSTANT", ENERGIES_2, backscatter);
    
    // REFRACTIVE INDEX                       
    std::vector<G4double> rIndex = {1.41, 1.41};
    mpt->AddProperty("RINDEX", ENERGIES_2, rIndex);
    
    return mpt;
  }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  G4MaterialPropertiesTable* Air(){
    // Air

    
    std::vector<G4double> photonEnergy = {
					  optPhotMinE_, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV,
					  2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
					  2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV, 2.757 * eV, 2.820 * eV,
					  2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
					  3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV,
					  4.002 * eV, optPhotMaxE_
    };
    
    std::vector<G4double> refractiveIndex2 = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
					       1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
					       1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
					       1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
					       1.0, 1.0, 1.0, 1.0 };
    
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex2);

    std::vector<G4double> ephoton = { 2.034 * eV, 4.136 * eV };
    
    // OpticalAirSurface                         
    std::vector<G4double> reflectivity = { 0.3, 0.5 };
    std::vector<G4double> efficiency   = { 0.8, 1.0 };

    mpt->AddProperty("REFLECTIVITY", ephoton, reflectivity);
    mpt->AddProperty("EFFICIENCY", ephoton, efficiency);
    
    return mpt;
  }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  G4MaterialPropertiesTable* CsI()
  {
    
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

    std::vector<G4double> energy = {
        optPhotMinE_,
        h_Planck * c_light / (460 * nm),
        h_Planck * c_light / (400 * nm),
        h_Planck * c_light / (380 * nm),
        h_Planck * c_light / (340 * nm),
        h_Planck * c_light / (320 * nm),
        h_Planck * c_light / (300 * nm),
        h_Planck * c_light / (280 * nm),
        h_Planck * c_light / (260 * nm),
        optPhotMaxE_};

    std::vector<G4double> rIndex = {
        1.766, 1.766, 1.794, 1.806, 1.845, 1.867, 1.902, 1.955, 2.043, 2.043
    };

    mpt->AddProperty("RINDEX", energy, rIndex);

    std::vector<G4double> absorption = {
      0.5 * m, 0.5 * m, 0.5 * m, 0.5 * m, 0.5 * m, 0.5 * m, 0.5 * m, 0.5 * m, 0.5 * m, 0.5 * m
    };

    mpt->AddProperty("ABSLENGTH", energy, absorption);

    std::vector<G4double> emission_intensity_warm = {
      0.0,  0.13, 0.17, 0.55, 0.98, 0.97, 0.83, 0.44, 0.16, 0.
    };

    std::vector<G4double> emission_intensity_cold = {
      0.0,  0.04, 0.10, 0.29, 0.67, 0.88, 0.29, 0.10, 0.02, 0
    };

    std::vector<G4double> reemission_prob = {
      0.1,  0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1
    };

    mpt->AddProperty("SCINTILLATIONCOMPONENT1", energy, emission_intensity_cold);
    mpt->AddProperty("FASTCOMPONENT", energy, emission_intensity_cold,true);
    
    mpt->AddProperty("SCINTILLATIONCOMPONENT2", energy, emission_intensity_cold);
    mpt->AddProperty("SLOWCOMPONENT", energy, emission_intensity_cold,true);

    mpt->AddProperty("REEMISSIONPROB", energy, reemission_prob,true);
    
    // mpt->AddProperty("ELSPECTRUM"            , energy, emission_intensity, 1);
    G4double csi_time_fast  =     0.81 * us;
    G4double csi_time_slow  =    28 * ns;

    // CONST PROPERTIES https://www.osti.gov/servlets/purl/1514707
    //mpt->AddConstProperty("SCINTILLATIONYIELD", 100000 / MeV);

    //This is fake, for test purpose, the real one is above
    mpt->AddConstProperty("SCINTILLATIONYIELD", 10000 / MeV);
    
    mpt->AddConstProperty("SCINTILLATIONYIELD1", 700 / MeV );
    mpt->AddConstProperty("SCINTILLATIONYIELD2", 300 / MeV );

    mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1",   csi_time_fast);
    mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT2",   csi_time_slow);
    mpt->AddConstProperty("RESOLUTIONSCALE",    1.0);

    return mpt;
  }


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  /// Optical Coupler ///
  G4MaterialPropertiesTable* OptCoupler()
  {
    // gel NyoGel OCK-451
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

    // REFRACTIVE INDEX
    G4double um2 = micrometer*micrometer;

    G4double constTerm  = 1.4954;
    G4double squareTerm = 0.008022 * um2;
    G4double quadTerm   = 0.;

    const G4int ri_entries = 100;
    G4double eWidth = (optPhotMaxE_ - optPhotMinE_) / ri_entries;

    std::vector<G4double> ri_energy;
    for (int i=0; i<ri_entries; i++) {
      ri_energy.push_back(optPhotMinE_ + i * eWidth);
    }

    std::vector<G4double> rIndex;
    for (int i=0; i<ri_entries; i++) {
      G4double wl = h_Planck * c_light / ri_energy[i];
      rIndex.push_back(constTerm + squareTerm/(wl*wl) + quadTerm/pow(wl,4));
      //G4cout << "* OptCoupler rIndex:  " << std::setw(5)
      //       << ri_energy[i]/eV << " eV -> " << rIndex[i] << G4endl;
    }
    mpt->AddProperty("RINDEX", ri_energy, rIndex);

    // ABSORPTION LENGTH
    // Values estimated from printed plot (to be improved).
    std::vector<G4double> abs_energy = {
      optPhotMinE_,  1.70 * eV,
      1.77 * eV,     2.07 * eV,  2.48 * eV,  2.76 * eV,
      2.92 * eV,     3.10 * eV,  3.31 * eV,  3.54 * eV,
      3.81 * eV,     4.13 * eV
    };

    std::vector<G4double> absLength = {
      noAbsLength_, noAbsLength_,
      1332.8 * mm,  1332.8 * mm,  1332.8 * mm,  666.17 * mm,
      499.5 * mm,   399.5 * mm,   199.5 * mm,  132.83 * mm,
        99.5 * mm,     4.5 * mm
    };
    mpt->AddProperty("ABSLENGTH", abs_energy, absLength);

    return mpt;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4MaterialPropertiesTable* GlassEpoxy()
  {
    // Optical properties of Optorez 1330 glass epoxy.
    // Obtained from http://refractiveindex.info and
    // https://www.zeonex.com/Optics.aspx.html#glass-like

    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

    // REFRACTIVE INDEX
    // The range is chosen to be up to ~10.7 eV because Sellmeier's equation
    // for fused silica is valid only in that range
    const G4int ri_entries = 200;
    G4double eWidth = (optPhotMaxE_ - optPhotMinE_) / ri_entries;

    std::vector<G4double> ri_energy;
    for (int i=0; i<ri_entries; i++) {
      ri_energy.push_back(optPhotMinE_ + i * eWidth);
    }

    std::vector<G4double> rIndex;
    for (int i=0; i<ri_entries; i++) {
      G4double lambda = h_Planck*c_light/ri_energy[i]*1000; // in micron
      G4double n2 = 2.291142 - 3.311944E-2*pow(lambda,2) - 1.630099E-2*pow(lambda,-2) +
                    7.265983E-3*pow(lambda,-4) - 6.806145E-4*pow(lambda,-6) +
                    1.960732E-5*pow(lambda,-8);
      rIndex.push_back(sqrt(n2));
      // G4cout << "* GlassEpoxy rIndex:  " << std::setw(5)
      //        << ri_energy[i]/eV << " eV -> " << rIndex[i] << G4endl;
    }
    mpt->AddProperty("RINDEX", ri_energy, rIndex);

    // ABSORPTION LENGTH
    std::vector<G4double> abs_energy = {
      optPhotMinE_, 2.000 * eV,
      2.132 * eV,   2.735 * eV,  2.908 * eV,  3.119 * eV,
      3.320 * eV,   3.476 * eV,  3.588 * eV,  3.749 * eV,
      3.869 * eV,   3.973 * eV,  4.120 * eV,  4.224 * eV,
      4.320 * eV,   4.420 * eV,  5.018 * eV
    };

    std::vector<G4double> absLength = {
      noAbsLength_, noAbsLength_,
      326.00 * mm,  117.68 * mm,  85.89 * mm,  50.93 * mm,
      31.25 * mm,   17.19 * mm,  10.46 * mm,   5.26 * mm,
        3.77 * mm,    2.69 * mm,   1.94 * mm,   1.33 * mm,
        0.73 * mm,    0.32 * mm,   0.10 * mm
    };
    mpt->AddProperty("ABSLENGTH", abs_energy, absLength);

    return mpt;
  }

   //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
   //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
   //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  G4MaterialPropertiesTable* SipmHamamatsu()
  {

    // SiPM efficiency set using the official Hamamatsu specs.

    const G4int entries = 15;
   
    G4double energies[entries]     = {
          optPhotMinE_,
          h_Planck * c_light / (809.722 * nm), h_Planck * c_light / (675.000 * nm),
          h_Planck * c_light / (587.500 * nm), h_Planck * c_light / (494.444 * nm),
          h_Planck * c_light / (455.556 * nm), h_Planck * c_light / (422.222 * nm),
          h_Planck * c_light / (395.833 * nm), h_Planck * c_light / (366.667 * nm),
          h_Planck * c_light / (344.444 * nm), h_Planck * c_light / (311.111 * nm),
          h_Planck * c_light / (293.056 * nm), h_Planck * c_light / (288.889 * nm),
          h_Planck * c_light / (279.167 * nm),
          optPhotMaxE_
		};
    
    G4double reflectivity[entries] = {
      0,
      0, 0,
      0, 0,
      0, 0,
      0, 0,
      0, 0,
      0, 0,
      0,
      0
    };

    G4double efficiency[entries]   = {
      0,
      0.0087, 0.192,
      0.311, 0.467,
      0.511, 0.502,
      0.469, 0.406,
      0.393, 0.324,
      0.180, 0.048,
      0.02,
      0
    };

   G4double um2 = micrometer*micrometer;

    G4double constTerm  = 1.4954;
    G4double squareTerm = 0.008022 * um2;
    G4double quadTerm   = 0.;

    const G4int ri_entries = 100;
    G4double eWidth = (optPhotMaxE_ - optPhotMinE_) / ri_entries;

    std::vector<G4double> ri_energy;
    for (int i=0; i<ri_entries; i++) {
      ri_energy.push_back(optPhotMinE_ + i * eWidth);
    }
    G4MaterialPropertiesTable* sipm_mt = new G4MaterialPropertiesTable();

    std::vector<G4double> rIndex;
    for (int i=0; i<ri_entries; i++) {
      G4double wl = h_Planck * c_light / ri_energy[i];
      rIndex.push_back(constTerm + squareTerm/(wl*wl) + quadTerm/pow(wl,4));
    }
    sipm_mt->AddProperty("RINDEX", ri_energy, rIndex);
    
    sipm_mt->AddProperty("EFFICIENCY", energies, efficiency, entries);
    sipm_mt->AddProperty("REFLECTIVITY", energies, reflectivity, entries);
    return sipm_mt;
  }

  
    
};

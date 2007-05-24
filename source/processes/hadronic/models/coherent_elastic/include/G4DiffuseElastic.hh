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
// $Id: G4DiffuseElastic.hh,v 1.2 2007-05-24 14:38:29 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// G4 Model: Low energy elastic scattering with 4-momentum balance 
//
// Class Description
// Final state production model for hadron nuclear elastic scattering; 
// Class Description - End
//
//
// 24.05.07 V. Grichine first implementation for proton elastic scattering


#ifndef G4DiffuseElastic_h
#define G4DiffuseElastic_h 1
 
#include "globals.hh"
#include "G4HadronicInteraction.hh"
#include "G4HadProjectile.hh"
#include "G4Nucleus.hh"

using namespace std;

enum G4ElasticGenerator
{
  fLElastic = 0,
  fHElastic,
  fQElastic,
  fSWave
};

class G4ParticleDefinition;
class G4VQCrossSection;
class G4ElasticHadrNucleusHE;

class G4DiffuseElastic : public G4HadronicInteraction
{
public:

  G4DiffuseElastic(G4ElasticHadrNucleusHE* HModel = 0);

  virtual ~G4DiffuseElastic();
 
  G4HadFinalState * ApplyYourself(const G4HadProjectile & aTrack, 
				  G4Nucleus & targetNucleus);

  G4VQCrossSection* GetCS();

  G4ElasticHadrNucleusHE* GetHElastic();

  void SetPlabLowLimit(G4double value);

  void SetHEModelLowLimit(G4double value);

  void SetQModelLowLimit(G4double value);

  void SetLowestEnergyLimit(G4double value);

  void SetRecoilKinEnergyLimit(G4double value);

  G4double SampleT(G4double p, G4double m1, G4double m2, G4double A);

  G4double GetDiffuseElasticXsc( G4ParticleDefiniton* particle, 
                                 G4double theta, 
			         G4double momentum, 
				 G4double A         );  
  G4double BesselJzero(G4double z);
  G4double BesselJone(G4double z);

private:

  G4int Rtmi(G4double* x, G4double xli, G4double xri, G4double eps, 
	     G4int iend,
	     G4double aa, G4double bb, G4double cc, G4double dd, 
	     G4double rr);

  G4double Fctcos(G4double t, 
		  G4double aa, G4double bb, G4double cc, G4double dd, 
		  G4double rr);


  G4VQCrossSection*           qCManager;
  G4ElasticHadrNucleusHE*     hElastic;

  G4ParticleDefinition* theProton;
  G4ParticleDefinition* theNeutron;
  G4ParticleDefinition* theDeuteron;
  G4ParticleDefinition* theAlpha;
  const G4ParticleDefinition* thePionPlus;
  const G4ParticleDefinition* thePionMinus;

  G4double lowEnergyRecoilLimit;  
  G4double lowEnergyLimitHE;  
  G4double lowEnergyLimitQ;  
  G4double lowestEnergyLimit;  
  G4double plabLowLimit;
};

inline void G4DiffuseElastic::SetRecoilKinEnergyLimit(G4double value)
{
  lowEnergyRecoilLimit = value;
}

inline void G4DiffuseElastic::SetPlabLowLimit(G4double value)
{
  plabLowLimit = value;
}

inline void G4DiffuseElastic::SetHEModelLowLimit(G4double value)
{
  lowEnergyLimitHE = value;
}

inline void G4DiffuseElastic::SetQModelLowLimit(G4double value)
{
  lowEnergyLimitQ = value;
}

inline void G4DiffuseElastic::SetLowestEnergyLimit(G4double value)
{
  lowestEnergyLimit = value;
}


/////////////////////////////////////////////////////////////
//
// Bessel J0 function based on rational approximation from 
// J.F. Hart, Computer Approximations, New York, Willey 1968, p. 141 

inline G4double G4DiffuseElastic::BesselJzero(G4double value)
{
  G4double modvalue, value2, fact1, fact2, arg, shift, bessel;

  modvalue = fabs(value);

  if ( value < 8.0 && value > -8.0 )
  {
    value2 = value*value;

    fact1  = 57568490574.0 + value2*(-13362590354.0 
                           + value2*( 651619640.7 
                           + value2*(-11214424.18 
                           + value2*( 77392.33017 
                           + value2*(-184.9052456   ) ) ) ) );

    fact2  = 57568490411.0 + value2*( 1029532985.0 
                           + value2*( 9494680.718
                           + value2*(59272.64853
                           + value2*(267.8532712 
                           + value2*1.0               ) ) ) );

    bessel = fact1/fact2;
  } 
  else 
  {
    arg    = 8.0/modvalue;

    value2 = arg*arg;

    shift  = modvalue-0.785398164;

    fact1  = 1.0 + value2*(-0.1098628627e-2 
                 + value2*(0.2734510407e-4
                 + value2*(-0.2073370639e-5 
                 + value2*0.2093887211e-6    ) ) );

    fact2  = -0.1562499995e-1 + value2*(0.1430488765e-3
                              + value2*(-0.6911147651e-5 
                              + value2*(0.7621095161e-6
                              - value2*0.934945152e-7    ) ) );

    bessel = sqrt(0.636619772/modvalue)*(cos(shift)*fact1 - arg*sin(shift)*fact2 );
  }
  return bessel;
}

/////////////////////////////////////////////////////////////
//
// Bessel J1 function based on rational approximation from 
// J.F. Hart, Computer Approximations, New York, Willey 1968, p. 141 

inline G4double G4DiffuseElastic::BesselJone(G4double value)
{
  G4double modvalue, value2, fact1, fact2, arg, shift, bessel;

  modvalue = fabs(value);

  if ( modvalue < 8.0 ) 
  {
    value2 = value*value;

    fact1  = value*(72362614232.0 + value2*(-7895059235.0 
                                  + value2*( 242396853.1
                                  + value2*(-2972611.439 
                                  + value2*( 15704.48260 
                                  + value2*(-30.16036606  ) ) ) ) ) );

    fact2  = 144725228442.0 + value2*(2300535178.0 
                            + value2*(18583304.74
                            + value2*(99447.43394 
                            + value2*(376.9991397 
                            + value2*1.0             ) ) ) );
    bessel = fact1/fact2;
  } 
  else 
  {
    arg    = 8.0/modvalue;

    value2 = arg*arg;

    shift  = modvalue - 2.356194491;

    fact1  = 1.0 + value2*( 0.183105e-2 
                 + value2*(-0.3516396496e-4
                 + value2*(0.2457520174e-5 
                 + value2*(-0.240337019e-6          ) ) ) );

    fact2 = 0.04687499995 + value2*(-0.2002690873e-3
                          + value2*( 0.8449199096e-5
                          + value2*(-0.88228987e-6
                          + value2*0.105787412e-6       ) ) );

    bessel = sqrt( 0.636619772/modvalue)*(cos(shift)*fact1 - arg*sin(shift)*fact2);

    if (value < 0.0) bessel = -bessel;
  }
  return bessel;
}




#endif

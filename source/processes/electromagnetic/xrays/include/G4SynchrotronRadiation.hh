//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4SynchrotronRadiation.hh,v 1.2 2006-05-19 10:05:27 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ------------------------------------------------------------
//      GEANT 4 class header file
//      CERN Geneva Switzerland
//
//
//      History:
//      21-5-98  1 version , V. Grichine
//      28-05-01, V.Ivanchenko minor changes to provide ANSI -wall compilation
//
//
// ------------------------------------------------------------

#ifndef G4SynchrotronRadiation_h
#define G4SynchrotronRadiation_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4VDiscreteProcess.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4Field.hh"
#include "G4ThreeVector.hh"
#include "G4PropagatorInField.hh"

#include "G4Track.hh"
#include "G4Step.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"


class G4SynchrotronRadiation : public G4VDiscreteProcess
{
public:

  G4SynchrotronRadiation(const G4String& pName = "SynchrotronRadiation",
		         G4ProcessType type = fElectromagnetic);

  virtual ~G4SynchrotronRadiation();

  G4double GetMeanFreePath( const G4Track& track,
                                     G4double previousStepSize,
                                     G4ForceCondition* condition );

  G4VParticleChange *PostStepDoIt( const G4Track& track,
                                      const G4Step& Step    );

  G4double GetPhotonEnergy( const G4Track& trackData,
                               const G4Step&  stepData      );

  G4double GetRandomEnergySR( G4double, G4double );

  G4double GetProbSpectrumSRforInt( G4double );
  G4double GetIntProbSR( G4double );

  G4double GetIntegrandForAngleK( G4double );
  G4double GetAngleK( G4double );
  G4double GetAngleNumberAtGammaKsi( G4double );

  G4double InvSynFracInt(G4double x);
  G4double Chebyshev(G4double a,G4double b,const G4double c[],G4int m,G4double x);

  void PrintInfoDefinition();

  G4bool IsApplicable(const G4ParticleDefinition&);

  G4double GetLambdaConst() { return fLambdaConst; };
  G4double GetEnergyConst() { return fEnergyConst; };

  void SetRootNumber(G4int rn){ fRootNumber = rn; };
  void SetKsi(G4double ksi){ fKsi = ksi; };
  void SetEta(G4double eta){ fEta = eta; };
  void SetPsiGamma(G4double psg){ fPsiGamma = psg; };
  void SetOrderAngleK(G4double ord){ fOrderAngleK = ord; }; // should be 1/3 or 2/3

private:

  G4SynchrotronRadiation & operator=(const G4SynchrotronRadiation &right);
  G4SynchrotronRadiation(const G4SynchrotronRadiation&);

  G4ParticleDefinition*       theGamma;
  const G4ParticleDefinition* theElectron;
  const G4ParticleDefinition* thePositron;

  G4double fLambdaConst;
  G4double fEnergyConst;

  const G4double fIntegralProbabilityOfSR[200];

  G4double fAlpha;
  G4int    fRootNumber;
  G4double fKsi;             // omega/omega_c
  G4double fPsiGamma;        // Psi-angle*gamma
  G4double fEta;             //
  G4double fOrderAngleK;     // 1/3 or 2/3

  G4PropagatorInField* fFieldPropagator;

};

//////////////////////////  INLINE METHODS  /////////////////////////////

inline G4bool
G4SynchrotronRadiation::IsApplicable( const G4ParticleDefinition& particle )
{

  return ( ( &particle == (const G4ParticleDefinition *)theElectron ) ||
           ( &particle == (const G4ParticleDefinition *)thePositron )    );

  // return ( particle.GetPDGCharge() != 0.0 );
}

inline G4double G4SynchrotronRadiation::Chebyshev(G4double a,G4double b,const G4double c[],G4int m,G4double x)
{
  G4double y;
  G4double y2=2.0*(y=(2.0*x-a-b)/(b-a)); // Change of variable.
  G4double d=0,dd=0;
  for (G4int j=m-1;j>=1;j--) // Clenshaw's recurrence.
  { G4double sv=d;
	d=y2*d-dd+c[j];
	dd=sv;
  }
  return y*d-dd+0.5*c[0];
}

#endif  // end of G4SynchrotronRadiation.hh


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
// $Id: G4UrbanMscModel2.hh,v 1.2 2008-03-10 15:08:51 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
//
// GEANT4 Class header file
//
//
// File name:     G4UrbanMscModel2
//
// Author:        Laszlo Urban
//
// Creation date: 06.03.2008
//
// Modifications:
//
//
//
// Class Description:
//
// Implementation of the model of multiple scattering based on
// H.W.Lewis Phys Rev 78 (1950) 526 and L.Urban model

// -------------------------------------------------------------------
//

#ifndef G4UrbanMscModel2_h
#define G4UrbanMscModel2_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VMscModel.hh"
#include "G4PhysicsTable.hh"
#include "G4MscStepLimitType.hh"

class G4ParticleChangeForMSC;
class G4SafetyHelper;
class G4LossTableManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UrbanMscModel2 : public G4VMscModel
{

public:

  G4UrbanMscModel2(const G4String& nam = "UrbanMscUni");

  virtual ~G4UrbanMscModel2();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  G4double ComputeCrossSectionPerAtom(const G4ParticleDefinition* particle,
				      G4double KineticEnergy,
				      G4double AtomicNumber,
				      G4double AtomicWeight=0., 
				      G4double cut =0.,
				      G4double emax=DBL_MAX);

  void SampleSecondaries(std::vector<G4DynamicParticle*>*, 
			 const G4MaterialCutsCouple*,
			 const G4DynamicParticle*,
			 G4double,
			 G4double);

  void SampleScattering(const G4DynamicParticle*,
			G4double safety);

  G4double ComputeTruePathLengthLimit(const G4Track& track,
				      G4PhysicsTable* theLambdaTable,
				      G4double currentMinimalStep);

  G4double ComputeGeomPathLength(G4double truePathLength);

  G4double ComputeTrueStepLength(G4double geomStepLength);

  G4double ComputeTheta0(G4double truePathLength,
                         G4double KineticEnergy);

private:

  G4double SampleCosineTheta(G4double trueStepLength, G4double KineticEnergy);

  G4double SampleDisplacement();

  G4double LatCorrelation();

  void GeomLimit(const G4Track& track);

  inline G4double GetLambda(G4double kinEnergy);

  inline void SetParticle(const G4ParticleDefinition*);

  //  hide assignment operator
  G4UrbanMscModel2 & operator=(const  G4UrbanMscModel2 &right);
  G4UrbanMscModel2(const  G4UrbanMscModel2&);

  const G4ParticleDefinition* particle;
  G4ParticleChangeForMSC*     fParticleChange;

  G4SafetyHelper*             safetyHelper;
  G4PhysicsTable*             theLambdaTable;
  const G4MaterialCutsCouple* couple;
  G4LossTableManager*         theManager;


  G4double mass;
  G4double charge;

  G4double masslimite,masslimitmu;

  G4double taubig;
  G4double tausmall;
  G4double taulim;
  G4double currentTau;
  G4double frscaling1,frscaling2;
  G4double tlimit;
  G4double tlimitmin;
  G4double tlimitminfix;

  G4double nstepmax;
  G4double geombig;
  G4double geommin;
  G4double geomlimit;
  G4double skindepth;
  G4double smallstep;

  G4double presafety;

  G4double lambda0;
  G4double lambdaeff;
  G4double tPathLength;
  G4double zPathLength;
  G4double par1,par2,par3;

  G4double stepmin;

  G4double currentKinEnergy;
  G4double currentRange; 
  G4double currentRadLength;

  G4double Zeff;

  G4int    currentMaterialIndex;

  G4bool   isInitialized;
  G4bool   inside;
  G4bool   insideskin;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline
G4double G4UrbanMscModel2::GetLambda(G4double e)
{
  G4double x;
  if(theLambdaTable) {
    G4bool b;
    x = ((*theLambdaTable)[currentMaterialIndex])->GetValue(e, b);
  } else {
    x = CrossSection(couple,particle,e);
  }
  if(x > DBL_MIN) x = 1./x;
  else            x = DBL_MAX;
  return x;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline
void G4UrbanMscModel2::SetParticle(const G4ParticleDefinition* p)
{
  if (p != particle) {
    particle = p;
    mass = p->GetPDGMass();
    charge = p->GetPDGCharge()/eplus;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


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
// $Id: G4MuBremsstrahlung.cc,v 1.36 2005-04-08 15:18:12 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4MuBremsstrahlung
//
// Author:        Laszlo Urban
//
// Creation date: 30.09.1997
//
// Modifications:
//
// 08-04-98 remove 'tracking cut' of muon in oIt, MMa
// 26-10-98 new cross section of R.Kokoulin,cleanup , L.Urban
// 10-02-00 modifications , new e.m. structure, L.Urban
// 29-05-01 V.Ivanchenko minor changes to provide ANSI -wall compilation
// 09-08-01 new methods Store/Retrieve PhysicsTable (mma)
// 17-09-01 migration of Materials to pure STL (mma)
// 26-09-01 completion of store/retrieve PhysicsTable (mma)
// 28-09-01 suppression of theMuonPlus ..etc..data members (mma)
// 29-10-01 all static functions no more inlined (mma)
// 08-11-01 particleMass becomes a local variable (mma)
// 19-08-02 V.Ivanchenko update to new design
// 23-12-02 Change interface in order to move to cut per region (V.Ivanchenko)
// 26-12-02 Secondary production moved to derived classes (V.Ivanchenko)
// 08-08-03 STD substitute standard  (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 10-02-04 Add lowestKinEnergy (V.Ivanchenko)
// 17-08-04 Utilise mu+ tables for mu- (V.Ivanchenko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4MuBremsstrahlung.hh"
#include "G4Gamma.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4MuBremsstrahlungModel.hh"
#include "G4UniversalFluctuation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4MuBremsstrahlung::G4MuBremsstrahlung(const G4String& name)
  : G4VEnergyLossProcess(name),
    theParticle(0),
    theBaseParticle(0),
    lowestKinEnergy(1.*GeV),
    isInitialised(false)
{
  SetDEDXBinning(120);
  SetLambdaBinning(120);
  SetMinKinEnergy(0.1*keV);
  SetMaxKinEnergy(100.0*TeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4MuBremsstrahlung::~G4MuBremsstrahlung()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4MuBremsstrahlung::InitialiseEnergyLossProcess(const G4ParticleDefinition* part,
                                                     const G4ParticleDefinition*)
{
  if(!isInitialised) {

    isInitialised = true;

    theParticle = part;
    SetSecondaryParticle(G4Gamma::Gamma());
    SetIonisation(false);

    G4MuBremsstrahlungModel* em = new G4MuBremsstrahlungModel();
    em->SetLowestKineticEnergy(lowestKinEnergy);

    G4VEmFluctuationModel* fm = new G4UniversalFluctuation();
    em->SetLowEnergyLimit(0.1*keV);
    em->SetHighEnergyLimit(100.0*TeV);
    AddEmModel(1, em, fm);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4MuBremsstrahlung::PrintInfo()
{
  G4cout << "      Parametrised model "
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


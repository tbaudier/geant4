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
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     eegg  -> unit test of e+e- -> gg model
//
//      Author:        V.Ivanchenko 
// 
//      Creation date: 19 October 2006
//
//      Modifications: 
//
// -------------------------------------------------------------------

#include "globals.hh"
#include "G4eeToTwoGammaModel.hh"
#include "G4eeToTwoGammaModel82.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4DynamicParticle.hh"
#include "G4ThreeVector.hh"
#include "G4Positron.hh"
#include "G4ios.hh"
#include "TFile.h"
#include "TH1F.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

int main(int argc, char** argv)
{
  //  G4int verbose = 2;
  G4int nstat   = 10000;
  G4double energy = 10.*GeV;
  G4double etot = energy + 2.0*electron_mass_c2;

  G4MaterialCutsCouple* couple = 0;
  G4ThreeVector   dir(0.0,0.0,1.0);
  G4DynamicParticle* dp0 = new G4DynamicParticle(G4Positron::Positron(),
						 dir,energy);
  G4DynamicParticle* dp;

  G4eeToTwoGammaModel* eegg = 
    new G4eeToTwoGammaModel(G4Positron::Positron(),"eegg");
  G4eeToTwoGammaModel82* eegg82 = 
    new G4eeToTwoGammaModel82(G4Positron::Positron(),"eegg82");

  std::vector<G4DynamicParticle*>* vdp;

  TFile tree("eegg10gev.root","RECREATE","eegg");

  TH1D* h1 = new TH1D( "h1","Gamma Energy",120,0,1);
  TH1D* h2 = new TH1D( "h2","Gamma Energy",120,0,1);
  TH1D* h3 = new TH1D( "h3","cos(theta) lab",100,-1,1);
  TH1D* h4 = new TH1D( "h4","cos(theta) lab",100,-1,1);

  G4int i, j, n;

  for(i=0; i<nstat; i++) {
    vdp = eegg->SampleSecondaries(couple,dp0,0,0);
    n = vdp->size();
    for(j=0; j<n; j++) {
      dp = (*vdp)[j]; 
      h1->Fill(dp->GetKineticEnergy()/etot);
      h3->Fill(dp->GetMomentumDirection().z());
      delete dp;
    }
    delete vdp;
    vdp = eegg82->SampleSecondaries(couple,dp0,0,0);
    n = vdp->size();
    for(j=0; j<n; j++) {
      dp = (*vdp)[j]; 
      h2->Fill(dp->GetKineticEnergy()/etot);
      h4->Fill(dp->GetMomentumDirection().z());
      delete dp;
    }
    delete vdp;
  }
  tree.Write();  
  tree.Close();       

  return 0;
}









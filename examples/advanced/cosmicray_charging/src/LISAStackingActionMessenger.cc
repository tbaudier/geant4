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
// ********************************************************************
// *                                                                  *
// * cosmicray_charging advanced example for Geant4                   *
// * (adapted simulation of test-mass charging in the LISA mission)   *
// *                                                                  *
// * Henrique Araujo (h.araujo@imperial.ac.uk) & Peter Wass           *
// * Imperial College London                                          *
// *                                                                  *
// * LISAStackingActionMessenger class                                *
// *                                                                  *
// ********************************************************************
//
// HISTORY
// 22/02/2004: migrated from LISA-V04
//
// ********************************************************************


#include "LISAStackingActionMessenger.hh"

#include <sstream>

#include "LISAStackingAction.hh"


LISAStackingActionMessenger::LISAStackingActionMessenger
  (LISAStackingAction* stackAct) : stackingAction(stackAct){

  // set flag for primaries survey
  SetPriSurvey = new G4UIcmdWithABool("/surveys/surveyPrimaries",this);
  SetPriSurvey->SetGuidance
    ("Dump primary energies (MeV) to file 'primaries.out'");
  SetPriSurvey->SetGuidance("Default = false");
  SetPriSurvey->SetParameterName("SurveyPrimariesFlag", false);
  SetPriSurvey->AvailableForStates(G4State_Idle);

  // set flag for particle survey
  SetPartSurvey = new G4UIcmdWithABool("/surveys/surveyParticles",this);
  SetPartSurvey->SetGuidance("Dump particle energies to different files");
  SetPartSurvey->SetGuidance("Fragments and nuclei in MeV/nucleon, other in MeV.");
  SetPartSurvey->SetGuidance("'gammas.out':    gammas above 1 MeV");
  SetPartSurvey->SetGuidance("'electrons.out': e- and e+ above 1 MeV");
  SetPartSurvey->SetGuidance("'neutrons.out':  all neutrons");
  SetPartSurvey->SetGuidance("'pions.out':     all pions");
  SetPartSurvey->SetGuidance("'muons.out':     all muons");
  SetPartSurvey->SetGuidance("'kaons.out':     all kaons");
  SetPartSurvey->SetGuidance("'fragments.out': 2H, 3H, He3, alphas");
  SetPartSurvey->SetGuidance("'nuclei.out':    all nuclei");
  SetPartSurvey->SetGuidance("'others.out':    others above 1 MeV");
  SetPartSurvey->SetGuidance("Default = false");
  SetPartSurvey->SetParameterName("SurveyParticlesFlag", false);
  SetPartSurvey->AvailableForStates(G4State_Idle);

}


LISAStackingActionMessenger::~LISAStackingActionMessenger() {

  delete SetPriSurvey;  
  delete SetPartSurvey;  

}


void LISAStackingActionMessenger::SetNewValue
   (G4UIcommand* command, G4String newValue) { 

  if(command == SetPriSurvey) {
    G4int vl;
    const char* t = newValue;
    std::istringstream is(t);
    is >> vl;
    stackingAction->SetPrimarySurvey(vl!=0);
  }

  else if(command == SetPartSurvey) {
    G4int vl;
    const char* t = newValue;
    std::istringstream is(t);
    is >> vl;
    stackingAction->SetParticleSurvey(vl!=0);
  }



}


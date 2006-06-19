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
// $Id: G4EmMessenger.cc,v 1.6 2006-06-19 21:34:47 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4EmMessenger
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 09.11.2005 V.Ivanchenko edit to provide a standard
// 19.06.2006 V.Ivanchenko add mu-nuclear process
//
//----------------------------------------------------------------------------
//

#include "G4EmMessenger.hh"
#include "G4EmExtraPhysics.hh"

G4EmMessenger::G4EmMessenger(G4EmExtraPhysics* ab)
{
  theB = ab;
  aDir1 = new G4UIdirectory("/physics_engine/");
  aDir1->SetGuidance("commands related to the physics simulation engine.");

  // general stuff.
  aDir2 = new G4UIdirectory("/physics_engine/tailor/");
  aDir2->SetGuidance("tailoring the processes");

  // command for synchrotron radiation.
  theSynch = new G4UIcmdWithAString("/physics_engine/tailor/SyncRadiation",this);
  theSynch->SetGuidance("Switching on/off synchrotron radiation.");
  theSynch->SetParameterName("status","off");
  theSynch->SetCandidates("on off");
  theSynch->SetDefaultValue("off");
  theSynch->AvailableForStates(G4State_PreInit);

  // command for gamma nuclear physics.
  theGN = new G4UIcmdWithAString("/physics_engine/tailor/GammaNuclear",this);
  theGN->SetGuidance("Switching on gamma nuclear physics.");
  theGN->SetParameterName("status","off");
  theGN->SetCandidates("on off");
  theGN->SetDefaultValue("on");
  theGN->AvailableForStates(G4State_PreInit);

  // command for muon nuclear physics.
  theMUN = new G4UIcmdWithAString("/physics_engine/tailor/MuonNuclear",this);
  theMUN->SetGuidance("Switching on muon nuclear physics.");
  theMUN->SetParameterName("status","off");
  theMUN->SetCandidates("on off");
  theMUN->SetDefaultValue("off");
  theMUN->AvailableForStates(G4State_PreInit);
}

G4EmMessenger::~G4EmMessenger()
{
  delete theSynch;
  delete theGN;
  delete theMUN;
  delete aDir1;
  delete aDir2;
}

void G4EmMessenger::SetNewValue(G4UIcommand* aComm, G4String aS)
{
  if(aComm==theSynch) theB->Synch(aS);
  if(aComm==theGN)    theB->GammaNuclear(aS);
  if(aComm==theMUN)   theB->MuonNuclear(aS);
}

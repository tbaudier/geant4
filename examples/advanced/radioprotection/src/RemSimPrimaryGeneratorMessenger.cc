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
// Code developed by:  S.Guatelli, guatelli@ge.infn.it
//
//    *****************************************
//    *                                       *
//    *    RemSimPrimaryGeneratorMessenger.cc *
//    *                                       *
//    *****************************************
//
//
// $Id: RemSimPrimaryGeneratorMessenger.cc,v 1.8 2005-09-08 06:56:18 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#include "RemSimPrimaryGeneratorMessenger.hh"
#include "RemSimPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

RemSimPrimaryGeneratorMessenger::RemSimPrimaryGeneratorMessenger( RemSimPrimaryGeneratorAction* prim): primary(prim)
{  
  gunDir = new G4UIdirectory("/gun/");
  gunDir->SetGuidance("Select the generation configuration of primary particles.");
        
  fluxCmd = new G4UIcmdWithAString("/gun/generator",this);
  fluxCmd -> SetGuidance("Assign the configuration of generation of primary particles."); 
  fluxCmd -> SetParameterName("choice",true);
  fluxCmd -> SetCandidates("Moon Interplanetary Basic");
  fluxCmd -> AvailableForStates(G4State_PreInit,G4State_Idle); 

  dataCmd = new G4UIcmdWithAString("/gun/data",this);
  dataCmd -> SetGuidance("Primary particle spectrum"); 
  dataCmd -> SetParameterName("choice",true);
  dataCmd -> AvailableForStates(G4State_PreInit,G4State_Idle); 
 }

RemSimPrimaryGeneratorMessenger::~RemSimPrimaryGeneratorMessenger()
{
  delete dataCmd;
  delete fluxCmd;
  delete gunDir;
} 
 
void RemSimPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
 if(command == fluxCmd) primary -> SelectPrimaries(newValue);
 if (command == dataCmd) primary -> Read(newValue);
}


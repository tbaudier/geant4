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
// $Id: DetectorMessenger.hh,v 1.1 2008-07-07 16:37:26 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
/////////////////////////////////////////////////////////////////////////
//
// DetectorMessenger
//
// Created: 20.06.08 V.Ivanchenko
//
// Modified:
//
////////////////////////////////////////////////////////////////////////
//

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
public:

  DetectorMessenger(DetectorConstruction* );
  virtual ~DetectorMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:

  DetectorConstruction* fDetector;

  G4UIdirectory*             ftestDir;
  G4UIcmdWithAString*        fmatCmd;
  G4UIcmdWithAString*        fmat1Cmd;
  G4UIcmdWithADoubleAndUnit* frCmd;
  G4UIcmdWithADoubleAndUnit* flCmd;
  G4UIcmdWithAnInteger*      fbinCmd;
  G4UIcmdWithAnInteger*      fnOfAbsCmd;
  G4UIcmdWithAnInteger*      fverbCmd;
  G4UIcmdWithAString*        fcsCmd;
  G4UIcmdWithAString*        fpartCmd;
  G4UIcmdWithADoubleAndUnit* fe1Cmd;
  G4UIcmdWithADoubleAndUnit* fe2Cmd;
  G4UIcmdWithADoubleAndUnit* fp1Cmd;
  G4UIcmdWithADoubleAndUnit* fp2Cmd;
  G4UIcmdWithoutParameter*   fupdateCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


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
// $Id: XrayFluoAnalysisManager.hh
// GEANT4 tag $Name: 
//
// Author: Elena Guardincerri (Elena.Guardincerri@ge.infn.it)
//
// History:
// -----------
//  28 Nov 2001  Elena Guardincerri   Created
//
// -------------------------------------------------------------------



#ifdef G4ANALYSIS_USE
#ifndef XrayFluoAnalysisManager_h
#define XrayFluoAnalysisManager_h 1

#include "globals.hh"

// Histogramming from AIDA 
#include "Interfaces/IHistogram1D.h"
#include "Interfaces/IHistogram2D.h"

// Histogramming from Anaphe
#include "Interfaces/IHistoManager.h"

// Ntuples from Anaphe
#include "NtupleTag/LizardNTupleFactory.h"
#include "NtupleTag/LizardQuantity.h"


class G4Step;
class XrayFluoAnalysisMessenger;
class NTuple;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class XrayFluoAnalysisManager
{
public:
 
  virtual ~XrayFluoAnalysisManager();
  
  void book();
  
  void finish();
  
  void analyseStepping(const G4Step* aStep);
 
  void analyseEnergyDep(G4double eDep);

 void analysePrimaryGenerator(G4double energy);

 static XrayFluoAnalysisManager* getInstance();
 

private:
  
  XrayFluoAnalysisManager();
 
  static XrayFluoAnalysisManager* instance;
  
  IHistoManager* histoManager;
 
  Lizard::NTupleFactory* factory;
  Lizard::NTuple* ntuple;
  
  XrayFluoAnalysisMessenger* analysisMessenger;

  // Quantities for the ntuple
  Lizard::Quantity<float> eDep;
  Lizard::Quantity<float> counts;
};
#endif
#endif




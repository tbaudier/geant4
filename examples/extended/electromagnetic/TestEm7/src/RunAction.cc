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
/// \file electromagnetic/TestEm7/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
// $Id: RunAction.cc,v 1.25 2010-09-17 18:45:43 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "StepMax.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PhysicsList* phys,
                     PrimaryGeneratorAction* kin)
:fDetector(det), fPhysics(phys), fKinematic(kin)
{ 
  fTallyEdep = new G4double[MaxTally];
  
  // Book predefined histograms
  BookHisto(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete [] fTallyEdep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  // save Rndm status
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  CLHEP::HepRandom::showEngineStatus();
     
  //initialize projected range, tallies, Ebeam, and book histograms
  //
  fNbPrimarySteps = 0;
  fRange = 0;
  fProjRange = fProjRange2 = 0.;
  fEdeptot = fEniel = 0.;
  for (G4int j=0; j<MaxTally; j++) fTallyEdep[j] = 0.;
  fKinematic->ResetEbeamCumul();
  
  if (fAnalysisManager->IsActive()) {
    fAnalysisManager->OpenFile(); 

    // histogram "1" is defined by the length of the target
    // zoomed histograms are defined by UI command  
    G4double length  = fDetector->GetAbsorSizeX();
    G4double stepMax = fPhysics->GetStepMaxProcess()->GetMaxStep();
    G4int nbmin = 100;
    G4int nbBins = (G4int)(0.5 + length/stepMax);
    if (nbBins < nbmin) nbBins = nbmin;
    fAnalysisManager->SetH1(1, nbBins, 0., length, "mm");
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbofEvents = aRun->GetNumberOfEvent();
  if (NbofEvents == 0) return;

  //run conditions
  //  
  G4Material* material = fDetector->GetAbsorMaterial();
  G4double density = material->GetDensity();
   
  G4String particle = fKinematic->GetParticleGun()->GetParticleDefinition()
                      ->GetParticleName();    
  G4double energy = fKinematic->GetParticleGun()->GetParticleEnergy();
  G4cout << "\n The run consists of " << NbofEvents << " "<< particle << " of "
         << G4BestUnit(energy,"Energy") << " through " 
         << G4BestUnit(fDetector->GetAbsorSizeX(),"Length") << " of "
         << material->GetName() << " (density: " 
         << G4BestUnit(density,"Volumic Mass") << ")" << G4endl;
         
  //compute projected range and straggling
  //
  if(fRange > 0) {
    fProjRange /= fRange; 
    fProjRange2 /= fRange;
  }
  G4double rms = fProjRange2 - fProjRange*fProjRange;        
  if (rms>0.) rms = std::sqrt(rms); else rms = 0.;

  G4double nstep = G4double(fNbPrimarySteps)/G4double(NbofEvents);

  G4cout.precision(6);       
  G4cout << "\n Projected Range= "<< G4BestUnit(fProjRange,"Length")
         << "   rms= "            << G4BestUnit( rms,"Length")
         << G4endl;
  G4cout << " Mean number of primary steps = "<< nstep << G4endl;

  //compute energy deposition and niel
  //
  fEdeptot /= NbofEvents; 
  G4cout << " Total energy deposit= "<< G4BestUnit(fEdeptot,"Energy")
         << G4endl;
  fEniel /= NbofEvents; 
  G4cout << " niel energy deposit = "<< G4BestUnit(fEniel,"Energy")
         << G4endl;
     
  //print dose in tallies
  //
  G4int tallyNumber = fDetector->GetTallyNumber();
  if (tallyNumber > 0) {
    G4double Ebeam = fKinematic->GetEbeamCumul();
    G4cout << "\n---------------------------------------------------------\n";
    G4cout << " Cumulated Doses : \tEdep      \tEdep/Ebeam \tDose" << G4endl;
    for (G4int j=1; j <= tallyNumber; j++) {
      G4double Edep = fTallyEdep[j], ratio = 100*Edep/Ebeam;
      G4double tallyMass = fDetector->GetTallyMass(j);      
      G4double Dose = Edep/tallyMass;
      G4cout << " tally " << j << ": \t \t"
             << G4BestUnit(Edep,"Energy") << "\t"
             << ratio << " % \t"
             << G4BestUnit(Dose,"Dose")   << G4endl;
    }
    G4cout << "\n---------------------------------------------------------\n";
    G4cout << G4endl; 
  }

  // normalize histograms
  //
  if (fAnalysisManager->IsActive() ) {    
    for (G4int j=1; j<3; j++) {  
      G4double binWidth = fAnalysisManager->GetH1Width(j);
      if(binWidth > 0.0) {
	G4double fac = (mm/MeV)/(NbofEvents * binWidth);
	fAnalysisManager->ScaleH1(j, fac);
      }
    }
    fAnalysisManager->ScaleH1(3, 1./NbofEvents);
 
    // save histograms
    fAnalysisManager->Write();
    fAnalysisManager->CloseFile();
  }
   
  // show Rndm status
  //
  CLHEP::HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BookHisto()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->SetFileName("testem7");
  fAnalysisManager->SetVerboseLevel(1);
  fAnalysisManager->SetActivation(true);  // enable inactivation of histograms

  // Define histograms start values
  const G4int kMaxHisto = 4;
  const G4String id[] = { "0", "1", "2", "3" };
  const G4String title[] = 
                { "dummy",                                      //0
                  "Edep (MeV/mm) along absorber ",              //1
                  "Edep (MeV/mm) along absorber zoomed",        //2
                  "projectile range"                            //3
                 };  

  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 0.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = fAnalysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    G4bool activ = false;
    if (1 == k) activ = true;
    fAnalysisManager->SetActivation(G4VAnalysisManager::kH1, ih, activ);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

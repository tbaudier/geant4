// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: Em3RunAction.cc,v 1.8 2001-03-26 16:01:58 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "Em3RunAction.hh"
#include "Em3RunActionMessenger.hh"

#include "G4Run.hh"
#include "G4Material.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include "g4std/iomanip"

#ifndef G4NOHIST
 #include "CLHEP/Hist/HBookFile.h"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em3RunAction::Em3RunAction(Em3DetectorConstruction* det)
:Detector(det)
{
  runMessenger = new Em3RunActionMessenger(this);   
  saveRndm = 1;
#ifndef G4NOHIST
  // init hbook
  hbookManager = new HBookFile("TestEm3.paw", 68);
  for (G4int k=0; k<MaxAbsor; k++) histo[k] = NULL;
#endif    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em3RunAction::~Em3RunAction()
{
  delete runMessenger;
  
#ifndef G4NOHIST
 // Write histogram file 
  hbookManager->write();  
 // Delete HBOOK stuff
  delete [] histo;
  delete hbookManager;
#endif  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em3RunAction::BeginOfRunAction(const G4Run* aRun)
{  
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  // save Rndm status
  if (saveRndm > 0)
    { HepRandom::showEngineStatus();
      HepRandom::saveEngineStatus("beginOfRun.rndm");
    }
       
  //initialize cumulative quantities
  //
  for (G4int k=0; k<MaxAbsor; k++)     
      sumEAbs[k]=sum2EAbs[k]=sumLAbs[k]=sum2LAbs[k]=0.;

  //drawing
  // 
  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager* UI = G4UImanager::GetUIpointer(); 
      UI->ApplyCommand("/vis/scene/notifyHandlers");
    }
    
  //histograms
  //
  bookHisto();     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em3RunAction::bookHisto()
{
#ifndef G4NOHIST
  // book histograms
  char str[25];
  strcpy(str,"Edep/Ebeam in absorber ");
  G4int nbins=100; G4double vmin=0., vmax=1.;
  G4int NbOfAbsor = Detector->GetNbOfAbsor();
  for (G4int k=0; k<NbOfAbsor; k++)
     {
      str[23] = (char)((int)('0') + k);
      if (histo[k]==NULL)
        { histo[k] = hbookManager->histogram(str,nbins,vmin,vmax);
          G4cout << "bookHisto: " << k << " " << histo[k] << G4endl;
	}  
     }   
#endif   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em3RunAction::SetHisto(G4int idh, G4int nbins, G4double vmin, G4double vmax)
{
#ifndef G4NOHIST
  // (re)book histograms
  char str[25];
  strcpy(str,"Edep/Ebeam in absorber ");
  str[23] = (char)((int)('0') + idh);  
///  if (histo[idh] != NULL) delete histo[idh];
  histo[idh] = hbookManager->histogram(str,nbins,vmin,vmax);
  G4cout << "SetHisto: " << idh << " " << histo[idh] << G4endl;  
#endif   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void Em3RunAction::EndOfRunAction(const G4Run* aRun)
{
  if (G4VVisManager::GetConcreteInstance())
     G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
     
  //compute and print statistic
  //     
  G4int NbOfEvents = aRun->GetNumberOfEvent();  
  G4double norme = 1./NbOfEvents;
  
  G4double MeanEAbs,rmsEAbs,MeanLAbs,rmsLAbs;
  
  G4long oldform = G4cout.setf(G4std::ios::fixed,G4std::ios::floatfield);
  G4int  oldprec = G4cout.precision(2);
    
  G4cout << "\n-------------------------------------------------------------\n"
         << G4std::setw(51) << "total energy dep" << G4std::setw(30) << "total tracklen \n \n";
	   
  for (G4int k=0; k<Detector->GetNbOfAbsor(); k++)
    {
     MeanEAbs = norme*sumEAbs[k];
      rmsEAbs = norme*sqrt(abs(NbOfEvents*sum2EAbs[k] - sumEAbs[k]*sumEAbs[k]));
  
     MeanLAbs = norme*sumLAbs[k];
      rmsLAbs = norme*sqrt(abs(NbOfEvents*sum2LAbs[k] - sumLAbs[k]*sumLAbs[k]));
  
     //print
     //    
     G4cout
     << " Absorber" << k 
     << " (" << G4std::setw(12) << Detector->GetAbsorMaterial(k)->GetName() << ") :" 
     << G4std::setw( 7) << G4BestUnit(MeanEAbs,"Energy") << " +- "
     << G4std::setw( 5) << G4BestUnit( rmsEAbs,"Energy")
     << G4std::setw(12) << G4BestUnit(MeanLAbs,"Length") << " +- "
     << G4std::setw( 5) << G4BestUnit( rmsLAbs,"Length")
     << G4endl;
    }
    
  G4cout << "\n-------------------------------------------------------------";
  G4cout << G4endl;  
  G4cout.setf(oldform,G4std::ios::floatfield);
  G4cout.precision(oldprec);
    
  // save Rndm status
  if (saveRndm > 0)
    { HepRandom::showEngineStatus();
      HepRandom::saveEngineStatus("endOfRun.rndm");
    }                         
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

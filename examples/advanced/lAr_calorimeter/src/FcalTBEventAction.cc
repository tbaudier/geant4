// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: FcalTBEventAction.cc,v 1.3 2002-10-01 14:05:13 araujo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "FcalTBEventAction.hh"

#include "ExN03CalorHit.hh"

//#include "g4rw/tvordvec.h"
#include "g4std/vector"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include "ExN03SteppingAction.hh"

#include "G4ios.hh"
#include "iostream.h"
#include "fstream.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

FcalTBEventAction::FcalTBEventAction(ExN03SteppingAction* SA)
:calorimeterCollID(-1),drawFlag("all"),printModulo(10), StepAction(SA)
{;}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

FcalTBEventAction::~FcalTBEventAction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void FcalTBEventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
    if (evtNb%printModulo == 0)
    { 
      G4cout << "\n---> Begin of event: " << evtNb+1 << G4endl;
//      HepRandom::showEngineStatus();
    }
    
  NTracksOutOfWorld = 0;
  NSecondaries = 0;
 
  StepAction->initialize(evtNb+1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void FcalTBEventAction::EndOfEventAction(const G4Event* evt)
{
  NTracksOutOfWorld = StepAction->GetOutOfWorldTracks(0, 0); 
  G4cout << "N Tracks out of world " << NTracksOutOfWorld << endl;

  // Write Leaving Particles in File
  //--------------------------------
  G4String FileName1 = "OutTracks_802_1mm.dat";
  G4int iostemp1;
  if(Init1 == 0) {
    iostemp1 = ios::out;
    Init1++;
  } else {
    iostemp1 = ios::out|ios::app; // ios::app;  
  };
  ofstream OutTracks(FileName1, iostemp1);

  OutTracks << NTracksOutOfWorld << endl;
  for(G4int i=1; i<= NTracksOutOfWorld ; i++){
    for(G4int j=1; j<11 ; j++) {
      OutTracks <<  StepAction->GetOutOfWorldTracks(i,j) << " " ; }
    OutTracks << endl;
  }
  OutTracks.close();


  NSecondaries = StepAction->GetSecondaries(0,0);
  G4cout << "N Scondaries " << NSecondaries << endl;   
  
  // Write Secondary Particles in File
  //--------------------------------
  G4String FileName2 = "SecndTracks_802_1mm.dat";
  G4int iostemp2;
  if(Init2 == 0) {
    iostemp2 = ios::out;
    Init2++;
  } else {
    iostemp2 = ios::out|ios::app; // ios::app;  
  };

  ofstream SecndTracks(FileName2, iostemp2);

  SecndTracks << NSecondaries << endl;
  for(i=1; i<= NSecondaries ; i++){
    for(G4int j=1; j<11 ; j++) {
      SecndTracks <<  StepAction->GetSecondaries(i,j) << " " ; }
    SecndTracks << endl;
  }
  SecndTracks.close();


  // Write Edep in FCAL1 and FCAL2 
  G4String FileName3 = "EdepFCAL_802_1mm.dat";
  G4int iostemp3;
  if(Init3 == 0) {
    iostemp3 = ios::out;
    Init3++;
  } else {
    iostemp3 = ios::out|ios::app; // ios::app;  
  };

  ofstream EdepFCAL(FileName3, iostemp3);
  
  EdepFCAL << StepAction->GetEdepFCAL("FCALEm") << " ";
  EdepFCAL << StepAction->GetEdepFCAL("FCALHad"); 
  EdepFCAL << endl;
  EdepFCAL.close();

  G4cout << "Edep in FCAL1 FCAl2 : " << StepAction->GetEdepFCAL("FCALEm") << " ";
  G4cout << StepAction->GetEdepFCAL("FCALHad") << endl;
}


  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

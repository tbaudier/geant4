#define hTestTrackingAction_CPP

//---------------------------------------------------------------------------
//
// ClassName:   hTestTrackingAction
//  
// Description: Implementation file for MC truth.
//
// Author:      V.Ivanchenko 17/03/01
//
// Modified:
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "hTestTrackingAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

hTestTrackingAction::hTestTrackingAction():
  theHisto(hTestHisto::GetPointer())
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

hTestTrackingAction::~hTestTrackingAction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void hTestTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{

  if(1 < theHisto->GetVerbose()) {
    G4cout << "hTestTrackingAction: Next track #" 
           << aTrack->GetTrackID() << G4endl;
  }

  G4bool primary = false;
  if(0 == aTrack->GetParentID()) primary = true;

  //Save primary parameters

  if(primary) {

    G4double kinE = aTrack->GetKineticEnergy();
    theHisto->SaveToTuple("TKIN", kinE/MeV);      

    G4ParticleDefinition* particle = aTrack->GetDefinition();

    G4double mass = 0.0;
    if(particle) {
      mass = particle->GetPDGMass();
      theHisto->SaveToTuple("MASS", mass/MeV);      
      theHisto->SaveToTuple("CHAR",(particle->GetPDGCharge())/eplus);      
    }

    G4ThreeVector pos = aTrack->GetPosition();
    G4ThreeVector dir = aTrack->GetMomentumDirection();
    theHisto->SaveToTuple("X0",(pos.x())/mm);
    theHisto->SaveToTuple("Y0",(pos.y())/mm);
    theHisto->SaveToTuple("Z0",(pos.z())/mm);
  

    if(1 < theHisto->GetVerbose()) {
      G4cout << "hTestTrackingAction: kinE(MeV)= " << kinE/MeV
           << "; m(MeV)= " << mass/MeV   
           << "; pos= " << pos << ";  dir= " << dir << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....









//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef myHpGe_h
#define myHPGeSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class myDetectorConstruction;
class G4HCofThisEvent;
class G4Step;
#include "mySensorHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class myHPGeSD : public G4VSensitiveDetector
{
  public:
  
      myHPGeSD(G4String, myDetectorConstruction* );
     ~myHPGeSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
  
      mySensorHitsCollection*  HPGeCollection;   
      myDetectorConstruction* HPGe;
      G4int*                   HitHPGeID;
};

#endif






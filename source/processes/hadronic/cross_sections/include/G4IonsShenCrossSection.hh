
#ifndef G4IonsShenCrossSection_h
#define G4IonsShenCrossSection_h
//
// Class Description
// Implementation of formulas 
// Shen et al. Nuc. Phys. A 491 130 (1989); 
// Total Reaction Cross Section for Heavy-Ion Collisions 
//
// Class Description - End
// 18-Sep-2003 First version is written by T. Koi

#include "globals.hh"
#include "G4Proton.hh"

#include "G4VCrossSectionDataSet.hh"

class G4IonsShenCrossSection : public G4VCrossSectionDataSet
{
   public:
      G4IonsShenCrossSection ():
         r0 ( 1.1 )
      {
      }
   
   virtual
   G4bool IsApplicable(const G4DynamicParticle* aDP, const G4Element*)
   {
      G4int baryonNumber = aDP->GetDefinition()->GetBaryonNumber();
      G4double kineticEnergy = aDP->GetKineticEnergy(); 
      if ( kineticEnergy / baryonNumber >= 10*MeV ) 
         return true;
      return false;
   }

   virtual
   G4double GetCrossSection(const G4DynamicParticle*, 
                            const G4Element*, G4double aTemperature);

   virtual
   void BuildPhysicsTable(const G4ParticleDefinition&)
   {}

   virtual
   void DumpPhysicsTable(const G4ParticleDefinition&) 
   {G4cout << "G4IonsShenCrossSection: uses Shen formula"<<G4endl;}

   private:
      const G4double r0;

      G4double calEcmValue ( const G4double , const G4double , const G4double ); 
      G4double calCeValue ( const G4double ); 
};

#endif

#include "Physics_LHEP_PRECO.hh"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include "g4std/iomanip"   

#include "GeneralPhysics.hh"
#include "EMPhysics.hh"
#include "MuonPhysics.hh"
#include "HadronPhysicsLHEP_PRECO.hh"
#include "IonPhysics.hh"

Physics_LHEP_PRECO::Physics_LHEP_PRECO():  G4VModularPhysicsList()
{
  // default cut value  (1.0mm) 
  // defaultCutValue = 1.0*mm;
  defaultCutValue = 0.7*mm;  
  SetVerboseLevel(1);

  // General Physics
  RegisterPhysics( new GeneralPhysics("general") );

  // EM Physics
  RegisterPhysics( new EMPhysics("standard EM"));

  // Muon Physics
  RegisterPhysics(  new MuonPhysics("muon"));

   // Hadron Physics
  RegisterPhysics(  new HadronPhysicsLHEP_PRECO("hadron"));

  // Ion Physics
  RegisterPhysics( new IonPhysics("ion"));


}

Physics_LHEP_PRECO::~Physics_LHEP_PRECO()
{
}

void Physics_LHEP_PRECO::SetCuts()
{
  if (verboseLevel >1){
    G4cout << "Physics_LHEP_PRECO::SetCuts:";
  }  
  //  " G4VUserPhysics_LHEP_PRECO::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 

  SetCutsWithDefault();   
  
  SetCutValue(kNuCut,"nu_e");
  SetCutValue(kNuCut,"nu_tau");
  SetCutValue(kNuCut,"nu_mu");
  SetCutValue(kNuCut,"anti_nu_e");
  SetCutValue(kNuCut,"anti_nu_tau");
  SetCutValue(kNuCut,"anti_nu_mu");
 
  DumpCutValues(G4Electron::ElectronDefinition() ); 
  DumpCutValues(G4Positron::PositronDefinition()); 
  DumpCutValues(G4MuonPlus::MuonPlusDefinition() ); 
  DumpCutValues(G4MuonMinus::MuonMinusDefinition()); 
  DumpCutValues(G4Gamma::GammaDefinition() ) ;
 
  
}



// 2002 by J.P. Wellisch

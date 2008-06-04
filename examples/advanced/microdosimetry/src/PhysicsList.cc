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
// -------------------------------------------------------------------
// $Id: PhysicsList.cc,v 1.1 2008-06-04 12:57:55 sincerti Exp $
// -------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "PhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PhysicsList::PhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 1*nanometer;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;
  cutForProton    = defaultCutValue;
  
  SetVerboseLevel(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PhysicsList::~PhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::ConstructParticle()
{
  ConstructBosons();
  ConstructLeptons();
  ConstructBarions();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::ConstructBosons()
{ 

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}
 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::ConstructLeptons()
{
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//DNA
#include "G4DNAGenericIonsManager.hh"
//ENDDNA

void PhysicsList::ConstructBarions()
{
  //  baryons
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();

  // Geant4 DNA new particles
  G4DNAGenericIonsManager * genericIonsManager;
  genericIonsManager=G4DNAGenericIonsManager::Instance();
  genericIonsManager->GetIon("alpha++");
  genericIonsManager->GetIon("alpha+");
  genericIonsManager->GetIon("helium");
  genericIonsManager->GetIon("hydrogen");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4MultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4LowEnergyPhotoElectric.hh"
#include "G4LowEnergyCompton.hh"
#include "G4LowEnergyGammaConversion.hh"
#include "G4LowEnergyRayleigh.hh"

#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"

#include "G4hLowEnergyIonisation.hh"
#include "G4StepLimiter.hh"

// Geant4 DNA header files

#include "G4DNAGenericIonsManager.hh"
#include "G4FinalStateProduct.hh"
#include "G4DNAProcess.hh"

#include "G4CrossSectionExcitationEmfietzoglou.hh"
#include "G4FinalStateExcitationEmfietzoglou.hh"

#include "G4CrossSectionElasticScreenedRutherford.hh"
#include "G4FinalStateElasticScreenedRutherford.hh"
#include "G4FinalStateElasticBrennerZaider.hh"

#include "G4CrossSectionExcitationBorn.hh"
#include "G4FinalStateExcitationBorn.hh"

#include "G4CrossSectionIonisationBorn.hh"
#include "G4FinalStateIonisationBorn.hh"

#include "G4CrossSectionIonisationRudd.hh"
#include "G4FinalStateIonisationRudd.hh"

#include "G4CrossSectionExcitationMillerGreen.hh"
#include "G4FinalStateExcitationMillerGreen.hh"

#include "G4CrossSectionChargeDecrease.hh"
#include "G4FinalStateChargeDecrease.hh"

#include "G4CrossSectionChargeIncrease.hh"
#include "G4FinalStateChargeIncrease.hh"

// Processes definition

char name1[]="ElasticScreenedRutherford";
typedef G4DNAProcess<G4CrossSectionElasticScreenedRutherford,G4FinalStateElasticScreenedRutherford,name1> 
  ElasticScreenedRutherford;

char name1bis[]="ElasticBrennerZaider";
typedef G4DNAProcess<G4CrossSectionElasticScreenedRutherford,G4FinalStateElasticBrennerZaider,name1bis> 
  ElasticBrennerZaider;

char name2[]="ExcitationEmfietzoglou";
typedef G4DNAProcess<G4CrossSectionExcitationEmfietzoglou,G4FinalStateExcitationEmfietzoglou,name2> 
  ExcitationEmfietzoglou;
  
char name3[]="ExcitationBorn";
typedef G4DNAProcess<G4CrossSectionExcitationBorn,G4FinalStateExcitationBorn,name3> 
  ExcitationBorn;
  
char name4[]="IonisationBorn";
typedef G4DNAProcess<G4CrossSectionIonisationBorn,G4FinalStateIonisationBorn,name4> 
  IonisationBorn;
  
char name5[]="IonisationRudd";
typedef G4DNAProcess<G4CrossSectionIonisationRudd,G4FinalStateIonisationRudd,name5> 
  IonisationRudd;
  
char name6[]="ExcitationMillerGreen";
typedef G4DNAProcess<G4CrossSectionExcitationMillerGreen,G4FinalStateExcitationMillerGreen,name6> 
  ExcitationMillerGreen;
  
char name7[]="ChargeDecrease";
typedef G4DNAProcess<G4CrossSectionChargeDecrease,G4FinalStateChargeDecrease,name7> 
  ChargeDecrease;
  
char name8[]="ChargeIncrease";
typedef G4DNAProcess<G4CrossSectionChargeIncrease,G4FinalStateChargeIncrease,name8> 
  ChargeIncrease;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::ConstructEM()
{
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {

    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

// DNA processes per particle type

   if (particleName == "e-") {
       pmanager->AddDiscreteProcess(new ExcitationEmfietzoglou);
       pmanager->AddDiscreteProcess(new ElasticScreenedRutherford);
       pmanager->AddDiscreteProcess(new ElasticBrennerZaider);
       pmanager->AddDiscreteProcess(new IonisationBorn);

    } else if ( particleName == "proton" ) {
       pmanager->AddDiscreteProcess(new ExcitationMillerGreen);
       pmanager->AddDiscreteProcess(new ExcitationBorn);
       pmanager->AddDiscreteProcess(new IonisationRudd);
       pmanager->AddDiscreteProcess(new IonisationBorn);
       pmanager->AddDiscreteProcess(new ChargeDecrease);

    } else if ( particleName == "hydrogen" ) {
       pmanager->AddDiscreteProcess(new IonisationRudd);
       pmanager->AddDiscreteProcess(new ChargeIncrease);

    } else if ( particleName == "alpha" ) {
       pmanager->AddDiscreteProcess(new ExcitationMillerGreen);
       pmanager->AddDiscreteProcess(new IonisationRudd);
       pmanager->AddDiscreteProcess(new ChargeDecrease);
    
    } else if ( particleName == "alpha+" ) {
       pmanager->AddDiscreteProcess(new ExcitationMillerGreen);
       pmanager->AddDiscreteProcess(new IonisationRudd);
       pmanager->AddDiscreteProcess(new ChargeDecrease);
       pmanager->AddDiscreteProcess(new ChargeIncrease);
    
    } else if ( particleName == "helium" ) {
       pmanager->AddDiscreteProcess(new ExcitationMillerGreen);
       pmanager->AddDiscreteProcess(new IonisationRudd);
       pmanager->AddDiscreteProcess(new ChargeIncrease);
    }
  } // Loop on particles
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::ConstructGeneral()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhysicsList::SetCuts()
{
  if (verboseLevel >0)
  {
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }  
  
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  
  // set cut values for proton and anti_proton before all other hadrons
  // because some processes for hadrons need cut values for proton/anti_proton 
  SetCutValue(cutForProton, "proton");
  SetCutValue(cutForProton, "anti_proton");
  
  if (verboseLevel>0) DumpCutValuesTable();
}

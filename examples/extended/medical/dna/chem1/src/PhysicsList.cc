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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// J. Comput. Phys. 274 (2014) 841-882
// The Geant4-DNA web site is available at http://geant4-dna.org
//
// $Id$
//
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class

#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmDNAChemistry.hh"
#include "G4DNAChemistryManager.hh"
#include "G4PhysicsConstructorRegistry.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
: G4VModularPhysicsList(), fpChemList(new G4EmDNAChemistry())
{
  double currentDefaultCut   = 1.*nanometer;

  // fixe lower limit for cut
  G4ProductionCutsTable::GetProductionCutsTable()->
      SetEnergyRange(100*eV, 1*GeV);

  SetDefaultCutValue(currentDefaultCut);
  SetVerboseLevel(1);

  // EM physics  
  RegisterConstructor("G4EmDNAPhysics");

  G4DNAChemistryManager::Instance()->SetChemistryList(fpChemList);
  // G4DNAChemistryManager will manage the pointer of the user chemistry list
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::RegisterConstructor(const G4String& name)
{
  RegisterPhysics(G4PhysicsConstructorRegistry::Instance()->
      GetPhysicsConstructor(name));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
  fpChemList->ConstructMolecule();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();

  // Contruct processes of the chemistry list
  /*
   * WARNING:
   * The instruction below should be called *AFTER*
   * G4VModularPhysicsList::ConstructProcess()
   * which defines physics processes
   * In G4EmDNAChemistry::ConstructProcess, the vibrational excitation
   * model is extended only if this one has been defined.
   */
  fpChemList->ConstructProcess();
}

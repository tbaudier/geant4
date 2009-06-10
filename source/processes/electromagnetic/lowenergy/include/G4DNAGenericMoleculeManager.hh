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
//
// ----------------- History -----------------
//
//     2 Apr 2009 - A.Mantero - created


#ifndef   G4DNAGENERICMOLECULEMANAGER_HH
 #define  G4DNAGENERICMOLECULEMANAGER_HH 1
 
 #include "globals.hh"
 #include "G4DNAGenericIonsManager.hh"
#include "G4ParticleTable.hh"
#include "G4DynamicMolecule.hh"

 #include <map>
 
 class G4Molecule;
 
 class G4DNAGenericMoleculeManager
 {
  public:
   static G4DNAGenericMoleculeManager *      Instance(void);
   G4Molecule *                GetMolecule(const G4String & name);

  private:
                                         G4DNAGenericMoleculeManager();
                                        ~G4DNAGenericMoleculeManager();

                                         G4DNAGenericMoleculeManager(const G4DNAGenericMoleculeManager &);
   const G4DNAGenericMoleculeManager        &operator=(const G4DNAGenericMoleculeManager &);

   static G4DNAGenericMoleculeManager *      theInstance;


   void CreateDecayChannels();
   
   typedef std::map<G4String, G4Molecule *> MoleculeMap;

   MoleculeMap map;

   G4DNAGenericIonsManager * ionManager;

 };
#endif /* G4DNAGENERICMOLECULEMANAGER_HH */

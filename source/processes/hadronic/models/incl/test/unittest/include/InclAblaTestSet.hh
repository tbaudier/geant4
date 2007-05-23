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
// $Id: InclAblaTestSet.hh,v 1.1 2007-05-23 09:55:55 miheikki Exp $ 
// Translation of INCL4.2/ABLA V3 
// Pekka Kaitaniemi, HIP (translation)
// Christelle Schmidt, IPNL (fission code)
// Alain Boudard, CEA (contact person INCL/ABLA)
// Aatos Heikkinen, HIP (project coordination)

// INCL/ABLA unit test

#ifndef InclAblaTestSet_hh
#define InclAblaTestSet_hh 1

#include <list>

#include "TObject.h"
#include "TString.h"

#include "InclAblaTest.hh"

class InclAblaTestSet : public TObject {

public:
  InclAblaTestSet();
  InclAblaTestSet(TString name);
  ~InclAblaTestSet();

  // Accessors
  void setName(TString name);
  TString getName();

  void addTest(InclAblaTest *aTest);
  std::list<InclAblaTest*> getListOfTests();

  void runTests();

private:
  TString theTestSetName; // The name of the software unit to be tested.

  std::list<InclAblaTest*> theListOfTests; // List of tests in this test set

  ClassDef(InclAblaTestSet, 0) // Set of tests
    };	

#endif

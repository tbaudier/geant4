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
// INCL++ intra-nuclear cascade model
// Pekka Kaitaniemi, CEA and Helsinki Institute of Physics
// Davide Mancusi, CEA
// Alain Boudard, CEA
// Sylvie Leray, CEA
// Joseph Cugnon, University of Liege
//
#define INCLXX_IN_GEANT4_MODE 1

#include "globals.hh"

/*
 * \file G4INCLRandomSeedVector.cc
 *
 * \date 17 May 2013
 * \author Davide Mancusi
 */

#include "G4INCLRandomSeedVector.hh"

namespace G4INCL {
  namespace Random {

    G4int SeedVector::at(const size_t i) const { return theSeeds.at(i); }
    G4int &SeedVector::operator[](const size_t i) { return theSeeds[i]; }
    const G4int &SeedVector::operator[](const size_t i) const { return theSeeds[i]; }
    size_t SeedVector::size() const { return theSeeds.size(); }
    void SeedVector::resize(const size_t n) { theSeeds.resize(n); }
    void SeedVector::push_back(const G4int val) { theSeeds.push_back(val); }
    void SeedVector::clear() { theSeeds.clear(); }

    std::ostream &operator<<(std::ostream &out, SeedVector const &sv) {
      if(sv.size()<=0)
        return out;
      for(size_t i=0; i<sv.size()-1; ++i)
        out << sv.at(i) << '\t';
      out << sv.at(sv.size()-1);
      return out;
    }

  }
}

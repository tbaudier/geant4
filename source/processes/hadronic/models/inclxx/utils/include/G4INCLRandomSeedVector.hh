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
 * \file G4INCLRandomSeedVector.hh
 *
 * \date 17 May 2013
 * \author Davide Mancusi
 */

#ifndef G4INCLRANDOMSEEDVECTOR_HH_
#define G4INCLRANDOMSEEDVECTOR_HH_

#include <vector>
#include <ostream>

namespace G4INCL {

  namespace Random {

    class SeedVector {
      public:
        G4int at(const size_t i) const;
        G4int &operator[](const size_t i);
        const G4int &operator[](const size_t i) const;
        size_t size() const;
        void resize(const size_t n);
        void push_back(const G4int val);
        void clear();

        friend std::ostream &operator<<(std::ostream &out, SeedVector const &sv);

      private:
        std::vector<G4int> theSeeds;
    };

  }
}

#endif /* G4INCLIRANDOMSEEDVECTOR_HH_ */


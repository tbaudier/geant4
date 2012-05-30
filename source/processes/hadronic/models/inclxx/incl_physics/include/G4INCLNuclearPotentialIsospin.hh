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
// INCL++ revision: v5.1_rc11
//
#define INCLXX_IN_GEANT4_MODE 1

#include "globals.hh"

/** \file G4INCLNuclearPotentialIsospin.hh
 * \brief Isospin-dependent nuclear potential.
 *
 * Provides an isospin-dependent nuclear potential.
 *
 * Created on: 28 February 2011
 *     Author: Davide Mancusi
 */

#ifndef G4INCLNUCLEARPOTENTIALISOSPIN_HH
#define G4INCLNUCLEARPOTENTIALISOSPIN_HH 1

#include "G4INCLINuclearPotential.hh"

namespace G4INCL {

  namespace NuclearPotential {

    class NuclearPotentialIsospin : public INuclearPotential {

      public:
        NuclearPotentialIsospin(NuclearDensity const * const density, const G4bool pionPotential, const G4bool hardFermiSphere=true);
        virtual ~NuclearPotentialIsospin();

        virtual G4double computePotentialEnergy(const Particle * const p) const;

      private:
        G4double vProton, vNeutron;
        G4double vDeltaPlusPlus, vDeltaPlus, vDeltaZero, vDeltaMinus;

        void initialize();

    };

  }
}

#endif /* G4INCLNUCLEARPOTENTIALISOSPIN_HH */

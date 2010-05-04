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
// $Id: G4PhysicsVectorCash.hh,v 1.1 2010-05-04 16:15:36 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
//---------------------------------------------------------------
//      GEANT 4 class header file
//
//  G4PhysicsVector.hh
//
//  Class description:
//
//    This class includes cash variables for G4PhysicsVector 
//
//  History:
//   04.May.2010 First Implementation       Hisaya
//---------------------------------------------------------------

#ifndef G4PhysicsVectorCash_h
#define G4PhysicsVectorCash_h 1

#include "globals.hh"

class G4PhysicsVectorCash 
{

  public:  

    G4PhysicsVectorCash(){
      lastEnergy  = -DBL_MAX;
      lastValue   = 0.;
      lastBin     =0  ; 
    }
         // constructor

    ~G4PhysicsVectorCash(){}
         // destructor

    G4double lastEnergy;        // Cache the last input value
    G4double lastValue;         // Cache the last output value   
    size_t lastBin;             // Cache the last bin location

};

#endif

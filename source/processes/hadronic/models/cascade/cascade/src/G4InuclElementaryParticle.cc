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
// $Id: G4InuclElementaryParticle.cc,v 1.4 2010-03-16 22:10:26 mkelsey Exp $
// Geant4 tag: $Name: not supported by cvs2svn $

#include "G4InuclElementaryParticle.hh"

#include "G4ParticleDefinition.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Gamma.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZero.hh"
#include "G4AntiKaonZero.hh"
#include "G4Lambda.hh"
#include "G4SigmaPlus.hh"
#include "G4SigmaZero.hh"
#include "G4SigmaMinus.hh"
#include "G4XiZero.hh"
#include "G4XiMinus.hh"
#include "G4Diproton.hh"
#include "G4UnboundPN.hh"
#include "G4Dineutron.hh"

// Convert internal particle codes to GEANT4 standard pointers
//                     known particle types:
//      1 - proton          11 - k+         111 - quasideuteron PP
//      2 - neutron         13 - k-         112 - quasideuteron PN
//      3 - pi+             15 - k0         122 - quasideuteron NN
//      5 - pi-             17 - k0bar
//      7 - pi 0            21 - lambda 
//     10 - photon          23 - sigma+
//                          25 - sigma0
//                          27 - sigma-
//                          29 - xi0
//                          31 - xi-

G4ParticleDefinition* 
G4InuclElementaryParticle::makeDefinition(G4int ityp) {
  switch(ityp) {
  case 1:   return G4Proton::Definition(); break;
  case 2:   return G4Neutron::Definition(); break;
  case 3:   return G4PionPlus::Definition(); break;
  case 5:   return G4PionMinus::Definition(); break;
  case 7:   return G4PionZero::Definition(); break;
  case 10:  return G4Gamma::Definition(); break;
  case 11:  return G4KaonPlus::Definition(); break;
  case 13:  return G4KaonMinus::Definition(); break;
  case 15:  return G4KaonZero::Definition(); break;
  case 17:  return G4AntiKaonZero::Definition(); break;
  case 21:  return G4Lambda::Definition(); break;
  case 23:  return G4SigmaPlus::Definition(); break;
  case 25:  return G4SigmaZero::Definition(); break;
  case 27:  return G4SigmaMinus::Definition(); break;
  case 29:  return G4XiZero::Definition(); break;
  case 31:  return G4XiMinus::Definition(); break;
  case 111: return G4Diproton::Definition(); break;	// Bertini class!
  case 112: return G4UnboundPN::Definition(); break;	// Bertini class!
  case 122: return G4Dineutron::Definition(); break;	// Bertini class!
  default:
    G4cerr << " uups, unknown particle type " << ityp << G4endl;
  }
  
  return 0;
}

// This is the inverse mapping to makeDefinition above

G4int G4InuclElementaryParticle::type(const G4ParticleDefinition *pd) {
  if (pd == 0) return 0;
  if (pd == G4Proton::Definition())       return 1;
  if (pd == G4Neutron::Definition())      return 2;
  if (pd == G4PionPlus::Definition())     return 3;
  if (pd == G4PionMinus::Definition())    return 5;
  if (pd == G4PionZero::Definition())     return 7;
  if (pd == G4Gamma::Definition())        return 10;
  if (pd == G4KaonPlus::Definition())     return 11;
  if (pd == G4KaonMinus::Definition())    return 13;
  if (pd == G4KaonZero::Definition())     return 15;
  if (pd == G4AntiKaonZero::Definition()) return 17;
  if (pd == G4Lambda::Definition())       return 21;
  if (pd == G4SigmaPlus::Definition())    return 23;
  if (pd == G4SigmaZero::Definition())    return 25;
  if (pd == G4SigmaMinus::Definition())   return 27;
  if (pd == G4XiZero::Definition())       return 29;
  if (pd == G4XiMinus::Definition())      return 31;
  if (pd == G4Diproton::Definition())     return 111;	// Bertini class!
  if (pd == G4UnboundPN::Definition())    return 112;	// Bertini class!
  if (pd == G4Dineutron::Definition())    return 122;	// Bertini class!

  G4cerr << " uups, unknown G4ParticleDefinition type" << G4endl;
  return 0;
}

void G4InuclElementaryParticle::setType(G4int ityp) {
  setDefinition(makeDefinition(ityp));
}


// Assignment operator for use with std::sort()
G4InuclElementaryParticle& 
G4InuclElementaryParticle::operator=(const G4InuclElementaryParticle& right) {
  generation = right.generation;
  G4InuclParticle::operator=(right);
  return *this;
}


G4double G4InuclElementaryParticle::getStrangeness(G4int type) {
  G4ParticleDefinition* pd = makeDefinition(type);
  return pd ? (pd->GetQuarkContent(3) - pd->GetAntiQuarkContent(3)) : 0.;
}

G4double G4InuclElementaryParticle::getParticleMass(G4int type) {
  G4ParticleDefinition* pd = makeDefinition(type);
  return pd ? pd->GetPDGMass()*MeV/GeV : 0.0;	// From G4 to Bertini units
}

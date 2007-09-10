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
// $Id: functorTest.cc,v 1.3 2007-09-10 22:05:01 tinslay Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
// J. Tinslay, May 2007. Functor demonstration.
//
#include "G4GPRFunctor.hh"
#include "G4GPRProcessWrappers.hh"
#include "G4VDiscreteProcess.hh"
#include "G4VParticleChange.hh"

using namespace G4GPRProcessWrappers;
// Regular G4VProcess
struct VProcess : public G4VDiscreteProcess
{
  VProcess():G4VDiscreteProcess("test"){}
  G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*){return 0;}

  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&) {
    G4cout<<"Execute VProcess::PostStepDoIt"<<G4endl;
    return 0;
  }
};

// Alternative process
struct OtherProcess
{
  G4VParticleChange* operator()(const G4Track&, const G4Step&)
  {
    G4cout<<"Execute OtherProcess::operator"<<G4endl;
    return 0;
  }
};

// Process function
G4VParticleChange* MyFunction(const G4Track&, const G4Step&) {
  G4cout<<"Execute MyFunction"<<G4endl;
  return 0;
}

int main(int argc, char** argv) {

  G4VProcess* vProcess = new VProcess();
  OtherProcess* otherProcess = new OtherProcess();
  
  G4GPRDoItWrapper wrapper1("VProcess", vProcess, &G4VProcess::PostStepDoIt);
  G4GPRDoItWrapper wrapper2("OtherProcess", otherProcess, &OtherProcess::operator());
  G4GPRDoItWrapper wrapper3("MyFunction", &MyFunction);
  
  G4Track* dummyTrk(0); 
  G4Step* dummyStep(0);
  
  wrapper1(*dummyTrk, *dummyStep);
  wrapper2(*dummyTrk, *dummyStep);
  wrapper3(*dummyTrk, *dummyStep);
  
  return 0;
}

// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: MySteppingAction.cc,v 1.6 2000-08-03 09:12:38 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// 16/Apr/1997  J. Allison:  For visualization/test/test19.
// 07/Oct/1998  G. Barrand: adapted to tests.

#include "MySteppingAction.hh"

#ifdef G4VIS_USE_OPACS
#include "MyRunAction.hh"
#endif

#include "G4SteppingManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Polyline.hh"

void MySteppingAction::UserSteppingAction(const G4Step*) {
#ifdef G4VIS_USE_OPACS
  OHistogram h1 = MyRunAction::get_1d();
  OHistogram h2 = MyRunAction::Get2d();

  HepRandom::setTheEngine (&theJamesEngine);
  double     james = G4RandGauss::shoot(0.3,0.1);
  OHistogramFillOneDimensional(h1,james,0.01);

  HepRandom::setTheEngine(&theDRand48Engine);
  double     d48 = G4RandGauss::shoot(0.7,0.1);
  OHistogramFillTwoDimensional(h2,james,d48,0.01);   
#endif
}





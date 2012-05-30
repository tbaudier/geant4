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

#include "G4INCLXXInterface.hh"
#include "G4INCLXXFactory.hh"
#include <cmath>
#include "G4GenericIon.hh"
#include "CLHEP/Random/Random.h"
#include "G4INCLCascade.hh"
#include "G4ReactionProductVector.hh"
#include "G4ReactionProduct.hh"
#include "G4INCLXXInterfaceConfig.hh"

G4INCLXXInterfaceMessenger *theINCLXXInterfaceMessenger = NULL;

G4INCLXXInterface::G4INCLXXInterface(const G4String& nam)
  :G4VIntraNuclearTransportModel(nam),
  theInterfaceConfig(G4INCLXXInterfaceConfig::GetInstance())
{
  // Register this interface with the G4INCLXXInterfaceConfig singleton
  theInterfaceConfig->RegisterINCLXXInterface(this);

  // Use the environment variable G4INCLXX_NO_DE_EXCITATION to disable de-excitation
  if(getenv("G4INCLXX_NO_DE_EXCITATION")) {
    G4cout <<"INCL++ Interface: WARNING: De-excitation is completely disabled!" << G4endl;
    theExcitationHandler = 0;
  } else {
    theExcitationHandler = new G4ExcitationHandler;
  }

  // Create the Config object
  theConfig = new G4INCL::Config;

  // The INCL model will be created at the first use
  theINCLModel = NULL;

}

G4INCLXXInterface::~G4INCLXXInterface()
{
  // the Config object is deleted by the INCL model
  if(theINCLModel)
    delete theINCLModel;
  else
    delete theConfig;

  delete theExcitationHandler;
}

G4bool G4INCLXXInterface::shouldUseInverseKinematics(const G4HadProjectile &aTrack, const G4Nucleus &theNucleus) {
  // Use direct kinematics if the projectile is a nucleon or a pion
  const G4ParticleDefinition *projectileDef = aTrack.GetDefinition();
  if(projectileDef == G4Proton::Proton()
     || projectileDef == G4Neutron::Neutron()
     || projectileDef == G4PionPlus::PionPlus()
     || projectileDef == G4PionZero::PionZero()
     || projectileDef == G4PionMinus::PionMinus())
    return false;

  // Here all projectiles should be light nuclei
  const G4int pA = projectileDef->GetAtomicMass();
  if(pA<=0) {
    G4cout << "INCL++ WARNING: the model does not know how to handle a collision between a "
      << projectileDef->GetParticleName()
      << " projectile and a Z=" << theNucleus.GetZ_asInt()
      << ", A=" << theNucleus.GetA_asInt()
      << G4endl;
    return true;
  }

  // Use direct kinematics if the target is larger theMaxProjMass
  const G4int tA = theNucleus.GetA_asInt();
  if(tA>theInterfaceConfig->GetMaxProjMass())
    return false;

  // If either nucleus is a LCP (A<=4), run the collision as light on heavy
  if(tA<=4 || pA<=4) {
    if(pA<tA)
      return false;
    else
      return true;
  }

  // In all other cases, use the global setting
  return theInterfaceConfig->GetUseInverseKinematics();
}

G4HadFinalState* G4INCLXXInterface::ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& theNucleus)
{
  const G4int maxTries = 200;

  // Check if inverse kinematics should be used
  const G4bool inverseKinematics = shouldUseInverseKinematics(aTrack, theNucleus);

  // If we are running in inverse kinematics, redefine aTrack and theNucleus
  G4LorentzRotation *toInverseKinematics = NULL;
  G4LorentzRotation *toDirectKinematics = NULL;
  G4HadProjectile const *aProjectileTrack = &aTrack;
  G4Nucleus *theTargetNucleus = &theNucleus;
  if(inverseKinematics) {
    G4ParticleTable * const theParticleTable = G4ParticleTable::GetParticleTable();
    const G4int oldTargetA = theNucleus.GetA_asInt();
    const G4int oldTargetZ = theNucleus.GetZ_asInt();
    G4ParticleDefinition *oldTargetDef = theParticleTable->GetIon(oldTargetZ, oldTargetA, 0.0);
    const G4ParticleDefinition *oldProjectileDef = aTrack.GetDefinition();

    if(oldProjectileDef != 0 && oldTargetDef != 0) {
      const G4int newTargetA = oldProjectileDef->GetAtomicMass();
      const G4int newTargetZ = oldProjectileDef->GetAtomicNumber();

      if(newTargetA > 0 && newTargetZ > 0) {
        // This should give us the same energy per nucleon
        theTargetNucleus = new G4Nucleus(newTargetA, newTargetZ);
        const G4double theProjectileMass = theParticleTable->GetIonTable()->GetIonMass(oldTargetZ, oldTargetA);
        toInverseKinematics = new G4LorentzRotation(aTrack.Get4Momentum().boostVector());
        G4LorentzVector theProjectile4Momentum(0.0, 0.0, 0.0, theProjectileMass);
        G4DynamicParticle swappedProjectileParticle(oldTargetDef, (*toInverseKinematics) * theProjectile4Momentum);
        aProjectileTrack = new G4HadProjectile(swappedProjectileParticle);
      } else {
        G4cout <<"Badly defined target after swapping. Falling back to normal (non-swapped) mode." << G4endl;
      }
    } else {
      G4cout <<"oldProjectileDef or oldTargetDef was null" << G4endl;
    }
  }

  // INCL assumes the projectile particle is going in the direction of
  // the Z-axis. Here we construct proper rotation to convert the
  // momentum vectors of the outcoming particles to the original
  // coordinate system.
  G4LorentzVector projectileMomentum = aProjectileTrack->Get4Momentum();

  // INCL++ assumes that the projectile is going in the direction of
  // the z-axis. In principle, if the coordinate system used by G4
  // hadronic framework is defined differently we need a rotation to
  // transform the INCL++ reaction products to the appropriate
  // frame. Please note that it isn't necessary to apply this
  // transform to the projectile because when creating the INCL++
  // projectile particle G4INCLXXFactory::createProjectile needs to
  // use only the projectile energy (direction is simply assumed to be
  // along z-axis).
  G4RotationMatrix toZ;
  toZ.rotateZ(-projectileMomentum.phi());
  toZ.rotateY(-projectileMomentum.theta());
  G4RotationMatrix toLabFrame3 = toZ.inverse();
  G4LorentzRotation toLabFrame(toLabFrame3);
  // However, it turns out that the projectile given to us by G4
  // hadronic framework is already going in the direction of the
  // z-axis so this rotation is actually unnecessary. Both toZ and
  // toLabFrame turn out to be unit matrices as can be seen by
  // uncommenting the folowing two lines:
  // G4cout <<"toZ = " << toZ << G4endl;
  // G4cout <<"toLabFrame = " << toLabFrame << G4endl;

  theResult.Clear(); // Make sure the output data structure is clean.
  theResult.SetStatusChange(stopAndKill);

  std::list<G4Fragment> remnants;

  G4int nTries = 0;
  // INCL can generate transparent events. However, this is meaningful
  // only in the standalone code. In Geant4 we must "force" INCL to
  // produce a valid cascade.
  G4bool eventIsOK = false;
  do {
    const G4INCL::ParticleSpecies theSpecies = G4INCLXXFactory::toINCLParticleSpecies(*aProjectileTrack);
    const G4double kineticEnergy = G4INCLXXFactory::toINCLKineticEnergy(*aProjectileTrack);
    if(!theINCLModel) {
      theConfig->setClusterMaxMass(theInterfaceConfig->GetMaxClusterMass());
      theINCLModel = new G4INCL::INCL(theConfig);
    }

    if(theInterfaceConfig->GetDumpInput()) {
      G4cout << theINCLModel->configToString() << G4endl;
    }
    const G4INCL::EventInfo eventInfo = theINCLModel->processEvent(theSpecies, kineticEnergy, theTargetNucleus->GetA_asInt(), theTargetNucleus->GetZ_asInt());
    //    eventIsOK = !eventInfo.transparent && nTries < maxTries;
    eventIsOK = !eventInfo.transparent;
    if(eventIsOK) {

      // If the collision was run in inverse kinematics, prepare to boost back
      // all the reaction products
      if(inverseKinematics) {
        toDirectKinematics = new G4LorentzRotation(toInverseKinematics->inverse());
      }

      for(G4int i = 0; i < eventInfo.nParticles; i++) {
	G4int A = eventInfo.A[i];
	G4int Z = eventInfo.Z[i];
	//	G4cout <<"INCL particle A = " << A << " Z = " << Z << G4endl;
	G4double kinE = eventInfo.EKin[i];
	G4double px = eventInfo.px[i];
	G4double py = eventInfo.py[i];
	G4double pz = eventInfo.pz[i];
	G4DynamicParticle *p = G4INCLXXFactory::toG4Particle(A, Z , kinE, px, py, pz);
	if(p != 0) {
	  G4LorentzVector momentum = p->Get4Momentum();

          // Apply the toLabFrame rotation
          momentum *= toLabFrame;
          // Apply the inverse-kinematics boost
          if(inverseKinematics) {
            momentum *= *toDirectKinematics;
            momentum.setVect(-momentum.vect());
          }

	  // Set the four-momentum of the reaction products
	  p->Set4Momentum(momentum);
	  theResult.AddSecondary(p);

	} else {
	  G4cout <<"Warning: INCL++ produced a particle that couldn't be converted to Geant4 particle." << G4endl;
	}
      }

      for(G4int i = 0; i < eventInfo.nRemnants; i++) {
	const G4int A = eventInfo.ARem[i];
	const G4int Z = eventInfo.ZRem[i];
	//	G4cout <<"INCL particle A = " << A << " Z = " << Z << G4endl;
	const G4double kinE = eventInfo.EKinRem[i];
	const G4double px = eventInfo.pxRem[i];
	const G4double py = eventInfo.pyRem[i];
	const G4double pz = eventInfo.pzRem[i];
        G4ThreeVector spin(
            eventInfo.jxRem[i]*hbar_Planck,
            eventInfo.jyRem[i]*hbar_Planck,
            eventInfo.jzRem[i]*hbar_Planck
            );
	const G4double excitationE = eventInfo.EStarRem[i];
	const G4double nuclearMass = G4NucleiProperties::GetNuclearMass(A, Z) + excitationE;
	const G4double scaling = G4INCLXXFactory::remnant4MomentumScaling(nuclearMass,
								    kinE,
								    px, py, pz);
	G4LorentzVector fourMomentum(scaling * px, scaling * py, scaling * pz,
				     nuclearMass + kinE);
	if(std::abs(scaling - 1.0) > 0.01) {
	  G4cout <<"INCL++ warning: momentum scaling = " << scaling << G4endl
	  <<"                Lorentz vector = " << fourMomentum << G4endl
	  <<"                A = " << A << ", Z = " << Z << G4endl
	  <<"                E* = " << excitationE << ", nuclearMass = " << nuclearMass << G4endl
          <<"                remnant i=" << i << ", nRemnants=" << eventInfo.nRemnants << G4endl
	  <<"                Reaction was: " << aTrack.GetKineticEnergy()/MeV << "-MeV "
          << aTrack.GetDefinition()->GetParticleName() << " + "
          << G4ParticleTable::GetParticleTable()->GetIon(theNucleus.GetZ_asInt(), theNucleus.GetA_asInt(), 0.0)->GetParticleName()
          << ", in " << (inverseKinematics ? "inverse" : "direct") << " kinematics."
          << G4endl;
	}

        // Apply the toLabFrame rotation
        fourMomentum *= toLabFrame;
        spin *= toLabFrame3;
        // Apply the inverse-kinematics boost
        if(inverseKinematics) {
          fourMomentum *= *toDirectKinematics;
          fourMomentum.setVect(-fourMomentum.vect());
        }

	G4Fragment remnant(A, Z, fourMomentum);
        remnant.SetAngularMomentum(spin);
	remnants.push_back(remnant);
      }
    }
    nTries++;
  } while(!eventIsOK && nTries < maxTries);
  
  // Clean up the objects that we created for the inverse kinematics
  if(inverseKinematics) {
    delete aProjectileTrack;
    delete theTargetNucleus;
    delete toInverseKinematics;
    delete toDirectKinematics;
  }

  // De-excitation:

  if(theExcitationHandler != 0) {
    for(std::list<G4Fragment>::const_iterator i = remnants.begin();
	i != remnants.end(); i++) {
      G4ReactionProductVector *deExcitationResult = theExcitationHandler->BreakItUp((*i));
    
      for(G4ReactionProductVector::iterator fragment = deExcitationResult->begin();
	  fragment != deExcitationResult->end(); ++fragment) {
	G4ParticleDefinition *def = (*fragment)->GetDefinition();
	if(def != 0) {
	  G4DynamicParticle *theFragment = new G4DynamicParticle(def, (*fragment)->GetMomentum());
	  theResult.AddSecondary(theFragment);
	}
      }

      for(G4ReactionProductVector::iterator fragment = deExcitationResult->begin();
	  fragment != deExcitationResult->end(); ++fragment) {
	delete (*fragment);
      }
      deExcitationResult->clear();
      delete deExcitationResult;    
    }
  }

  remnants.clear();

  return &theResult;
}
  
G4ReactionProductVector* G4INCLXXInterface::Propagate(G4KineticTrackVector* , G4V3DNucleus* ) {
  return 0;
}



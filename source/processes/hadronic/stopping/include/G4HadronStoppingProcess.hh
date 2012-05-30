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
// $Id: G4HadronStoppingProcess.hh,v 1.23 2008-10-02 20:57:52 dennis Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------
//
// GEANT4 Class header file
//
// File name:     G4HadronStoppingProcess
//
// Author V.Ivanchenko 21 April 2012 on base of G4MuonMinusCaptureAtRest
//
//
// Class Description:
//
// Base process class for nuclear capture of negatively charged particles
//
// Modifications: 
//
//------------------------------------------------------------------------

#ifndef G4HadronStoppingProcess_h
#define G4HadronStoppingProcess_h 1
 
#include "globals.hh"
#include "G4HadronicProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ElementSelector.hh"
#include "G4HadronicInteraction.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ForceCondition.hh"
#include "G4HadronicProcessType.hh"
#include "G4HadFinalState.hh"

class G4HadronStoppingProcess : public G4HadronicProcess
{ 
public:
 
  G4HadronStoppingProcess(const G4String& name = "hadronCaptureAtRest");

  virtual ~G4HadronStoppingProcess();

  virtual G4bool IsApplicable(const G4ParticleDefinition&);

  virtual void PreparePhysicsTable(const G4ParticleDefinition&);

  virtual void BuildPhysicsTable(const G4ParticleDefinition&);

  virtual G4double 
  AtRestGetPhysicalInteractionLength(const G4Track& track,
				     G4ForceCondition* condition);

  virtual G4double 
  PostStepGetPhysicalInteractionLength(const G4Track& track,
				       G4double   previousStepSize,
				       G4ForceCondition* condition);

  virtual G4VParticleChange* AtRestDoIt(const G4Track&, const G4Step&);

  virtual void ProcessDescription(std::ostream& outFile) const;

  inline void SetElementSelector(G4ElementSelector* ptr);

  inline void SetEmCascade(G4HadronicInteraction* ptr);

  inline void SetBoundDecay(G4HadronicInteraction* ptr);

private:

  // hide assignment operator as private 
  G4HadronStoppingProcess& operator=(const G4HadronStoppingProcess &right);
  G4HadronStoppingProcess(const G4HadronStoppingProcess& );

  G4ElementSelector* fElementSelector;

  G4HadronicInteraction* fEmCascade;
  G4HadronicInteraction* fBoundDecay;

  G4HadFinalState result;   
};

inline void 
G4HadronStoppingProcess::SetElementSelector(G4ElementSelector* ptr)
{
  if(fElementSelector != ptr) {
    delete fElementSelector;
    fElementSelector = ptr;
  }
}

inline void 
G4HadronStoppingProcess::SetEmCascade(G4HadronicInteraction* ptr)
{
  fEmCascade = ptr;
}

inline void 
G4HadronStoppingProcess::SetBoundDecay(G4HadronicInteraction* ptr)
{
  fBoundDecay = ptr;
}

#endif
 






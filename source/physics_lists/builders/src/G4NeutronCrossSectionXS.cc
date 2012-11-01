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
// $Id: G4NeutronCrossSectionXS.cc,v 1.4 2010-07-29 10:52:14 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//---------------------------------------------------------------------------
//
// ClassName:   G4NeutronCrossSectionXS
//
// Author: 3 June 2010 V. Ivanchenko
//
// Modified:
//
//----------------------------------------------------------------------------
//
// Inelastic and capture cross section from XS database added
// on top of existing Physics List

#include "G4NeutronCrossSectionXS.hh"
#include "G4DataQuestionaire.hh"

//#include "G4NeutronInelasticXS.hh"
#include "G4NeutronCaptureXS.hh"

#include "G4ParticleDefinition.hh"
#include "G4HadronicProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4HadronicProcessType.hh"

#include "G4Neutron.hh"

G4NeutronCrossSectionXS::G4NeutronCrossSectionXS(G4int ver) :
  G4VPhysicsConstructor("NeutronXS"), verbose(ver) 
{
  G4DataQuestionaire q(no,no,no,no,no,neutronxs);
}

G4NeutronCrossSectionXS::~G4NeutronCrossSectionXS() 
{}

void G4NeutronCrossSectionXS::ConstructParticle() 
{
  G4Neutron::Neutron();
}

void G4NeutronCrossSectionXS::ConstructProcess() 
{

//  G4NeutronInelasticXS* xinel = new G4NeutronInelasticXS();
  G4NeutronCaptureXS* xcap = new G4NeutronCaptureXS();

  const G4ParticleDefinition* neutron = G4Neutron::Neutron();
  if(verbose > 1) {
    G4cout << "### G4NeutronCrossSectionXS: use alternative neutron X-sections"
	   << G4endl;
  }

  G4ProcessVector* pv = neutron->GetProcessManager()->GetProcessList();
  G4int n = pv->size();
  G4HadronicProcess* had = 0;
  for(G4int i=0; i<n; i++) {
    if(fHadronInelastic == ((*pv)[i])->GetProcessSubType()) {
//      had = static_cast<G4HadronicProcess*>((*pv)[i]);
//      had->AddDataSet(xinel);
    } else if(fCapture == ((*pv)[i])->GetProcessSubType()) {
      had = static_cast<G4HadronicProcess*>((*pv)[i]);
      had->AddDataSet(xcap);
    }
  }
}

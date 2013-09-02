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
#include "G4UserWorkerInitialization.hh"
#include "G4WorkerThread.hh"
#include "G4WorkerRunManager.hh"
#include "G4MTRunManagerKernel.hh"
#include "G4VUserActionInitialization.hh"
#include "G4UImanager.hh"
#include "G4VUserPhysicsList.hh"
#include "G4AutoLock.hh"
#include <sstream>

//Will need this for TPMalloc
//#ifdef G4MULTITHREADED
//#define TPMALLOCDEFINESTUB
//#include "tpmalloc/tpmallocstub.h"
//#endif

#ifdef G4MULTITHREADED
G4Thread* G4UserWorkerInitialization::CreateAndStartWorker(G4WorkerThread* wTC)
{
    //Note: this method is called by G4MTRunManager, here we are still sequential
    //Create a new thread/worker structure
    G4Thread* worker = new G4Thread;
    G4THREADCREATE(worker,&G4MTRunManagerKernel::StartThread , wTC );
    return worker;
}
#else
G4Thread* G4UserWorkerInitialization::CreateAndStartWorker(G4WorkerThread*)
{
    return new G4Thread;
}
#endif

//Avoid compilation warning in sequential
#ifdef G4MULTITHREADED
void G4UserWorkerInitialization::JoinWorker(G4Thread* aThread)
{
    G4THREADJOIN(*aThread);
}
#else
void G4UserWorkerInitialization::JoinWorker(G4Thread*)
{
}
#endif

G4UserWorkerInitialization::G4UserWorkerInitialization()
{;}

G4UserWorkerInitialization::~G4UserWorkerInitialization()
{;}

void G4UserWorkerInitialization::WorkerInitialize() const
{;}

void G4UserWorkerInitialization::WorkerStart() const
{;}

void G4UserWorkerInitialization::WorkerRunStart() const
{;}

void G4UserWorkerInitialization::WorkerRunEnd() const
{;}

void G4UserWorkerInitialization::WorkerStop() const
{;}

void G4UserWorkerInitialization::SetUserAction(G4VUserPrimaryGeneratorAction* action) const
{ G4RunManager::GetRunManager()->SetUserAction(action); } 

void G4UserWorkerInitialization::SetUserAction(G4UserRunAction* action) const
{ G4RunManager::GetRunManager()->SetUserAction(action); } 

void G4UserWorkerInitialization::SetUserAction(G4UserEventAction* action) const
{ G4RunManager::GetRunManager()->SetUserAction(action); } 

void G4UserWorkerInitialization::SetUserAction(G4UserStackingAction* action) const
{ G4RunManager::GetRunManager()->SetUserAction(action); } 

void G4UserWorkerInitialization::SetUserAction(G4UserTrackingAction* action) const
{ G4RunManager::GetRunManager()->SetUserAction(action); } 

void G4UserWorkerInitialization::SetUserAction(G4UserSteppingAction* action) const
{ G4RunManager::GetRunManager()->SetUserAction(action); } 

namespace {
    G4Mutex rngCreateMutex = G4MUTEX_INITIALIZER;
}

void G4UserWorkerInitialization::SetupRNGEngine(const CLHEP::HepRandomEngine* aNewRNG) const
{
    //No default available, let's create the instance of random stuff
    //A Call to this just forces the creation to defaults
    G4Random::getTheEngine();
    //Poor man's solution to check which RNG Engine is used in master thread
    // Need to make these calls thread safe
    G4AutoLock l(&rngCreateMutex);
    if ( dynamic_cast<const CLHEP::HepJamesRandom*>(aNewRNG) ) { G4Random::setTheEngine(new CLHEP::HepJamesRandom); return; }
    if ( dynamic_cast<const CLHEP::RanecuEngine*>(aNewRNG) ) { G4Random::setTheEngine(new CLHEP::RanecuEngine); return; }
    if ( dynamic_cast<const CLHEP::Ranlux64Engine*>(aNewRNG) ) { G4Random::setTheEngine(new CLHEP::Ranlux64Engine); return; }
    if ( dynamic_cast<const CLHEP::MTwistEngine*>(aNewRNG) ) { G4Random::setTheEngine(new CLHEP::MTwistEngine); return; }
    if ( dynamic_cast<const CLHEP::DualRand*>(aNewRNG) ) { G4Random::setTheEngine(new CLHEP::DualRand); return; }
    if ( dynamic_cast<const CLHEP::RanluxEngine*>(aNewRNG) ) { G4Random::setTheEngine(new CLHEP::RanluxEngine); return;}
    if ( dynamic_cast<const CLHEP::RanshiEngine*>(aNewRNG) ) { G4Random::setTheEngine(new CLHEP::RanshiEngine); return; }
}

G4WorkerRunManager* G4UserWorkerInitialization::CreateWorkerRunManager() const
{
    return new G4WorkerRunManager();
}


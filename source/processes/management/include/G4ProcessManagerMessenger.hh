// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ProcessManagerMessenger.hh,v 1.4 1999-11-07 17:11:44 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//---------------------------------------------------------------
//
//  G4ProcessManagerMessenger.hh
//
// Class Description:
//    This is a messenger class to interface to exchange information
//    between ProcessManager and UI.
//-
//  /particle/process/   Process Manager control commands.
//   Commands : 
//     dump * dump process manager information.
//     Verbose * Set Verbose Level for Process Manager and/or process
//     Activate * Activate process  
//     inactivate * Inctivate process  
//
//  History:
//    13 June 1997, H. Kurashige   : The 1st version created.
//    10 Nov. 1997  H. Kurashige   : fixed bugs 
//    08 Jan. 1998, H. Kurashige   : new UIcommand
//
//---------------------------------------------------------------

#ifndef G4ProcessManagerMessenger_h
#define G4ProcessManagerMessenger_h 1

class G4ParticleDefinition;
class G4ParticleTable;
class G4ProcessManager;
class G4ProcessVector;
class G4VProcess;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger; 
class G4UIcommand;

#include "G4UImessenger.hh"
#include "globals.hh"

class G4ProcessManagerMessenger: public G4UImessenger
{
  public:
    G4ProcessManagerMessenger(G4ParticleTable* pTable = 0);
    // constructor

    virtual ~G4ProcessManagerMessenger();
    // destructor 
 
public: // with description
     virtual void SetNewValue(G4UIcommand * command,G4String newValues);
    // set new value for command string

    virtual G4String GetCurrentValue(G4UIcommand * command);
    // get current value for command string
  
  private:
    G4ProcessManagerMessenger(const G4ProcessManagerMessenger&){};
    // hide copy constructor as private

  private:
    G4ParticleDefinition* SetCurrentParticle();
    // set particle currently concerned 
    
  private:
    G4ParticleTable* theParticleTable;
    G4ParticleDefinition* currentParticle;
    G4VProcess* currentProcess;
    G4ProcessManager*     theManager;
    G4ProcessVector*      theProcessList;

    G4UIdirectory *             thisDirectory;
    G4UIcmdWithAnInteger *      dumpCmd;
    G4UIcommand          *      verboseCmd;
    G4UIcmdWithAnInteger *      activateCmd;
    G4UIcmdWithAnInteger *      inactivateCmd;
};

#endif


//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4ExceptionHandler.hh,v 1.1 2002-08-19 18:33:29 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file 
//
//
//      ---------------- G4ExceptionHandler ----------------
//
// Authors: M.Asai - August 2002
//
// ------------------------------------------------------------
//
// Class description:
//
// Abstract base class which need to be notified when G4Exception occurs.
// The concrete class object derived from this base class will be automatically 
// registered to G4StateManager and the virtual method Notify() will be invoked 
// when G4Exception occurs.

// ------------------------------------------------------------

#ifndef G4ExceptionHandler_h
#define G4ExceptionHandler_h 1

#include "globals.hh"
#include "G4VExceptionHandler.hh"
#include "G4ExceptionSeverity.hh"

class G4ExceptionHandler : public G4VExceptionHandler
{

public:

  G4ExceptionHandler();
  virtual ~G4ExceptionHandler();
  G4int operator==(const G4ExceptionHandler &right) const;
  G4int operator!=(const G4ExceptionHandler &right) const;

public: // with description

  virtual G4bool Notify(const char* originOfException,
                        const char* exceptionCode,
                        G4ExceptionSeverity severity,
                        const char* description);
    // Virtual method which will be invoked by G4StateManager when
    // G4Exception occurs.
    // If TRUE returned, core dump will be generated, while FALSE returned,
    // program execution continues.

private:

  G4ExceptionHandler(const G4ExceptionHandler &right);
  G4ExceptionHandler& operator=(const G4ExceptionHandler &right);

};

#endif

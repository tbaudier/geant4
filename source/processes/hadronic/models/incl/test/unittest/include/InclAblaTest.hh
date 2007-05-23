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
// $Id: InclAblaTest.hh,v 1.1 2007-05-23 09:55:55 miheikki Exp $ 
// Translation of INCL4.2/ABLA V3 
// Pekka Kaitaniemi, HIP (translation)
// Christelle Schmidt, IPNL (fission code)
// Alain Boudard, CEA (contact person INCL/ABLA)
// Aatos Heikkinen, HIP (project coordination)

// INCL/ABLA unit test

#ifndef InclAblaTest_hh
#define InclAblaTest_hh 1

#include "TObject.h"
#include "TString.h"

// INCL4/ABLA headers
//#include "init_incl42.hh"
//#include "incl4.2.hh"
//#include "fission_distri.hh"
//#include "abla_v3p.hh"

#ifndef __CINT__
//#include "commonwrapper.hh"
//#include "functionwrapper.hh"
#endif

class InclAblaTest : public TObject {

protected:
  InclAblaTest();

  ~InclAblaTest();

public:
  virtual void runMe() = 0;

  // Accessors
  void setUnitName(TString name);
  TString getUnitName();

  void setOriginalUnitName(TString name);
  TString getOriginalUnitName();

  void setPlotFileName(TString name);
  TString getPlotFileName();

  void setLogFileName(TString name);
  TString getLogFileName();

  void setLinesOfCode(Int_t lines);
  Int_t getLinesOfCode();
  
  Bool_t getTestStatus();
  void setTestStatus(Bool_t status);

  Double_t getErrorMarginal();
  void setErrorMarginal(Double_t marginal);
  
  void writeTestLogFile();
  
private:
  TString unitName; // The name of the software unit to be tested.
  TString originalUnitName; // The name of the corresponding FORTRAN software unit.
  TString plotFileName; // The name of the plot file generated by the test.
  TString logFileName; // The name of the log file generated by the test.

  Int_t linesOfCode; // How many lines of FORTRAN code
  Bool_t theTestStatus; // Test status: false = fail, true = pass
  Double_t theErrorMarginal; // The error marginal for the test.
  Double_t theIntegral; // Integral over histogram/graph differences.
  
  ClassDef(InclAblaTest, 0) // Test base class
};	

#endif

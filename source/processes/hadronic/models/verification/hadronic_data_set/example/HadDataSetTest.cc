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
// $Id: HadDataSetTest.cc,v 1.3 2004-01-30 14:58:57 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - HadDataSetTest 
//
// --------------------------------------------------------------
// Comments: Test for handling of hadronic data set
//           Vladimir.Grichine@cern.ch
//   	     Simone.Gilardoni@cern.ch
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "Randomize.hh"
#include "G4Element.hh"


#include "G4VHadDataManager.hh"
#include "G4HadDataHandler.hh"
#include "G4HadDataReading.hh"
#include "G4VHadDataWriting.hh"
#include "G4EXFORHadData.hh"
#include "G4MSUHadData.hh"
#include "G4ReadHadTotalXSC.hh"
#include "G4ReadHadDoubleDiffXSC.hh"
#include "G4HadFileFinder.hh"
#include "G4HadFileSpec.hh"

#include <fstream>
#include <strstream>

#include <string.h>

int main(int argc, char** argv) 
{
  G4double a, z, density;
  G4int iz,n;
  G4String name, symbol;

  G4String proton("proton");
  G4String pionmi("pi-");
  G4String neutron("neutron");
  G4String nothing("");

  if( argc < 2 ) 
  {
       G4cout << "Input file is not specified! Exit" << G4endl;
       exit(1);
  } 
  std::ifstream* fin = new std::ifstream();
  G4String fname = argv[1];
  fin->open(fname.c_str());
   
  if( !fin->is_open()) 
  {
       G4cout << "Input file <" << fname << "> does not exist! Exit" << G4endl;
       exit(1);
  }  
  G4String whatIdo;
   
  (*fin) >> whatIdo;
   
   
  ////
  //
  // Material definition
  //
  /// 
   
  density = 2.700*g/cm3;
  a = 26.98*g/mole;
  G4Material* Al = new G4Material(name="Aluminium", z=13., a, density);
   
  density = 2.265*g/cm3;
  a = 12.011*g/mole;
  G4Material* C = new G4Material(name="Carbon", z=6., a, density);
   
  density = 0.534*g/cm3;
  a = 6.941*g/mole;
  G4Material* Li = new G4Material(name="Li", z=3., a, density);
   
   
   
   
  if (whatIdo=="1")
  {
       
       // File conversion from EXFOR data
       
       // G4String fn = "Al_n_113MeV";
       
       G4String fn = "C_p_200MeV";
       
       //G4String fn = "C_n_200MeV";
       
       //G4String fn = "Li_n_186MeV";
       
       G4String process="dd";
       
       G4HadFileSpec fileIwant(proton,C,neutron,process);
       
       G4EXFORHadData* exforAl = new G4EXFORHadData(fn,fileIwant);
  }
  else if (whatIdo=="2") 
  {
       char* path = getenv("G4HADATASET");
       G4String pathstarttwo(path);

       G4String temp;
       

       temp="HELPME";
   
       G4HadFileFinder* findit=new G4HadFileFinder(pathstarttwo,temp);
  } 
  else if (whatIdo=="3")
  {
       char* pathstart = getenv("G4HADATASET");
     
       G4String temp;
       G4String pathstarttwo(pathstart);

       G4String neutron("neutron");
     
       G4String process2="dd";
              
       G4HadFileSpec fileIwant(proton,C,neutron,process2);
       
       //G4cout << fileIwant.G4HDSFilename()<< G4endl;
       //G4cout << fileIwant.G4HDSFilepath()<< G4endl;
       
       //  temp=fileIwant.G4HDSFilename();
       //  temp="*Z*A*mat";
       temp="*Z*A*mat";
       pathstarttwo=fileIwant.G4HDSFilepath();
       

       G4HadFileFinder* findit=new G4HadFileFinder(pathstarttwo,temp);    
  }  
  else if (whatIdo=="4") 
  {
       char* pathstart = getenv("G4HADATASET");
       G4String temp;
       G4String pathstarttwo(pathstart);

       G4String neutron("neutron");
       
       G4String process2="dd";
       
       G4HadFileSpec fileIwant(proton,C,neutron,process2);
       
       //G4cout << fileIwant.G4HDSFilename()<< G4endl;
       //G4cout << fileIwant.G4HDSFilepath()<< G4endl;
       
       temp=fileIwant.G4HDSFilename();
       
       // G4HadFileFinder* findit=new G4HadFileFinder(pathstart,temp); 
       
       G4ReadHadDoubleDiffXSC* readExforAl = new G4ReadHadDoubleDiffXSC(fileIwant);    
  }
   
   
  /*
  G4UImanager* UI = G4UImanager::GetUIpointer();  
   
  if (argc==1)   // Define UI terminal for interactive mode  
  { 
     G4UIsession * session = new G4UIterminal;
     UI->ApplyCommand("/control/execute init.mac");    
     session->SessionStart();
     delete session;
  }
  else           // Batch mode
  { 
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);
  }
  */
  // return 0;
}


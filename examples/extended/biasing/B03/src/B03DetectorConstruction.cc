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
// $Id: B03DetectorConstruction.cc,v 1.10 2002-11-22 17:47:58 dressel Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "g4std/strstream"
#include "globals.hh"

#include "B03DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "PhysicalConstants.h"

B03DetectorConstruction::B03DetectorConstruction()
{;}

B03DetectorConstruction::~B03DetectorConstruction()
{;}

G4VPhysicalVolume* B03DetectorConstruction::Construct()
{
  char line[255];
  
  G4double pos_x;
  G4double pos_y;
  G4double pos_z; 

  G4double density, pressure, temperature;
  G4double A;
  G4int Z;

  G4String name, symbol;
  G4double z;
  G4double fractionmass;

  A = 1.01*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , Z= 1, A);

  A = 12.01*g/mole;
  G4Element* elC  = new G4Element(name="Carbon"  ,symbol="C" , Z = 6, A);

  A = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , Z= 8, A);

  A = 22.99*g/mole; 
  G4Element* elNa  = new G4Element(name="Natrium"  ,symbol="Na" , Z=11 , A);

  A = 200.59*g/mole; 
  G4Element* elHg  = new G4Element(name="Hg"  ,symbol="Hg" , Z=80, A);

  A = 26.98*g/mole; 
  G4Element* elAl  = new G4Element(name="Aluminium"  ,symbol="Al" , Z=13, A);

  A = 28.09*g/mole;
  G4Element* elSi  = new G4Element(name="Silicon", symbol="Si", Z=14, A);

  A = 39.1*g/mole; 
  G4Element* elK  = new G4Element(name="K"  ,symbol="K" , Z=19 , A);

  A = 69.72*g/mole; 
  G4Element* elCa  = new G4Element(name="Calzium"  ,symbol="Ca" , Z=31 , A);

  A = 55.85*g/mole;
  G4Element* elFe = new G4Element(name="Iron"    ,symbol="Fe", Z=26, A);

  density     = universe_mean_density;            //from PhysicalConstants.h
  pressure    = 3.e-18*pascal;
  temperature = 2.73*kelvin;
  G4Material *Galactic = 
    new G4Material(name="Galactic", z=1., A=1.01*g/mole, density,
                   kStateGas,temperature,pressure);

  density = 2.03*g/cm3;
  G4Material* Concrete = new G4Material("Concrete", density, 10);
  Concrete->AddElement(elH , fractionmass= 0.01);
  Concrete->AddElement(elO , fractionmass= 0.529);
  Concrete->AddElement(elNa , fractionmass= 0.016);
  Concrete->AddElement(elHg , fractionmass= 0.002);
  Concrete->AddElement(elAl , fractionmass= 0.034);
  Concrete->AddElement(elSi , fractionmass= 0.337);
  Concrete->AddElement(elK , fractionmass= 0.013);
  Concrete->AddElement(elCa , fractionmass= 0.044);
  Concrete->AddElement(elFe , fractionmass= 0.014);
  Concrete->AddElement(elC , fractionmass= 0.001);

  /////////////////////////////
  // world cylinder volume
  ////////////////////////////


  G4double innerRadiusCylinder = 0*cm;
  G4double outerRadiusCylinder = 101*cm; // dont't have scoring
                   // cells coinside eith world volume boundary
  G4double hightCylinder       = 105*cm;
  G4double startAngleCylinder  = 0*deg;
  G4double spanningAngleCylinder    = 360*deg;

  G4Tubs *worldCylinder = new G4Tubs("worldCylinder",
                                     innerRadiusCylinder,
                                     outerRadiusCylinder,
                                     hightCylinder,
                                     startAngleCylinder,
                                     spanningAngleCylinder);

  // logical world

  G4LogicalVolume *worldCylinder_log = 
    new G4LogicalVolume(worldCylinder, Galactic, "worldCylinder_log");

  name = "shieldWorld";
  G4PVPlacement *pWorldVolume = new 
    G4PVPlacement(0, G4ThreeVector(0,0,0), worldCylinder_log,
		  name, 0, false, 0);


  // creating 1 slobs of 180 cm thick concrete

  G4double innerRadiusShield = 0*cm;
  G4double outerRadiusShield = 100*cm;
  G4double hightShield       = 90*cm;
  G4double startAngleShield  = 0*deg;
  G4double spanningAngleShield    = 360*deg;

  G4Tubs *aShield = new G4Tubs("aShield",
                               innerRadiusShield,
                               outerRadiusShield,
                               hightShield,
                               startAngleShield,
                               spanningAngleShield);
  
  // logical shield

  G4LogicalVolume *aShield_log = 
    new G4LogicalVolume(aShield, Concrete, "aShield_log");

  // physical shield

   
  name = "ConcreateBlock";
  
  pos_x = 0*cm;
  pos_y = 0*cm;
  pos_z = 0*cm;
  G4VPhysicalVolume *pvol = 
    new G4PVPlacement(0, 
		      G4ThreeVector(pos_x, pos_y, pos_z),
		      aShield_log, 
		      name, 
		      worldCylinder_log, 
		      false, 
		      0);
  
  return pWorldVolume;
}

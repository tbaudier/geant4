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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//
#include "G4MIRDSkull.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4EllipticalTube.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Ellipsoid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4VSolid.hh"
#include "G4HumanPhantomColour.hh"

G4MIRDSkull::G4MIRDSkull()
{
}

G4MIRDSkull::~G4MIRDSkull()
{

}

G4VPhysicalVolume* G4MIRDSkull::ConstructOrgan(G4VPhysicalVolume* mother,G4bool sensitivity,
					       const G4String& volumeName,
					       const G4String& colourName
					       , G4bool wireFrame)
{
  
  G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
   
  G4cout << "Construct "<<volumeName <<G4endl;
   
  G4Material* skeleton = material -> GetMaterial("skeleton");
 
  delete material;

  // Outer cranium
  G4double ax = 6.8 * cm;//a out skull
  G4double by = 9.8 * cm; // bout
  G4double cz = 8.3 * cm; //cout
 
  G4Ellipsoid* craniumOut =  new G4Ellipsoid("CraniumOut", ax, by, cz);

  ax = 6. * cm; //a in
  by = 9. * cm; //b in 
  cz= 6.5 * cm; // cin
 
  G4Ellipsoid* craniumIn =  new G4Ellipsoid("CraniumIn", ax, by, cz);
 

  G4SubtractionSolid* cranium =  new G4SubtractionSolid("Cranium",
						      craniumOut,
							craniumIn,0,
						      G4ThreeVector(0.0, 0.0,1. * cm));

  G4LogicalVolume* logicSkull = new G4LogicalVolume(cranium, skeleton, 
						    "logical" + volumeName,
						    0, 0, 0);
  
  // Define rotation and position here!
  G4VPhysicalVolume* physSkull = new G4PVPlacement(0,
						   G4ThreeVector(0., 0.,7.75 * cm),
						   "physicalSkull",
						   logicSkull,
						   mother,
						   false,
						   0, true);

  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicSkull->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  //G4VisAttributes* SkullVisAtt = new G4VisAttributes(G4Colour(0.46,0.53,0.6));
 G4HumanPhantomColour* colourPointer = new G4HumanPhantomColour();
  G4Colour colour = colourPointer -> GetColour(colourName);
  G4VisAttributes* SkullVisAtt = new G4VisAttributes(colour);
  SkullVisAtt->SetForceSolid(wireFrame);
  logicSkull->SetVisAttributes(SkullVisAtt);

  G4cout << "Skull created !!!!!!" << G4endl;


  // Testing Skull Volume
  G4double SkullVol = logicSkull->GetSolid()->GetCubicVolume();
  G4cout << "Volume of Skull = " << SkullVol/cm3 << " cm^3" << G4endl;
  
  // Testing Skull Material
  G4String SkullMat = logicSkull->GetMaterial()->GetName();
  G4cout << "Material of Skull = " << SkullMat << G4endl;
  
  // Testing Density
  G4double SkullDensity = logicSkull->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << SkullDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double SkullMass = (SkullVol)*SkullDensity;
  G4cout << "Mass of Skull = " << SkullMass/gram << " g" << G4endl;

  
  return physSkull;
}

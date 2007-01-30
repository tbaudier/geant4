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
#include "G4MIRDMiddleLowerSpine.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4EllipticalTube.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4HumanPhantomColour.hh"

G4MIRDMiddleLowerSpine::G4MIRDMiddleLowerSpine()
{
}

G4MIRDMiddleLowerSpine::~G4MIRDMiddleLowerSpine()
{
}

G4VPhysicalVolume* G4MIRDMiddleLowerSpine::ConstructOrgan(G4VPhysicalVolume* mother, G4bool sensitivity,
							  const G4String& volumeName,
							  const G4String& colourName
							  , G4bool wireFrame )
{
 G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
   
 G4cout << "Construct "<< volumeName  <<G4endl;
   
  G4Material* skeleton = material -> GetMaterial("skeleton");
 
  delete material;
 
  G4double dx = 2. *cm;
  G4double dy = 2.5 *cm;
  G4double dz = 24. *cm;

  G4VSolid* middleLowerSpine = new G4EllipticalTube("MiddleLowerSpine",dx, dy, dz);

  G4LogicalVolume* logicMiddleLowerSpine = new G4LogicalVolume( middleLowerSpine, skeleton,
								"logical" + volumeName,
								0, 0, 0);   
  // Define rotation and position here!
  G4VPhysicalVolume* physMiddleLowerSpine = new G4PVPlacement(0,G4ThreeVector(0.0 *cm, 5.5 * cm,11. * cm),
							      "physicalMiddleLowerSpine",
							      logicMiddleLowerSpine,
							      mother,
							      false,
							      0, true);

  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicMiddleLowerSpine->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  // G4VisAttributes* MiddleLowerSpineVisAtt = new G4VisAttributes(G4Colour(0.46,0.53,0.6));
 
 G4HumanPhantomColour* colourPointer = new G4HumanPhantomColour();
  G4Colour colour = colourPointer -> GetColour(colourName);
  G4VisAttributes* MiddleLowerSpineVisAtt = new G4VisAttributes(colour);
  MiddleLowerSpineVisAtt->SetForceSolid(wireFrame);
  logicMiddleLowerSpine->SetVisAttributes(MiddleLowerSpineVisAtt);

  G4cout << "MiddleLowerSpine created !!!!!!" << G4endl;

  // Testing MiddleLowerSpine Volume
  G4double MiddleLowerSpineVol = logicMiddleLowerSpine->GetSolid()->GetCubicVolume();
  G4cout << "Volume of MiddleLowerSpine = " << MiddleLowerSpineVol/cm3 << " cm^3" << G4endl;
  
  // Testing MiddleLowerSpine Material
  G4String MiddleLowerSpineMat = logicMiddleLowerSpine->GetMaterial()->GetName();
  G4cout << "Material of MiddleLowerSpine = " << MiddleLowerSpineMat << G4endl;
  
  // Testing Density
  G4double MiddleLowerSpineDensity = logicMiddleLowerSpine->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << MiddleLowerSpineDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double MiddleLowerSpineMass = (MiddleLowerSpineVol)*MiddleLowerSpineDensity;
  G4cout << "Mass of MiddleLowerSpine = " << MiddleLowerSpineMass/gram << " g" << G4endl;

  
  return physMiddleLowerSpine;
}

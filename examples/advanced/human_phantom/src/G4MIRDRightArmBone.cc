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

#include "G4MIRDRightArmBone.hh"
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
#include "G4EllipticalCone.hh"
#include "G4HumanPhantomColour.hh"
G4MIRDRightArmBone::G4MIRDRightArmBone()
{
}

G4MIRDRightArmBone::~G4MIRDRightArmBone()
{
}

G4VPhysicalVolume* G4MIRDRightArmBone::ConstructOrgan(G4VPhysicalVolume* mother,G4bool sensitivity,
						     const G4String& volumeName, 
						     const G4String& colourName,G4bool wireFrame)
{
  // Remind! the elliptical cone gives problems! Intersections of volumes, 
  // wrong calculation of the volume!
   
  G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
   
  G4cout << "Construct " << volumeName << G4endl;
   
  G4Material* skeleton = material -> GetMaterial("skeleton");
  
  delete material;

  G4double dx = 1.4 * cm;//a
  G4double dy = 2.7 * cm;//b
  // G4double dz= 46. * cm;//z0

  //G4EllipticalCone* arm = new G4EllipticalCone("OneRightArmBone",dx/2.,dy/2.,dz, 34.5 *cm);
  G4EllipticalTube* rightArm = new G4EllipticalTube("OneRightArmBone",dx,dy,34.5 *cm);
 
  G4LogicalVolume* logicRightArmBone = new G4LogicalVolume(rightArm,
						      skeleton,
						      "logical" + volumeName,
						      0, 0,0);

  G4RotationMatrix* matrix = new G4RotationMatrix();
  matrix -> rotateX(180. * degree);

  G4VPhysicalVolume* physRightArmBone = new G4PVPlacement(matrix,
			       G4ThreeVector(-18.4 * cm, 0.0, -0.5*cm),
						     //-x0
      			       "physicalRightArmBone",
  			       logicRightArmBone,
			       mother,
			       false,0,true);
			      

  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicRightArmBone->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }


  // Visualization Attributes
  //G4VisAttributes* RightArmBoneVisAtt = new G4VisAttributes(G4Colour(0.46,0.53,0.6));
 G4HumanPhantomColour* colourPointer = new G4HumanPhantomColour();
  G4Colour colour = colourPointer -> GetColour(colourName);
  G4VisAttributes* RightArmBoneVisAtt = new G4VisAttributes(colour);

  RightArmBoneVisAtt->SetForceSolid(wireFrame);
  logicRightArmBone->SetVisAttributes(RightArmBoneVisAtt);

  G4cout << "RightArmBone created !!!!!!" << G4endl;
 
  // Testing RightArmBone Volume
  G4double RightArmBoneVol = logicRightArmBone->GetSolid()->GetCubicVolume();
  G4cout << "Volume of RightArmBone = " << RightArmBoneVol/cm3 << " cm^3" << G4endl;
  
  // Testing RightArmBone Material
  G4String RightArmBoneMat = logicRightArmBone->GetMaterial()->GetName();
  G4cout << "Material of RightArmBone = " << RightArmBoneMat << G4endl;
  
  // Testing Density
  G4double RightArmBoneDensity = logicRightArmBone->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << RightArmBoneDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double RightArmBoneMass = (RightArmBoneVol)*RightArmBoneDensity;
  G4cout << "Mass of RightArmBone = " << RightArmBoneMass/gram << " g" << G4endl;
  
  return physRightArmBone;
}

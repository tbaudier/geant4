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
#include "G4MIRDPancreas.hh"

#include "globals.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Ellipsoid.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4HumanPhantomColour.hh"
G4MIRDPancreas::G4MIRDPancreas()
{
}

G4MIRDPancreas::~G4MIRDPancreas()
{

}

G4VPhysicalVolume* G4MIRDPancreas::ConstructOrgan(G4VPhysicalVolume* mother, G4bool sensitivity,
						 const G4String& volumeName, 
						  const G4String& colourName
						  , G4bool wireFrame)
{

  G4cout << "Construct "<< volumeName << G4endl;
 
 G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
 G4Material* soft = material -> GetMaterial("soft_tissue");
 delete material;

  G4double ax= 3.*cm; //c
  G4double by= 1.*cm;//b
  G4double cz= 15.*cm;//a
  G4double zcut1= -15. *cm;// -a
  G4double zcut2= 0.0 *cm; 

  G4Ellipsoid* pancreasFirst =  new G4Ellipsoid("PancreasFirst",ax, by, cz,
						zcut1, zcut2);

  G4double xx = 6. * cm;// 2*c
  G4double yy = 2. * cm;// 2*b
  G4double zz = 12. * cm; // cz - x1 = 3 cm
  G4Box* subtrPancreas = new G4Box("SubtrPancreas",xx/2., yy/2., zz/2.);

  G4SubtractionSolid* pancreas = new G4SubtractionSolid("pancreas",
							pancreasFirst,
							subtrPancreas,
							0, 
							G4ThreeVector(-3 * cm,0.0,-9.*cm));
  // 
  G4LogicalVolume* logicPancreas = new G4LogicalVolume(pancreas, soft,
						       "logical" + volumeName,
						       0, 0, 0);
  G4RotationMatrix* rotation = new G4RotationMatrix();
  rotation ->rotateY(90. * degree);


  G4VPhysicalVolume* physPancreas = new G4PVPlacement(rotation,
						      G4ThreeVector(-0. *cm, 0.0, 2*cm),//x0, 0, 2 cm
      			       "physicalPancreas",
  			       logicPancreas,
			       mother,
			       false,
			       0, true);
  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicPancreas->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  // G4VisAttributes* PancreasVisAtt = new G4VisAttributes(G4Colour(0.28,0.82,0.8));
  G4HumanPhantomColour* colourPointer = new G4HumanPhantomColour();
  G4Colour colour = colourPointer -> GetColour(colourName);
  G4VisAttributes* PancreasVisAtt = new G4VisAttributes(colour);
  PancreasVisAtt->SetForceSolid(wireFrame);
  logicPancreas->SetVisAttributes(PancreasVisAtt);

  G4cout << "Pancreas created !!!!!!" << G4endl;

  // Testing Pancreas Volume
  G4double PancreasVol = logicPancreas->GetSolid()->GetCubicVolume();
  G4cout << "Volume of Pancreas = " << PancreasVol/cm3 << " cm^3" << G4endl;
  
  // Testing Pancreas Material
  G4String PancreasMat = logicPancreas->GetMaterial()->GetName();
  G4cout << "Material of Pancreas = " << PancreasMat << G4endl;
  
  // Testing Density
  G4double PancreasDensity = logicPancreas->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << PancreasDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double PancreasMass = (PancreasVol)*PancreasDensity;
  G4cout << "Mass of Pancreas = " << PancreasMass/gram << " g" << G4endl;

  
  return physPancreas;
}

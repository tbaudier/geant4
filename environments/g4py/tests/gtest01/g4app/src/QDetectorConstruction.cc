// $Id: QDetectorConstruction.cc,v 1.1 2006-02-27 10:05:24 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   QDetectorConstruction.cc
//
//                                         2005 Q
// ====================================================================
#include "QDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

// ====================================================================
//
// class description
//
// ====================================================================

// constants (detector parameters)
static const G4double DXYZ_AREA= 30.*cm;
static const G4double DW= 10.*cm;

//////////////////////////////////////////////
QDetectorConstruction::QDetectorConstruction()
//////////////////////////////////////////////
{
}

///////////////////////////////////////////////
QDetectorConstruction::~QDetectorConstruction()
///////////////////////////////////////////////
{
}

/////////////////////////////////////////////////////
G4VPhysicalVolume* QDetectorConstruction::Construct()
/////////////////////////////////////////////////////
{
  G4Material* mate;
  G4VisAttributes* va;

  // ==============================================================
  // world volume
  // ==============================================================
  G4Box* areaSolid= new G4Box("AREA", 
			      DXYZ_AREA/2., DXYZ_AREA/2., DXYZ_AREA/2.);

  G4Material* vacuum= G4Material::GetMaterial("Vacuum");
  G4LogicalVolume* areaLV= new G4LogicalVolume(areaSolid, vacuum, "AREA_LV");
  G4PVPlacement* area= new G4PVPlacement(0, G4ThreeVector(), "AREA_PV", 
					 areaLV, 0, false, 0);
  // vis. attributes
  va= new G4VisAttributes(G4Color(1.,1.,1.));
  va-> SetForceWireframe(true);
  areaLV-> SetVisAttributes(va);
  
  // ==============================================================
  // detectors
  // ==============================================================
  // voxel
  const G4double dvoxel= 10.*mm;
  const G4double dl= 10.*cm;

  G4Box* svoxel= new G4Box("voxel", dvoxel, dl, dvoxel);
  mate= G4Material::GetMaterial("Vacuum");
  G4LogicalVolume* lvoxel= new G4LogicalVolume(svoxel, mate, "voxel");
  va= new G4VisAttributes(G4Color(0.,0.8,0.8));
  va-> SetVisibility(false);
  lvoxel-> SetVisAttributes(va);

  G4int ix, iz;
  G4int index=0;
  for (iz=0; iz<5; iz++) {
    for (ix=-7; ix<=7; ix++) {
      G4double x0= (2.*ix)*cm;
      G4double z0= (-13.+2.*iz)*cm;
      G4PVPlacement* pvoxel= new 
	G4PVPlacement(0, G4ThreeVector(x0, 0., z0),
		      lvoxel, "voxel", areaLV, false, index);
      index++;
    }
  }

  // tube
  //G4Tubs* stube= new G4Tubs("tube", 15./2.*mm, 19./2.*mm, dl,
  G4Tubs* stube= new G4Tubs("tube", 0.*mm, 19./2.*mm, dl,
			    0., 360.*deg);
  mate= G4Material::GetMaterial("Al");
  G4LogicalVolume* ltube= new G4LogicalVolume(stube, mate, "tube");
  va= new G4VisAttributes(G4Color(0.,0.8,0.8));
  ltube-> SetVisAttributes(va);

  G4RotationMatrix* rmtube= new G4RotationMatrix;
  rmtube-> rotateX(-90.*deg);
  G4PVPlacement* ptube= new 
    G4PVPlacement(rmtube, G4ThreeVector(),
		  ltube, "tube", lvoxel, false, 0);
  
  // cal
  const G4double dxycal= 25.*mm;
  const G4double dzcal= 3.*cm;

  G4Box* scal= new G4Box("cal", dxycal, dxycal, dzcal);
  mate= G4Material::GetMaterial("CsI");
  G4LogicalVolume* lcal= new G4LogicalVolume(scal, mate, "cal");
  va= new G4VisAttributes(G4Color(0.5,0.5,0.));
  lcal-> SetVisAttributes(va);

  index= 0;
  for (ix=-2; ix<=2; ix++) {
    G4double x0= (5.*ix)*cm;
    G4PVPlacement* pcal= new 
      G4PVPlacement(0, G4ThreeVector(x0, 0., 2.*cm),
		    lcal, "cal", areaLV, false, index);
    index++;
  }

  return area;
}


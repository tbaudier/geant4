#include "RemSimVGeometryComponent.hh"
#include "RemSimMaterial.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "RemSimShieldingDecorator.hh"
#include "RemSimDecorator.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "RemSimSensitiveDetector.hh"
#include "RemSimROGeometry.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"

RemSimShieldingDecorator::RemSimShieldingDecorator(RemSimVGeometryComponent* comp)
  : RemSimDecorator(comp)
{
 shieldingX = 5.*m;
 shieldingY = 5.*m;
 shieldingZ = 10.*cm;  
 translation = -4.6* m;
 pMaterial = new RemSimMaterial();

 shieldingVisAtt = 0;
}
RemSimShieldingDecorator::~RemSimShieldingDecorator()
{
  delete pMaterial;
}
void RemSimShieldingDecorator::ConstructComponent(G4VPhysicalVolume* motherVolume)
{
  RemSimDecorator::ConstructComponent(motherVolume);
  ConstructShielding(motherVolume);
}

void RemSimShieldingDecorator::DestroyComponent()
{
 delete shieldingVisAtt;
 shieldingVisAtt = 0;
 
 delete shieldingPhys; 
 shieldingPhys = 0;

 delete shieldingLog;
 shieldingLog = 0;

 delete shielding;
 shielding = 0;
}
void RemSimShieldingDecorator::ConstructShielding(G4VPhysicalVolume* motherVolume)
{
  // Geometry definition
 
  G4Material* water = pMaterial -> GetMaterial("Water");
  
  shielding = new G4Box("shielding",shieldingX/2.,shieldingY/2.,shieldingZ/2.);

  shieldingLog = new G4LogicalVolume(shielding, water,
                                     "shieldingLog",0,0,0);
  
  shieldingPhys = new G4PVPlacement(0,
             G4ThreeVector(0.,0.,translation + shieldingZ/2.),
            "shieldingPhys", shieldingLog, motherVolume,false,0); 

  G4cout<< " Thickness cm:"<<shieldingZ/cm<<G4endl;
  //Visualisation attributes
  G4Colour  red      (1.0,0.0,0.0);
  shieldingVisAtt = new G4VisAttributes(red);
  shieldingVisAtt -> SetVisibility(true);
  shieldingVisAtt -> SetForceSolid(true);
  shieldingLog -> SetVisAttributes(shieldingVisAtt);  
  PrintDetectorParameters();
}

void RemSimShieldingDecorator::ChangeThickness(G4double thick)
{
  PrintDetectorParameters();
  shielding -> SetZHalfLength(thick/2.);
  shieldingPhys -> SetTranslation(G4ThreeVector 
                            (0.,0.,translation + thick/2.));
}

void RemSimShieldingDecorator::ChangeMaterial(G4String materialName)
{ 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);     
   if (pttoMaterial)
    {
      shieldingLog -> SetMaterial(pttoMaterial);
      PrintDetectorParameters();
    } 
  else
    G4cout << "WARNING: material '" << materialName
           << "' not available!" << G4endl;         
}
void RemSimShieldingDecorator::PrintDetectorParameters()
{
  G4cout << "-----------------------------------------------------------------------"
         << G4endl
         << "the shielding is a box whose thickness is: " << G4endl
         << shieldingZ/cm
         << " cm along the Z axis"
         << G4endl
         << "material of the shielding: "
         << shieldingLog -> GetMaterial() -> GetName() <<G4endl
         << G4endl;
}

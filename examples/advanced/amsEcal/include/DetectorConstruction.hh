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
// $Id: DetectorConstruction.hh,v 1.3 2009-05-06 18:39:32 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;
            
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction();
 ~DetectorConstruction();

public:
     
  G4VPhysicalVolume* Construct();
  
  void PrintCalorParameters();
  void SetMagField(G4double);
  void UpdateGeometry();
       
public:

  G4VPhysicalVolume* GetPvolWorld()         {return pvol_world;};
  G4Material*        GetWorldMaterial()     {return worldMat;};
  G4double           GetWorldSizeX()        {return worldSizeX;};
  G4double           GetCalorThickness()    {return calorThickness;};  
  G4double           GetCalorSizeYZ()       {return fiberLength;};
	
  G4LogicalVolume*   GetLvolFiber()         {return lvol_fiber;};
  G4LogicalVolume*   GetLvolLayer()         {return lvol_layer;};  	
  G4LogicalVolume*   GetLvolSuperLayer()    {return lvol_superlayer;};
  G4LogicalVolume*   GetLvolCalorimeter()   {return lvol_calorimeter;};
  G4LogicalVolume*   GetLvolWorld()         {return lvol_world;};  
  
  G4int              GetNbFibers()          {return nbOfFibers;};  
  G4int              GetNbLayers()          {return nbOfLayers;};    
  G4int              GetNbSuperLayers()     {return nbOfSuperLayers;};
  
  G4int              GetNbLayersPerPixel()  {return nbOfLayersPerPixel;};
  G4int              GetNbPixels()          {return nbOfPixels;};
      			 
private:

  //fibers
  //
  G4Material*      fiberMat;  
  G4double         fiberDiameter, fiberLength;
  G4LogicalVolume* lvol_fiber;
  
  //layers
  //
  G4Material*      absorberMat;
  G4int            nbOfFibers;
  G4double         distanceInterFibers, distanceInterLayers;
  G4LogicalVolume* lvol_layer;
    
  //superLayers
  //
  G4Material*      superLayerMat;  
  G4int            nbOfLayers;    
  G4LogicalVolume* lvol_superlayer;  
           
  //calorimeter
  //
  G4Material*      calorimeterMat;  
  G4int            nbOfSuperLayers;
  G4double         calorThickness;
  G4LogicalVolume* lvol_calorimeter;            
  
  //world
  //
  G4Material*        worldMat;
  G4double           worldSizeX;
  G4LogicalVolume*   lvol_world;                
  G4VPhysicalVolume* pvol_world;
  
  G4Material*        defaultMat;

  //pixel readout
  //
  G4int  nbOfLayersPerPixel;
  G4int  nbOfPixels;
        
  G4UniformMagField* magField;
  DetectorMessenger* detectorMessenger;
      
private:

  void DefineMaterials();
  G4VPhysicalVolume* ConstructCalorimeter();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


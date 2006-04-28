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
// $Id: testG4PathFinder.cc,v 1.1 2006-04-28 19:55:13 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $ 
//
// 
// Moving with PathFinder in simple boxlike geometry, 

//  TO DO //    both with and without voxels. 

#include <assert.h>
#include "ApproxEqual.hh"

// Global defs
#include "globals.hh"

// To create the geometry 
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "G4Box.hh"

#include "G4GeometryManager.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

//   For the unit under test 
//                                 #include "G4Navigator.hh"
#include "G4PathFinder.hh"

// Build simple geometry:
// 4 small cubes + 1 slab (all G4Boxes) are positioned inside a larger cuboid
G4VPhysicalVolume* BuildGeometry()
{

    G4Box *myBigBox= new G4Box ("BixBox",25,25,20);

    G4LogicalVolume *worldLog=
      new G4LogicalVolume(myBigBox,0, "World",0,0,0);
    // No material, field, sensitive detector or user limits
    G4PVPlacement *worldPhys=
      new G4PVPlacement(0,G4ThreeVector(0,0,0),
			"World",worldLog, 0,false,0);
    // No displacement, no mother pointer - world!

    G4Box *myBox=new G4Box("cube-10",10,10,10);
    G4LogicalVolume *boxLog=
                new G4LogicalVolume(myBox,0, "Crystal Box",0,0,0);

    G4Box *mySlab= new G4Box("slab",10,25,10);
    G4LogicalVolume *slabLog=new G4LogicalVolume(mySlab,0,
						 "Crystal Slab",0,0,0);
    new G4PVPlacement(0,G4ThreeVector(-15,15,-10), "Target 1",boxLog,
		      worldPhys,false,1);
    new G4PVPlacement(0,G4ThreeVector(-15,-15,-10), "Target 2",boxLog,
		      worldPhys,false,2);

    // G4PVPlacement *offYPhys=
    new G4PVPlacement(0,G4ThreeVector(15,0,-10), "Target 3",slabLog,
		      worldPhys,false,3);

    new G4PVPlacement(0,G4ThreeVector(0,15,10), "Target 4",boxLog,
		      worldPhys,false,4);
    
    new G4PVPlacement(0,G4ThreeVector(0,-15,10), "Target 5",boxLog,
		      worldPhys,false,5);


    return worldPhys;
}

#include "G4TransportationManager.hh"
//
// Test LocateGlobalPointAndSetup
//
G4bool testG4PathFinder1(G4VPhysicalVolume *pTopNode)
{
    MyNavigator myNav;
    G4Navigator* pNav= new G4Navigator();   // (&myNav); 

    G4VPhysicalVolume *located;
    myNav.SetWorldVolume(pTopNode);

    G4PathFinder* pathFinder= G4PathFinder::GetInstance();
               //===========  --------------------------
    static G4TransportationManager* transportMgr=
      G4TransportationManager::GetTransportationManager(); 

    G4bool 
    registered=  transportMgr->RegisterNavigator( pNav ); 
    assert( registered ); 

    G4int navId= transportMgr->ActivateNavigator( pNav ); 
    assert ( registered == 0 ); 

    G4ThreeVector position( 0., 0., 0.), dirUx(1.,0.,0.); 
    pathFinder->PrepareNewTrack( position, dirUx ); 
  //==========  ---------------
    // Should also locate !!

    G4double t0=0.0, Ekin=100.00*MeV, restMass= 0.511*MeV, charge= -1, magDipole=0.0, s=0.0; 
    G4ThreeVector Spin(0.,0.,0.); 
    G4FieldTrack  startFT( position, t0, dirUx, Ekin, restMass, charge, Spin, magDipole, s), 
                  endFT('a');     //  Default values for return

    G4int stepNo=0; 
    G4double steplen= 1.0 * mm, safetyRet=0.0; 
    ELimited  limited;
    pathFinder->ComputeStep( startFT, steplen, navId, stepNo, safetyRet, limited, endFT );
  //==========  -----------

    transportMgr->DeActivateNavigator( pNav ); 
    transportMgr->DeRegisterNavigator( pNav ); 
    
    exit(1); 
    
    //--------------------   END                   --------------------------------

    assert(!myNav.LocateGlobalPointAndSetup(G4ThreeVector(kInfinity,0,0),0,false));
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(0,0,0),0,false);
    assert(located->GetName()=="World");

    assert(!myNav.LocateGlobalPointAndSetup(G4ThreeVector(kInfinity,0,0)));

// Check relative search that causes backup one level and then search down:
// Nonrel' finds Target 3, then rel' with point in Target 5 finds Target 5
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,0,-10),0,false);
    assert(located->GetName()=="Vari' Blocks");

    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(0,-15,20));
    assert(located->GetName()=="Target 5");
    assert(ApproxEqual(myNav.CurrentLocalCoordinate(),G4ThreeVector(0,0,10)));
// Check that outside point causes stack to unwind
    assert(!myNav.LocateGlobalPointAndSetup(G4ThreeVector(kInfinity,0,0)));

// Check parameterised volumes

// Replication 0
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,-15,-10));
    assert(located->GetName()=="Vari' Blocks");
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,-15,-16));
    assert(located->GetName()=="Target 3");

// Replication 1
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,0,-10));
    assert(located->GetName()=="Vari' Blocks");
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,0,-17));
    assert(located->GetName()=="Target 3");

// Replication 2
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,15,-10));
    assert(located->GetName()=="Vari' Blocks");
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,15,-18));
    assert(located->GetName()=="Target 3");

    return true;
}


//
// Test Stepping
//
G4bool testG4Navigator2(G4VPhysicalVolume *pTopNode)
{
    MyNavigator myNav;
    G4VPhysicalVolume *located;
    G4double Step,physStep,safety;
    G4ThreeVector xHat(1,0,0),yHat(0,1,0),zHat(0,0,1);
    G4ThreeVector mxHat(-1,0,0),myHat(0,-1,0),mzHat(0,0,-1);
    
    myNav.SetWorldVolume(pTopNode);
  
//
// Test location & Step computation
//  
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(0,0,-10));
    assert(located->GetName()=="World");
    physStep=kInfinity;
    Step=myNav.ComputeStep(G4ThreeVector(0,0,-10),mxHat,physStep,safety);
    assert(ApproxEqual(Step,25));
//    assert(ApproxEqual(safety,5));
    assert(safety>=0);

    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(0,0,-10));
    assert(located->GetName()=="World");
    physStep=kInfinity;
    Step=myNav.ComputeStep(G4ThreeVector(0,0,-10),xHat,physStep,safety);
    assert(ApproxEqual(Step,5));
//    assert(ApproxEqual(safety,5));
    assert(safety>=0);
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(5,0,-10),0,true);
    assert(located->GetName()=="Vari' Blocks");

    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(0,0,-10));
    assert(located->GetName()=="World");
    physStep=kInfinity;
    Step=myNav.ComputeStep(G4ThreeVector(0,0,-10),zHat,physStep,safety);
    assert(ApproxEqual(Step,30));
//    assert(ApproxEqual(safety,5));
    assert(safety>=0);

    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(0,0,-10));
    assert(located->GetName()=="World");
    physStep=kInfinity;
    Step=myNav.ComputeStep(G4ThreeVector(0,0,-10),mzHat,physStep,safety);
    assert(ApproxEqual(Step,10));
//    assert(ApproxEqual(safety,5));
    assert(safety>=0);


//
// Test stepping through common boundaries
//
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(-7,7,-20));
    assert(located->GetName()=="Target 1");
    physStep=kInfinity;
    Step=myNav.ComputeStep(G4ThreeVector(-7,7,-20),zHat,physStep,safety);
    assert(ApproxEqual(Step,20));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(-7,7,0));
    assert(located->GetName()=="Target 4");
    Step=myNav.ComputeStep(G4ThreeVector(-7,7,0),zHat,physStep,safety);
    assert(ApproxEqual(Step,20));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(-7,7,20));
    assert(!located);

//
// Test mother limited Step
//
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(-25,0,10));
    assert(located->GetName()=="World");
    physStep=kInfinity;
    Step=myNav.ComputeStep(G4ThreeVector(-25,0,10),xHat,physStep,safety);
    assert(ApproxEqual(Step,50));
    assert(ApproxEqual(safety,0));

//
// Test stepping through parameterised volumes
//
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,-25,-10),0,false);
    assert(located->GetName()=="Target 3");
    physStep=kInfinity;
    Step=myNav.ComputeStep(G4ThreeVector(15,-25,-10),yHat,physStep,safety);
    assert(ApproxEqual(Step,5));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,-20,-10));
    assert(located->GetName()=="Vari' Blocks");
    Step=myNav.ComputeStep(G4ThreeVector(15,-20,-10),yHat,physStep,safety);
    assert(ApproxEqual(Step,10));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,-10,-10));
    assert(located->GetName()=="Target 3");
    Step=myNav.ComputeStep(G4ThreeVector(15,-10,-10),yHat,physStep,safety);
    assert(ApproxEqual(Step,4));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,-6,-10));
    assert(located->GetName()=="Vari' Blocks");
    Step=myNav.ComputeStep(G4ThreeVector(15,-6,-10),yHat,physStep,safety);
    assert(ApproxEqual(Step,12));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,6,-10));
    assert(located->GetName()=="Target 3");
    Step=myNav.ComputeStep(G4ThreeVector(15,6,-10),yHat,physStep,safety);
    assert(ApproxEqual(Step,2));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,8,-10));
    assert(located->GetName()=="Vari' Blocks");
    Step=myNav.ComputeStep(G4ThreeVector(15,8,-10),yHat,physStep,safety);
    assert(ApproxEqual(Step,14));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,22,-10));
    assert(located->GetName()=="Target 3");
    Step=myNav.ComputeStep(G4ThreeVector(15,22,-10),yHat,physStep,safety);
    assert(ApproxEqual(Step,3));
    assert(ApproxEqual(safety,0));
    myNav.SetGeometricallyLimitedStep();
    located=myNav.LocateGlobalPointAndSetup(G4ThreeVector(15,25,-10));
    assert(!located);

    return true;
}

int main()
{
    G4VPhysicalVolume *myTopNode;
    myTopNode=BuildGeometry();	// Build the geometry
    G4GeometryManager::GetInstance()->CloseGeometry(false);
    testG4PathFinder1(myTopNode);
    // testG4Navigator2(myTopNode);
// Repeat tests but with full voxels
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4GeometryManager::GetInstance()->CloseGeometry(true);
    testG4PathFinder1(myTopNode);
    // testG4Navigator2(myTopNode);

    G4GeometryManager::GetInstance()->OpenGeometry();
    return 0;
}

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
// Source axis is oriented along Z axis. 
// Voxel data on the X-Z plan
//e is output to ASCII file
// "Brachy.out".
//
// For information related to this code contact the developers.
//
// --------------------------------------------------------------  

//                                  *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************

//
// --------------------------------------------------------------
//                 GEANT 4 - Brachytherapy example
// --------------------------------------------------------------
//
// Code developed by:
// S. Agostinelli, F. Foppiano, S. Garelli and M. Tropeano
//
// Brachytherapy simulates the dose deposition in a cubic (30*cm)
// water phantom for a Ir-192 MicroSelectron High Dose Rate
// brachytherapy source.
//
// Simplified gamma generation is used.
 
// 
// $Id: Brachy.cc,v 1.7 2001-11-29 14:07:47 tropeano Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "BrachyEventAction.hh"
#include "BrachyDetectorConstruction.hh"
#include "BrachyPhysicsList.hh"
#include "BrachyPrimaryGeneratorAction.hh"
#include "BrachyWaterBoxSD.hh"


#include "Randomize.hh"  
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"

int main()
{
 // Number of generated photons
 G4int NumberOfEvents = 1000;

 // Define number of voxels in X-Z plane
 G4int NumVoxelX = 101;
 G4int NumVoxelZ = 101;

 // Construct the default run manager
 G4RunManager* pRunManager = new G4RunManager;

 // Set mandatory initialization classes
 G4String SDName = "WaterBox";
 BrachyDetectorConstruction *pDetectorConstruction;
 pRunManager->SetUserInitialization(pDetectorConstruction = new BrachyDetectorConstruction(SDName,NumVoxelX,NumVoxelZ));
 pRunManager->SetUserInitialization(new BrachyPhysicsList);

 // Set mandatory user action class
 pRunManager->SetUserAction(new BrachyPrimaryGeneratorAction);

 // Initialize G4 kernel
 pRunManager->Initialize();

 // Alloc and initialize voxel matrix
 G4float* pVoxel = new G4float[NumVoxelX*NumVoxelZ];
 for(G4int j=0;j<NumVoxelX*NumVoxelZ;j++)
	pVoxel[j] = 0.0F;
 
 BrachyEventAction *pEventAction;
 pRunManager->SetUserAction(pEventAction = new BrachyEventAction(pVoxel,NumVoxelX,NumVoxelZ));

 // Get the pointer to the UI manager and set verbosities
 G4UImanager* UI = G4UImanager::GetUIpointer();
 UI->ApplyCommand("/run/verbose 0");
 UI->ApplyCommand("/event/verbose 0");
 UI->ApplyCommand("/tracking/verbose 0");

 pRunManager->BeamOn(NumberOfEvents);

 if(pVoxel)
	{
	G4std::ofstream ofs;

	// Output voxel data to text file
	// Format = x coord [mm] <tab> z coord [mm] <tab> edep [MeV] <eol>
	ofs.open("Brachy.out");
		{

		  ofs<<" x(mm)"<<'\t'<<"z(mm)"<<'\t'<<"released energy(Mev)"
		     <<G4endl;
                                
		

		G4double VoxelWidth_X = pDetectorConstruction->GetBoxDim_X()/NumVoxelX;
		G4double VoxelWidth_Z = pDetectorConstruction->GetBoxDim_Z()/NumVoxelZ;

		G4double x,z;

		for(G4int k=0;k<NumVoxelZ;k++)
			{
			z = (-NumVoxelZ+1+2*k)*VoxelWidth_Z/2;

			for(G4int i=0;i<NumVoxelX;i++)
				{
				G4int j = i+k*NumVoxelX;
				x = (-NumVoxelX+1+2*i)*VoxelWidth_X/2;
				
				// Do not consider near voxels
				if(fabs(x) > 3*mm || fabs(z) > 6*mm)	
				{	ofs << x << '\t' << z << '\t' << pVoxel[j] << G4endl;
				//check released energy	
				// if(pVoxel[j]!=0)
				//  G4cout<<x<<'\t'<<z<< '\t'<<pVoxel[j]<<G4endl;
				}      
		}
			}
		ofs.close();
		}
	}

 delete[] pVoxel; 

 // Job termination
 delete pRunManager;

 return 0;
}











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
// $Id: HadrontherapyMatrix.cc;
// See more at: http://g4advancedexamples.lngs.infn.it/Examples/hadrontherapy//

#include "HadrontherapyMatrix.hh"
#include "HadrontherapyAnalysisManager.hh"
#include "G4RunManager.hh"
#include "HadrontherapyPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include <fstream>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "globals.hh"

HadrontherapyMatrix* HadrontherapyMatrix::instance = NULL;
// Only return a pointer to matrix
HadrontherapyMatrix* HadrontherapyMatrix::GetInstance() 
{
  return instance;
}
// This STATIC method delete (!) the old matrix and rewrite a new object returning a pointer to it
// TODO A check on the parameters is required!
HadrontherapyMatrix* HadrontherapyMatrix::GetInstance(G4int voxelX, G4int voxelY, G4int voxelZ)  
{
	if (instance) delete instance;
	instance = new HadrontherapyMatrix(voxelX, voxelY, voxelZ);
	instance -> Initialize();// XXX
	return instance;
}

HadrontherapyMatrix::HadrontherapyMatrix(G4int voxelX, G4int voxelY, G4int voxelZ)
{  
// Number of the voxels of the phantom
// For Y = Z = 1 the phantom is divided in slices (and not in voxels)
// orthogonal to the beam axis
  numberOfVoxelAlongX = voxelX;
  numberOfVoxelAlongY = voxelY;
  numberOfVoxelAlongZ = voxelZ; 
  // Create the dose matrix
  matrix = new G4double[numberOfVoxelAlongX*numberOfVoxelAlongY*numberOfVoxelAlongZ];
  if (matrix)
  {
      G4cout << "Memory space to store physical dose into " <<  
                numberOfVoxelAlongX*numberOfVoxelAlongY*numberOfVoxelAlongZ <<
	        " voxels has been allocated " << G4endl;
  }
  else G4Exception(" HadrontherapyMatrix::HadrontherapyMatrix. Can't allocate memory to store physical dose!");
// Hit voxel (TrackID) marker
// This array mark the status of voxel, if a hit occur, with the trackID of the particle
// Must be initialized
  hitTrack = new G4int[numberOfVoxelAlongX*numberOfVoxelAlongY*numberOfVoxelAlongZ];
  ClearHitTrack();
}

HadrontherapyMatrix::~HadrontherapyMatrix()
{
    delete[] matrix;
    delete[] hitTrack;
    // clear fluences/dose data
    Clear();
}

void HadrontherapyMatrix::Clear()
{
    for (size_t i=0; i<ionStore.size(); i++)
    {
	delete[] ionStore[i].dose; 
	delete[] ionStore[i].fluence; 
    }
    ionStore.clear();
}

void HadrontherapyMatrix::flush()
{
    for(int i=0;i<numberOfVoxelAlongX*numberOfVoxelAlongY*numberOfVoxelAlongZ;i++)
    {
	matrix[i] = 0;
    }
}
// Initialise the elements of the matrix to zero
void HadrontherapyMatrix::Initialize()
{ 
    Clear();
    // Fill dose matrix with zero
    for(G4int i = 0; i < numberOfVoxelAlongX; i++)
    {
	for(G4int j = 0; j < numberOfVoxelAlongY; j++)
	{
	    for(G4int k = 0; k < numberOfVoxelAlongZ; k++)

		matrix[Index(i,j,k)] = 0.;
	} 
    }
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Print generated nuclides list
void HadrontherapyMatrix::PrintNuclides()
{
    for (size_t i=0; i<ionStore.size(); i++)
    {
	G4cout << ionStore[i].name << G4endl;
    }
}
/////////////////////////////////////////////////////////////////////////////
// Clear Hit voxel (TrackID) markers
void HadrontherapyMatrix::ClearHitTrack()
{
  for(G4int i=0; i<numberOfVoxelAlongX*numberOfVoxelAlongY*numberOfVoxelAlongZ; i++) hitTrack[i] = 0;
}
// Return Hit status
G4int* HadrontherapyMatrix::GetHitTrack(G4int i, G4int j, G4int k)
{
   return &(hitTrack[Index(i,j,k)]);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Methods to store data to filenames...
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
// General method to store matrix data to filename
void HadrontherapyMatrix::StoreMatrix(G4String filename, void* data, size_t psize)
{
    if (data)
    {
	ofs.open(filename, std::ios::out);
	if (ofs)
	{
	    for(G4int i = 0; i < numberOfVoxelAlongX; i++) 
		for(G4int j = 0; j < numberOfVoxelAlongY; j++) 
		    for(G4int k = 0; k < numberOfVoxelAlongZ; k++) 
		    {
			G4int n = Index(i, j, k);
			// Check for data type: u_int, G4double, XXX 
			if (psize == sizeof(u_int))
			{
			    u_int* pdata = (u_int*)data;
			    if (pdata[n]) ofs << i << '\t' << j << '\t' <<
						 k << '\t' << pdata[n] << G4endl;
			}
			else if (psize == sizeof(G4double))
			{
			    G4double* pdata = (G4double*)data;
			    if (pdata[n]) ofs << i << '\t' << j << '\t' <<
						 k << '\t' << pdata[n] << G4endl;
			}
		    }
	    ofs.close();
	}
    }
}

// Store fluence per single ion in multiple files
void HadrontherapyMatrix::StoreFluenceData()
{
    for (size_t i=0; i < ionStore.size(); i++){
         StoreMatrix(ionStore[i].name + "_Fluence.out", ionStore[i].fluence, sizeof(u_int));
    }
}
// Store dose per single ion in multiple files
void HadrontherapyMatrix::StoreDoseData()
{

    for (size_t i=0; i < ionStore.size(); i++){
         StoreMatrix(ionStore[i].name + "_Dose.out", ionStore[i].dose, sizeof(G4double));
    }
}

// Store dose for all ions into a single file and into an ntuple
void HadrontherapyMatrix::StoreData(G4String filename)
{
    // Sort like periodic table
    std::sort(ionStore.begin(), ionStore.end());
#define width 15L
    if (ionStore.size())
    {
	if (ofs)
	{
	    ofs.open(filename, std::ios::out);
	    //
	    // Write the voxels index and the list of particles/ions 
	    ofs << std::setprecision(6) << std::left <<
		"i\tj\tk\t"; 
/*	    
            G4RunManager *runManager = G4RunManager::GetRunManager();
            HadrontherapyPrimaryGeneratorAction *pPGA = (HadrontherapyPrimaryGeneratorAction*)runManager -> GetUserPrimaryGeneratorAction();
            G4String name = pPGA -> GetParticleGun() -> GetParticleDefinition() -> GetParticleName();
  */         
	    // Total dose 
	    ofs << std::setw(width) << "Dose";

	    for (size_t l=0; l < ionStore.size(); l++)
	    {
		G4String a = (ionStore[l].isPrimary) ? "_1":""; // is it a primary?
		ofs << std::setw(width) << ionStore[l].name + "_D" + a <<
		       std::setw(width) << ionStore[l].name + "_F" + a;
	    }
	    ofs << G4endl;

/////////////////////////////////////////////////////////////////////////
            ofs << std::setprecision(6) << std::left <<
		"0\t0\t0\t"; 

	    // Total dose 
	    ofs << std::setw(width) << '0';

	    for (size_t l=0; l < ionStore.size(); l++)
	    {
		ofs << std::setw(width) << ionStore[l].PDGencoding  <<
		       std::setw(width) << ionStore[l].PDGencoding;
	    }
	    ofs << G4endl;

//////////////////////////////////////////////////////////////////////////
	    ofs << std::setfill('_');
	    for (size_t l=0; l < 2*ionStore.size()+3; l++)
	    {
		ofs << std::setw(width) <<  "_";
	    }
	    ofs << std::setfill(' ');

	    // Write data
	    for(G4int i = 0; i < numberOfVoxelAlongX; i++) 
		for(G4int j = 0; j < numberOfVoxelAlongY; j++) 
		    for(G4int k = 0; k < numberOfVoxelAlongZ; k++) 
		    {
			G4int n = Index(i, j, k);
			for (size_t m=0; m < ionStore.size(); m++)
			{
			    // Write only not identically null data lines
			    if(ionStore[m].dose[n] || ionStore[m].fluence[n])
			    {
				ofs << G4endl;
				ofs << i << '\t' << j << '\t' << k << '\t';
				// Total dose 
				ofs << std::setw(width) << matrix[n]; 
				for (size_t l=0; l < ionStore.size(); l++)
				{
#ifdef G4ANALYSIS_USE_ROOT
				    // Do the same work for .root file: fill dose ntuple  
	                            static HadrontherapyAnalysisManager* analysis = HadrontherapyAnalysisManager::getInstance();
				    analysis -> FillVoxelFragmentTuple(i, j, k, ionStore[l].A, ionStore[l].Z, ionStore[l].dose[n] );
#endif
				    // Fill ASCII file rows
				    ofs << std::setw(width) << ionStore[l].dose[n] <<
					   std::setw(width) << ionStore[l].fluence[n]; 
				}
				break;
			    }
			}
		    }
	    ofs.close();
	}
    }
}
/////////////////////////////////////////////////////////////////////////////
// Dose methods...

// Fill DOSE/fluence matrix for particle/ion: 
// If fluence parameter is true (default value is FALSE) then fluence at voxel (i, j, k) is increased. 
// The energyDeposit parameter fill the dose matrix for voxel (i,j,k) 
G4bool HadrontherapyMatrix::Fill(G4int trackID,
	G4ParticleDefinition* particleDef,
	G4int i, G4int j, G4int k, 
	G4double energyDeposit,
	G4bool fluence) 
{
    if (energyDeposit <=0. && !fluence) return false;
    // Get Particle Data Group id
    G4int PDGencoding = particleDef -> GetPDGEncoding();

    // Search for already allocated data...
    for (size_t l=0; l < ionStore.size(); l++)
    {
	if (ionStore[l].PDGencoding == PDGencoding ) 
	{   // Is it a primary or a secondary particle? 
	    if ( trackID ==1 && ionStore[l].isPrimary || trackID !=1 && !ionStore[l].isPrimary)
	    {
		if (energyDeposit > 0.) ionStore[l].dose[Index(i, j, k)] += energyDeposit;
		if (fluence) ionStore[l].fluence[Index(i, j, k)]++;
		return true;
	    }
	}

    }
    // XXX Check if new operator fails
     
    G4int Z = particleDef-> GetAtomicNumber();
    G4int A = particleDef-> GetAtomicMass();
    G4String fullName = particleDef -> GetParticleName();
    G4String name = fullName.substr (0, fullName.find("[") ); // cut excitation energy  
    //if (trackID == 1) { name +="_1"; } 
    // Let's put a new particle in our store...
    ion newIon = 
    {
	(trackID == 1) ? true:false,
	PDGencoding,
	name,
	name.length(), 
	Z, 
	A, 
	new G4double[numberOfVoxelAlongX * numberOfVoxelAlongY * numberOfVoxelAlongZ],
	new u_int[numberOfVoxelAlongX * numberOfVoxelAlongY * numberOfVoxelAlongZ]
    }; 
    // Initialize data
    if (newIon.dose && newIon.fluence)
    {
	for(G4int m=0; m<numberOfVoxelAlongX*numberOfVoxelAlongY*numberOfVoxelAlongZ; m++)
	{
	    newIon.dose[m] = 0.;
	    newIon.fluence[m] = 0;
	}
	if (energyDeposit > 0.) newIon.dose[Index(i, j, k)] += energyDeposit;
	if (fluence) newIon.fluence[Index(i, j, k)]++;

	ionStore.push_back(newIon);

	// TODO Put some verbosity check
	/*
	   G4cout << "Memory space to store the DOSE/FLUENCE into " <<  
	   numberOfVoxelAlongX*numberOfVoxelAlongY*numberOfVoxelAlongZ << 
	   " voxels has been allocated for the nuclide " << newIon.name << 
	   " (Z = " << Z << ", A = " << A << ")" << G4endl ;
	   */
	return true;
    }
    else // XXX Can't allocate memory! XXX
    {
	return false;
    }

}

void HadrontherapyMatrix::Fill(G4int i, G4int j, G4int k, 
			       G4double energyDeposit)
{
    if (matrix)
	matrix[Index(i,j,k)] += energyDeposit;

    // Store the energy deposit in the matrix element corresponding 
    // to the phantom voxel  
}
void HadrontherapyMatrix::TotalEnergyDeposit()
{
  // Store the information of the matrix in a ntuple and in 
  // a 1D Histogram
    if (matrix)
    {  
	//StoreMatrix(filename, matrix, sizeof(*matrix));

#ifdef ANALYSIS_USE
	HadrontherapyAnalysisManager* analysis = HadrontherapyAnalysisManager::getInstance();
	for(G4int i = 0; i < numberOfVoxelAlongX; i++) 
	    for(G4int j = 0; j < numberOfVoxelAlongY; j++) 
		for(G4int k = 0; k < numberOfVoxelAlongZ; k++)
		{ 
		    G4int n = Index(i,j,k);
		    analysis -> FillEnergyDeposit(i, j, k, matrix[n]);
		    analysis -> BraggPeak(i, matrix[n]);
		}
#endif
    }
}


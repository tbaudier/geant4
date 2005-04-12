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
// $Id: G4VEmFluctuationModel.hh,v 1.8 2005-04-12 11:21:25 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4VEmFluctuationModel
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 03.01.2002
//
// Modifications:
//
// 28-12-02 add method Dispersion (V.Ivanchenko)
// 07-02-03 change signature (V.Ivanchenko)
// 13-02-03 Add name (V.Ivanchenko)
// 16-10-03 Changed interface to Initialisation (V.Ivanchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
//
//
// Class Description: 
//
// Abstract class for interface to simualtion of energy loss fluctuations

// -------------------------------------------------------------------
//

#ifndef G4VEmFluctuationModel_h
#define G4VEmFluctuationModel_h 1


#include "globals.hh"

class G4ParticleDefinition;
class G4DynamicParticle;
class G4Material;

class G4VEmFluctuationModel 
{

public:

  G4VEmFluctuationModel(const G4String& nam);

  virtual ~G4VEmFluctuationModel();

  //------------------------------------------------------------------------
  // Virtual methods to be implemented for the concrete model
  //------------------------------------------------------------------------

  virtual G4double SampleFluctuations(const G4Material*,
				      const G4DynamicParticle*,
				      G4double& tmax,
				      G4double& length,
				      G4double& meanLoss) = 0;

  virtual G4double Dispersion(const G4Material*,
                              const G4DynamicParticle*,
			      G4double& tmax,
			      G4double& length) = 0;

  //------------------------------------------------------------------------
  // Methods with standard implementation; may be overwritten if needed 
  //------------------------------------------------------------------------

  virtual void InitialiseMe(const G4ParticleDefinition*);

  //------------------------------------------------------------------------
  // Generic methods common to all models
  //------------------------------------------------------------------------

  G4String GetName() const;

private:

  // hide assignment operator
  G4VEmFluctuationModel & operator=(const  G4VEmFluctuationModel &right);
  G4VEmFluctuationModel(const  G4VEmFluctuationModel&);

  const G4String   name;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


inline G4VEmFluctuationModel::G4VEmFluctuationModel(const G4String& nam)
  : name(nam) 
{}

inline G4VEmFluctuationModel::~G4VEmFluctuationModel() 
{}

inline void G4VEmFluctuationModel::InitialiseMe(const G4ParticleDefinition*)
{}

inline G4String G4VEmFluctuationModel::GetName() const 
{
  return name;
}

#endif


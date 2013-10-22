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
// $Id$
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Author: Alexander Bagulya
//         11 March 2012
//         on the base of G4LivermoreComptonModel
//
// History:
// --------
// 18 Apr 2009   V Ivanchenko Cleanup initialisation and generation of secondaries:
//                  - apply internal high-energy limit only in constructor 
//                  - do not apply low-energy limit (default is 0)
//                  - remove GetMeanFreePath method and table
//                  - added protection against numerical problem in energy sampling 
//                  - use G4ElementSelector
// 26 Dec 2010   V Ivanchenko Load data tables only once to avoid memory leak

#include "G4LivermoreComptonModel.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Electron.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4LossTableManager.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4AtomicShell.hh"
#include "G4Gamma.hh"
#include "G4ShellData.hh"
#include "G4DopplerProfile.hh"
#include "G4Log.hh"
#include "G4Exp.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4int G4LivermoreComptonModel::maxZ = 99;
G4LPhysicsFreeVector* G4LivermoreComptonModel::data[] = {0};
G4ShellData*       G4LivermoreComptonModel::shellData = 0;
G4DopplerProfile*  G4LivermoreComptonModel::profileData = 0;

static const G4double ln10 = G4Log(10.);

G4LivermoreComptonModel::G4LivermoreComptonModel(const G4ParticleDefinition*, 
						 const G4String& nam)
  : G4VEmModel(nam),isInitialised(false)
{
  lowestEnergy = 10 * eV;

  verboseLevel=1 ;
  // Verbosity scale:
  // 0 = nothing 
  // 1 = warning for energy non-conservation 
  // 2 = details of energy budget
  // 3 = calculation of cross sections, file openings, sampling of atoms
  // 4 = entering in methods

  if(  verboseLevel>1 ) { 
    G4cout << "Livermore Compton model is constructed " << G4endl;
  }

  //Mark this model as "applicable" for atomic deexcitation
  SetDeexcitationFlag(true);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4LivermoreComptonModel::~G4LivermoreComptonModel()
{  
  if(IsMaster()) {
    for(G4int i=0; i<=maxZ; ++i) { 
      delete data[i];
      data[i] = 0;
    }
    delete shellData;
    shellData = 0;
    delete profileData;
    profileData = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4LivermoreComptonModel::Initialise(const G4ParticleDefinition* particle,
					 const G4DataVector& cuts)
{
  if (verboseLevel > 1) {
    G4cout << "Calling G4LivermoreComptonModel::Initialise()" << G4endl;
  }

  // Initialise element selector
  
  if(IsMaster()) {
    InitialiseElementSelectors(particle, cuts);

    // Access to elements

    char* path = getenv("G4LEDATA");

    G4ProductionCutsTable* theCoupleTable = 
      G4ProductionCutsTable::GetProductionCutsTable();
    G4int numOfCouples = theCoupleTable->GetTableSize();
  
    for(G4int i=0; i<numOfCouples; ++i) {
      const G4Material* material = 
	theCoupleTable->GetMaterialCutsCouple(i)->GetMaterial();
      const G4ElementVector* theElementVector = material->GetElementVector();
      G4int nelm = material->GetNumberOfElements();
    
      for (G4int j=0; j<nelm; ++j) {
	G4int Z = (G4int)(*theElementVector)[j]->GetZ();
	if(Z < 1)          { Z = 1; }
	else if(Z > maxZ)  { Z = maxZ; }
	if( (!data[Z]) ) { ReadData(Z, path); }
      }
    }

    // For Doppler broadening
    if(!shellData) {
      shellData = new G4ShellData(); 
      shellData->SetOccupancyData();
      G4String file = "/doppler/shell-doppler";
      shellData->LoadData(file);
    }
    if(!profileData) { profileData = new G4DopplerProfile(); }
  }

  if (verboseLevel > 2) {
    G4cout << "Loaded cross section files" << G4endl;
  }
 
  if( verboseLevel>1 ) { 
    G4cout << "G4LivermoreComptonModel is initialized " << G4endl
	   << "Energy range: "
	   << LowEnergyLimit() / eV << " eV - "
	   << HighEnergyLimit() / GeV << " GeV"
	   << G4endl;
  }
  //  
  if(isInitialised) { return; }

  fParticleChange = GetParticleChangeForGamma();
  fAtomDeexcitation  = G4LossTableManager::Instance()->AtomDeexcitation();
  isInitialised = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4LivermoreComptonModel::InitialiseLocal(const G4ParticleDefinition*,
					      G4VEmModel* masterModel)
{
  SetElementSelectors(masterModel->GetElementSelectors());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4LivermoreComptonModel::ReadData(size_t Z, const char* path)
{
  if (verboseLevel > 1) 
  {
    G4cout << "G4LivermoreComptonModel::ReadData()" 
	   << G4endl;
  }
  if(data[Z]) { return; }  
  const char* datadir = path;
  if(!datadir) 
  {
    datadir = getenv("G4LEDATA");
    if(!datadir) 
    {
      G4Exception("G4LivermoreComptonModel::ReadData()",
		  "em0006",FatalException,
		  "Environment variable G4LEDATA not defined");
      return;
    }
  }
  
  data[Z] = new G4LPhysicsFreeVector();
  
  // Activation of spline interpolation
  data[Z]->SetSpline(false);
  
  std::ostringstream ost;
  ost << datadir << "/livermore/comp/ce-cs-" << Z <<".dat";
  std::ifstream fin(ost.str().c_str());
  
  if( !fin.is_open()) 
    {
      G4ExceptionDescription ed;
      ed << "G4LivermoreComptonModel data file <" << ost.str().c_str()
	 << "> is not opened!" << G4endl;
    G4Exception("G4LivermoreComptonModel::ReadData()",
		"em0003",FatalException,
		ed,"G4LEDATA version should be G4EMLOW6.34 or later");
      return;
    } else {
      if(verboseLevel > 3) {
	G4cout << "File " << ost.str() 
	       << " is opened by G4LivermoreComptonModel" << G4endl;
      }   
      data[Z]->Retrieve(fin, true);
      data[Z]->ScaleVector(MeV, MeV*barn);
    }   
  fin.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double 
G4LivermoreComptonModel::ComputeCrossSectionPerAtom(const G4ParticleDefinition*,
						    G4double GammaEnergy,
						    G4double Z, G4double,
						    G4double, G4double)
{
  if (verboseLevel > 3) {
    G4cout << "G4LivermoreComptonModel::ComputeCrossSectionPerAtom()" 
	   << G4endl;
  }
  G4double cs = 0.0; 

  if (GammaEnergy < lowestEnergy) { return 0.0; }

  G4int intZ = G4lrint(Z);
  if(intZ < 1 || intZ > maxZ) { return cs; } 

  G4LPhysicsFreeVector* pv = data[intZ];
  // element was not initialised
  if(!pv) 
    {
      char* path = getenv("G4LEDATA");
      ReadData(intZ, path);
      pv = data[intZ];

      if(!pv) { return cs; }
    }

  G4int n = pv->GetVectorLength() - 1;   
  G4double e1 = pv->Energy(0);
  G4double e2 = pv->Energy(n);

  if(GammaEnergy <= e1)      { cs = GammaEnergy/(e1*e1)*pv->Value(e1); }
  else if(GammaEnergy <= e2) { cs = pv->Value(GammaEnergy)/GammaEnergy; }
  else if(GammaEnergy > e2)  { cs = pv->Value(e2)/GammaEnergy; }
 
  return cs;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void G4LivermoreComptonModel::SampleSecondaries(
                             std::vector<G4DynamicParticle*>* fvect,
			     const G4MaterialCutsCouple* couple,
			     const G4DynamicParticle* aDynamicGamma,
			     G4double, G4double)
{

  // The scattered gamma energy is sampled according to Klein - Nishina 
  // formula then accepted or rejected depending on the Scattering Function 
  // multiplied by factor from Klein - Nishina formula.
  // Expression of the angular distribution as Klein Nishina
  // angular and energy distribution and Scattering fuctions is taken from
  // D. E. Cullen "A simple model of photon transport" Nucl. Instr. Meth.
  // Phys. Res. B 101 (1995). Method of sampling with form factors is different
  // data are interpolated while in the article they are fitted.
  // Reference to the article is from J. Stepanek New Photon, Positron
  // and Electron Interaction Data for GEANT in Energy Range from 1 eV to 10
  // TeV (draft).
  // The random number techniques of Butcher & Messel are used
  // (Nucl Phys 20(1960),15).

  G4double photonEnergy0 = aDynamicGamma->GetKineticEnergy();

  if (verboseLevel > 3) {
    G4cout << "G4LivermoreComptonModel::SampleSecondaries() E(MeV)= " 
	   << photonEnergy0/MeV << " in " << couple->GetMaterial()->GetName() 
	   << G4endl;
  }
  
  // low-energy gamma is absorpted by this process
  if (photonEnergy0 <= lowestEnergy) 
    {
      fParticleChange->ProposeTrackStatus(fStopAndKill);
      fParticleChange->SetProposedKineticEnergy(0.);
      fParticleChange->ProposeLocalEnergyDeposit(photonEnergy0);
      return ;
    }

  G4double e0m = photonEnergy0 / electron_mass_c2 ;
  G4ParticleMomentum photonDirection0 = aDynamicGamma->GetMomentumDirection();

  // Select randomly one element in the current material
  const G4ParticleDefinition* particle =  aDynamicGamma->GetDefinition();
  const G4Element* elm = SelectRandomAtom(couple,particle,photonEnergy0);

  G4int Z = G4lrint(elm->GetZ());

  G4double epsilon0Local = 1. / (1. + 2. * e0m);
  G4double epsilon0Sq = epsilon0Local * epsilon0Local;
  G4double alpha1 = -G4Log(epsilon0Local);
  G4double alpha2 = 0.5 * (1. - epsilon0Sq);

  G4double wlPhoton = h_Planck*c_light/photonEnergy0;

  // Sample the energy of the scattered photon
  G4double epsilon;
  G4double epsilonSq;
  G4double oneCosT;
  G4double sinT2;
  G4double gReject;

  if (verboseLevel > 3) {
    G4cout << "Started loop to sample gamma energy" << G4endl;
  }
  
  do {
    if ( alpha1/(alpha1+alpha2) > G4UniformRand())
      {
        epsilon = std::exp(-alpha1 * G4UniformRand());  
        epsilonSq = epsilon * epsilon;
      }
      else
      {
        epsilonSq = epsilon0Sq + (1. - epsilon0Sq) * G4UniformRand();
        epsilon = std::sqrt(epsilonSq);
      }

      oneCosT = (1. - epsilon) / ( epsilon * e0m);
      sinT2 = oneCosT * (2. - oneCosT);
      G4double x = std::sqrt(oneCosT/2.) * cm / wlPhoton;
      G4double scatteringFunction = ComputeScatteringFunction(x, Z);
      gReject = (1. - epsilon * sinT2 / (1. + epsilonSq)) * scatteringFunction;

  } while(gReject < G4UniformRand()*Z);

  G4double cosTheta = 1. - oneCosT;
  G4double sinTheta = std::sqrt (sinT2);
  G4double phi = twopi * G4UniformRand() ;
  G4double dirx = sinTheta * std::cos(phi);
  G4double diry = sinTheta * std::sin(phi);
  G4double dirz = cosTheta ;

  // Doppler broadening -  Method based on:
  // Y. Namito, S. Ban and H. Hirayama, 
  // "Implementation of the Doppler Broadening of a Compton-Scattered Photon 
  // into the EGS4 Code", NIM A 349, pp. 489-494, 1994
  
  // Maximum number of sampling iterations
  static G4int maxDopplerIterations = 1000;
  G4double bindingE = 0.;
  G4double photonEoriginal = epsilon * photonEnergy0;
  G4double photonE = -1.;
  G4int iteration = 0;
  G4double eMax = photonEnergy0;

  G4int shellIdx = 0;

  if (verboseLevel > 3) {
    G4cout << "Started loop to sample broading" << G4endl;
  }

  do
    {
      ++iteration;
      // Select shell based on shell occupancy
      shellIdx = shellData->SelectRandomShell(Z);
      bindingE = shellData->BindingEnergy(Z,shellIdx);

      if (verboseLevel > 3) {
	G4cout << "Shell ID= " << shellIdx 
	       << " Ebind(keV)= " << bindingE/keV << G4endl;
      }
      
      eMax = photonEnergy0 - bindingE;
     
      // Randomly sample bound electron momentum 
      // (memento: the data set is in Atomic Units)
      G4double pSample = profileData->RandomSelectMomentum(Z,shellIdx);
      if (verboseLevel > 3) {
	G4cout << "pSample= " << pSample << G4endl;
      }
      // Rescale from atomic units
      G4double pDoppler = pSample * fine_structure_const;
      G4double pDoppler2 = pDoppler * pDoppler;
      G4double var2 = 1. + oneCosT * e0m;
      G4double var3 = var2*var2 - pDoppler2;
      G4double var4 = var2 - pDoppler2 * cosTheta;
      G4double var = var4*var4 - var3 + pDoppler2 * var3;
      if (var > 0.)
	{
	  G4double varSqrt = std::sqrt(var);        
	  G4double scale = photonEnergy0 / var3;  
          // Random select either root
 	  if (G4UniformRand() < 0.5) { photonE = (var4 - varSqrt) * scale; }
	  else                       { photonE = (var4 + varSqrt) * scale; }
	} 
      else
	{
	  photonE = -1.;
	}
   } while (iteration <= maxDopplerIterations && photonE > eMax); 
  // (photonE < 0. || photonE > eMax || photonE < eMax*G4UniformRand()) );
	    
 
  // End of recalculation of photon energy with Doppler broadening
  // Revert to original if maximum number of iterations threshold 
  // has been reached

  if (iteration >= maxDopplerIterations)
    {
      photonE = photonEoriginal;
      bindingE = 0.;
    }

  // Update G4VParticleChange for the scattered photon

  G4ThreeVector photonDirection1(dirx,diry,dirz);
  photonDirection1.rotateUz(photonDirection0);
  fParticleChange->ProposeMomentumDirection(photonDirection1) ;

  G4double photonEnergy1 = photonE;

  if (photonEnergy1 > 0.) {
    fParticleChange->SetProposedKineticEnergy(photonEnergy1) ;

  } else {
    // photon absorbed
    photonEnergy1 = 0.;
    fParticleChange->SetProposedKineticEnergy(0.) ;
    fParticleChange->ProposeTrackStatus(fStopAndKill);   
    fParticleChange->ProposeLocalEnergyDeposit(photonEnergy0);
    return;
  }

  // Kinematics of the scattered electron
  G4double eKineticEnergy = photonEnergy0 - photonEnergy1 - bindingE;

  // protection against negative final energy: no e- is created
  if(eKineticEnergy < 0.0) {
    fParticleChange->ProposeLocalEnergyDeposit(photonEnergy0 - photonEnergy1);
    return;
  }

  G4double eTotalEnergy = eKineticEnergy + electron_mass_c2;

  G4double electronE = photonEnergy0 * (1. - epsilon) + electron_mass_c2; 
  G4double electronP2 = 
    electronE*electronE - electron_mass_c2*electron_mass_c2;
  G4double sinThetaE = -1.;
  G4double cosThetaE = 0.;
  if (electronP2 > 0.)
    {
      cosThetaE = (eTotalEnergy + photonEnergy1 )* 
	(1. - epsilon) / std::sqrt(electronP2);
      sinThetaE = -1. * sqrt(1. - cosThetaE * cosThetaE); 
    }
  
  G4double eDirX = sinThetaE * std::cos(phi);
  G4double eDirY = sinThetaE * std::sin(phi);
  G4double eDirZ = cosThetaE;

  G4ThreeVector eDirection(eDirX,eDirY,eDirZ);
  eDirection.rotateUz(photonDirection0);
  G4DynamicParticle* dp = new G4DynamicParticle (G4Electron::Electron(),
						 eDirection,eKineticEnergy) ;
  fvect->push_back(dp);

  // sample deexcitation
  //

  if (verboseLevel > 3) {
    G4cout << "Started atomic de-excitation " << fAtomDeexcitation << G4endl;
  }

  if(fAtomDeexcitation && iteration < maxDopplerIterations) {
    G4int index = couple->GetIndex();
    if(fAtomDeexcitation->CheckDeexcitationActiveRegion(index)) {
      size_t nbefore = fvect->size();
      G4AtomicShellEnumerator as = G4AtomicShellEnumerator(shellIdx);
      const G4AtomicShell* shell = fAtomDeexcitation->GetAtomicShell(Z, as);
      fAtomDeexcitation->GenerateParticles(fvect, shell, Z, index);
      size_t nafter = fvect->size();
      if(nafter > nbefore) {
	for (size_t i=nbefore; i<nafter; ++i) {
	  bindingE -= ((*fvect)[i])->GetKineticEnergy();
	} 
      }
    }
  }
  if(bindingE < 0.0) { bindingE = 0.0; }
  fParticleChange->ProposeLocalEnergyDeposit(bindingE);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double 
G4LivermoreComptonModel::ComputeScatteringFunction(G4double x, G4int Z)
{
  G4double value = Z;
  if (x <= ScatFuncFitParam[Z][2]) { 

    //G4double lgq = log10(x); 
    G4double lgq = G4Log(x)/ln10; 
      
    if (lgq < ScatFuncFitParam[Z][1]) { 
      value = ScatFuncFitParam[Z][3] + lgq*ScatFuncFitParam[Z][4];
    } else {
      value = ScatFuncFitParam[Z][5] + lgq*ScatFuncFitParam[Z][6] + 
	lgq*lgq*ScatFuncFitParam[Z][7] + lgq*lgq*lgq*ScatFuncFitParam[Z][8];
    }
    value = G4Exp(value*ln10);
  }
  return value;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Fitting data to compute scattering function 
const G4double G4LivermoreComptonModel::ScatFuncFitParam[101][9] = 
  {
    {  0,    0.,          0.,      0.,    0.,       0.,     0.,     0.,    0.},
    {  1, 6.673, 1.49968E+08, -14.352, 1.999, -143.374, 50.787, -5.951, 0.230},
    {  2, 6.500, 2.50035E+08, -14.215, 1.970, -53.649, 13.892, -0.948, 0.007},
    {  3, 6.551, 3.99945E+08, -13.555, 1.993, -62.090, 21.462, -2.453, 0.093},
    {  4, 6.500, 5.00035E+08, -13.746, 1.998, -127.906, 46.491, -5.614, 0.226},
    {  5, 6.500, 5.99791E+08, -13.800, 1.998, -131.153, 47.132, -5.619, 0.223},
    {  6, 6.708, 6.99842E+08, -13.885, 1.999, -128.143, 45.379, -5.325, 0.208},
    {  7, 6.685, 7.99834E+08, -13.885, 2.000, -131.048, 46.314, -5.421, 0.212},
    {  8, 6.669, 7.99834E+08, -13.962, 2.001, -128.225, 44.818, -5.183, 0.200},
    {  9, 6.711, 7.99834E+08, -13.999, 2.000, -122.112, 42.103, -4.796, 0.182},
    { 10, 6.702, 7.99834E+08, -14.044, 1.999, -110.143, 37.225, -4.143, 0.153},
    { 11, 6.425, 1.00000E+09, -13.423, 1.993, -41.137, 12.313, -1.152, 0.034},
    { 12, 6.542, 1.00000E+09, -13.389, 1.997, -53.549, 17.420, -1.840, 0.064},
    { 13, 6.570, 1.49968E+09, -13.401, 1.997, -66.243, 22.297, -2.460, 0.090},
    { 14, 6.364, 1.49968E+09, -13.452, 1.999, -78.271, 26.757, -3.008, 0.113},
    { 15, 6.500, 1.49968E+09, -13.488, 1.998, -85.069, 29.164, -3.291, 0.124},
    { 16, 6.500, 1.49968E+09, -13.532, 1.998, -93.640, 32.274, -3.665, 0.139},
    { 17, 6.500, 1.49968E+09, -13.584, 2.000, -98.534, 33.958, -3.857, 0.146},
    { 18, 6.500, 1.49968E+09, -13.618, 1.999, -100.077, 34.379, -3.891, 0.147},
    { 19, 6.500, 1.99986E+09, -13.185, 1.992, -53.819, 17.528, -1.851, 0.065},
    { 20, 6.490, 1.99986E+09, -13.123, 1.993, -52.221, 17.169, -1.832, 0.065},
    { 21, 6.498, 1.99986E+09, -13.157, 1.994, -55.365, 18.276, -1.961, 0.070},
    { 22, 6.495, 1.99986E+09, -13.183, 1.994, -57.412, 18.957, -2.036, 0.073},
    { 23, 6.487, 1.99986E+09, -13.216, 1.995, -58.478, 19.270, -2.065, 0.074},
    { 24, 6.500, 1.99986E+09, -13.330, 1.997, -62.192, 20.358, -2.167, 0.077},
    { 25, 6.488, 1.99986E+09, -13.277, 1.997, -58.007, 18.924, -2.003, 0.070},
    { 26, 6.500, 5.00035E+09, -13.292, 1.997, -61.176, 20.067, -2.141, 0.076},
    { 27, 6.500, 5.00035E+09, -13.321, 1.998, -61.909, 20.271, -2.159, 0.076},
    { 28, 6.500, 5.00035E+09, -13.340, 1.998, -62.402, 20.391, -2.167, 0.077},
    { 29, 6.500, 5.00035E+09, -13.439, 1.998, -67.305, 21.954, -2.331, 0.082},
    { 30, 6.500, 5.00035E+09, -13.383, 1.999, -62.064, 20.136, -2.122, 0.074},
    { 31, 6.500, 5.00035E+09, -13.349, 1.997, -61.068, 19.808, -2.086, 0.073},
    { 32, 6.500, 5.00035E+09, -13.373, 1.999, -63.126, 20.553, -2.175, 0.077},
    { 33, 6.500, 5.00035E+09, -13.395, 1.999, -65.674, 21.445, -2.278, 0.081},
    { 34, 6.500, 5.00035E+09, -13.417, 1.999, -69.457, 22.811, -2.442, 0.087},
    { 35, 6.500, 5.00035E+09, -13.442, 2.000, -72.283, 23.808, -2.558, 0.092},
    { 36, 6.500, 5.00035E+09, -13.451, 1.998, -74.696, 24.641, -2.653, 0.095},
    { 37, 6.500, 5.00035E+09, -13.082, 1.991, -46.235, 14.519, -1.458, 0.048},
    { 38, 6.465, 5.00035E+09, -13.022, 1.993, -41.784, 13.065, -1.300, 0.043},
    { 39, 6.492, 5.00035E+09, -13.043, 1.994, -44.609, 14.114, -1.429, 0.048},
    { 40, 6.499, 5.00035E+09, -13.064, 1.994, -47.142, 15.019, -1.536, 0.052},
    { 41, 6.384, 5.00035E+09, -13.156, 1.996, -53.114, 17.052, -1.766, 0.061},
    { 42, 6.500, 5.00035E+09, -13.176, 1.996, -54.590, 17.550, -1.822, 0.063},
    { 43, 6.500, 5.00035E+09, -13.133, 1.997, -51.272, 16.423, -1.694, 0.058},
    { 44, 6.500, 5.00035E+09, -13.220, 1.996, -58.314, 18.839, -1.969, 0.068},
    { 45, 6.500, 5.00035E+09, -13.246, 1.998, -59.674, 19.295, -2.020, 0.070},
    { 46, 6.500, 5.00035E+09, -13.407, 1.999, -72.228, 23.693, -2.532, 0.090},
    { 47, 6.500, 5.00035E+09, -13.277, 1.998, -60.890, 19.647, -2.053, 0.071},
    { 48, 6.500, 5.00035E+09, -13.222, 1.998, -56.152, 18.002, -1.863, 0.064},
    { 49, 6.500, 5.00035E+09, -13.199, 1.997, -56.208, 18.052, -1.872, 0.065},
    { 50, 6.500, 5.00035E+09, -13.215, 1.998, -58.478, 18.887, -1.973, 0.069},
    { 51, 6.500, 5.00035E+09, -13.230, 1.998, -60.708, 19.676, -2.066, 0.072},
    { 52, 6.500, 7.99834E+09, -13.246, 1.998, -63.341, 20.632, -2.180, 0.077},
    { 53, 6.500, 5.00035E+09, -13.262, 1.998, -66.339, 21.716, -2.310, 0.082},
    { 54, 6.500, 7.99834E+09, -13.279, 1.998, -67.649, 22.151, -2.357, 0.084},
    { 55, 6.500, 5.00035E+09, -12.951, 1.990, -45.302, 14.219, -1.423, 0.047},
    { 56, 6.425, 5.00035E+09, -12.882, 1.992, -39.825, 12.363, -1.214, 0.039},
    { 57, 6.466, 2.82488E+09, -12.903, 1.992, -38.952, 11.982, -1.160, 0.037},
    { 58, 6.451, 5.00035E+09, -12.915, 1.993, -41.959, 13.118, -1.302, 0.043},
    { 59, 6.434, 5.00035E+09, -12.914, 1.993, -40.528, 12.555, -1.230, 0.040},
    { 60, 6.444, 5.00035E+09, -12.922, 1.992, -39.986, 12.329, -1.200, 0.038},
    { 61, 6.414, 7.99834E+09, -12.930, 1.993, -42.756, 13.362, -1.327, 0.044},
    { 62, 6.420, 7.99834E+09, -12.938, 1.992, -42.682, 13.314, -1.319, 0.043},
    { 63, 6.416, 7.99834E+09, -12.946, 1.993, -42.399, 13.185, -1.301, 0.042},
    { 64, 6.443, 7.99834E+09, -12.963, 1.993, -43.226, 13.475, -1.335, 0.044},
    { 65, 6.449, 7.99834E+09, -12.973, 1.993, -43.232, 13.456, -1.330, 0.043},
    { 66, 6.419, 7.99834E+09, -12.966, 1.993, -42.047, 12.990, -1.270, 0.041},
    { 67, 6.406, 7.99834E+09, -12.976, 1.993, -42.405, 13.106, -1.283, 0.041},
    { 68, 6.424, 7.99834E+09, -12.986, 1.993, -41.974, 12.926, -1.259, 0.040},
    { 69, 6.417, 7.99834E+09, -12.989, 1.993, -42.132, 12.967, -1.262, 0.040},
    { 70, 6.405, 7.99834E+09, -13.000, 1.994, -42.582, 13.122, -1.280, 0.041},
    { 71, 6.449, 7.99834E+09, -13.015, 1.994, -42.586, 13.115, -1.278, 0.041},
    { 72, 6.465, 7.99834E+09, -13.030, 1.994, -43.708, 13.509, -1.324, 0.043},
    { 73, 6.447, 7.99834E+09, -13.048, 1.996, -44.838, 13.902, -1.369, 0.045},
    { 74, 6.452, 7.99834E+09, -13.073, 1.997, -45.545, 14.137, -1.395, 0.046},
    { 75, 6.432, 7.99834E+09, -13.082, 1.997, -46.426, 14.431, -1.428, 0.047},
    { 76, 6.439, 7.99834E+09, -13.100, 1.997, -47.513, 14.806, -1.471, 0.048},
    { 77, 6.432, 7.99834E+09, -13.110, 1.997, -48.225, 15.042, -1.497, 0.049},
    { 78, 6.500, 7.99834E+09, -13.185, 1.997, -53.256, 16.739, -1.687, 0.056},
    { 79, 6.500, 7.99834E+09, -13.200, 1.997, -53.900, 16.946, -1.709, 0.057},
    { 80, 6.500, 7.99834E+09, -13.156, 1.998, -49.801, 15.536, -1.547, 0.051},
    { 81, 6.500, 7.99834E+09, -13.128, 1.997, -49.651, 15.512, -1.548, 0.051},
    { 82, 6.500, 7.99834E+09, -13.134, 1.997, -51.021, 16.018, -1.609, 0.054},
    { 83, 6.500, 7.99834E+09, -13.148, 1.998, -52.693, 16.612, -1.679, 0.056},
    { 84, 6.500, 7.99834E+09, -13.161, 1.998, -54.415, 17.238, -1.754, 0.059},
    { 85, 6.500, 7.99834E+09, -13.175, 1.998, -56.083, 17.834, -1.824, 0.062},
    { 86, 6.500, 7.99834E+09, -13.189, 1.998, -57.860, 18.463, -1.898, 0.065},
    { 87, 6.500, 7.99834E+09, -12.885, 1.990, -39.973, 12.164, -1.162, 0.036},
    { 88, 6.417, 7.99834E+09, -12.816, 1.991, -34.591, 10.338, -0.956, 0.029},
    { 89, 6.442, 7.99834E+09, -12.831, 1.992, -36.002, 10.867, -1.021, 0.031},
    { 90, 6.463, 7.99834E+09, -12.850, 1.993, -37.660, 11.475, -1.095, 0.034},
    { 91, 6.447, 7.99834E+09, -12.852, 1.993, -37.268, 11.301, -1.071, 0.033},
    { 92, 6.439, 7.99834E+09, -12.858, 1.993, -37.695, 11.438, -1.085, 0.034},
    { 93, 6.437, 1.00000E+10, -12.866, 1.993, -39.010, 11.927, -1.146, 0.036},
    { 94, 6.432, 7.99834E+09, -12.862, 1.993, -37.192, 11.229, -1.057, 0.033},
    { 95, 6.435, 7.99834E+09, -12.869, 1.993, -37.589, 11.363, -1.072, 0.033},
    { 96, 6.449, 1.00000E+10, -12.886, 1.993, -39.573, 12.095, -1.162, 0.037},
    { 97, 6.446, 1.00000E+10, -12.892, 1.993, -40.007, 12.242, -1.178, 0.037},
    { 98, 6.421, 1.00000E+10, -12.887, 1.993, -39.509, 12.041, -1.152, 0.036},
    { 99, 6.414, 1.00000E+10, -12.894, 1.993, -39.939, 12.183, -1.168, 0.037},
    {100, 6.412, 1.00000E+10, -12.900, 1.993, -39.973, 12.180, -1.166, 0.037}
  };

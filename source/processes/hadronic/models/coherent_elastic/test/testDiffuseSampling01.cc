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
//
// Unit test for diffuse elastic models
//
//  18.05.07 V. Grichine
//
//

#include "G4ios.hh"
#include <fstream>
#include <cmath>
#include "globals.hh"
#include "Randomize.hh"
#include "G4UnitsTable.hh"

#include <iomanip>

#include "G4DiffuseElastic.hh"


#include "G4UHadronElasticProcess.hh"
#include "G4HadProjectile.hh"
#include "G4DynamicParticle.hh"

#include "G4Element.hh"
#include "G4NistManager.hh"

#include "G4PiNuclearCrossSection.hh"
#include "G4NucleonNuclearCrossSection.hh"

#include "G4ParticleDefinition.hh"
#include "G4PionPlus.hh"
#include "G4Proton.hh"

#include "G4VCrossSectionDataSet.hh"

#include  "G4ParticleTable.hh"
#include  "G4IonTable.hh"


using namespace std;


int main()
{

  G4int i, j, k, iMax, iMod;
  G4double x;

  //  G4DiffuseElastic* diffelastic = new G4DiffuseElastic();
  /*
  std::ofstream writeb("bessel.dat", std::ios::out ) ;
  writeb.setf( std::ios::scientific, std::ios::floatfield );

  for( i = 0; i < 51; i++) // J0,1 test for the table from Hanbook of Special Functions p.390
  {
    x = 0.1*i;
    G4cout<<x<<"\t"<<diffelastic->BesselJzero(x)<<"\t"<<diffelastic->BesselJone(x)<<G4endl;
    writeb<<x<<"\t"<<diffelastic->BesselJzero(x)<<"\t"<<diffelastic->BesselJone(x)<<G4endl;
  }
  */
  // Element definition

  G4Element*     theElement;
  G4Material*    theMaterial;
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(1);

  G4cout << " 1 hydrogen" << G4endl;
  G4cout << " 2 helium" << G4endl;
  G4cout << " 4 berillium" << G4endl;
  G4cout << " 6 carbon" << G4endl;
  G4cout << " 7 nitrogen" << G4endl;
  G4cout << " 8 oxigen" << G4endl;
  G4cout << "13 aluminium" << G4endl;
  G4cout << "14 silicon" << G4endl;
  G4cout << "18 argon" << G4endl;
  G4cout << "26 iron" << G4endl;
  G4cout << "29 copper" << G4endl;
  G4cout << "48 cadmium" << G4endl;
  G4cout << "74 tugnsten" << G4endl;
  G4cout << "82 lead" << G4endl;
  G4cout << "92 uranium" << G4endl;
  G4int choice;
  // G4cin >> choice;

  choice = 82;


  switch (choice)
  {
    case 1:

      theElement  = man->FindOrBuildElement("H");
      theMaterial = man->FindOrBuildMaterial("G4_H");
      break;

    case 2:

      theElement  = man->FindOrBuildElement("He");
      theMaterial = man->FindOrBuildMaterial("G4_He");
      break;

    case 4:

      theElement  = man->FindOrBuildElement("Be");
      theMaterial = man->FindOrBuildMaterial("G4_Be");
      break;

    case 6:

      theElement  = man->FindOrBuildElement("C");
      theMaterial = man->FindOrBuildMaterial("G4_C");
      break;

    case 7:

      theElement  = man->FindOrBuildElement("N");
      theMaterial = man->FindOrBuildMaterial("G4_N");
      break;


    case 8:

      theElement  = man->FindOrBuildElement("O");
      theMaterial = man->FindOrBuildMaterial("G4_O");
      break;

    case 13:

      theElement  = man->FindOrBuildElement("Al");
      theMaterial = man->FindOrBuildMaterial("G4_Al");
      break;

    case 14:

      theElement  = man->FindOrBuildElement("Si");
      theMaterial = man->FindOrBuildMaterial("G4_Si");
      break;

    case 18:

      theElement  = man->FindOrBuildElement("Ar");
      theMaterial = man->FindOrBuildMaterial("G4_Ar");
      break;

    case 26:

      theElement  = man->FindOrBuildElement("Fe");
      theMaterial = man->FindOrBuildMaterial("G4_Fe");
      break;

    case 29:

      theElement  = man->FindOrBuildElement("Cu");
      theMaterial = man->FindOrBuildMaterial("G4_Cu");
      break;

    case 48:

      theElement  = man->FindOrBuildElement("Cd");
      theMaterial = man->FindOrBuildMaterial("G4_Cd");
      break;


    case 74:

      theElement  = man->FindOrBuildElement("W");
      theMaterial = man->FindOrBuildMaterial("G4_W");
      break;

    case 82:

      theElement  = man->FindOrBuildElement("Pb");
      theMaterial = man->FindOrBuildMaterial("G4_Pb");
      break;

    case 92:

      theElement  = man->FindOrBuildElement("U");
      theMaterial = man->FindOrBuildMaterial("G4_U");
      break;
  }

// Particle definition

  G4cout << " 1 proton" << G4endl;
  G4cout << " 2 neutron" << G4endl;
  G4cout << " 3 pion+" << G4endl;
  G4cout << " 4 pion-" << G4endl;
  G4cout << " 4 kaon+" << G4endl;
  G4cout << " 5 kaon0short" << G4endl;

  //  G4cin >> choice;
  choice = 1;

  G4ParticleDefinition* theParticleDefinition;

  // G4NucleonNuclearCrossSection* barash = new G4NucleonNuclearCrossSection();

  G4PiNuclearCrossSection* barash = new G4PiNuclearCrossSection();

  switch (choice)
  {
    case 1:

      theParticleDefinition = G4Proton::ProtonDefinition();
      // barash = new G4NucleonNuclearCrossSection();
      break;

    case 2:

      theParticleDefinition = G4Neutron::NeutronDefinition();
      // barash = new G4NucleonNuclearCrossSection();     
      break;

    case 3:

      theParticleDefinition = G4PionPlus::PionPlusDefinition();
      // barash = new G4PiNuclearCrossSection();
    
      break;

    case 4:

      theParticleDefinition = G4PionMinus::PionMinusDefinition();
      // barash = new G4PiNuclearCrossSection();
      
      break;
 
    case 5:

      theParticleDefinition = G4KaonPlus::KaonPlusDefinition();
     
      break;

    case 6:

      theParticleDefinition = G4KaonZeroShort::KaonZeroShortDefinition();
     
      break;

  }

  // Initialisation


  G4DiffuseElastic* diffelastic = new G4DiffuseElastic(theParticleDefinition);

  // Physics data

  G4double momentum = 9.92*GeV;
  G4double pMass    = theParticleDefinition->GetPDGMass();

  G4double thetaMax  = 15.*degree; 

  // G4double kinEnergy = std::sqrt(momentum*momentum + pMass*pMass) - pMass;

  G4double kinEnergy = 1.*GeV;

  G4DynamicParticle*  theDynamicParticle = new G4DynamicParticle(theParticleDefinition,
                                              G4ParticleMomentum(0.,0.,1.),
                                              kinEnergy);
  G4HadProjectile* projectile = new G4HadProjectile(*theDynamicParticle); 

  G4double m1 = theParticleDefinition->GetPDGMass();
  G4double plab = theDynamicParticle->GetTotalMomentum();
  G4cout <<"lab momentum, plab = "<<plab/GeV<<" GeV"<<G4endl;
  G4double plabLowLimit = 20.0*MeV;

  G4int Z   = G4int(theElement->GetZ());
  G4int A    = G4int(theElement->GetN()+0.5);


  /*
  G4ParticleDefinition * theTargetDef = 0;

  if      (Z == 1 && A == 1) theTargetDef = G4Proton::Proton();
  else if (Z == 1 && A == 2) theTargetDef = G4Deuteron::Deuteron();
  else if (Z == 1 && A == 3) theTargetDef = G4Triton::Triton();
  else if (Z == 2 && A == 3) theTargetDef = G4He3::He3();
  else if (Z == 2 && A == 4) theTargetDef = G4Alpha::Alpha();
  else                       theTargetDef = G4ParticleTable::GetParticleTable()->FindIon(Z,A,0,Z);
 
  G4double m2 = theTargetDef->GetPDGMass();
  */


  G4double m2 = man->GetAtomicMassAmu(Z)*GeV;
  // G4double m2 = man->GetAtomicMass( Z, A);
  G4cout <<" target mass, m2 = "<<m2/GeV<<" GeV"<<G4endl;

  G4LorentzVector lv1 = theDynamicParticle->Get4Momentum();
  G4LorentzVector lv(0.0,0.0,0.0,m2);   
  lv += lv1;

  G4ThreeVector bst = lv.boostVector();
  lv1.boost(-bst);

  G4ThreeVector p1   = lv1.vect();
  G4double      ptot = p1.mag();
  G4cout <<"cms momentum, ptot = "<<ptot/GeV<<" GeV"<<G4endl;
  G4double      tmax = 4.0*ptot*ptot;
  G4double      t    = 0.0;

  // Choose generator
  G4bool swave = false;

  // S-wave for very low energy
  if(plab < plabLowLimit) swave = true;

  // Angle sampling

  G4int  numberOfSimPoints =0;
  G4double pData, sData, dData, tData[200];
  std::ifstream simRead;
  simRead.open("pPbT1GeV.dat");

  simRead>>numberOfSimPoints;

  for( i = 0; i < numberOfSimPoints; i++ )
  {
    tData[i] = 0.0 ;
  }
  for( i = 0; i < numberOfSimPoints; i++ )
  {
    // simRead>>pData>>sData>>dData;
    // tData[i] = pData*GeV*GeV;
    simRead>>tData[i]>>sData;
  }
  simRead.close();

  const G4int kAngle = numberOfSimPoints;
  G4double angleDistr[kAngle];

  for( k = 0; k < kAngle; k++) 
  {
    tData[k] *= degree;
    angleDistr[k] = 0;
  }
  std::ofstream writes("sigma.dat", std::ios::out ) ;
  writes.setf( std::ios::scientific, std::ios::floatfield );

  G4double theta, thetaLab, thetaCMS, sigma, integral, transfer;

  iMax = 1000000;   // numberOfSimPoints;
  iMod = iMax/10;
  writes << iMax  << G4endl;

  for( i = 0; i < iMax; i++)
  {
    // normal sampling in CMS

    // theta = (4.5 + 1.*i)*milliradian;

    // theta = std::acos(1-2*tData[i]/tmax);

    // theta = std::sqrt(tData[i]/plab/plab);
    // theta = tData[i]*degree;


    // sigma = diffelastic->GetDiffuseElasticXsc( theParticleDefinition, theta, plab, A);
    // integral = diffelastic->IntegralElasticProb( theParticleDefinition, theta, plab, A);

    // theta = diffelastic->SampleThetaLab(projectile, m2, A);

    // transfer = diffelastic->SampleTableT(theParticleDefinition, ptot, Z);

    // thetaCMS = std::acos( 1 - std::fabs(transfer)/2./ptot/ptot );

    thetaCMS = diffelastic->SampleTableThetaCMS(theParticleDefinition, ptot, Z, A);

    theta = diffelastic->ThetaCMStoThetaLab(theDynamicParticle, m2, thetaCMS);

    // G4cout << transfer/GeV/GeV <<"\t" << thetaCMS/degree << "\t"<< theta/degree << G4endl;

    for( k = 0; k < kAngle; k++)
    {
      if( theta <= tData[k] )
      {
        // k = G4int(theta*kAngle/thetaMax);
        // k = G4int(t*kAngle*100/tmax);

        angleDistr[k] += 1;
        break;
      }
    }
    if (i%iMod == 0) G4cout <<"i = "<<i<<G4endl;
  }
  G4double sum = 0;

  for( k = 0; k < kAngle; k++) sum += angleDistr[k];
  
  G4cout<<"relative number of event inside array = "<<sum/iMax<<G4endl;
 

  std::ofstream writef("angle.dat", std::ios::out ) ;
  writef.setf( std::ios::scientific, std::ios::floatfield );

  writef <<kAngle<<G4endl;

  for( k = 0; k < kAngle; k++) 
  {
    // angleDistr[k] *= sig/iMax/(2*pi)/std::sin(k*thetaMax/kAngle);
    // angleDistr[k] *= steradian/millibarn;
    // G4cout <<k*thetaMax/kAngle/degree<<"\t"<<"\t"<<angleDistr[k]<<G4endl;
    // writef <<k*thetaMax/kAngle/degree<<"\t"<<angleDistr[k]<<G4endl;
    G4cout <<tData[k]/degree<<"\t"<<"\t"<<angleDistr[k]<<G4endl;
    writef <<tData[k]/degree<<"\t"<<angleDistr[k]<<G4endl;
  }


  iMax = 1;

  for( i = 0; i < iMax; i++)
  {
    thetaLab = i*degree;
    thetaCMS = diffelastic->ThetaLabToThetaCMS(theDynamicParticle, m2, thetaLab);     
    theta    = diffelastic->ThetaCMStoThetaLab(theDynamicParticle, m2, thetaCMS);     
    // G4cout<<thetaLab<<"\t"<<theta<<G4endl;
  } 
  G4cout<<"energy in GeV"<<"\t"<<"cross-section in millibarn"<<G4endl;
  G4cout << " elastic cross section for " <<
            theParticleDefinition->GetParticleName() <<
           " on " << theElement->GetName() << G4endl;
  G4cout <<"with atomic weight = "<<theElement->GetN() << G4endl;

  return 1;
} // end of main

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
// $Id: G4eLowEnergyLoss.hh,v 1.11 2003-05-20 20:03:34 pia Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------

// Class description:
// Low Energy electromagnetic process, electron energy loss
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------
//
// This class is the implementation of the unified Energy Loss process.
// It calculates the continuous energy loss for e+/e-.
// The following processes give contributions to the continuous 
// energy loss (by default) :
//  ---  ionisation (= cont.ion.loss + delta ray production)
//  --- bremsstrahlung (= cont.loss due to soft brems+discrete bremsstrahlung)
//   more can be added   ..........
// This class creates static dE/dx and range tables for e+ and e-,
// which tables can be used by other processes , too.
// G4eLowEnergyLoss is the base class for the processes giving contribution
// to the (continuous) energy loss of e+/e- .
//
//      History: first implementation, based on object model of
//      2nd December 1995, G.Cosmo
//      ---------- G4eLowEnergyLoss physics process -----------
//                by Laszlo Urban, 20 March 1997 
//
//  27.05.98 OldGetRange removed + other corrs , L.Urban
//  10.09.98 cleanup
//  16.10.98 public method SetStepFunction() + messenger class 
//  20.01.99 new data members , L.Urban
//  10.02.00 modifications, new e.m. structure , L.Urban
//  18.10.01 Revision to improve code quality and consistency with design
//  23.11.01 V.Ivanchenko Move static member-functions from header to source
//  28.03.02 V.Ivanchenko add fluorescence flag
//  21.01.03 V.Ivanchenko cut per region
// ------------------------------------------------------------

#ifndef G4eLowEnergyLoss_h
#define G4eLowEnergyLoss_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4VeLowEnergyLoss.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4ParticleChangeForLoss.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsLinearVector.hh"
#include "G4EnergyLossTables.hh"

class G4EnergyLossMessenger;

class G4eLowEnergyLoss : public G4VeLowEnergyLoss

{
  public:

    G4eLowEnergyLoss(const G4String& );

   ~G4eLowEnergyLoss();

    G4bool IsApplicable(const G4ParticleDefinition&);
    //  true for e+/e- , false otherwise

    void BuildDEDXTable(const G4ParticleDefinition& aParticleType);
    //  It builds dE/dx and range tables for aParticleType and
    //  for every material contained in the materialtable.

    G4double GetContinuousStepLimit(const G4Track& track,
                                    G4double previousStepSize,
                                    G4double currentMinimumStep,
                                    G4double& currentSafety);
    // Computes the steplimit due to the energy loss process.

    G4VParticleChange* AlongStepDoIt(const G4Track& track,
                                     const G4Step& Step) ;
    // Performs the computation of the (continuous) energy loss
    // after the step (with fluctuation).

    virtual G4double GetMeanFreePath(const G4Track& track,
                                     G4double previousStepSize,
                                     G4ForceCondition* condition) = 0;
    // Virtual function to be overridden in the derived classes
    // ( ionisation and bremsstrahlung) .

    virtual G4VParticleChange* PostStepDoIt(const G4Track& track,
                                            const G4Step& step) = 0;
    // Virtual function to be overridden in the derived classes
    // ( ionisation and bremsstrahlung) .

    static void  SetNbOfProcesses(G4int nb);
    // Sets number of processes giving contribution to the energy loss

    static void  PlusNbOfProcesses();
    // Increases number of processes giving contribution to the energy loss

    static void  MinusNbOfProcesses();
    // Decreases number of processes giving contribution to the energy loss

    static G4int GetNbOfProcesses();
    // Gets number of processes giving contribution to the energy loss
    // ( default value = 2)

    static void SetLowerBoundEloss(G4double val);
    static void SetUpperBoundEloss(G4double val);
    static void SetNbinEloss(G4int nb);

    static G4double GetLowerBoundEloss();
    static G4double GetUpperBoundEloss();
    static G4int    GetNbinEloss();

    void ActivateFluorescence(G4bool val);
    // Set fluorescence flag on/off

    G4bool Fluorescence() const;
    // Get flurescence flag

  protected:

    virtual G4std::vector<G4DynamicParticle*>* DeexciteAtom(const G4MaterialCutsCouple* ,
							    G4double, G4double) // incidentEnergy, eLoss
  { return 0; };

    G4PhysicsTable* theLossTable;

    G4double MinKineticEnergy ;     // particle with kinetic energy
                                    // smaller than MinKineticEnergy
                                    // is stopped in  AlongStepDoIt

    G4double Charge,lastCharge ;

    //basic DEDX and Range tables
    static G4PhysicsTable* theDEDXElectronTable ;
    static G4PhysicsTable* theDEDXPositronTable ;
    static G4PhysicsTable* theRangeElectronTable ;
    static G4PhysicsTable* theRangePositronTable ;

    //inverse tables of the range tables
    static G4PhysicsTable* theInverseRangeElectronTable;
    static G4PhysicsTable* theInverseRangePositronTable;

    // lab and proper time tables
    static G4PhysicsTable* theLabTimeElectronTable ;
    static G4PhysicsTable* theLabTimePositronTable ;
    static G4PhysicsTable* theProperTimeElectronTable ;
    static G4PhysicsTable* theProperTimePositronTable ;

    //processes inherited from G4eLowEnergyLoss
    //register themselves  in the static array Recorder
    //for electrons/positrons separately
    //nb of contributing processes = NbOfProcesses
    static G4int NbOfProcesses;
    static G4int CounterOfElectronProcess;
    static G4int CounterOfPositronProcess ;
    static G4PhysicsTable** RecorderOfElectronProcess;
    static G4PhysicsTable** RecorderOfPositronProcess;


  private:

    G4double GetConstraints(const G4DynamicParticle* aParticle,
                            const G4MaterialCutsCouple* couple);

    // hide  assignment operator
    G4eLowEnergyLoss (G4eLowEnergyLoss &);
    G4eLowEnergyLoss & operator=(const G4eLowEnergyLoss &right);


    G4PhysicsTable* theDEDXTable;

    G4int            CounterOfProcess;
    G4PhysicsTable** RecorderOfProcess;

    G4double fdEdx;                       // computed in GetConstraints
    G4double fRangeNow;                   // computed in GetConstraints

    G4double linLossLimit ;               // used in AlongStepDoIt


    //New ParticleChange
    G4ParticleChangeForLoss fParticleChange ;

 //
 // static part of the class
 //

    static G4int NbinEloss;               // number of bins in table,
                                          // calculated in BuildPhysicTable
    static G4double LowerBoundEloss;
    static G4double UpperBoundEloss;
    static G4double RTable,LOGRTable;    // LOGRTable=log(UpperBoundEloss-
                                         // LowerBoundEloss)/NbinEloss
                                         // RTable = exp(LOGRTable)

    //for interpolation within the tables
    static G4PhysicsTable* theeRangeCoeffATable;
    static G4PhysicsTable* theeRangeCoeffBTable;
    static G4PhysicsTable* theeRangeCoeffCTable;
    static G4PhysicsTable* thepRangeCoeffATable;
    static G4PhysicsTable* thepRangeCoeffBTable;
    static G4PhysicsTable* thepRangeCoeffCTable;

    static G4EnergyLossMessenger* eLossMessenger;

    G4bool theFluo;                     // Fluorescence flag

};

#include "G4eLowEnergyLoss.icc"

#endif


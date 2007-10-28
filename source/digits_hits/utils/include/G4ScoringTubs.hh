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
// $Id: G4ScoringTubs.hh,v 1.5 2007-10-28 02:13:44 akimura Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4ScoringTubs_h
#define G4ScoringTubs_h 1

#include "globals.hh"
#include "G4VScoringMesh.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VPrimitiveScorer;

#include <vector>

class G4ScoringTubs : public G4VScoringMesh
{
  public:
      G4ScoringTubs(G4String wName);
      ~G4ScoringTubs();

  public:
      virtual void Construct(G4VPhysicalVolume* fWorldPhys);
      virtual void List() const;
      virtual void Draw(std::map<G4int, G4double*> * map, G4int axflg=111);
  //virtual void DumpToFile(G4String & psName, G4String & fileName, G4String & option);

  void SetRMinMax(G4double rMinMax[2]) {
    for(int i = 0; i < 2; i++) fSize[i] = rMinMax[i];
  }
  void SetRMin(G4double rMin) {fSize[0] = rMin;}
  void SetRMax(G4double rMax) {fSize[1] = rMax;}
  void SetZSize(G4double zSize) {fSize[2] = zSize;}

  void SetSegmentDirection(G4int dir) {fSegmentDirection = dir;} // supports the r-direction only at present.

  void RegisterPrimitives(std::vector<G4VPrimitiveScorer *> & vps);

private:
  G4int fSegmentDirection; // =1: r, =2: phi, =3: z
  G4LogicalVolume * fMeshElementLogical;
  
  void SetupGeometry(G4VPhysicalVolume * fWorldPhys);
  void GetSegmentOrder(G4int segDir, G4int nseg[3], G4int segOrd[3], G4double segParam[3][3]);
};


#endif


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
// $Id: G4VScoringMesh.hh,v 1.5 2007-07-13 11:04:31 akimura Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4VScoringMesh_h
#define G4VScoringMesh_h 1

#include "globals.hh"
#include "G4THitsMap.hh"
class G4VPhysicalVolume;
#include <map>

enum MeshShape { boxMesh, cylinderMesh, sphereMesh };

// class description:
//
//  This class represents a parallel world for interactive scoring purposes.
//

class G4VScoringMesh 
{
  public:
      G4VScoringMesh(G4String wName);
      ~G4VScoringMesh();

  public:
      virtual void Construct(G4VPhysicalVolume* fWorldPhys)=0;
      virtual void List() const=0;


  public:
      inline const G4String& GetWorldName() const
      { return fWorldName; }
      inline G4bool IsActive() const
      { return fActive; }
      inline void Activate(G4bool vl = true)
      { fActive = vl; }
      inline MeshShape GetShape() const
      { return fShape; }
      inline void Accumulate(G4THitsMap<G4double> * map) const;

  protected:
      G4String  fWorldName;
      G4bool    fConstructed;
      G4bool    fActive;
      MeshShape fShape;
      std::map<G4String, G4THitsMap<G4double> > fMap;
};

void G4VScoringMesh::Accumulate(G4THitsMap<G4double> * map) const
{ 
  G4String psName = map->GetName();
  std::map<G4String, G4THitsMap<G4double> >::const_iterator fMapItr = fMap.find(psName);
  fMapItr->second += *map;
}



#endif


// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4PlacedSolid.hh,v 1.5 2000-11-08 14:22:03 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4PlacedSolid
//
// Class description:
//
// Class for a generic BREP solid placed in a 3D space.
// Attributes are: the solid, a traslation vector and a
// rotation matrix.

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
// ----------------------------------------------------------------------
#ifndef G4PLACEDSOLID_HH
#define G4PLACEDSOLID_HH

#include "G4BREPSolid.hh"
#include "G4RotationMatrix.hh"

class G4PlacedSolid
{

public:  // with description

  G4PlacedSolid();
  G4PlacedSolid(G4BREPSolid*, G4Axis2Placement3D* =0);
  ~G4PlacedSolid();
    // Constructors & destructor

  inline G4bool operator==(const G4PlacedSolid& ps) const;
    // Equality operator.

  inline const G4VSolid*         GetSolid() const;
  inline const G4RotationMatrix* GetRotation() const;
  inline const G4ThreeVector*    GetTranslation() const;
    // Accessors.

private:

  G4PlacedSolid(const G4PlacedSolid&);
  G4PlacedSolid& operator=(const G4PlacedSolid&);
    // Private copy constructor and assignment operator.

private:

  G4BREPSolid*      solid;
  G4RotationMatrix* solidRotation;
  G4ThreeVector*    solidTranslation;

};

#include "G4PlacedSolid.icc"

#endif

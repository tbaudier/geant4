// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ModelingParameters.hh,v 1.4 1999-11-25 14:18:48 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  31st December 1997.
//
// Class Description:
//
// Parameters associated with the modeling of GEANT4 objects.

#ifndef G4MODELINGPARAMETERS_HH
#define G4MODELINGPARAMETERS_HH

#include "globals.hh"
#include "G4VisExtent.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VisAttributes;

class G4ModelingParameters {

public: // With description

  friend ostream& operator << (ostream& os, const G4ModelingParameters&);

  enum RepStyle {
    wireframe,  // Use G4Wireframe.
    polyhedron, // Use G4Polyhedron.
    nurbs,      // Use G4NURBS.
    hierarchy   // Draw as geometry hierarchy (DTREE in GEANT3 language!).
  };
  // RepStyle is used to determine which graphics_reps classes to use,
  // if required.

  G4ModelingParameters ();

  G4ModelingParameters (const G4VisAttributes* pDefaultVisAttributes,
			RepStyle repStyle,
			G4bool isCulling,
			G4bool isCullingInvisible,
			G4bool isDensityCulling,
			G4double visibleDensity,
			G4bool isCullingCovered,
			G4int noOfSides);
  // noOfSides is suggested no. of sides per circle in case a
  // polygonal representation is produced.

  G4ModelingParameters (const G4VisAttributes* pDefaultVisAttributes,
			RepStyle repStyle,
			G4bool isCulling,
			G4bool isCullingInvisible,
			G4bool isDensityCulling,
			G4double visibleDensity,
			G4bool isCullingCovered,
			G4int noOfSides,
			G4bool isViewGeom,
			G4bool isViewHits,
			G4bool isViewDigis);
  // noOfSides is suggested no. of sides per circle in case a
  // polygonal representation is produced.

  ~G4ModelingParameters ();

  // Note: uses default assignment operator and copy constructor.

  G4bool operator != (const G4ModelingParameters&) const;

  // Get and Is functions...
  const G4VisAttributes* GetDefaultVisAttributes () const;
  RepStyle         GetRepStyle                   () const;
  G4bool           IsCulling                     () const;
  G4bool           IsCullingInvisible            () const;
  G4bool           IsDensityCulling              () const;
  G4double         GetVisibleDensity             () const;
  G4bool           IsCullingCovered              () const;
  G4int            GetNoOfSides                  () const;
  G4bool           IsViewGeom                    () const;
  G4bool           IsViewHits                    () const;
  G4bool           IsViewDigis                   () const;

  // Set functions...
  void SetDefaultVisAttributes (const G4VisAttributes* pDefaultVisAttributes);
  void SetRepStyle             (RepStyle);
  void SetCulling              (G4bool);
  void SetCullingInvisible     (G4bool);
  void SetDensityCulling       (G4bool);
  void SetVisibleDensity       (G4double);
  void SetCullingCovered       (G4bool);
  void SetNoOfSides            (G4int);
  void SetViewGeom             ();
  void UnsetViewGeom           ();
  void SetViewHits             ();
  void UnsetViewHits           ();
  void SetViewDigis            ();
  void UnsetViewDigis          ();

  // Other functions...
  void PrintDifferences (const G4ModelingParameters& that) const;

private:

  // Data members...
  const G4VisAttributes* fpDefaultVisAttributes;
  RepStyle    fRepStyle;        // Representation style.
  G4bool      fCulling;         // Culling requested.
  G4bool      fCullInvisible;   // Cull (don't Draw) invisible objects.
  G4bool      fDensityCulling;  // Density culling requested.  If so...
  G4double    fVisibleDensity;  // ...density lower than this not drawn.
  G4bool      fCullCovered;     // Cull daughters covered by opaque mothers.
  G4int       fNoOfSides;       // ...if polygon approximates circle.
  G4bool      fViewGeom;        // View geometry objects.
  G4bool      fViewHits;        // View hits, if any.
  G4bool      fViewDigis;       // View digis, if any.

};

#include "G4ModelingParameters.icc"

#endif

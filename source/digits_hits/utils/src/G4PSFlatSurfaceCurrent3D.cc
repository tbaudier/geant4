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
// $Id: G4PSFlatSurfaceCurrent3D.cc,v 1.1 2007-08-14 16:51:10 taso Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4PSFlatSurfaceCurrent3D
#include "G4PSFlatSurfaceCurrent3D.hh"

///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Current.
//  Current version assumes only for G4Box shape, and the surface
//  is defined at the -Z plane of the box.
//   The current is given in the unit of area.
//    e.g.  (Number of tracks)/mm2.
//
// Surface is defined at the -Z surface.
// Direction                  -Z   +Z
//   0  IN || OUT            ->|<-  |      fCurrent_InOut
//   1  IN                   ->|    |      fCurrent_In
//   2  OUT                    |<-  |      fCurrent_Out
//
//
// Created: 2008-08-14  Tsukasa ASO
///////////////////////////////////////////////////////////////////////////////


G4PSFlatSurfaceCurrent3D::G4PSFlatSurfaceCurrent3D(G4String name, G4int direction,
						   G4int nx, G4int ny, G4int nz,
						   G4int depX, G4int depY, G4int depZ)
    :G4PSFlatSurfaceCurrent(name,direction),fNx(nx),fNy(ny),fNz(nz),
     fDepthX(depX),fDepthY(depY),fDepthZ(depZ)
{;}

G4PSFlatSurfaceCurrent3D::~G4PSFlatSurfaceCurrent3D()
{;}

G4int G4PSFlatSurfaceCurrent3D::GetIndex(G4Step* aStep)
{
  const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int ix = touchable->GetReplicaNumber(fDepthX);
  G4int iy = touchable->GetReplicaNumber(fDepthY);
  G4int iz = touchable->GetReplicaNumber(fDepthZ);
  
  return iy*fNx*fNz+ix*fNz+iz;
}

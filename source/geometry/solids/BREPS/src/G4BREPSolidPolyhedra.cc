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
// $Id: G4BREPSolidPolyhedra.cc,v 1.20 2002-02-12 18:39:25 radoone Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// G4BREPSolidPolyhedra.cc
//
// ----------------------------------------------------------------------
// The polygonal solid G4BREPSolidPolyhedra is a shape defined by an inner 
// and outer polygonal surface and two planes perpendicular to the Z axis. 
// Each polygonal surface is created by linking a series of polygons created 
// at different planes perpendicular to the Z-axis. All these polygons all 
// have the same number of sides (sides) and are defined at the same Z planes 
// for both inner and outer polygonal surfaces. 
// ----------------------------------------------------------------------
//
// History
// -------
// Bugfix 266 by Radovan Chytracek:
// The situation when phi1 = 0 dphi1 = 2*pi and all RMINs = 0.0 is handled
// now. In this case the inner planes are not created. The fix goes even
// further this means it consideres more than 2 z-planes and inner planes
// are not created whenever two consecutive RMINs are = 0.0 .
// 
// Corrections by S.Giani:
// - Xaxis now corresponds to phi=0
// - partial angle = phiTotal / Nsides
// - end planes exact boundary calculation for phiTotal < 2pi 
//   (also including case with RMIN=RMAX) 
// - Xaxis now properly rotated to compute correct scope of vertixes
// - corrected surface orientation for outer faces parallel to Z
// - completed explicit setting of the orientation for all faces
// - some comparison between doubles avoided by using tolerances 
// - visualisation parameters made consistent with the use made by
//   constructor of the input arguments (i.e. circumscribed radius).
// ----------------------------------------------------------------------

#include "G4BREPSolidPolyhedra.hh"
#include "G4FPlane.hh"

G4BREPSolidPolyhedra::G4BREPSolidPolyhedra(const G4String& name,
					   G4double phi1,
					   G4double dphi,
					   G4int    sides,
					   G4int    num_z_planes,      
					   G4double z_start,
					   G4double z_values[],
					   G4double RMIN[],
					   G4double RMAX[] )
  : G4BREPSolid(name)
{
  G4int sections           = num_z_planes - 1;
  
  if( dphi >= 2*pi-perMillion ) {
    nb_of_surfaces = 2*(sections * sides) + 2;
  } else {
    nb_of_surfaces = 2*(sections * sides) + 4;
  }


  //SurfaceVec = new G4Surface*[nb_of_surfaces];
  G4int       MaxNbOfSurfaces = nb_of_surfaces;
  G4Surface** MaxSurfaceVec   = new G4Surface*[MaxNbOfSurfaces];
  
  G4Vector3D Axis(0,0,1);
  G4Vector3D XAxis(1,0,0);
  G4Vector3D TmpAxis;
  G4Point3D  Origin(0,0,z_start);    
  G4Point3D  LocalOrigin(0,0,z_start);    
  G4double   Length;
  G4int      Count     = 0 ;
  G4double   PartAngle = (dphi)/sides;


  ///////////////////////////////////////////////////
   

  for(G4int a=0;a<sections;a++)
  {
    TmpAxis= XAxis;
    TmpAxis.rotateZ(phi1);
    Length = z_values[a+1] - z_values[a];
    
    if( Length != 0.0 ) {
      // L. Broglia: Be careful in the construction of the planes, see G4FPlane 
      for( G4int b = 0; b < sides; b++ ) {
        // Create inner side by calculation of points for the planar surface boundary
        // The order of the points gives the surface sense -> changed to explicit sense set-up by R. Chytracek, 12/02/2002
        // We must check if a pair of two consecutive RMINs is not = 0.0, this means no inner plane exists!
        if( RMIN[a] != 0.0 ) {
          if( RMIN[a+1] != 0.0 ) {
            // Standard case
            MaxSurfaceVec[Count] = CreateTrapezoidalSurface( RMIN[a], RMIN[a+1], LocalOrigin, Length,
                                                             TmpAxis, PartAngle, EInverse );
          } else {
            // The special case of r1 > r2 where we end at the point (0,0,z[a+1])
            MaxSurfaceVec[Count] = CreateTriangularSurface( RMIN[a], RMIN[a+1], LocalOrigin, Length,
                                                            TmpAxis, PartAngle, EInverse );
          }
        } else if( RMIN[a+1] != 0.0 ) {
            // The special case of r1 < r2 where we start at the point ( 0,0,z[a])
            MaxSurfaceVec[Count] = CreateTriangularSurface( RMIN[a], RMIN[a+1], LocalOrigin, Length,
                                                            TmpAxis, PartAngle, EInverse );
        } else {
          // Insert nothing into the vector of sufaces, we'll replicate the vector anyway later
          MaxSurfaceVec[Count] = 0;
          // We need to reduce the number of planes by 1, one we have just skipped
          nb_of_surfaces--;
        }      

        Count++;

        // Rotate axis back for the other surface point calculation
        TmpAxis.rotateZ(-PartAngle);

        // Create outer side

        if( RMAX[a] != 0.0 ) {
          if( RMAX[a+1] != 0.0 ) {
            // Standard case
            MaxSurfaceVec[Count] = CreateTrapezoidalSurface( RMAX[a], RMAX[a+1], LocalOrigin, Length,
                                                             TmpAxis, PartAngle, ENormal );
          } else {
            // The special case of r1 > r2 where we end at the point (0,0,z[a+1])
            MaxSurfaceVec[Count] = CreateTriangularSurface( RMAX[a], RMAX[a+1], LocalOrigin, Length,
                                                            TmpAxis, PartAngle, ENormal );
          }
        } else if( RMAX[a+1] != 0.0 ) {
            // The special case of r1 < r2 where we start at the point ( 0,0,z[a])
            MaxSurfaceVec[Count] = CreateTriangularSurface( RMAX[a], RMAX[a+1], LocalOrigin, Length,
                                                            TmpAxis, PartAngle, ENormal );
        } else {
            // Two consecutive RMAX values can't be zero as it's against the definition of BREP polyhedra
            G4Exception( "\nTwo consecutive RMAX values can't be zero!\n" );
        }

        Count++;

        LocalOrigin = LocalOrigin + (Length*Axis);
      }
    } else {
      // We're about to create a planar surface perpendicular to z-axis
    }
  }
  
  // Create end planes
  if(dphi >= 2*pi-perMillion) {
    // Create only end planes
    G4Point3DVector EndPointList(sides);
    G4Point3DVector InnerPointList(sides);
    G4Point3DVector EndPointList2(sides);
    G4Point3DVector InnerPointList2(sides);	
    TmpAxis = XAxis;
    TmpAxis.rotateZ(phi1);	
    TmpAxis.rotateZ(dphi);
    
    for(G4int c=0;c<sides;c++) {
      // outer polyline for origin end
      EndPointList[c] = Origin + (RMAX[0] * TmpAxis);
      InnerPointList[c] = Origin + (RMIN[0] * TmpAxis);
      EndPointList2[c] = LocalOrigin + (RMAX[sections] * TmpAxis);
      InnerPointList2[c] = LocalOrigin + (RMIN[sections] * TmpAxis);
      TmpAxis.rotateZ(-PartAngle);
    }
    
    if( RMIN[0] != 0.0 && RMAX[0] != 0.0 ) {
      // Add to surface list and set sense
//      MaxSurfaceVec[MaxNbOfSurfaces-2] = new G4FPlane(&EndPointList, &InnerPointList);
      MaxSurfaceVec[Count] = new G4FPlane(&EndPointList, &InnerPointList, EInverse);
    } else if( RMAX[0] != 0.0 ) {
      // Add to surface list and set sense
//      MaxSurfaceVec[MaxNbOfSurfaces-2] = new G4FPlane(&EndPointList, 0);
      MaxSurfaceVec[Count] = new G4FPlane(&EndPointList, 0, EInverse);
    } else {
      // No surface being created
      MaxSurfaceVec[Count] = 0;
      nb_of_surfaces--;
    }
    
    Count++;

    if( RMIN[sections] != 0.0 && RMAX[sections] != 0.0 ) {    
      // Add to surface list and reverse sense
//      MaxSurfaceVec[MaxNbOfSurfaces-1] = new G4FPlane(&EndPointList2, &InnerPointList2, 0);
      MaxSurfaceVec[Count] = new G4FPlane(&EndPointList2, &InnerPointList2, 0);
    } else if( RMAX[sections] != 0.0 ) {
      // Add to surface list and reverse sense
//      MaxSurfaceVec[MaxNbOfSurfaces-1] = new G4FPlane(&EndPointList2, 0, 0);
      MaxSurfaceVec[Count] = new G4FPlane(&EndPointList2, 0, 0);
    } else {
      // No surface being created
      MaxSurfaceVec[Count] = 0;
      nb_of_surfaces--;
    }
  } else {
    // If phi section, create a single boundary (case with RMIN=0 included)
    TmpAxis = XAxis;
    TmpAxis.rotateZ(phi1);	
    TmpAxis.rotateZ(dphi);
    
    // Create end planes 
    G4Point3DVector EndPointList ((sides+1)*2);
    G4Point3DVector EndPointList2((sides+1)*2);	      
    
    for(G4int c=0;c<sides+1;c++) {
      // outer polylines for origin end and opposite side
      EndPointList[c]  = Origin + (RMAX[0] * TmpAxis);
      EndPointList[(sides+1)*2-1-c]  = Origin + (RMIN[0] * TmpAxis);
      EndPointList2[c] = LocalOrigin + (RMAX[sections] * TmpAxis);
      EndPointList2[(sides+1)*2-1-c] = LocalOrigin + (RMIN[sections] * TmpAxis);
      TmpAxis.rotateZ(-PartAngle);
    }
    
    // Create the lateral planars
    TmpAxis             = XAxis;
    G4Vector3D TmpAxis2 = XAxis;
    TmpAxis.rotateZ(phi1);
    TmpAxis2.rotateZ(phi1);
    TmpAxis2.rotateZ(dphi);	
    
    LocalOrigin      = Origin;
    G4int points     = sections*2+2;
    G4int PointCount = 0;
    
    G4Point3DVector GapPointList(points);
    G4Point3DVector GapPointList2(points);
    
    for(G4int d=0;d<sections+1;d++) {
      GapPointList[PointCount] = LocalOrigin + (RMAX[d]*TmpAxis);
      GapPointList[points-1-PointCount] = LocalOrigin + (RMIN[d]*TmpAxis);	    
      
      GapPointList2[PointCount] = LocalOrigin + (RMAX[d]*TmpAxis2);
      GapPointList2[points-1-PointCount] = LocalOrigin + (RMIN[d]*TmpAxis2);	 
   	         
      PointCount++;

      Length = z_values[d+1] - z_values[d];
      LocalOrigin = LocalOrigin+(Length*Axis);
    }
    
    // Add the lateral planars to the surfaces list and set/reverse sense
    
//    SurfaceVec[nb_of_surfaces-4] = new G4FPlane(&GapPointList);
//    SurfaceVec[nb_of_surfaces-3] = new G4FPlane(&GapPointList2, 0, 0);
    MaxSurfaceVec[Count++] = new G4FPlane( &GapPointList,  0, ENormal );
    MaxSurfaceVec[Count++] = new G4FPlane( &GapPointList2, 0, EInverse );
    
    //Add the end planes to the surfaces list and set/reverse sense
    
    if(RMAX[0]-RMIN[0] >= perMillion) {
//      SurfaceVec[nb_of_surfaces-2] = new G4FPlane(&EndPointList);
      MaxSurfaceVec[Count] = new G4FPlane( &EndPointList, 0, EInverse );
    }
    else {
      MaxSurfaceVec[Count] = 0;
      nb_of_surfaces--;
    };
    
    Count++;
    
    if(RMAX[sections]-RMIN[sections] >= perMillion) {
//      SurfaceVec[nb_of_surfaces-1] = new G4FPlane(&EndPointList2, 0, 0);
      MaxSurfaceVec[Count] = new G4FPlane( &EndPointList2, 0, ENormal );
    } else {
      MaxSurfaceVec[Count] = 0;
      nb_of_surfaces--;
    };    	    
  }

  // Now let's replicate the relevant surfaces into polyhedra's vector of surfaces
  SurfaceVec = new G4Surface*[nb_of_surfaces];
  G4int sf = 0;
  for( G4int srf = 0; srf < MaxNbOfSurfaces; srf++ ) {
    if( MaxSurfaceVec[srf] != 0 ) {
      if( sf < nb_of_surfaces ) {
        SurfaceVec[sf] = MaxSurfaceVec[srf];
      }
      sf++;
//        SurfaceVec[sf++] = MaxSurfaceVec[srf];
    }
  }
    G4cout << "sf: "              << sf
           << " nb_of_surfaces: " << nb_of_surfaces
           << " Count: "          << Count
           << G4endl;

  if( sf != nb_of_surfaces ) {
    G4cerr << "Bad number of surfaces!\a\n"
           << "sf: "              << sf
           << " nb_of_surfaces: " << nb_of_surfaces
           << " Count: "          << Count
           << G4endl;
    // Should we call G4Exception here ?
  }

  // Clean up the temporary vector of surfaces
  delete [] MaxSurfaceVec;
    
  // Store the original parameters, to be used in visualisation
  // Note radii are not scaled because this BREP uses the radius of the
  // circumscribed circle and also graphics_reps/G4Polyhedron uses the radius of
  // the circumscribed circle.
  original_parameters.Start_angle= phi1;
  original_parameters.Opening_angle= dphi;		   
  original_parameters.Sides= sides;
  original_parameters.Num_z_planes= num_z_planes; 
  original_parameters.Z_values= new G4double[num_z_planes];
  original_parameters.Rmin= new G4double[num_z_planes];
  original_parameters.Rmax= new G4double[num_z_planes];
  
  G4double rFactor = 1.;

  for(G4int is=0;is<num_z_planes;is++)
  {
    original_parameters.Z_values[is]= z_values[is]; 
    original_parameters.Rmin[is]= RMIN[is]/rFactor;
    original_parameters.Rmax[is]= RMAX[is]/rFactor;
  }

  ///////////////////////////////////////////////////
  // Print for debugging

#ifdef G4VERBOSE
  static G4int print_pgone_parameters = 1;
  
  if(print_pgone_parameters)
  {
    G4cout << "Parameters of the G4PGone " << name << G4endl;
    G4cout << "  starting angle =" << original_parameters.Start_angle << G4endl;
    G4cout << "  opening angle =" << original_parameters.Opening_angle << G4endl;
    G4cout << "  sides =" << original_parameters.Sides << G4endl;
    G4cout << "  nb of z planes=" << original_parameters.Num_z_planes << G4endl;

    for (G4int nb = 0; nb <= sections; nb++)
    {
      G4cout << "   Z[" << nb << "] = " << original_parameters.Z_values[nb];
      G4cout << "   Rmin[" << nb << "] = " << original_parameters.Rmin[nb];
      G4cout << "   Rmax[" << nb << "] = " << original_parameters.Rmax[nb] 
	     << G4endl;
    }   
  }
#endif

  // z_values[0]  should be equal to z_start, for consistency 
  //   with what the constructor does.
  // Otherwise the z_values that are shifted by (z_values[0] - z_start) , 
  //   because z_values are only used in the form 
  //      length = z_values[d+1] - z_values[d];         // JA Apr 2, 97
  
  if( z_values[0] != z_start )
  {
    G4cerr << "ERROR in creating G4BREPSolidPolyhedra: "  << 
      " z_values[0]= " << z_values[0] << " is not equal to " << 
      " z_start= " , z_start;
    // G4Exception(" Error in creating G4BREPSolidPolyhedra: z_values[0] must be equal to z_start" );
    original_parameters.Z_values[0]= z_start; 
  }

  active=1;
  Initialize(); 
}

G4BREPSolidPolyhedra::~G4BREPSolidPolyhedra()
{
  delete[] original_parameters.Z_values;
  delete[] original_parameters.Rmin;
  delete[] original_parameters.Rmax;
}

void G4BREPSolidPolyhedra::Initialize()
{
  // Calc bounding box for solids and surfaces
  // Convert concave planes to convex     
  ShortestDistance=1000000;
  CheckSurfaceNormals();
  if(!Box || !AxisBox)
    IsConvex();
  
  CalcBBoxes();
}

void G4BREPSolidPolyhedra::Reset() const
{
  Active(1);
  ((G4BREPSolidPolyhedra*)this)->intersectionDistance=kInfinity;
  StartInside(0);
  for(register G4int a=0;a<nb_of_surfaces;a++)
    SurfaceVec[a]->Reset();
  ShortestDistance = kInfinity;
}

EInside G4BREPSolidPolyhedra::Inside(register const G4ThreeVector& Pt) const
{
  // This function find if the point Pt is inside, 
  // outside or on the surface of the solid

  G4Vector3D v(1, 0, 0.01);
  G4Vector3D Pttmp(Pt);
  G4Vector3D Vtmp(v);
  G4Ray r(Pttmp, Vtmp);
  
  // Check if point is inside the Polyhedra bounding box
  if( !GetBBox()->Inside(Pttmp) )
  {
    return kOutside;
  }

  // Set the surfaces to active again
  Reset();
  
  // Test if the bounding box of each surface is intersected
  // by the ray. If not, the surface become deactive.
  TestSurfaceBBoxes(r);
  
  G4int hits=0, samehit=0;

  for(G4int a=0; a < nb_of_surfaces; a++)
  {
    if(SurfaceVec[a]->IsActive())
    {
      // count the number of intersections.
      // if this number is odd, the start of the ray is
      // inside the volume bounded by the surfaces, so 
      // increment the number of intersection by 1 if the 
      // point is not on the surface and if this intersection 
      // was not found before
      if( (SurfaceVec[a]->Intersect(r)) & 1 )
      {
	      // test if the point is on the surface
	      if(SurfaceVec[a]->GetDistance() <= kCarTolerance*kCarTolerance)
        {
	        return kSurface;
        }
  
	      // test if this intersection was found before
	      for(G4int i=0; i<a; i++)
	        if(SurfaceVec[a]->GetDistance() == SurfaceVec[i]->GetDistance())
	        {
	          samehit++;
	          break;
	        }
	
	      // count the number of surfaces intersected by the ray
	      if(!samehit)
        {
	        hits++;
        }
      }
    }
  }
   
  // if the number of surfaces intersected is odd,
  // the point is inside the solid
  if(hits&1)
  {
    return kInside;
  }
  else
  {
    return kOutside;
  }
}

G4ThreeVector
G4BREPSolidPolyhedra::SurfaceNormal(const G4ThreeVector& Pt) const
{
  // This function calculates the normal of the surface
  // at a point on the surface
  // Note : the sense of the normal depends on the sense of the surface 

  G4int        iplane;
    
  G4Vector3D norm;
  G4Ray r( Pt, G4Vector3D(1, 0, 0) );

  // Find on which surface the point is
  for(iplane = 0; iplane < nb_of_surfaces; iplane++)
  {
    if(SurfaceVec[iplane]->HowNear(Pt) < kCarTolerance)
      // the point is on this surface
      break;
  }
  
  // calcul of the normal at this point
  norm = SurfaceVec[iplane]->SurfaceNormal(Pt);

  G4ThreeVector n( norm.x(), norm.y(), norm.z() );
  n = n.unit();

  return n;
}

G4double G4BREPSolidPolyhedra::DistanceToIn(const G4ThreeVector& Pt) const
{
  // Calculates the shortest distance ("safety") from a point
  // outside the solid to any boundary of this solid.
  // Return 0 if the point is already inside.


  G4double *dists = new G4double[nb_of_surfaces];
  G4int a;

  // Set the surfaces to active again
  Reset();
   
  // compute the shortest distance of the point to each surfaces
  // Be careful : it's a signed value
  for(a=0; a< nb_of_surfaces; a++)  
    dists[a] = SurfaceVec[a]->HowNear(Pt);
     
  G4double Dist = kInfinity;
  
  // if dists[] is positive, the point is outside
  // so take the shortest of the shortest positive distances
  // dists[] can be equal to 0 : point on a surface
  // ( Problem with the G4FPlane : there is no inside and no outside...
  //   So, to test if the point is inside to return 0, utilize the Inside
  //   function. But I don`t know if it is really needed because dToIn is 
  //   called only if the point is outside )
  for(a = 0; a < nb_of_surfaces; a++)
    if( fabs(Dist) > fabs(dists[a]) ) 
      //if( dists[a] >= 0)
	Dist = dists[a];
  
  delete[] dists;

  if(Dist == kInfinity)
  {
    // the point is inside the solid or on a surface
    return 0;
  }
  else 
  {
    //return Dist;
    return fabs(Dist);
  }
}

G4double
G4BREPSolidPolyhedra::DistanceToIn(register const G4ThreeVector& Pt, 
				   register const G4ThreeVector& V) const
{
  // Calculates the distance from a point outside the solid
  // to the solid`s boundary along a specified direction vector.
  // 	
  // Note : Intersections with boundaries less than the 
  //	    tolerance must be ignored if the direction 
  // 	    is away from the boundary
  
  G4int a;
  
  // Set the surfaces to active again
  Reset();
  
  G4double halfTolerance = kCarTolerance*0.5;    
  G4Vector3D Pttmp(Pt);
  G4Vector3D Vtmp(V);   
  G4Ray r(Pttmp, Vtmp);

  // Test if the bounding box of each surface is intersected
  // by the ray. If not, the surface become deactive.
  TestSurfaceBBoxes(r);
  
  ShortestDistance = kInfinity;
  
  for(a=0; a< nb_of_surfaces; a++)
  {
    if(SurfaceVec[a]->IsActive())
    {
      G4int intersects = SurfaceVec[a]->Intersect(r);
      // test if the ray intersect the surface
      if( intersects != 0 )
      {
        G4double surfDistance = SurfaceVec[a]->GetDistance();
        
	      // if more than 1 surface is intersected,
	      // take the nearest one
	      if( surfDistance < ShortestDistance )
        {
	        //if( surfDistance > halfTolerance )
	        if( surfDistance > halfTolerance*halfTolerance )
	        {
	          ShortestDistance = surfDistance;
	        }
	        else
	        {
	          // the point is within the boundary
	          // ignored it if the direction is away from the boundary	     
	          G4Vector3D Norm = SurfaceVec[a]->SurfaceNormal(Pttmp);

	          if( (Norm * Vtmp) < 0 )
            {
	            ShortestDistance = surfDistance;
            }
	        }
        }
      }
    }
  }
  
  // Be carreful !
  // SurfaceVec->Distance is in fact the squared distance
  if(ShortestDistance != kInfinity)
  {
    return sqrt(ShortestDistance);
  }
  else
  {
    // no intersection, return kInfinity
    return kInfinity; 
  }
}

G4double
G4BREPSolidPolyhedra::DistanceToOut(register const G4ThreeVector& Pt, 
				    register const G4ThreeVector& V, 
				    const G4bool calcNorm, 
				    G4bool *validNorm, 
				    G4ThreeVector *n            ) const
{
  // Calculates the distance from a point inside the solid
  // to the solid`s boundary along a specified direction vector.
  // Return 0 if the point is already outside (even number of
  // intersections greater than the tolerance).
  //
  // Note : If the shortest distance to a boundary is less 
  // 	    than the tolerance, it is ignored. This allows
  // 	    for a point within a tolerant boundary to leave
  //	    immediately

  G4int parity = 0;

  // Set the surfaces to active again
  Reset();

  const G4double halfTolerance = kCarTolerance*0.5;    
  G4Vector3D Ptv = Pt;
  G4int a;

  // I don`t understand this line
  if(validNorm)
    *validNorm=false;

  G4Vector3D Pttmp(Pt);
  G4Vector3D Vtmp(V);   
  
  G4Ray r(Pttmp, Vtmp);

  // Test if the bounding box of each surface is intersected
  // by the ray. If not, the surface become deactive.
  TestSurfaceBBoxes(r);
  
  ShortestDistance = kInfinity; // this is actually the square of the distance
 
  for(a=0; a< nb_of_surfaces; a++)
  {
    double surfDistance = SurfaceVec[a]->GetDistance();
    
    if(SurfaceVec[a]->IsActive())
    {
      G4int intersects = SurfaceVec[a]->Intersect(r);
      // test if the ray intersects the surface
      if( intersects != 0 )
      {
        parity += 1;
	
	      // if more than 1 surface is intersected,
	      // take the nearest one
	      if( surfDistance < ShortestDistance )
        {
          if( surfDistance > halfTolerance*halfTolerance )
	        {
	          ShortestDistance = surfDistance;
	        }
	        else
	        {
	          // the point is within the boundary: ignore it
	          parity -= 1;
	        }
        }
      }      
    }
  }

  // Be careful !
  // SurfaceVec->Distance is in fact the squared distance
  if((ShortestDistance != kInfinity) && (parity&1))
  {
    return sqrt(ShortestDistance);
  }
  else
  {
    // if no intersection is found, the point is outside
    // so return 0
    return 0; 
  }
}

G4double G4BREPSolidPolyhedra::DistanceToOut(const G4ThreeVector& Pt) const
{
  // Calculates the shortest distance ("safety") from a point
  // inside the solid to any boundary of this solid.
  // Return 0 if the point is already outside.	

  G4double *dists = new G4double[nb_of_surfaces];
  G4int a;

  // Set the surfaces to active again
  Reset();
  
  // calcul of the shortest distance of the point to each surfaces
  // Be carreful : it's a signed value
  for(a=0; a< nb_of_surfaces; a++) {
    dists[a] = SurfaceVec[a]->HowNear(Pt);
  }

  G4double Dist = kInfinity;
  
  // if dists[] is negative, the point is inside
  // so take the shortest of the shortest negative distances
  // dists[] can be equal to 0 : point on a surface
  // ( Problem with the G4FPlane : there is no inside and no outside...
  //   So, to test if the point is outside to return 0, utilize the Inside
  //   function. But I don`t know if it is really needed because dToOut is 
  //   called only if the point is inside )

  for(a = 0; a < nb_of_surfaces; a++)  {
    if( fabs(Dist) > fabs(dists[a]) ) {
      //if( dists[a] <= 0)
	    Dist = dists[a];
    }
  }
  
  delete[] dists;

  if(Dist == kInfinity) {
    // the point is ouside the solid or on a surface
    return 0;
  } else {
    // return Dist;
    return fabs(Dist);
  }
}

G4Surface* G4BREPSolidPolyhedra::CreateTrapezoidalSurface( G4double r1, G4double r2,
                                                           const G4Point3D& origin, G4double distance,
                                                           G4Vector3D& xAxis, G4double partAngle,
                                                           ESurfaceSense sense )
{
  // The surface to be returned
  G4Surface* trapsrf = 0;
  G4Point3DVector PointList(4);
  G4Vector3D zAxis(0,0,1);
  
  PointList[0] = origin + ( r1       * xAxis);
  PointList[3] = origin + ( distance * zAxis)   + (r2 * xAxis);
  
  xAxis.rotateZ( partAngle );
  
  PointList[2] = origin + ( distance * zAxis)   + (r2 * xAxis);
  PointList[1] = origin + ( r1       * xAxis);	  

  // Return the planar trapezoidal surface
  trapsrf = new G4FPlane( &PointList, 0, sense );
  
  return trapsrf;
}

G4Surface* G4BREPSolidPolyhedra::CreateTriangularSurface( G4double r1, G4double r2,
                                                          const G4Point3D& origin, G4double distance,
                                                          G4Vector3D& xAxis, G4double partAngle,
                                                          ESurfaceSense sense )
{
  // The surface to be returned
  G4Surface*      trapsrf = 0;
  G4Point3DVector PointList(3);
  G4Vector3D      zAxis(0,0,1);
  
  PointList[0] = origin + ( r1       * xAxis);
  PointList[2] = origin + ( distance * zAxis)   + (r2 * xAxis);
  
  xAxis.rotateZ( partAngle );
  
  if( r1 < r2 ) {
    PointList[1] = origin + ( distance * zAxis)   + (r2 * xAxis);
  } else {
    PointList[1] = origin + ( r1       * xAxis);	  
  }

  // Return the planar trapezoidal surface
  trapsrf = new G4FPlane( &PointList, 0, sense );
  
  return trapsrf;
}

//  In graphics_reps:   
#include "G4Polyhedron.hh"   

G4Polyhedron* G4BREPSolidPolyhedra::CreatePolyhedron() const
{
  return new G4PolyhedronPgon( original_parameters.Start_angle, 
			       original_parameters.Opening_angle, 
			       original_parameters.Sides, 
			       original_parameters.Num_z_planes, 
			       original_parameters.Z_values,
			       original_parameters.Rmin,
			       original_parameters.Rmax);
}

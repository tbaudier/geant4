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
//////////////////////////////////////////////////////////////////////////
// $Id: G4BREPSolidBoxTest.cc,v 1.8 2001-07-20 16:07:38 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//////////////////////////////////////////////////////////////////////////
//
//
// BREP solid test, create by L. Broglia, 20/10/98
// modification of old G4Gerep test
//


#include "G4Timer.hh"
#include "globals.hh"
#include "G4BREPSolid.hh"
#include "G4BREPSolidBox.hh"


int main()
{
  G4ThreeVector tStart(19000,0,10.1);
  G4ThreeVector tDir(-1,0,0);
  G4ThreeVector tStart2(0,0,10);
  G4ThreeVector pt(0,0,50);
  G4ThreeVector pt2(100000,0,50);

G4cout << "\n ============   Box test   ================"; 

  G4BREPSolidBox *myCalBox = new G4BREPSolidBox 
    ( "MyBox",
      G4Point3D(-1500, -1500, -1000),
      G4Point3D(-1500, -1500,  1000),       
      G4Point3D(-1500,  1500,  1000),
      G4Point3D(-1500,  1500, -1000),
      G4Point3D( 1500, -1500, -1000),       
      G4Point3D( 1500, -1500,  1000),
      G4Point3D( 1500,  1500,  1000),
      G4Point3D( 1500,  1500, -1000) );

  G4cout << "\n\nBox created ! ";
  
  // -> Check methods :
  //  - Inside
  //  - DistanceToIn
  //  - DistanceToOut

  G4ThreeVector Pt[4];
  Pt[0] = G4ThreeVector(    1,    1,  100);
  Pt[1] = G4ThreeVector( 1000, 1000, 5000);
  Pt[2] = G4ThreeVector(-1000,-1000,-5000);
  Pt[3] = G4ThreeVector(    0,    0, -100);

  G4ThreeVector Dir[4];
  Dir[0] = G4ThreeVector(    1,    0,    0);
  Dir[1] = G4ThreeVector(    0,    1,    0);
  Dir[2] = G4ThreeVector(    0,    0,    1);
  Dir[3] = G4ThreeVector(    1,    1,   -1);

  EInside in[4];
  G4double dist[4][3];

  G4cout<<"\n\n";

  for (G4int a = 0; a<4; a++)
  {
    in[a] = myCalBox->Inside(Pt[a]);

    G4cout<<"----------------------------\n\n";

    G4cout<<"x="<<Pt[a].x()
	  <<"  y="<<Pt[a].y()<<"  z="<<Pt[a].z();
      
    if( in[a] == kInside )
    {
      G4cout <<" is inside"<<G4endl;

      dist[a][1] = myCalBox->DistanceToOut(Pt[a]);
      G4cout<<"\nDistance to out is :"<<dist[a][1]<<G4endl;

      G4cout << "\nDir   : x=" << Dir[a].x() 
	   << " y=" << Dir[a].y() 
	   << " z=" << Dir[a].z()<<G4endl;
      dist[a][2] = myCalBox->DistanceToOut(Pt[a], Dir[a]);
      G4cout<<"Distance to out is :"<<dist[a][2]<<G4endl;

    }
    else
    {
      G4cout <<" is outside"<<G4endl;

      dist[a][1] = myCalBox->DistanceToIn(Pt[a]);
      G4cout<<"\nDistance to in is :"<<dist[a][1]<<G4endl;

      G4cout << "\nDir   : x=" << Dir[a].x() 
	   << " y=" << Dir[a].y() 
	   << " z=" << Dir[a].z()<<G4endl;
      dist[a][2] = myCalBox->DistanceToIn(Pt[a], Dir[a]);
      G4cout<<"Distance to in is :"<<dist[a][2];
    }
    G4cout<<G4endl;
  }

  return EXIT_SUCCESS;
}


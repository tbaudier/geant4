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
// $Id: G4OpenGLSceneHandler.cc,v 1.46 2007-01-05 16:43:01 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Andrew Walkden  27th March 1996
// OpenGL stored scene - creates OpenGL display lists.
// OpenGL immediate scene - draws immediately to buffer
//                           (saving space on server).

#ifdef G4VIS_BUILD_OPENGL_DRIVER

// Included here - problems with HP compiler if not before other includes?
#include "G4NURBS.hh"

// Here follows a special for Mesa, the OpenGL emulator.  Does not affect
// other OpenGL's, as far as I'm aware.   John Allison 18/9/96.
#define CENTERLINE_CLPP  /* CenterLine C++ workaround: */
// Also seems to be required for HP's CC and AIX xlC, at least.

#include "G4OpenGLSceneHandler.hh"
#include "G4OpenGLViewer.hh"
#include "G4OpenGLFontBaseStore.hh"
#include "G4OpenGLTransform3D.hh"
#include "G4Point3D.hh"
#include "G4Normal3D.hh"
#include "G4Transform3D.hh"
#include "G4Polyline.hh"
#include "G4Text.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4VMarker.hh"
#include "G4Polyhedron.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Scene.hh"
#include "G4VisExtent.hh"

G4OpenGLSceneHandler::G4OpenGLSceneHandler (G4VGraphicsSystem& system,
			      G4int id,
			      const G4String& name):
  G4VSceneHandler (system, id, name)
{}

G4OpenGLSceneHandler::~G4OpenGLSceneHandler ()
{
  ClearStore ();
}

const GLubyte G4OpenGLSceneHandler::fStippleMaskHashed [128] = {
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55
};

const G4Polyhedron* G4OpenGLSceneHandler::CreateSectionPolyhedron ()
{
  // Clipping done in G4OpenGLViewer::SetView.
  return 0;

  // But...OpenGL no longer seems to reconstruct clipped edges, so,
  // when the BooleanProcessor is up to it, abandon this and use
  // generic clipping in G4VSceneHandler::CreateSectionPolyhedron...
  // return G4VSceneHandler::CreateSectionPolyhedron();
}

const G4Polyhedron* G4OpenGLSceneHandler::CreateCutawayPolyhedron ()
{
  // Cutaway done in G4OpenGLViewer::SetView.
  return 0;

  // But...if not, when the BooleanProcessor is up to it...
  // return G4VSceneHandler::CreateCutawayPolyhedron();
}

void G4OpenGLSceneHandler::AddPrimitive (const G4Polyline& line)
{
  G4int nPoints = line.size ();
  if (nPoints <= 0) return;

  // Note: colour treated in sub-class.

  if (fpViewer -> GetViewParameters ().IsMarkerNotHidden ())
    glDisable (GL_DEPTH_TEST);
  else {glEnable (GL_DEPTH_TEST); glDepthFunc (GL_LESS);}

  glDisable (GL_LIGHTING);

  // Get vis attributes - pick up defaults if none.
  const G4VisAttributes* pVA =
    fpViewer -> GetApplicableVisAttributes (line.GetVisAttributes ());

  G4double lineWidth = GetLineWidth(pVA);
  glLineWidth(lineWidth);

  glBegin (GL_LINE_STRIP);
  for (G4int iPoint = 0; iPoint < nPoints; iPoint++) {
  G4double x, y, z;
    x = line[iPoint].x(); 
    y = line[iPoint].y();
    z = line[iPoint].z();
    glVertex3d (x, y, z);
  }
  glEnd ();
}

void G4OpenGLSceneHandler::AddPrimitive (const G4Text& text) {

  const G4Colour& c = GetTextColour (text);  // Picks up default if none.
  MarkerSizeType sizeType;
  G4double size = GetMarkerSize (text, sizeType);
  G4ThreeVector position (text.GetPosition ());
  G4String textString = text.GetText();

  G4int font_base = G4OpenGLFontBaseStore::GetFontBase(fpViewer,size);
  if (font_base < 0) {
    static G4int callCount = 0;
    ++callCount;
    if (callCount <= 10 || callCount%100 == 0) {
      G4cout <<
	"G4OpenGLSceneHandler::AddPrimitive (const G4Text&) call count "
	     << callCount <<
	"\n  No fonts available."
	"\n  Called with text \""
	     << text.GetText ()
	     << "\"\n  at " << position
	     << ", size " << size
	     << ", offsets " << text.GetXOffset () << ", " << text.GetYOffset ()
	     << ", type " << G4int(sizeType)
	     << ", colour " << c
	     << G4endl;
    }
    return;
  }
  const char* textCString = textString.c_str();
  glColor3d (c.GetRed (), c.GetGreen (), c.GetBlue ());
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_LIGHTING);
  
  glRasterPos3f(position.x(),position.y(),position.z());
  // No action on offset or layout at present.
  glPushAttrib(GL_LIST_BIT);
  glListBase(font_base);
  glCallLists(strlen(textCString), GL_UNSIGNED_BYTE, (GLubyte *)textCString);
  glPopAttrib();
}

void G4OpenGLSceneHandler::AddPrimitive (const G4Circle& circle) {
  glEnable (GL_POINT_SMOOTH);
  AddCircleSquare (circle, 24);
}

void G4OpenGLSceneHandler::AddPrimitive (const G4Square& square) {
  glDisable (GL_POINT_SMOOTH);
  AddCircleSquare (square, 4);
}

void G4OpenGLSceneHandler::AddCircleSquare
(const G4VMarker& marker,
 G4int nSides) {

  // Note: colour treated in sub-class.

  if (fpViewer -> GetViewParameters ().IsMarkerNotHidden ())
    glDisable (GL_DEPTH_TEST);
  else {glEnable (GL_DEPTH_TEST); glDepthFunc (GL_LESS);}
  
  glDisable (GL_LIGHTING);
  
  // Get vis attributes - pick up defaults if none.
  const G4VisAttributes* pVA =
    fpViewer -> GetApplicableVisAttributes (marker.GetVisAttributes ());

  G4double lineWidth = GetLineWidth(pVA);
  glLineWidth(lineWidth);

  G4VMarker::FillStyle style = marker.GetFillStyle();
  
  switch (style) {
  case G4VMarker::noFill: 
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    break;
    
  case G4VMarker::hashed:
    /*
    G4cout << "Hashed fill style in G4OpenGLSceneHandler."
	   << "\n  Not implemented.  Using G4VMarker::filled."
	   << G4endl;
    */
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glPolygonStipple (fStippleMaskHashed);
    // See also:
    //   if (style == G4VMarker::filled || style == G4VMarker::hashed)...
    // (twice) below.
    break;
    
  case G4VMarker::filled:
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    break;
    
  default:
    G4cout << "Unrecognised fill style in G4OpenGLSceneHandler."
	   << "\n  Using G4VMarker::filled."
	   << G4endl;
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    break;
    
  }

  // A few useful quantities...
  G4Point3D centre = marker.GetPosition();
  const G4Vector3D& viewpointDirection =
    fpViewer -> GetViewParameters().GetViewpointDirection();
  const G4Vector3D& up = fpViewer->GetViewParameters().GetUpVector();
  MarkerSizeType sizeType;
  G4double size = GetMarkerSize(marker, sizeType);

  // Find "size" of marker in world space (but see note below)...
  G4double worldSize;
  if (sizeType == world) {  // Size specified in world coordinates.
    worldSize = size;
  }
  else { // Size specified in screen (window) coordinates.

    // Find window coordinates of centre...
    GLdouble modelMatrix[16];
    glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix);
    G4double projectionMatrix[16];
    glGetDoublev (GL_PROJECTION_MATRIX, projectionMatrix);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    GLdouble winx, winy, winz;
    gluProject(centre.x(), centre.y(), centre.z(),
	       modelMatrix, projectionMatrix, viewport,
	       &winx, &winy, &winz);

    // Determine ratio window:world...
    const G4Vector3D inScreen = (up.cross(viewpointDirection)).unit();
    const G4Vector3D p = centre + inScreen;
    GLdouble winDx, winDy, winDz;
    gluProject(p.x(), p.y(), p.z(),
               modelMatrix, projectionMatrix, viewport,
               &winDx, &winDy, &winDz);
    G4double winWorldRatio = std::sqrt(std::pow(winx - winDx, 2) +
				  std::pow(winy - winDy, 2));
    worldSize = size / winWorldRatio;
  }

  // Draw...
  DrawXYPolygon (worldSize, centre, nSides);
}

/***************************************************
Note: We have to do it this way round so that when a global
transformation is applied, such as with /vis/viewer/set/viewpoint,
the markers follow the world coordinates without having to
recreate the display lists.  The down side is that the markers
rotate.  The only way to avoid this is to play with the modelview
and projection matrices of OpenGL - which I need to think about.
For future reference, here is the code to draw in window
coordinates; its down side is that markers do not follow global
transformations.  Some clever stuff is needed.

  ...
  // Find window coordinates of centre...
  GLdouble modelMatrix[16];
  glGetDoublev (GL_MODELVIEW_MATRIX, modelMatrix);
  G4double projectionMatrix[16];
  glGetDoublev (GL_PROJECTION_MATRIX, projectionMatrix);
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  GLdouble winx, winy, winz;
  gluProject(centre.x(), centre.y(), centre.z(),
             modelMatrix, projectionMatrix, viewport,
             &winx, &winy, &winz);

  // Find window size...
  G4double winSize;
  if (size) {  // Size specified in world coordinates.
    // Determine size in window coordinates...
    (Note: improve this by using an inScreen vector as above.)
    GLdouble winx1, winy1, winz1;
    gluProject(centre.x() + size, centre.y() + size, centre.z() + size,
               modelMatrix, projectionMatrix, viewport,
               &winx1, &winy1, &winz1);
    winSize = std::sqrt((std::pow(winx - winx1, 2) +
                    std::pow(winy - winy1, 2) +
                    std::pow(winz - winz1, 2)) / 3.);
  }
  else {
    winSize = scale *
      userSpecified ? marker.GetScreenSize() : def.GetScreenSize();
  }

  // Prepare to draw in window coordinates...
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(GLdouble(viewport[0]),
             GLdouble(viewport[0] + viewport[2]),
             GLdouble(viewport[1]),
             GLdouble(viewport[1] + viewport[3]));
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Draw in window coordinates...
  DrawScreenPolygon (winSize, G4Point3D(winx, winy, winz), nSides);

  // Re-instate matrices...
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix();
  ...
}

void G4OpenGLSceneHandler::DrawScreenPolygon
(G4double size,
 const G4Point3D& centre,
 G4int nSides) {
  glBegin (GL_POLYGON);
  const G4double dPhi = twopi / nSides;
  const G4double r = size / 2.;
  G4double phi;
  G4int i;
  for (i = 0, phi = -dPhi / 2.; i < nSides; i++, phi += dPhi) {
    G4double x, y, z;
    x = centre.x() + r * std::cos(phi);
    y = centre.y() + r * std::sin(phi);
    z = centre.z();
    glVertex3d (x, y, z);
  }
  glEnd ();
}
**********************************************/

void G4OpenGLSceneHandler::DrawXYPolygon
(G4double size,
 const G4Point3D& centre,
 G4int nSides) {
  const G4Vector3D& viewpointDirection =
    fpViewer -> GetViewParameters().GetViewpointDirection();
  const G4Vector3D& up = fpViewer->GetViewParameters().GetUpVector();
  const G4double dPhi = twopi / nSides;
  const G4double radius = size / 2.;
  G4Vector3D start = radius * (up.cross(viewpointDirection)).unit();
  G4double phi;
  G4int i;
  glBegin (GL_POLYGON);
  for (i = 0, phi = 0.; i < nSides; i++, phi += dPhi) {
    G4Vector3D r = start; r.rotate(phi, viewpointDirection);
    G4Vector3D p = centre + r;
    glVertex3d (p.x(), p.y(), p.z());
  }
  glEnd ();
}

//Method for handling G4Polyhedron objects for drawing solids.
void G4OpenGLSceneHandler::AddPrimitive (const G4Polyhedron& polyhedron) {

  // Assume all facets are planar convex quadrilaterals.
  // Draw each facet individually
  
  if (polyhedron.GetNoFacets() == 0) return;

  // Get vis attributes - pick up defaults if none.
  const G4VisAttributes* pVA =
    fpViewer -> GetApplicableVisAttributes (polyhedron.GetVisAttributes ());

  // Get view parameters that the user can force through the vis
  // attributes, thereby over-riding the current view parameter.
  G4ViewParameters::DrawingStyle drawing_style = GetDrawingStyle (pVA);

  //Get colour, etc...
  G4bool transparency_enabled = true;
  G4OpenGLViewer* pViewer = dynamic_cast<G4OpenGLViewer*>(fpViewer);
  if (pViewer) transparency_enabled = pViewer->transparency_enabled;
  const G4Colour& c = pVA->GetColour();
  GLfloat materialColour [4];
  materialColour [0] = c.GetRed ();
  materialColour [1] = c.GetGreen ();
  materialColour [2] = c.GetBlue ();
  if (transparency_enabled) {
    materialColour [3] = c.GetAlpha ();
  } else {
    materialColour [3] = 1.;
  }

  G4double lineWidth = GetLineWidth(pVA);
  glLineWidth(lineWidth);

  GLfloat clear_colour[4];
  glGetFloatv (GL_COLOR_CLEAR_VALUE, clear_colour);

  G4bool isAuxEdgeVisible = GetAuxEdgeVisible (pVA);

  G4bool clipping = pViewer->fVP.IsSection() || pViewer->fVP.IsCutaway();

  switch (drawing_style) {
  case (G4ViewParameters::hlhsr):
    // Set up as for hidden line removal but paint polygon faces later...
  case (G4ViewParameters::hlr):
    glEnable (GL_STENCIL_TEST);
    // The stencil buffer is cleared in G4OpenGLViewer::ClearView.
    // The procedure below leaves it clear.
    glStencilFunc (GL_ALWAYS, 0, 1);
    glStencilOp (GL_INVERT, GL_INVERT, GL_INVERT);
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
    if (materialColour[3] < 1.) {
      // Transparent...
      glDisable (GL_CULL_FACE);
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    } else {
      // Opaque...
      if (clipping) {
	glDisable (GL_CULL_FACE);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      } else {
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glPolygonMode (GL_FRONT, GL_LINE);
      }
    }
    glDisable (GL_LIGHTING);
    glColor3d (c.GetRed (), c.GetGreen (), c.GetBlue ());
    break;
  case (G4ViewParameters::hsr):
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);    
    if (materialColour[3] < 1.) {
      // Transparent...
      glDepthMask (0);  // Make depth buffer read-only.
      glDisable (GL_CULL_FACE);
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColour);
    } else {
      // Opaque...
      glDepthMask (1);  // Make depth buffer writable (default).
      if (clipping) {
	glDisable (GL_CULL_FACE);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      } else {
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glPolygonMode (GL_FRONT, GL_FILL);
      }
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColour);
    }
    glEnable (GL_LIGHTING);
    break;
  case (G4ViewParameters::wireframe):
  default:
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);    //??? was GL_ALWAYS
    glDisable (GL_CULL_FACE);
    glDisable (GL_LIGHTING);
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    glColor3d (c.GetRed (), c.GetGreen (), c.GetBlue ());
    break;
  }

  //Loop through all the facets...
  glBegin (GL_QUADS);
  G4bool notLastFace;
  do {

    //First, find vertices, edgeflags and normals and note "not last facet"...
    G4Point3D vertex[4];
    G4int edgeFlag[4];
    G4Normal3D normals[4];
    G4int n;
    notLastFace = polyhedron.GetNextFacet(n, vertex, edgeFlag, normals);

    //Loop through the four edges of each G4Facet...
    G4int edgeCount = 0;
    for(edgeCount = 0; edgeCount < n; ++edgeCount) {
      // Check to see if edge is visible or not...
      if (isAuxEdgeVisible) {
	edgeFlag[edgeCount] = 1;
      }
      if (edgeFlag[edgeCount] > 0) {
	glEdgeFlag (GL_TRUE);
      } else {
	glEdgeFlag (GL_FALSE);
      }
      glNormal3d (normals[edgeCount].x(), 
		  normals[edgeCount].y(),
		  normals[edgeCount].z());
      glVertex3d (vertex[edgeCount].x(), 
		  vertex[edgeCount].y(),
		  vertex[edgeCount].z());
    }
    // HepPolyhedron produces triangles too; in that case add an extra
    // vertex identical to first...
    if (n == 3) {
      edgeCount = 3;
      normals[edgeCount] = normals[0];
      vertex[edgeCount] = vertex[0];
      edgeFlag[edgeCount] = -1;
      glEdgeFlag (GL_FALSE);
      glNormal3d (normals[edgeCount].x(),
		  normals[edgeCount].y(), 
		  normals[edgeCount].z());
      glVertex3d (vertex[edgeCount].x(),
		  vertex[edgeCount].y(), 
		  vertex[edgeCount].z());
    }
    // Trap situation where number of edges is > 4...
    if (n > 4) {
      G4cerr <<
	"G4OpenGLSceneHandler::AddPrimitive(G4Polyhedron): WARNING";
      G4PhysicalVolumeModel* pPVModel =
	dynamic_cast<G4PhysicalVolumeModel*>(fpModel);
      if (pPVModel) {
	G4VPhysicalVolume* pCurrentPV = pPVModel->GetCurrentPV();
	G4LogicalVolume* pCurrentLV = pPVModel->GetCurrentLV();
	G4cerr <<
	"\n  Volume " << pCurrentPV->GetName() <<
	", Solid " << pCurrentLV->GetSolid()->GetName() <<
	  " (" << pCurrentLV->GetSolid()->GetEntityType();
      }
      G4cerr<<
	"\n   G4Polyhedron facet with " << n << " edges" << G4endl;
    }

    // Do it all over again (twice) for hlr...
    if  (drawing_style == G4ViewParameters::hlr ||
	 drawing_style == G4ViewParameters::hlhsr) {

      glEnd ();  // Placed here to balance glBegin above, allowing GL
		 // state changes below, then glBegin again.  Avoids
		 // having glBegin/End pairs *inside* loop in the more
		 // usual case of no hidden line removal.

      // Draw through stencil...
      glStencilFunc (GL_EQUAL, 0, 1);
      glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
      if (drawing_style == G4ViewParameters::hlhsr) {
	glEnable (GL_LIGHTING);
      }
      glEnable (GL_DEPTH_TEST);
      glDepthFunc (GL_LEQUAL);    
      if (materialColour[3] < 1.) {
	// Transparent...
	glDepthMask (0);  // Make depth buffer read-only.
	glDisable (GL_CULL_FACE);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      } else {
	// Opaque...
	glDepthMask (1);  // Make depth buffer writable (default).
	if (clipping) {
	  glDisable (GL_CULL_FACE);
	  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	} else {
	  glEnable (GL_CULL_FACE);
	  glCullFace (GL_BACK);
	  glPolygonMode (GL_FRONT, GL_FILL);
	}
      }
      GLfloat* painting_colour;
      if  (drawing_style == G4ViewParameters::hlr) {
	if (materialColour[3] < 1.) {
	  // Transparent - don't paint...
	  goto end_of_drawing_through_stencil;
	}
	painting_colour = clear_colour;
      } else {  // drawing_style == G4ViewParameters::hlhsr
	painting_colour = materialColour;
      }
      if (materialColour[3] < 1.) {
	// Transparent...
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, painting_colour);
      } else {
	// Opaque...
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, painting_colour);
      }
      glColor4fv (painting_colour);
      glBegin (GL_QUADS);
      for (int edgeCount = 0; edgeCount < 4; ++edgeCount) {
	if (edgeFlag[edgeCount] > 0) {
	  glEdgeFlag (GL_TRUE);
	} else {
	  glEdgeFlag (GL_FALSE);
	}
	glNormal3d (normals[edgeCount].x(), 
		    normals[edgeCount].y(),
		    normals[edgeCount].z());
	glVertex3d (vertex[edgeCount].x(), 
		    vertex[edgeCount].y(),
		    vertex[edgeCount].z());
      }
      glEnd ();
    end_of_drawing_through_stencil:

      // and once more to reset the stencil bits...
      glStencilFunc (GL_ALWAYS, 0, 1);
      glStencilOp (GL_INVERT, GL_INVERT, GL_INVERT);
      glDepthFunc (GL_LEQUAL);  // to make sure line gets drawn.  
      if (materialColour[3] < 1.) {
	// Transparent...
	glDisable (GL_CULL_FACE);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      } else {
	// Opaque...
	if (clipping) {
	  glDisable (GL_CULL_FACE);
	  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	} else {
	  glEnable (GL_CULL_FACE);
	  glCullFace (GL_BACK);
	  glPolygonMode (GL_FRONT, GL_LINE);
	}
      }
      glDisable (GL_LIGHTING);
      glColor3d (c.GetRed (), c.GetGreen (), c.GetBlue ());
      glBegin (GL_QUADS);
      for (int edgeCount = 0; edgeCount < 4; ++edgeCount) {
	if (edgeFlag[edgeCount] > 0) {
	  glEdgeFlag (GL_TRUE);
	} else {
	  glEdgeFlag (GL_FALSE);
	}
	glNormal3d (normals[edgeCount].x(), 
		    normals[edgeCount].y(),
		    normals[edgeCount].z());
	glVertex3d (vertex[edgeCount].x(), 
		    vertex[edgeCount].y(),
		    vertex[edgeCount].z());
      }
      glEnd ();
      glDepthFunc (GL_LEQUAL);   // Revert for next facet.
      glBegin (GL_QUADS);      // Ready for next facet.  GL
			       // says it ignores incomplete
			       // quadrilaterals, so final empty
			       // glBegin/End sequence should be OK.
    }
  } while (notLastFace);  
  
  glEnd ();
  glDisable (GL_STENCIL_TEST);  // Revert to default for next primitive.
  glDepthMask (1);              // Revert to default for next primitive.
}

//Method for handling G4NURBS objects for drawing solids.
//Knots and Ctrl Pnts MUST be arrays of GLfloats.
void G4OpenGLSceneHandler::AddPrimitive (const G4NURBS& nurb) {

  GLUnurbsObj *gl_nurb;
  gl_nurb = gluNewNurbsRenderer ();

  GLfloat *u_knot_array, *u_knot_array_ptr;
  u_knot_array = u_knot_array_ptr = new GLfloat [nurb.GetnbrKnots(G4NURBS::U)];
  G4NURBS::KnotsIterator u_iterator (nurb, G4NURBS::U);
  while (u_iterator.pick (u_knot_array_ptr++));

  GLfloat *v_knot_array, *v_knot_array_ptr;
  v_knot_array = v_knot_array_ptr = new GLfloat [nurb.GetnbrKnots(G4NURBS::V)];
  G4NURBS::KnotsIterator v_iterator (nurb, G4NURBS::V);
  while (v_iterator.pick (v_knot_array_ptr++));

  GLfloat *ctrl_pnt_array, *ctrl_pnt_array_ptr;
  ctrl_pnt_array = ctrl_pnt_array_ptr =
    new GLfloat [nurb.GettotalnbrCtrlPts () * G4NURBS::NofC];
  G4NURBS::CtrlPtsCoordsIterator c_p_iterator (nurb);
  while (c_p_iterator.pick (ctrl_pnt_array_ptr++));

  // Get vis attributes - pick up defaults if none.
  const G4VisAttributes* pVA =
    fpViewer -> GetApplicableVisAttributes (nurb.GetVisAttributes ());

  // Get view parameters that the user can force through the vis
  // attributes, thereby over-riding the current view parameter.
  G4ViewParameters::DrawingStyle drawing_style = GetDrawingStyle (pVA);
  //G4bool isAuxEdgeVisible = GetAuxEdgeVisible (pVA);
  
  //Get colour, etc..
  const G4Colour& c = pVA -> GetColour ();

  switch (drawing_style) {

  case (G4ViewParameters::hlhsr):
    //    G4cout << "Hidden line removal not implememented in G4OpenGL.\n"
    // << "Using hidden surface removal." << G4endl;
  case (G4ViewParameters::hsr):
    {
      glEnable (GL_LIGHTING);
      glEnable (GL_DEPTH_TEST);
      glEnable (GL_AUTO_NORMAL);
      glEnable (GL_NORMALIZE);
      gluNurbsProperty (gl_nurb, GLU_DISPLAY_MODE, GLU_FILL);
      gluNurbsProperty (gl_nurb, GLU_SAMPLING_TOLERANCE, 50.0);
      GLfloat materialColour [4];
      materialColour [0] = c.GetRed ();
      materialColour [1] = c.GetGreen ();
      materialColour [2] = c.GetBlue ();
      materialColour [3] = 1.0;  // = c.GetAlpha () for transparency -
				 // but see complication in
				 // AddPrimitive(const G4Polyhedron&).
      glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColour);
      break;
    }
  case (G4ViewParameters::hlr):
    //    G4cout << "Hidden line removal not implememented in G4OpenGL.\n"
    // << "Using wireframe." << G4endl;
  case (G4ViewParameters::wireframe):
  default:
    glDisable (GL_LIGHTING);
//    glDisable (GL_DEPTH_TEST);
    glEnable (GL_DEPTH_TEST);
    glDisable (GL_AUTO_NORMAL);
    glDisable (GL_NORMALIZE);
    gluNurbsProperty (gl_nurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
    gluNurbsProperty (gl_nurb, GLU_SAMPLING_TOLERANCE, 50.0);
    glColor3d (c.GetRed (), c.GetGreen (), c.GetBlue ());
    break;
  }	

  gluBeginSurface (gl_nurb);
  G4int u_stride = 4;
  G4int v_stride = nurb.GetnbrCtrlPts(G4NURBS::U) * 4;

  gluNurbsSurface (gl_nurb, 
		   nurb.GetnbrKnots (G4NURBS::U), (GLfloat*)u_knot_array, 
		   nurb.GetnbrKnots (G4NURBS::V), (GLfloat*)v_knot_array,
		   u_stride,
		   v_stride,  
		   ctrl_pnt_array,
		   nurb.GetUorder (),
		   nurb.GetVorder (), 
		   GL_MAP2_VERTEX_4);
  
  gluEndSurface (gl_nurb);

  delete [] u_knot_array;  // These should be allocated with smart allocators
  delete [] v_knot_array;  // to avoid memory explosion.
  delete [] ctrl_pnt_array;

  gluDeleteNurbsRenderer (gl_nurb);
}

void G4OpenGLSceneHandler::AddCompound(const G4VTrajectory& traj) {
  G4VSceneHandler::AddCompound(traj);  // For now.
}

void G4OpenGLSceneHandler::AddCompound(const G4VHit& hit) {
  G4VSceneHandler::AddCompound(hit);  // For now.
}

#endif

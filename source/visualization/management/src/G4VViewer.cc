// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4VViewer.cc,v 1.2 1999-01-10 13:25:45 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  27th March 1996
// Abstract interface class for graphics views.

#include "G4VViewer.hh"

#include "G4ios.hh"
#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "G4VisManager.hh"
#include "G4VGraphicsSystem.hh"
#include "G4VSceneHandler.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4Event.hh"

G4VViewer::G4VViewer (G4VSceneHandler& scene, G4int id, const G4String& name):
fScene (scene),
fViewId (id),
fModified (true),
fNeedKernelVisit (true)
{
  G4VisManager* pVMan = G4VisManager::GetInstance ();
  fVP = pVMan -> GetCurrentViewParameters ();
  if (name == "") {
    char charname [50];
    ostrstream ost (charname, 50);
    ost << fScene.GetName () << '-' << fViewId << ends;
    fName = charname;
  }
  else {
    fName = name;
  }
}

G4VViewer::~G4VViewer () {}

const G4VisAttributes* G4VViewer::GetApplicableVisAttributes
(const G4VisAttributes* pVisAttribs) const {
  // If the pointer is null, pick up the default vis attributes from
  // the view parameters.
  if (!pVisAttribs)
    pVisAttribs = GetViewParameters ().GetDefaultVisAttributes ();
  return pVisAttribs;
}

void G4VViewer::NeedKernelVisit () {
  // Notify all views that a kernel visit is required.
  const G4ViewerList& viewList = fScene.GetViewList ();
  for (int i = 0; i < viewList.entries (); i++) {
    viewList [i] -> SetNeedKernelVisit ();
  }
}

void G4VViewer::ShowView () {}

void G4VViewer::ProcessView () {

  // If view parameters have been modified, SetView () works out consequences. 
  if (fModified) {
    fModified = false;
    SetView ();
  }

  // If the scene data has changed (fNeedVisit is set to true in
  // G4VSceneHandler::SetSceneData), or if the concrete view has decided that it
  // necessary to visit the kernel (this should be Done in the concrete
  // object's DrawView ())...
  if (fNeedKernelVisit) {
    fNeedKernelVisit = false;
    fScene.ProcessScene (*this);
  }
}

void G4VViewer::SetViewParameters (const G4ViewParameters& vp) {
  fVP = vp;
  fModified = true;
}

ostream& operator << (ostream& os, const G4VViewer& v) {
  os << "View " << v.fName << ":\n";
  os << v.fVP;
  return os;
}

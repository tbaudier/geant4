///////////////////
//G4RayTrajectory.cc
///////////////////


#include "G4RayTrajectory.hh"
#include "G4RayTrajectoryPoint.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4TransportationManager.hh"
#include "G4ios.hh"

G4Allocator<G4RayTrajectory> G4RayTrajectoryAllocator;


G4RayTrajectory :: G4RayTrajectory()
{
  positionRecord = new G4RWTPtrOrderedVector<G4RayTrajectoryPoint>;
}

G4RayTrajectory :: G4RayTrajectory(G4RayTrajectory & right)
{
  positionRecord = new G4RWTPtrOrderedVector<G4RayTrajectoryPoint>;
  for(int i=0;i<right.positionRecord->entries();i++)
  {
    G4RayTrajectoryPoint* rightPoint = (G4RayTrajectoryPoint*)
				((*(right.positionRecord))[i]);
    positionRecord->insert(new G4RayTrajectoryPoint(*rightPoint));
  }
}

G4RayTrajectory :: ~G4RayTrajectory()
{
  positionRecord->clearAndDestroy();
  delete positionRecord;
}

void G4RayTrajectory::AppendStep(const G4Step* aStep)
{
  G4RayTrajectoryPoint* trajectoryPoint = new G4RayTrajectoryPoint();

  trajectoryPoint->SetStepLength(aStep->GetStepLength());

  G4Navigator* theNavigator 
    = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4bool valid;
  G4ThreeVector theLocalNormal = theNavigator->GetLocalExitNormal(&valid);
  if(valid) { theLocalNormal = -theLocalNormal; }
  G4ThreeVector theGrobalNormal 
    = theNavigator->GetLocalToGlobalTransform().TransformAxis(theLocalNormal);
  trajectoryPoint->SetSurfaceNormal(theGrobalNormal);

  G4VPhysicalVolume* prePhys = aStep->GetPreStepPoint()->GetPhysicalVolume();
  const G4VisAttributes* preVisAtt = prePhys->GetLogicalVolume()->GetVisAttributes();
  trajectoryPoint->SetPreStepAtt(preVisAtt);

  const G4VPhysicalVolume* postPhys = aStep->GetPostStepPoint()->GetPhysicalVolume();
  const G4VisAttributes* postVisAtt = NULL;
  if(postPhys) { postVisAtt = postPhys->GetLogicalVolume()->GetVisAttributes(); }
  trajectoryPoint->SetPostStepAtt(postVisAtt);

  positionRecord->append(trajectoryPoint);
}

void G4RayTrajectory::ShowTrajectory() const
{ }

void G4RayTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  G4RayTrajectory* seco = (G4RayTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(G4int i=0;i<ent;i++)
  { positionRecord->append(seco->positionRecord->removeAt(0)); }
}


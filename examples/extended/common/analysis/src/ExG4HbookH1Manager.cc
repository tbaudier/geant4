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
// $Id$
//
/// \file common/analysis/src/ExG4HbookH1Manager.cc
/// \brief Implementation of the ExG4HbookH1Manager class

// Author: Ivana Hrivnacova, 15/06/2011  (ivana@ipno.in2p3.fr)

#ifdef G4_USE_HBOOK

#include "ExG4HbookH1Manager.hh"
#include "ExG4HbookFileManager.hh"
#include "G4HnManager.hh"
#include "G4AnalysisManagerState.hh"

#include <fstream>

//_____________________________________________________________________________
ExG4HbookH1Manager::ExG4HbookH1Manager(const G4AnalysisManagerState& state)
 : G4VH1Manager(state),
   fFileManager(0),
   fH1HbookIdOffset(-1),
   fH1Vector(),
   fH1BookingVector(),
   fH1NameIdMap()
{
}

//_____________________________________________________________________________
ExG4HbookH1Manager::~ExG4HbookH1Manager()
{  
  // Delete h1
  Reset();

  // Delete h1 booking 
  std::vector<h1_booking*>::iterator it;
  for ( it = fH1BookingVector.begin(); it != fH1BookingVector.end(); it++ ) {
    delete *it;
  }  
}

// 
// private methods
//

//_____________________________________________________________________________
void ExG4HbookH1Manager::SetH1HbookIdOffset()
{
// Set  fH1HbookIdOffset if needed

  if ( fH1HbookIdOffset == -1 ) {
    if ( fFirstId > 0 ) 
      fH1HbookIdOffset = 0;
    else
      fH1HbookIdOffset = 1;
        
    if ( fH1HbookIdOffset > 0 ) {
      G4ExceptionDescription description;
      description << "H1 will be defined in HBOOK with ID = G4_firstHistoId + 1";
      G4Exception("ExG4HbookH1Manager::SetH1HbookIdOffset()",
                  "Analysis_W011", JustWarning, description);
    }              
  }
}  

//_____________________________________________________________________________
void ExG4HbookH1Manager::CreateH1FromBooking()
{
// Create h1 from h1_booking.

  // Do nothing if any h1 histogram already exists
  // or no h1 histograms are booked
  if ( fH1Vector.size() || ( fH1BookingVector.size() == 0 ) ) return;       

  // Go to histograms directory if defined
  if ( fFileManager->GetHistoDirectoryName() != "" ) {
    G4String histoPath = "//PAWC/LUN1/";
    histoPath.append(fFileManager->GetHistoDirectoryName().data());
    tools::hbook::CHCDIR(histoPath.data()," ");
  }  

  // Create histograms
  G4int index = 0;
  std::vector<h1_booking*>::const_iterator it;
  for ( it = fH1BookingVector.begin(); it != fH1BookingVector.end(); ++it) {
    // Get information
    G4int id = index + fFirstId;    
    G4HnInformation* info = fHnManager->GetHnInformation(id, "CreateH1FromBooking");
    // Hbook index
    G4int hbookIndex = fH1HbookIdOffset + index + fFirstId;
    ++index;

#ifdef G4VERBOSE
    if ( fState.GetVerboseL4() ) 
      fState.GetVerboseL4()->Message("create from booking", "h1", info->fName);
#endif

    // Create h1
    tools::hbook::h1* h1 
      = new tools::hbook::h1(hbookIndex, (*it)->fTitle, 
                             (*it)->fNbins, (*it)->fXmin, (*it)->fXmax);
    fH1Vector.push_back(h1);

#ifdef G4VERBOSE
    if ( fState.GetVerboseL3() ) { 
      G4ExceptionDescription description;
      description << " name : " << info->fName << " hbook index : " << hbookIndex; 
      fState.GetVerboseL3()->Message("create from booking", "h1", description);
    }  
#endif
  } 
  
  if ( fFileManager->GetHistoDirectoryName() != "" ) {
    // Return to //PAWC/LUN1 :
    tools::hbook::CHCDIR("//PAWC/LUN1"," ");
  }  
}   

//_____________________________________________________________________________
void ExG4HbookH1Manager::Reset()
{
// Reset histograms and ntuple  

  // Delete histograms
  std::vector<tools::hbook::h1*>::iterator it;
  for (it = fH1Vector.begin(); it != fH1Vector.end(); it++ ) {
    delete *it;
  }  

  // Clear vectors
  fH1Vector.clear();
}  
 
//_____________________________________________________________________________
h1_booking*  ExG4HbookH1Manager::GetH1Booking(G4int id, G4bool warn) const 
{
  G4int index = id - fFirstId;
  if ( index < 0 || index >= G4int(fH1BookingVector.size()) ) {
    if ( warn) {
      G4ExceptionDescription description;
      description << "      " << "histo " << id << " does not exist.";
      G4Exception("G4HbookAnalysisManager::GetH1Booking()",
                  "Analysis_W007", JustWarning, description);
    }
    return 0;         
  }

  return fH1BookingVector[index];
}

//
// protected methods
//

//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::WriteOnAscii(std::ofstream& output)
{
// Write selected objects on ASCII file
// (Only H1 implemented by now)
// According to the implementation by Michel Maire, originally in
// extended examples.

  // h1 histograms
  for ( G4int i=0; i<G4int(fH1Vector.size()); ++i ) {
    G4int id = i + fFirstId;
    G4HnInformation* info 
      = fHnManager->GetHnInformation(id, "WriteOnAscii"); 
    // skip writing if activation is enabled and H1 is inactivated
    if ( ! info->fAscii ) continue; 
    tools::hbook::h1* h1 = fH1Vector[i];

#ifdef G4VERBOSE
    if ( fState.GetVerboseL3() ) 
      fState.GetVerboseL3()->Message("write on ascii", "h1", info->fName);
#endif
  
    output << "\n  1D histogram " << id << ": " << h1->title() 
           << "\n \n \t     X \t\t     Y" << G4endl;
    
    for (G4int j=0; j< G4int(h1->axis().bins()); ++j) {
       output << "  " << j << "\t" 
              << h1->axis().bin_center(j) << "\t"
              << h1->bin_height(j) << G4endl;
    } 
  }
  
  return true;
}  

//_____________________________________________________________________________
tools::hbook::h1*  ExG4HbookH1Manager::GetH1InFunction(G4int id, 
                                      G4String functionName, G4bool warn,
                                      G4bool onlyIfActive) const
{
  G4int index = id - fFirstId;
  if ( index < 0 || index >= G4int(fH1Vector.size()) ) {
    if ( warn) {
      G4String inFunction = "ExG4HbookH1Manager::";
      inFunction += functionName;
      G4ExceptionDescription description;
      description << "      " << "histogram " << id << " does not exist.";
      G4Exception(inFunction, "Analysis_W007", JustWarning, description);
    }
    return 0;         
  }
  
  // Do not return histogram if inactive 
  if ( fState.GetIsActivation() && onlyIfActive && ( ! fHnManager->GetActivation(id) ) ) {
    return 0; 
  }  
  
  return fH1Vector[index];
}  
                                      
// 
// public methods
//

//_____________________________________________________________________________
G4int ExG4HbookH1Manager::CreateH1(const G4String& name, const G4String& title,
                               G4int nbins, G4double xmin, G4double xmax,
                               const G4String& unitName, const G4String& fcnName)
{
#ifdef G4VERBOSE
  if ( fState.GetVerboseL4() ) 
    fState.GetVerboseL4()->Message("create", "H1", name);
#endif

  // Create h1 booking & information
  G4int index = fH1BookingVector.size();
  G4double unit = GetUnitValue(unitName);
  G4Fcn fcn = GetFunction(fcnName);
  G4String newTitle(title);
  UpdateTitle(newTitle, unitName, fcnName);  
  h1_booking* h1Booking = new h1_booking(nbins, fcn(xmin), fcn(xmax)); 
           // h1_booking object is deleted in destructor
  h1Booking->fTitle = newTitle;
  fH1BookingVector.push_back(h1Booking);
  fHnManager->AddH1Information(name, unitName, fcnName, unit, fcn);
  
  // Set  fH1HbookIdOffset if needed
  SetH1HbookIdOffset();
  
  // Hbook index
  G4int hbookIndex = fH1HbookIdOffset + index + fFirstId;

  // Create h1 if the file is open
  if ( fFileManager->IsFile() ) {
    // Go to histograms directory
    G4String histoPath = "//PAWC/LUN1/";
    if ( fFileManager->GetHistoDirectoryName() != "" ) {
      histoPath.append(fFileManager->GetHistoDirectoryName().data());
      tools::hbook::CHCDIR(histoPath.data()," ");
    }  
    tools::hbook::CHCDIR(histoPath.data()," ");
    
    // Create histogram    
    tools::hbook::h1* h1 
      = new tools::hbook::h1(hbookIndex, newTitle, nbins, fcn(xmin), fcn(xmax));
            // h1 objects are deleted when closing a file.
    fH1Vector.push_back(h1);
 
    if ( fFileManager->GetHistoDirectoryName() != "" ) {
      // Return to //PAWC/LUN1 :
      tools::hbook::CHCDIR("//PAWC/LUN1"," ");
    }
  }
  
  fLockFirstId = true;

#ifdef G4VERBOSE
    if ( fState.GetVerboseL2() ) { 
      G4ExceptionDescription description;
      description << " name : " << name << " hbook index : " << hbookIndex; 
      fState.GetVerboseL2()->Message("create", "H1", description);
    }  
#endif

  fH1NameIdMap[name] = index + fFirstId;
  return index + fFirstId;
}                                         

//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::SetH1(G4int id,
                                   G4int nbins, G4double xmin, G4double xmax,
                                   const G4String& unitName, 
                                   const G4String& fcnName)
{                                
  h1_booking* h1Booking = GetH1Booking(id, false);
  if ( ! h1Booking ) {
    G4ExceptionDescription description;
    description << "      " << "histogram " << id << " does not exist.";
    G4Exception("G4HbookAnalysisManager::SetH1()",
                "Analysis_W007", JustWarning, description);
    return false;
  }

  G4HnInformation* info = fHnManager->GetHnInformation(id,"SetH1");
#ifdef G4VERBOSE
  if ( fState.GetVerboseL4() ) 
    fState.GetVerboseL4()->Message("configure", "H1", info->fName);
#endif

  // Keep new parameters in booking & information
  G4double unit = GetUnitValue(unitName);
  G4Fcn fcn = GetFunction(fcnName);
  h1Booking->fNbins = nbins;
  h1Booking->fXmin = fcn(xmin);
  h1Booking->fXmax = fcn(xmax);
  info->fXUnitName = unitName;
  info->fYUnitName = unitName;
  info->fXFcnName = fcnName;
  info->fYFcnName = fcnName;
  info->fXUnit = unit;
  info->fYUnit = unit;
  info->fXFcn = fcn;
  info->fYFcn = fcn;
  fHnManager->SetActivation(id, true); 

  G4String newTitle(h1Booking->fTitle);
  UpdateTitle(newTitle, unitName, fcnName);  
  h1Booking->fTitle = newTitle;  
  
  // Re-configure histogram if it was already defined
  if ( fH1Vector.size() ) {
    tools::hbook::h1* h1 = GetH1(id);
    h1->configure(nbins, fcn(xmin), fcn(xmax));
  }  
  
  return true;
}
  
//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::ScaleH1(G4int id, G4double factor)
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "ScaleH1", false, false);
  if ( ! h1 ) return false;

  return h1->scale(factor);
}  

//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::FillH1(G4int id, G4double value, G4double weight)
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "FillH1", true, false);
  if ( ! h1 ) return false;

  if ( fState.GetIsActivation() && ( ! fHnManager->GetActivation(id) ) ) {
    //G4cout << "Skipping FillH1 for " << id << G4endl; 
    return false; 
  }  

  G4HnInformation* info = fHnManager->GetHnInformation(id, "FillH1");
  h1->fill(info->fXFcn(value/info->fXUnit), weight);
#ifdef G4VERBOSE
  if ( fState.GetVerboseL4() ) {
    G4ExceptionDescription description;
    description << " id " << id << " value " << value;
    fState.GetVerboseL4()->Message("fill", "H1", description);
  }  
#endif
  return true;
}

//_____________________________________________________________________________
tools::hbook::h1*  ExG4HbookH1Manager::GetH1(G4int id, G4bool warn,
                                             G4bool onlyIfActive) const 
{
  return GetH1InFunction(id, "GetH1", warn, onlyIfActive);
}

//_____________________________________________________________________________
G4int  ExG4HbookH1Manager::GetH1Id(const G4String& name, G4bool warn) const
{
  std::map<G4String, G4int>::const_iterator it = fH1NameIdMap.find(name);
  if ( it ==  fH1NameIdMap.end() ) {  
    if ( warn) {
      G4String inFunction = "ExG4HbookH1Manager::GetH1Id";
      G4ExceptionDescription description;
      description << "      " << "histogram " << name << " does not exist.";
      G4Exception(inFunction, "Analysis_W007", JustWarning, description);
    }
    return -1;         
  }
  return it->second;
}  
                                      
//_____________________________________________________________________________
G4int ExG4HbookH1Manager::GetH1Nbins(G4int id) const
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "GetH1Nbins");
  if ( ! h1 ) return 0;
  
  return h1->axis().bins();
}  

//_____________________________________________________________________________
G4double ExG4HbookH1Manager::GetH1Xmin(G4int id) const
{
// Returns xmin value with applied unit and histogram function

  tools::hbook::h1* h1 = GetH1InFunction(id, "GetH1Xmin");
  if ( ! h1 ) return 0;
  
  G4HnInformation* info = fHnManager->GetHnInformation(id, "GetH1Xmin");
  return info->fXFcn(h1->axis().lower_edge()*info->fXUnit);
}  

//_____________________________________________________________________________
G4double ExG4HbookH1Manager::GetH1Xmax(G4int id) const
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "GetH1Xmax");
  if ( ! h1 ) return 0;
  
  G4HnInformation* info = fHnManager->GetHnInformation(id, "GetH1Xmax");
  return info->fXFcn(h1->axis().upper_edge()*info->fXUnit);
}  

//_____________________________________________________________________________
G4double ExG4HbookH1Manager::GetH1Width(G4int id) const
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "GetH1XWidth", true, false);
  if ( ! h1 ) return 0;
  
  G4int nbins = h1->axis().bins();
  if ( ! nbins ) {
    G4ExceptionDescription description;
    description << "    nbins = 0 (for h1 id = " << id << ").";
    G4Exception("ExG4HbookH1Manager::GetH1Width",
                "Analysis_W014", JustWarning, description);
    return 0;
  }              
  
  G4HnInformation* info = fHnManager->GetHnInformation(id, "GetH1XWidth");
  return ( info->fXFcn(h1->axis().upper_edge()) 
           - info->fXFcn(h1->axis().lower_edge()))*info->fXUnit/nbins;
}  

//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::SetH1Title(G4int id, const G4String& title)
{
  h1_booking* h1Booking = GetH1Booking(id, false);
  if ( ! h1Booking ) {
    G4ExceptionDescription description;
    description << "      " << "histogram " << id << " does not exist.";
    G4Exception("G4HbookAnalysisManager::SetH1Title()",
                "Analysis_W007", JustWarning, description);
    return false;
  }

  h1Booking->fTitle = title;
  return true;
}  

//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::SetH1XAxisTitle(G4int id, const G4String& title)
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "SetH1XAxisTitle");
  if ( ! h1 ) return false;
  
  h1->add_annotation(tools::hbook::key_axis_x_title(), title);
  return true;
}  

//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::SetH1YAxisTitle(G4int id, const G4String& title)
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "SetH1YAxisTitle");
  if ( ! h1 ) return false;
  
  h1->add_annotation(tools::hbook::key_axis_y_title(), title);
  return true;
}  

//_____________________________________________________________________________
G4String ExG4HbookH1Manager::GetH1Title(G4int id) const
{
  h1_booking* h1Booking = GetH1Booking(id, false);
  if ( ! h1Booking ) {
    G4ExceptionDescription description;
    description << "      " << "histogram " << id << " does not exist.";
    G4Exception("G4HbookAnalysisManager::GetH1Title()",
                "Analysis_W007", JustWarning, description);
    return "";
  }
  
  return h1Booking->fTitle;
}  


//_____________________________________________________________________________
G4String ExG4HbookH1Manager::GetH1XAxisTitle(G4int id) const 
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "GetH1XAxisTitle");
  if ( ! h1 ) return "";
  
  G4String title;
  G4bool result = h1->annotation(tools::hbook::key_axis_x_title(), title);
  if ( ! result ) {
    G4ExceptionDescription description;
    description << "    Failed to get x_axis title for h1 id = " << id << ").";
    G4Exception("ExG4HbookH1Manager::GetH1XAxisTitle",
                "Analysis_W014", JustWarning, description);
    return "";
  }
  
  return title;              
}  

//_____________________________________________________________________________
G4String ExG4HbookH1Manager::GetH1YAxisTitle(G4int id) const 
{
  tools::hbook::h1* h1 = GetH1InFunction(id, "GetH1YAxisTitle");
  if ( ! h1 ) return "";
  
  G4String title;
  G4bool result = h1->annotation(tools::hbook::key_axis_y_title(), title);
  if ( ! result ) {
    G4ExceptionDescription description;
    description << "    Failed to get y_axis title for h1 id = " << id << ").";
    G4Exception("ExG4HbookH1Manager::GetH1YAxisTitle",
                "Analysis_W014", JustWarning, description);
    return "";
  }
  
  return title;              
}  

//_____________________________________________________________________________
G4bool ExG4HbookH1Manager::SetH1HbookIdOffset(G4int offset) 
{
  if ( fH1Vector.size() ) {
    G4ExceptionDescription description;
    description 
      << "Cannot set H1HbookIdOffset as some H1 histogramms already exist.";
    G4Exception("G4HbookAnalysisManager::SetH1HbookIdOffset()",
                 "Analysis_W009", JustWarning, description);
    return false;             
  }
  
  if ( fFirstId + offset < 1 ) {
    G4ExceptionDescription description;
    description << "The first histogram HBOOK id must be >= 1.";
    G4Exception("G4HbookAnalysisManager::SetH1HbookIdOffset()",
                 "Analysis_W009", JustWarning, description);
    return false;             
  }
  
  fH1HbookIdOffset = offset;
  return true;
}  

#endif

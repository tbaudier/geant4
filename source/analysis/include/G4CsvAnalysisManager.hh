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

// Author: Ivana Hrivnacova, 15/06/2011  (ivana@ipno.in2p3.fr)

#ifndef G4CsvAnalysisManager_h
#define G4CsvAnalysisManager_h 1


#include "G4VAnalysisManager.hh"
#include "G4CsvNtupleDescription.hh"
#include "globals.hh"

#include "tools/wcsv_ntuple"

#include <fstream>
#include <vector>
#include <map>

class G4CsvAnalysisManager : public G4VAnalysisManager
{
  public:
    G4CsvAnalysisManager(G4bool isMaster = true);
    ~G4CsvAnalysisManager();
    
    // static methods
    static G4CsvAnalysisManager* Create(G4bool isMaster = true);
    static G4CsvAnalysisManager* Instance();

    // Methods to manipulate files
    using G4VAnalysisManager::OpenFile;
    virtual G4bool OpenFile(const G4String& fileName);
    virtual G4bool Write();
    virtual G4bool CloseFile(); 

    // Methods for handling histogrammes, ntuples
    virtual G4int CreateH1(const G4String& name, const G4String& title,
                           G4int nbins, G4double xmin, G4double xmax,
                           const G4String& unitName = "none",
                           const G4String& fcnName = "none");
    virtual G4int CreateH2(const G4String& name, const G4String& title,
                           G4int nxbins, G4double xmin, G4double xmax, 
                           G4int nybins, G4double ymin, G4double ymax,
                           const G4String& xunitName = "none", 
                           const G4String& yunitName = "none",
                           const G4String& xfcnName = "none", 
                           const G4String& yfcnName = "none");
                           
    virtual G4bool SetH1(G4int id,
                           G4int nbins, G4double xmin, G4double xmax,
                           const G4String& unitName = "none",
                           const G4String& fcnName = "none");
    virtual G4bool SetH2(G4int id,
                           G4int nxbins, G4double xmin, G4double xmax, 
                           G4int nybins, G4double ymin, G4double ymax,
                           const G4String& xunitName = "none", 
                           const G4String& yunitName = "none",
                           const G4String& xfcnName = "none", 
                           const G4String& yfcnName = "none");
 
    virtual G4bool ScaleH1(G4int id, G4double factor);
    virtual G4bool ScaleH2(G4int id, G4double factor);
                           
    virtual G4int CreateNtuple(const G4String& name, const G4String& title);
    // Create columns in the last created ntuple
    virtual G4int CreateNtupleIColumn(const G4String& name);
    virtual G4int CreateNtupleFColumn(const G4String& name);
    virtual G4int CreateNtupleDColumn(const G4String& name);   
    virtual void  FinishNtuple();   
    // Create columns in the ntuple with given id
    virtual G4int CreateNtupleIColumn(G4int ntupleId, const G4String& name);
    virtual G4int CreateNtupleFColumn(G4int ntupleId, const G4String& name);
    virtual G4int CreateNtupleDColumn(G4int ntupleId, const G4String& name);   
    virtual void  FinishNtuple(G4int ntupleId);   
    
    // Methods to fill histogrammes, ntuples
    virtual G4bool FillH1(G4int id, G4double value, G4double weight = 1.0);
    virtual G4bool FillH2(G4int id, G4double xvalue, G4double yvalue,
                          G4double weight = 1.0);
    // Methods for ntuple with id = FirstNtupleId                     
    virtual G4bool FillNtupleIColumn(G4int columnId, G4int value);
    virtual G4bool FillNtupleFColumn(G4int columnId, G4float value);
    virtual G4bool FillNtupleDColumn(G4int columnId, G4double value);
    virtual G4bool AddNtupleRow();
    // Methods for ntuple with id > FirstNtupleId (when more ntuples exist)                      
    virtual G4bool FillNtupleIColumn(G4int ntupleId, G4int columnId, G4int value);
    virtual G4bool FillNtupleFColumn(G4int ntupleId, G4int columnId, G4float value);
    virtual G4bool FillNtupleDColumn(G4int ntupleId, G4int columnId, G4double value);
    virtual G4bool AddNtupleRow(G4int ntupleId);
    
    // Access methods
    virtual tools::wcsv::ntuple* GetNtuple() const;
    virtual tools::wcsv::ntuple* GetNtuple(G4int ntupleId) const;

    // Access to H1 parameters
    virtual G4int    GetH1Nbins(G4int id) const;
    virtual G4double GetH1Xmin(G4int id) const;
    virtual G4double GetH1Xmax(G4int id) const;
    virtual G4double GetH1Width(G4int id) const;

    // Access to H2 parameters
    virtual G4int    GetH2Nxbins(G4int id) const;
    virtual G4double GetH2Xmin(G4int id) const;
    virtual G4double GetH2Xmax(G4int id) const;
    virtual G4double GetH2XWidth(G4int id) const;
    virtual G4int    GetH2Nybins(G4int id) const;
    virtual G4double GetH2Ymin(G4int id) const;
    virtual G4double GetH2Ymax(G4int id) const;
    virtual G4double GetH2YWidth(G4int id) const;

    // Setters for attributes for plotting
    virtual G4bool SetH1Title(G4int id, const G4String& title);
    virtual G4bool SetH1XAxisTitle(G4int id, const G4String& title);
    virtual G4bool SetH1YAxisTitle(G4int id, const G4String& title);
    virtual G4bool SetH2Title(G4int id, const G4String& title);
    virtual G4bool SetH2XAxisTitle(G4int id, const G4String& title);
    virtual G4bool SetH2YAxisTitle(G4int id, const G4String& title);
    virtual G4bool SetH2ZAxisTitle(G4int id, const G4String& title);

    // Access attributes for plotting
    virtual G4String GetH1Title(G4int id) const;
    virtual G4String GetH1XAxisTitle(G4int id) const;
    virtual G4String GetH1YAxisTitle(G4int id) const;
    virtual G4String GetH2Title(G4int id) const;
    virtual G4String GetH2XAxisTitle(G4int id) const;
    virtual G4String GetH2YAxisTitle(G4int id) const;
    virtual G4String GetH2ZAxisTitle(G4int id) const;
        
  protected:
    virtual G4bool WriteOnAscii(std::ofstream& output);

  private:
    // static data members
    //
    static G4CsvAnalysisManager* fgMasterInstance;
    static G4ThreadLocal G4CsvAnalysisManager* fgInstance;

    // methods
    //
    G4bool CreateNtupleFile(G4CsvNtupleDescription* ntupleDescription);
    G4bool CloseNtupleFile(G4CsvNtupleDescription* ntupleDescription); 

    void   CreateNtuplesFromBooking();
    tools::wcsv::ntuple::column<int>*    
      GetNtupleIColumn(G4int ntupleId, G4int columnId) const;
    tools::wcsv::ntuple::column<float>*  
      GetNtupleFColumn(G4int ntupleId, G4int columnId) const;
    tools::wcsv::ntuple::column<double>* 
      GetNtupleDColumn(G4int ntupleId, G4int columnId) const;

    virtual G4CsvNtupleDescription*  GetNtupleInFunction(G4int id, 
                                        G4String function,
                                        G4bool warn = true,
                                        G4bool onlyIfActive = true) const;
    virtual G4bool Reset();
    void  ExceptionForHistograms(const G4String& inFunction) const;
 
    // data members
    //
    std::vector<G4CsvNtupleDescription*> fNtupleVector;
};

#endif


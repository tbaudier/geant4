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
// $Id: HistoManager.cc,v 1.2 2009-10-23 13:40:40 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
///////////////////////////////////////////////////////////////////////////////////////..

#include "HistoManager.hh"
#include "HistoMessenger.hh"
#include "G4UnitsTable.hh"

#ifdef G4ANALYSIS_USE
#include "AIDA/AIDA.h"
#endif

//////////////////////////////////////////////////////////////////////////////////.....

HistoManager::HistoManager()
:
af(0),
tree(0),
factoryOn(false),
fNdNdOmega(0),
fNTheta(0),
fNTransverseCoordinates(0),
fNTransverseRadius(0),
fThetaZero(0.)
{
#ifdef G4ANALYSIS_USE
  // Creating the analysis factory
  af = AIDA_createAnalysisFactory();

  if(!af) 
  {
    G4cout << " HistoManager::HistoManager() :" 
           << " problem creating the AIDA analysis factory."
           << G4endl;
  }	     
#endif 
 
  fileName[0] = "testem5";
  fileType    = "root";
  fileOption  = "--noErrors export=root uncompress";  

  // histograms

  for (G4int k=0; k<MaxHisto; k++) 
  {
    histo[k] = 0;
    exist[k] = false;
    Unit[k]  = 1.0;
    Width[k] = 1.0;
    ascii[k] = false;        
  }

  histoMessenger = new HistoMessenger(this);
}

/////////////////////////////////////////////////////////////////////////////....

HistoManager::~HistoManager()
{
  delete histoMessenger;
  
#ifdef G4ANALYSIS_USE  
  delete af;
#endif    
}

////////////////////////////////////////////////////////////////////////////////////////..

void HistoManager::book()
{
#ifdef G4ANALYSIS_USE
  if(!af) return;
  
  // Creating a tree mapped to an hbook file.
  fileName[1] = fileName[0] + "." + fileType;  
  G4bool readOnly  = false;
  G4bool createNew = true;

  AIDA::ITreeFactory* tf  = af->createTreeFactory();
  tree = tf->create(fileName[1], fileType, readOnly, createNew, fileOption);

  delete tf;

  if(!tree) 
  {
    G4cout << "HistoManager::book() :" 
           << " problem creating the AIDA tree with "
           << " storeName = " << fileName[1]
           << " storeType = " << fileType
           << " readOnly = "  << readOnly
           << " createNew = " << createNew
           << " options = "   << fileOption
           << G4endl;
    return;
  }

  // Creating a histogram factory, whose histograms will be handled by the tree

  AIDA::IHistogramFactory* hf = af->createHistogramFactory(*tree);

  // create selected histograms

  for ( G4int k = 0; k < MaxHisto; k++ ) 
  {
    if ( exist[k] ) 
    {
      histo[k] = hf->createHistogram1D( Label[k], Title[k],
                                                  Nbins[k], Vmin[k], Vmax[k]);
      factoryOn = true;
    }
  }
  delete hf;  
  if(factoryOn) 
  {
      G4cout << "\n----> Histogram Tree is opened in " << fileName[1] << G4endl;
  }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////...

void HistoManager::save()
{
#ifdef G4ANALYSIS_USE
  if (factoryOn) 
  {
    saveAscii();          // Write ascii file, if any     
    tree->commit();       // Writing the histograms to the file
    tree->close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved in " << fileName[1] << G4endl;
    
    delete tree;
    tree = 0;
    factoryOn = false;
  }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Called from TrackingManager..

void HistoManager::FillHisto(G4int ih, G4double e, G4double weight)
{
  if ( ih > MaxHisto ) 
  {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << "does not exist; e= " << e << " w= " << weight << G4endl;
    return;
  }
  else if( ih == 12 )
  {
    fNdNdOmega += 1;
    fVectordNdOmegaX.push_back(e/Unit[ih]);  // vmg, check units
    fVectordNdOmegaY.push_back(weight);
  }
  else if( ih == 13 )
  {
    fNTheta += 1;
    fVectorTheta.push_back(e/Unit[ih]);  // vmg, check units 
  }
  else if( ih == 13 )
  {
    fNTransverseCoordinates += 1;
    fVectorTransverseCoordinates.push_back(e/Unit[ih]);  // vmg, check units 
  }
  else if( ih == 14 )
  {
    fNTransverseRadius += 1;
    fVectorTransverseRadius.push_back(e/Unit[ih]);  // vmg, check units 
  }







#ifdef G4ANALYSIS_USE
  if( exist[ih] ) histo[ih]->fill(e/Unit[ih], weight);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////...

void HistoManager::SetHisto(G4int ih,
            G4int nbins, G4double valmin, G4double valmax, const G4String& unit)
{
  if (ih > MaxHisto) {
    G4cout << "---> warning from HistoManager::SetHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }

  const G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                         "10","11","12","13","14","15","16","17","18","19",
			 "20","21","22","23","24","25","26","27","28","29",
			 "30","31","32","33","34","35","36","37","38","39",
			 "40","41","42","43","44","45","46","47","48","49" 
			};
			
  const G4String title[] =
                { "dummy",						//0
                  "energy deposit in absorber",				//1
                  "energy of charged secondaries at creation",		//2
                  "energy of gammas at creation (std::log10(ekin/MeV))",//3
                  "x_vertex of charged secondaries (all)",		//4
                  "x_vertex of charged secondaries (not absorbed)",	//5
		  "dummy","dummy","dummy","dummy",			//6-9
		  "(transmit, charged) : kinetic energy at exit",	//10
		  "(transmit, charged) : ener fluence: dE(MeV)/dOmega",	//11
		  "(transmit, charged) : space angle: dN/dOmega",	//12
		  "(transmit, charged) : projected angle at exit",	//13
		  "(transmit, charged) : projected position at exit",	//14
		  "(transmit, charged) : radius at exit",		//15
		  "dummy","dummy","dummy","dummy",			//16-19
		  "(transmit, neutral) : kinetic energy at exit",	//20
		  "(transmit, neutral) : ener fluence: dE(MeV)/dOmega",	//21
		  "(transmit, neutral) : space angle: dN/dOmega",	//22
		  "(transmit, neutral) : projected angle at exit",	//23
		  "dummy","dummy","dummy","dummy","dummy","dummy",	//24-29
		  "(reflect , charged) : kinetic energy at exit",	//30
		  "(reflect , charged) : ener fluence: dE(MeV)/dOmega",	//31
		  "(reflect , charged) : space angle: dN/dOmega",	//32
		  "(reflect , charged) : projected angle at exit",	//33
		  "dummy","dummy","dummy","dummy","dummy","dummy",	//34-39
		  "(reflect , neutral) : kinetic energy at exit",	//40
		  "(reflect , neutral) : ener fluence: dE(MeV)/dOmega",	//41
		  "(reflect , neutral) : space angle: dN/dOmega",	//42
		  "(reflect , neutral) : projected angle at exit",	//43
		  "dummy","dummy","dummy","dummy","dummy","dummy"	//44-49
                 };

  G4String titl = title[ih];
  G4double vmin = valmin, vmax = valmax;
  Unit[ih]      = 1.;

  if (ih == 3) 
  { 
    vmin = std::log10(valmin/MeV); 
    vmax = std::log10(valmax/MeV);
  }
  else if (unit != "none") 
  {
    titl     = title[ih] + " (" + unit + ")";
    Unit[ih] = G4UnitDefinition::GetValueOf(unit);
    vmin     = valmin/Unit[ih]; vmax = valmax/Unit[ih];
  }
  exist[ih] = true;
  Label[ih] = id[ih];
  Title[ih] = titl;
  Nbins[ih] = nbins;
  Vmin[ih]  = vmin;
  Vmax[ih]  = vmax;
  Width[ih] = (valmax-valmin)/nbins;

  G4cout << "----> SetHisto " << ih << ": " << titl << ";  "
         << nbins << " bins from "
         << vmin << " " << unit << " to " << vmax << " " << unit << G4endl;

}

///////////////////////////////////////////////////////////////////////////////////////////...

void HistoManager::RemoveHisto(G4int ih)
{
 if (ih > MaxHisto) 
 {
    G4cout << "---> warning from HistoManager::RemoveHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }

  histo[ih] = 0;  exist[ih] = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////..

void HistoManager::Scale(G4int ih, G4double fac)
{
 if (ih > MaxHisto) 
 {
    G4cout << "---> warning from HistoManager::Scale() : histo " << ih
           << "does not exist.  (fac = " << fac << ")" << G4endl;
    return;
  }
#ifdef G4ANALYSIS_USE
  if(exist[ih]) histo[ih]->scale(fac);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////..

void HistoManager::PrintHisto(G4int ih)
{
  if (ih < MaxHisto) 
  { 
    ascii[ih] = true; 
    ascii[0]  = true; 
  }
  else
  {
    G4cout << "---> warning from HistoManager::PrintHisto() : histo " << ih
           << "does not exist" << G4endl;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////..

#include <fstream>

void HistoManager::saveAscii()
{
#ifdef G4ANALYSIS_USE

 if (!ascii[0]) return;
 
 G4String name = fileName[0] + ".ascii";

 std::ofstream File(name, std::ios::out);
 File.setf( std::ios::scientific, std::ios::floatfield );
 
 //write selected histograms

 for ( G4int ih = 0; ih < MaxHisto; ih++ ) 
 {
    if (exist[ih] && ascii[ih]) 
    {
      File << "\n  1D histogram " << ih << ": " << Title[ih] 
           << "\n \n \t     X \t\t     Y" << G4endl;
     
      for (G4int iBin=0; iBin<Nbins[ih]; iBin++) 
      {
         File << "  " << iBin << "\t" 
              << 0.5*(histo[ih]->axis().binLowerEdge(iBin) +
	              histo[ih]->axis().binUpperEdge(iBin)) << "\t"	      
	      << histo[ih]->binHeight(iBin) 
	      << G4endl;
      } 
    }
 }
#endif
}

//////////////////////////////////////////////////////////////////////////////////////
//
//

void HistoManager::WriteFiles()
{



  return;
}




//
//
///////////////////////////////////////////////////////////////////////////////////////////////..



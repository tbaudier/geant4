// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ParticleTableIterator.hh,v 1.2 1999-10-29 08:03:54 kurasige Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
//	For information related to this code contact:
//	CERN, IT Division, ASD group
//	History: first implementation, based on object model of
//	28 Oct 1999, H.Kurashige
// ------------------------------------------------------------

#ifndef G4ParticleTableIterator_h
#define G4ParticleTableIterator_h 1

#include "g4std/map"

template < class K, class V > class G4ParticleTableIterator
{
 public:
  typedef  G4std::map<K, V, less<K> > Map;
  G4ParticleTableIterator( Map &adict):
    mydict(&adict),it(adict.begin()),defined(false){}

  G4bool operator++ ()
    {
      if(!defined) return false;
      it++;
      return it!=mydict->end() ? true : false;
    }
      
  G4bool operator()()
    {
      if(defined) {
	    return operator++();
      } else {
	    defined=true;
	    it=mydict->begin();
	    return it!=mydict->end() ? true : false;
      }
    }

  void reset (){defined=false;}
  K* key() const { return &((*it).first); }
  V  value() const { return (*it).second; }

 private:
  typename Map::iterator it;  
  Map * mydict;
  G4bool defined;
};

#endif

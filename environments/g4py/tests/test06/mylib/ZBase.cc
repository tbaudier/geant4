// $Id: ZBase.cc,v 1.1 2006-02-27 10:05:25 kmura Exp $
// ====================================================================
//   ZBase.cc
//
//                                         2005 Q
// ====================================================================
#include "ZBase.hh"
#include <iostream>

// ====================================================================
//
// class description
//
// ====================================================================

//////////////
ZBase::ZBase()
//////////////
{
}

///////////////
ZBase::~ZBase()
///////////////
{
}

/////////////////////
void ZBase::AMethod()
/////////////////////
{
  std::cout << "ZBase::AMethod is called." << std::endl;
}

////////////////////////////////////////
void ZBase::VMethod(std::string message)
////////////////////////////////////////
{
  std::cout << "ZBase::VMethod is called:"
	    << message
	    << std::endl;
}


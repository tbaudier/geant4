// $Id: AClass.cc,v 1.1 2006-02-27 10:05:25 kmura Exp $
// ====================================================================
//   AClass.cc
//
//                                         2005 Q
// ====================================================================
#include "AClass.hh"
#include <iostream>

// ====================================================================
//
// class description
//
// ====================================================================

////////////////
AClass::AClass()
  : ival(0)
////////////////
{
}

///////////////////////////////
AClass::AClass(int i, double d)
  : ival(i)
///////////////////////////////
{
  std::cout << "*** AClass constructor, d= "<< d << std::endl;
}

/////////////////
AClass::~AClass()
/////////////////
{
}


/////////////////////
int AClass::AMethod()
/////////////////////
{
  return 0;
}

//////////////////////////
int AClass::AMethod(int i)
//////////////////////////
{
  return 1;
}


////////////////////////////////////
int AClass::AMethod(int i, double d)
////////////////////////////////////
{
  return 2;
}



//////////////////////
int  AClass::BMethod()
//////////////////////
{
  return 0;
}

////////////////////////////////
double AClass::BMethod(double d)
////////////////////////////////
{
  return 1.;

}

///////////////////////////////////////////////////
double AClass::CMethod(int i, double d1, double d2)
///////////////////////////////////////////////////
{
  return i*d1*d2;
}


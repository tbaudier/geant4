// $Id: Step.hh,v 1.1 2006-02-27 10:05:24 kmura Exp $
// ====================================================================
//   Step.hh
//
//                                              2004 Q
// ====================================================================
#ifndef STEP_H
#define STEP_H

// ====================================================================
//
// class definition
//
// ====================================================================

class Step {
private:
  double x;

public:
  Step();
  ~Step();

  void SetX(double x0);
  double GetX() const;

};

// ====================================================================
// inline functions
// ====================================================================
inline void Step::SetX(double x0) { x= x0; }
inline double Step::GetX() const { return x; }

#endif

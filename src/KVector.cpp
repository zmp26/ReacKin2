#include "KVector.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

KVector::KVector()
: mX(0),
  mY(0),
  mZ(0)
{

}

KVector::KVector(double x, double y, double z)
: mX(x),
  mY(y),
  mZ(z)
{
	//nothing needs to happen here
}

KVector operator+(KVector v1, KVector v2){
	return KVector(v1.GetX()+v2.GetX(), v1.GetY()+v2.GetY(), v1.GetZ()+v2.GetZ());
}

KVector operator-(KVector v1, KVector v2){
	return KVector(v1.GetX()-v2.GetX(), v1.GetY()-v2.GetY(), v1.GetZ()-v2.GetZ());
}

KVector operator*(KVector v1, KVector v2){
	KVector v;
	v.SetX(v1.GetY()*v2.GetZ() - v1.GetZ()*v2.GetY());
	v.SetY(v1.GetZ()*v2.GetX() - v1.GetX()*v2.GetZ());
	v.SetZ(v1.GetX()*v2.GetY() - v1.GetY()*v2.GetX());
	return v;
}

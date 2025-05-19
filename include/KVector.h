#ifndef KVECTOR_H
#define KVECTOR_H

#include <iostream>
#include <cmath>

class KVector{
private:
	double mX, mY, mZ;

public:
	KVector();
	KVector(double x, double y, double z);

	double GetX() { return mX; }
	double GetY() { return mY; }
	double GetZ() { return mZ; }
	double Mag2() { return mX*mX + mY*mY + mZ*mZ; }
	double Mag() { return sqrt(Mag2()); }

	void SetX(double x) { mX = x; }
	void SetY(double y) { mY = y; }
	void SetZ(double z) { mZ = z; }
	void Set(double x, double y, double z) { mX = x; mY = y; mZ = z; }
	void Print() { std::cout << "(" << mX << "," << mY << "," << mZ << ")" << std::endl; }
	void Scale(double factor) { mX*=factor; mY*=factor; mZ*=factor; }
	void Copy(KVector v) { mX = v.GetX(); mY = v.GetY(); mZ = v.GetZ(); }

	KVector Unit() { return KVector(GetX()/Mag(), GetY()/Mag(), GetZ()/Mag()); }
};

#endif
#include "KNucleus.h"
#include "KVector.h"
#include <string>
#include <cmath>
#include <iostream>

KNucleus::KNucleus()
: mN(0),
  mZ(0),
  mA(0),
  mSymbol(""),
  mMassExcess(0.),
  mMass(0.),
  mLabEnergy(0.),
  mLabTheta(0.),
  mLabPhi(0.),
  mCMEnergy(0.),
  mCMTheta(0.),
  mCMPhi(0.),
  mExEnergy(0.),
  mLabMomentum(KVector())
{
	// default constructor
}

KNucleus::KNucleus(int n, int z, std::string symbol)
: mN(n),
  mZ(z),
  mA(n+z),
  mSymbol(symbol),
  mMassExcess(0.),
  mMass(0.),
  mLabEnergy(0.),
  mLabTheta(0.),
  mLabPhi(0.),
  mCMEnergy(0.),
  mCMTheta(0.),
  mCMPhi(0.),
  mExEnergy(0.),
  mLabMomentum(KVector()),
  mCMMomentum(KVector())
{
	// constructor
}

void KNucleus::SetMomentumFromEnergy(){
	double momentumMag = sqrt(2*GetMass()*GetLabEnergy());
	mLabMomentum.SetX(momentumMag*sin(GetLabTheta()*M_PI/180.)*cos(GetLabPhi()*M_PI/180.));
	mLabMomentum.SetY(momentumMag*sin(GetLabTheta()*M_PI/180.)*sin(GetLabPhi()*M_PI/180.));
	mLabMomentum.SetZ(momentumMag*cos(GetLabTheta()*M_PI/180.));
}

void KNucleus::SetEnergyFromMomentum(){
	SetLabEnergy(GetAbsMomentum()*GetAbsMomentum()/(2.*GetMass()));
}

void KNucleus::SetLabAngleFromMomentum(){
	double angle;
	if(GetAbsMomentum()!=0){
		angle = acos(mLabMomentum.GetZ()/GetAbsMomentum())*180./M_PI;
		SetLabTheta(angle);
	}
	if(mLabMomentum.GetX() != 0){
		angle = atan2(mLabMomentum.GetY(), mLabMomentum.GetX())*180./M_PI;
		if(angle < 0) angle += 360.;
		SetLabPhi(angle);
	}
}

void KNucleus::PrintAll() const {
	std::cout << "particle is " << mA << mSymbol << " (E,theta,phi): (" << mLabEnergy << ", " << mLabTheta << ", " << mLabPhi << ")" << std::endl;
}

void KNucleus::PrintMomenta() {
	std::cout << "3-momentum is: (" << mLabMomentum.GetX() << ", " << mLabMomentum.GetY() << ", " << mLabMomentum.GetZ() << ") ";
	std::cout << "Absolute value: " << GetAbsMomentum() << std::endl;
}

void KCompositeNucleus::SetDecay(int n, int z, int a, char* sym){
	mDecay.SetAll(n,z,a,sym);
}

void KCompositeNucleus::SetDaughter(int n, int z, int a, char* sym){
	mDecay.SetAll(n,z,a,sym);
}
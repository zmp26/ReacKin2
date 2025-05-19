#ifndef KNUCLEUS_H
#define KNUCLEUS_H

#include <string>

#include "KVector.h"

class KNucleus{
private:
	// int mN, mZ, mA;
	// std::string mSymbol;
	// double mMassExcess;
	// double mMass;
	// double mLabEnergy;
	// double mLabTheta;
	// double mLabPhi;
	// double mCMTheta;
	// double mCMPhi;
	// double mCMEnergy;
	// double mExEnergy;
	// KVector mLabMomentum;
	int mN, mZ, mA;
	std::string mSymbol;
	double mMassExcess;
	double mMass;
	double mLabEnergy;
	double mLabTheta;
	double mLabPhi;
	double mCMEnergy;
	double mCMTheta;
	double mCMPhi;
	double mExEnergy;
	KVector mLabMomentum;
	KVector mCMMomentum;

public:
	KNucleus();
	KNucleus(int n, int z, std::string symbol);

	void SetN(int n) { mN = n; }
	void SetZ(int z) { mZ = z; }
	void SetA(int a) { mA = a; }
	void SetSymbol(std::string sym) { mSymbol = sym; };
	void SetAll(int n, int z, int a, std::string sym){ SetN(n); SetZ(z); SetA(a); SetSymbol(sym);}
	
	void SetExEnergy(double e){ mExEnergy = e; }
	void SetMassExcess(double massExcess){ mMassExcess = massExcess; }
	void SetMass(double mass){ mMass = mass; }
	void SetLabEnergy(double energy) { mLabEnergy = energy; }
	void SetLabTheta(double theta){ mLabTheta = theta; }
	void SetLabPhi(double phi){ mLabPhi = phi; }
	void SetCMEnergy(double energy) { mCMEnergy = energy; }
	void SetCMTheta(double cmtheta){ mCMTheta = cmtheta; }
	void SetCMPhi(double cmphi){ mCMPhi = cmphi; }
	void SetMomentum(double px, double py, double pz) { mLabMomentum.Set(px,py,pz); };
	void SetMomentum(KVector momentum) { mLabMomentum.Copy(momentum); }

	void SetMomentumFromEnergy();
	void SetEnergyFromMomentum();
	void SetLabAngleFromMomentum();

	int GetN() const { return mN; }
	int GetZ() const { return mZ; }
	int GetA() const { return mA; }
	const std::string& GetSymbol() const { return mSymbol; }

	double GetExEnergy() const { return mExEnergy; }
	double GetMassExcess() const { return mMassExcess; }
	double GetMass() const { return mMass; }
	double GetLabEnergy() const { return mLabEnergy; }
	double GetLabTheta() const { return mLabTheta; }
	double GetLabPhi() const { return mLabPhi; }
	double GetCMEnergy() const { return mCMEnergy; }
	double GetCMTheta() const { return mCMTheta; }
	double GetCMPhi() const { return mCMPhi; }
	KVector GetMomentum() const { return mLabMomentum; }
	double GetAbsMomentum() { return mLabMomentum.Mag(); }

	void PrintAll() const;
	void PrintMomenta();

};

class KCompositeNucleus: public KNucleus{
private:
	KNucleus mDecay;
	KNucleus mDaughter;

public:
	void SetDecay(int n, int z, int a, char* sym);
	void SetDaughter(int n, int z, int a, char* sym);
	KNucleus* GetDecay() { return &mDecay; }
	KNucleus* GetDaughter() { return &mDaughter; }
};

#endif
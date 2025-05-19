#ifndef KREACTION_H
#define KREACTION_H

#include <cmath>

#include "KMassTable.h"
#include "KNucleus.h"

class KReaction{
private:
	KNucleus mTarget;
	KNucleus mBeam;
	KNucleus mEjectile;
	KNucleus mProduct;

	double mBeamEnergy;//MeV
	double mQValue;//MeV
	double mJacobian;//to handle Lab<->COM

	int mIsBreakUp;
	int mXPrint;
	int mYPrint;
	bool mDebug = false;

	const double DEGRAD = M_PI/180.;

public:
	KReaction();

	void InitMassTable(char *masstablename);
	void SetTarget(int a, char* sym);
	void SetBeam(int a, char* sym);
	void SetEjectile(int a, char* sym);
	void SetProduct(int a, char* sym);
	KNucleus* GetTarget() { return &mTarget; }
	KNucleus* GetBeam() { return &mBeam; }
	KNucleus* GetEjectile() { return &mEjectile; }
	KNucleus* GetProduct() { return &mProduct; }

	void SetBeamEnergy(double beamenergy);
	void SetExEnergy(double exc1, double exc2);
	void SetQValue();
	void SetDebug(bool debug){ mDebug = debug; }
	void SetAll(int at, char* symt, int ab, char* symb, int ae, char* syme);

	double GetBeamEnergy() { return mBeamEnergy; }
	double GetParticleExEnergy(int particle) { if(particle==0){ return mEjectile.GetExEnergy(); } else if(particle==1) { return mProduct.GetExEnergy(); } else { return -666.; }}
	double GetQValue() { return mQValue; }


	void PrintReaction();


	void Resolve();
	void SetEjectileAngle(double eangle) { mEjectile.SetLabTheta(eangle); };

	float GetThetaMax(void);

	bool CalculateKinematicEvent(int sol);

protected:
	KMassTable mMassTable; 

};


#endif
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>

#include "KVector.h"
#include "KReaction.h"
#include "KNucleus.h"
#include "KMassTable.h"

KReaction::KReaction()
: mIsBreakUp(false)
{
	//default constructor
	//do nothing here, handle by manual setting in KEventHandler
}

void KReaction::InitMassTable(char *masstablename){
	if(!mMassTable.GetInitStatus()){
		mMassTable.Init(masstablename);
	}
}

void KReaction::SetTarget(int a, char* sym){
	mTarget.SetAll(mMassTable.GetN(sym,a),mMassTable.GetZ(sym),a,sym);
	mTarget.SetMass(mMassTable.GetMassMeV(mTarget.GetN(),mTarget.GetZ(),mTarget.GetA()));
	mTarget.SetMassExcess(mMassTable.GetMassExcess(mTarget.GetN(), mTarget.GetZ(), mTarget.GetA()));
}

void KReaction::SetBeam(int a, char* sym){
	mBeam.SetAll(mMassTable.GetN(sym,a),mMassTable.GetZ(sym),a,sym);
	mBeam.SetMass(mMassTable.GetMassMeV(mBeam.GetN(),mBeam.GetZ(),mBeam.GetA()));
	mBeam.SetMassExcess(mMassTable.GetMassExcess(mBeam.GetN(), mBeam.GetZ(), mBeam.GetA()));
}

void KReaction::SetBeamEnergy(double beamenergy){
	mBeamEnergy = beamenergy;
	mBeam.SetLabEnergy(mBeamEnergy);
	double pbeam = sqrt(2.*mBeam.GetMass()*mBeamEnergy);
	mBeam.SetMomentum(0.,0.,pbeam);
}

void KReaction::SetExEnergy(double exc1, double exc2){
	mEjectile.SetExEnergy(exc1);
	mProduct.SetExEnergy(exc2);
}

void KReaction::SetEjectile(int a, char* sym){
	mEjectile.SetAll(mMassTable.GetN(sym,a),mMassTable.GetZ(sym),a,sym);
	mEjectile.SetMassExcess(mMassTable.GetMassExcess(mEjectile.GetN(), mEjectile.GetZ(), mEjectile.GetA()));
}

void KReaction::SetProduct(int a, char* sym){
	mProduct.SetAll(mMassTable.GetN(sym,a),mMassTable.GetZ(sym),a,sym);
	mProduct.SetMassExcess(mMassTable.GetMassExcess(mProduct.GetN(),mProduct.GetZ(),mProduct.GetA()));	
}

void KReaction::SetQValue(){
	mQValue = mTarget.GetMassExcess()+mBeam.GetMassExcess()-mEjectile.GetMassExcess()-mProduct.GetMassExcess();
}

void KReaction::SetAll(int at, char* symt, int ab, char* symb, int ae, char* syme){
	if(!mIsBreakUp){
		mTarget.SetAll(mMassTable.GetN(symt,at),mMassTable.GetZ(symt),at,symt);
		mTarget.SetMassExcess(mMassTable.GetMassExcess(mTarget.GetN(),mTarget.GetZ(),mTarget.GetA()));
		mTarget.SetMass(mMassTable.GetMassMeV(mTarget.GetN(),mTarget.GetZ(),mTarget.GetA()));
	} else {
		mTarget.SetMassExcess(0.0);
		mTarget.SetMass(0.);
	}

	mBeam.SetAll(mMassTable.GetN(symb,ab),mMassTable.GetZ(symb),ab,symb);
	mBeam.SetMassExcess(mMassTable.GetMassExcess(mBeam.GetN(),mBeam.GetZ(),mBeam.GetA()));
	mBeam.SetMass(mMassTable.GetMassMeV(mBeam.GetN(),mBeam.GetZ(),mBeam.GetA()));

	mEjectile.SetAll(mMassTable.GetN(syme,ae),mMassTable.GetZ(syme),ae,syme);
	mEjectile.SetMassExcess(mMassTable.GetMassExcess(mEjectile.GetN(),mEjectile.GetZ(),mEjectile.GetA()));
	mEjectile.SetMass(mMassTable.GetMassMeV(mEjectile.GetN(),mEjectile.GetZ(),mEjectile.GetA()));
	//std::cout << "ejectile mass: " << mEjectile.GetMass() << std::endl;

	mProduct.SetZ(mTarget.GetZ()+mBeam.GetZ()-mEjectile.GetZ());
	mProduct.SetA(mTarget.GetA()+mBeam.GetA()-mEjectile.GetA());
	mProduct.SetN(mTarget.GetN()+mBeam.GetN()-mEjectile.GetN());
	mProduct.SetSymbol(mMassTable.GetSymbol(mProduct.GetZ()));
	mProduct.SetMassExcess(mMassTable.GetMassExcess(mProduct.GetN(),mProduct.GetZ(),mProduct.GetA()));
	mProduct.SetMass(mMassTable.GetMassMeV(mProduct.GetN(),mProduct.GetZ(),mProduct.GetA()));
	//std::cout << "product mass: " << mProduct.GetMass() << std::endl;

	mQValue = mTarget.GetMassExcess()+mBeam.GetMassExcess()-mEjectile.GetMassExcess()-mProduct.GetMassExcess();
}

void KReaction::Resolve(){
	mProduct.SetZ(mTarget.GetZ()+mBeam.GetZ()-mEjectile.GetZ());
	mProduct.SetA(mTarget.GetA()+mBeam.GetA()-mEjectile.GetA());
	mProduct.SetN(mTarget.GetN()+mBeam.GetN()-mEjectile.GetN());
	mProduct.SetSymbol(mMassTable.GetSymbol(mProduct.GetZ()));
	mProduct.SetMassExcess(mMassTable.GetMassExcess(mProduct.GetN(),mProduct.GetZ(),mProduct.GetA()));
	mProduct.SetMass(mMassTable.GetMassMeV(mProduct.GetN(),mProduct.GetZ(),mProduct.GetA()));

	mQValue = mTarget.GetMassExcess()+mBeam.GetMassExcess()-mEjectile.GetMassExcess()-mProduct.GetMassExcess();
}



bool KReaction::CalculateKinematicEvent(int sol){
/*
	This function assumes that all of the KNucleus objects are filled with
	proper physics data. A helper function is called at the beginning of this
	function to set momentum and excitation energies. That helper function can
	contain any possible way to get data - reading in from file, user input, etc

	Returns true if the event calculation succeeds, false otherwise
	
*/


	double a,b,c,disc;//a,b,c are coefficients of a qudratic in p_ejectile
	double pej, eej;

	//calculate coefficients:
	//a = (2M_ej)^-1 + (2M_rec)^-1
	//b = (-P_beam*cos(theta_ej)/M_rec)
	//c = (P_beam)^2/(2M_rec) - E_beam - QValue + ExE_ej + ExE_rec

	a = 1./(2.*mEjectile.GetMass()) + 1./(2.*mProduct.GetMass());
	b = -(mBeam.GetAbsMomentum())*cos(mEjectile.GetLabTheta()*DEGRAD)/mProduct.GetMass();
	c = (mBeam.GetAbsMomentum()*mBeam.GetAbsMomentum())/(2.*mProduct.GetMass()) - mBeamEnergy - mQValue + mEjectile.GetExEnergy() + mProduct.GetExEnergy();
	disc = b*b - 4.*a*c;
	double k = mBeam.GetAbsMomentum()/mProduct.GetMass();
	double x = cos(DEGRAD*mEjectile.GetLabTheta());
	if(disc<0) return false;

	if(sol==0){
		//calculate pej from quadratic formula
		pej = (-b + sqrt(disc))/(2.*a);
		double pejx = pej*sin(DEGRAD*mEjectile.GetLabTheta())*cos(DEGRAD*mEjectile.GetLabPhi());
		double pejy = pej*sin(DEGRAD*mEjectile.GetLabTheta())*sin(DEGRAD*mEjectile.GetLabPhi());
		double pejz = pej*cos(DEGRAD*mEjectile.GetLabTheta());
		eej = pej*pej/(2.*mEjectile.GetMass());
		mEjectile.SetLabEnergy(eej);
		mEjectile.SetMomentum(pejx,pejy,pejz);

		//now calculate prec assuming px, py are equal and opposite
		double precx = -pejx;
		double precy = -pejy;
		double precz = mBeam.GetMomentum().GetZ()-mEjectile.GetMomentum().GetZ();
		mProduct.SetMomentum(precx,precy,precz);
		mProduct.SetEnergyFromMomentum();
		mProduct.SetLabAngleFromMomentum();

		//now do CM stuff
		double vocm = (mBeam.GetMass()/(mBeam.GetMass()+mTarget.GetMass()))*(mBeam.GetAbsMomentum()/mBeam.GetMass());
		double ecm = (mTarget.GetMass()/(mTarget.GetMass()+mBeam.GetMass()))*mBeamEnergy;
		double massterm = 1./(1./(2.*mEjectile.GetMass()) + 1./(2.*mProduct.GetMass()));
		double v1cm = sqrt((ecm+mQValue-mEjectile.GetExEnergy()-mProduct.GetExEnergy())*massterm)/mEjectile.GetMass();
		double v1 = sqrt(2.*mEjectile.GetLabEnergy()/mEjectile.GetMass());
		double cosalpha = (vocm*vocm + v1cm*v1cm - v1*v1)/(2.*vocm*v1cm);

		if(cosalpha > -1 && cosalpha < 1){
			mEjectile.SetCMTheta(180.-acos(cosalpha)/DEGRAD);
		}
		mJacobian=(k*x + sqrt(disc))*(k+2*k*k*x/sqrt(disc))/((vocm*v1cm)*(4*a*a*mEjectile.GetMass()*mEjectile.GetMass()));



	} else if(sol==1){
		//calculate pej from quadratic formula
		pej = (-b - sqrt(disc))/(2.*a);
		double pejx = pej*sin(DEGRAD*mEjectile.GetLabTheta())*cos(DEGRAD*mEjectile.GetLabPhi());
		double pejy = pej*sin(DEGRAD*mEjectile.GetLabTheta())*sin(DEGRAD*mEjectile.GetLabPhi());
		double pejz = pej*cos(DEGRAD*mEjectile.GetLabTheta());
		eej = pej*pej/(2.*mEjectile.GetMass());
		mEjectile.SetLabEnergy(eej);
		mEjectile.SetMomentum(pejx,pejy,pejz);

		//now calculate prec assuming px, py are equal and opposite
		double precx = -pejx;
		double precy = -pejy;
		double precz = mBeam.GetMomentum().GetZ()-mEjectile.GetMomentum().GetZ();
		mProduct.SetMomentum(precx,precy,precz);
		mProduct.SetEnergyFromMomentum();
		mProduct.SetLabAngleFromMomentum();

		//now do CM stuff
	}

	return true;
}
#include <random>
#include <fstream>
#include <string>
#include <sstream>

#include "KEventHandler.h"
#include "KNucleus.h"
#include "KReaction.h"

namespace {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<double> thetaDist(0.,180.);//degrees
	std::uniform_real_distribution<double> phiDist(0.,360.);//degrees
}

KEventHandler::KEventHandler(){
	std::string masstablepath = "../data/masstable.dat";
	mReaction.InitMassTable(masstablepath.data());
}

void KEventHandler::ConfigureReaction(int at, const char* symt,
		                  			  int ab, const char* symb,
		                 			  int aej, const char* symej,
		             		          double beamEnergy, double exEjectile,double exProduct){
	mBeamEnergy = beamEnergy;
	mExEjectile = exEjectile;
	mExProduct = exProduct;

	mReaction.SetAll(at, const_cast<char*>(symt), ab, const_cast<char*>(symb),aej,const_cast<char*>(symej));
	mReaction.SetBeamEnergy(mBeamEnergy);
	mReaction.SetExEnergy(mExEjectile,mExProduct);
}

double KEventHandler::SampleTheta(){
	return thetaDist(rng);
}

double KEventHandler::SamplePhi(){
	return phiDist(rng);
}

void KEventHandler::RunEvent_output(bool output){
	double theta = SampleTheta();
	double phi = SamplePhi();

	//get ejectile and product from reaction
	KNucleus* ejectile = mReaction.GetEjectile();
	KNucleus* product = mReaction.GetProduct();

	//std::cout << "ejectile mass: " << ejectile->GetMass() << std::endl;
	//std::cout << "product mass: " << product->GetMass() << std::endl;

	//set ejectile angles in the lab
	ejectile->SetLabTheta(theta);
	ejectile->SetLabPhi(phi);

	bool success = mReaction.CalculateKinematicEvent(0);
	if(!success){
		if(output) std::cerr << "Kinematic solution failed for theta = " << theta << ", phi = " << phi << std::endl;
	} else {
		if(output) std::cout << "Event calculated!" << std::endl;
		double e1 = ejectile->GetLabEnergy();
		double theta1 = ejectile->GetLabTheta();
		double phi1 = ejectile->GetLabPhi();

		double thetacm = ejectile->GetCMTheta();

		double e2 = product->GetLabEnergy();
		double theta2 = product->GetLabTheta();
		double phi2 = product->GetLabPhi();

		if(output) std::printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n",e1,theta1,phi1,thetacm,e2,theta2,phi2);
	}
}

std::string KEventHandler::RunEvent(){
	std::string retval = "-666\t-666\t-666\t-666\t-666\t-666\t-666";
	std::ostringstream result;

	double theta = SampleTheta();
	double phi = SamplePhi();

	//get ejectile and product from reaction
	KNucleus* ejectile = mReaction.GetEjectile();
	KNucleus* product = mReaction.GetProduct();

	//set ejectile angles in the lab
	ejectile->SetLabTheta(theta);
	ejectile->SetLabPhi(phi);

	bool success = mReaction.CalculateKinematicEvent(0);
	if(!success){
		//std::cerr << "Kinematic solution failed for theta = " << theta << ", phi = " << phi << endl;
		return retval;

	} else {
		double e1 = ejectile->GetLabEnergy();
		double theta1 = ejectile->GetLabTheta();
		double phi1 = ejectile->GetLabPhi();

		double thetacm = ejectile->GetCMTheta();

		double e2 = product->GetLabEnergy();
		double theta2 = product->GetLabTheta();
		double phi2 = product->GetLabPhi();

		result << e1 << "\t" << theta1 << "\t" << phi1 << "\t" << thetacm << "\t" << e2 << "\t" << theta2 << "\t" << phi2;
		retval = result.str();
	}

	return retval;
}

void KEventHandler::RunBatch(int n, bool output){
	for(int i=0; i<n; i++){
		RunEvent_output(output);
	}
}

void KEventHandler::RunBatch(int n, std::ofstream& outfile, bool header){
	if(header){
		outfile << "e1\ttheta1\tphi1\tthetacm\te2\ttheta2\tphi2" << std::endl;
	}
	for(int i=0; i<n; i++){
		outfile << RunEvent() << std::endl;
		if((i+1)%100000==0) std::cout << "Processed " << i+1 << " events of " << n << " (" << (i+1)*100./float(n) << "%)" << std::endl;
	}
}
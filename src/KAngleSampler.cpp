#include "KAngleSampler.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

KAngleSampler::KAngleSampler()
: mRandomEngine(std::random_device{}()),
  mUniformDistribution(0.,1.)
{
	//
}

void KAngleSampler::SetLegendreCoefficients(const std::vector<double>& coefficients){
	mLegendreCoefficients = coefficients;
	mUseLegendreExpansion = !coefficients.empty();
}

void KAngleSampler::Sample(double& theta, double& phi, int maxIterations){
	if(mUseLegendreExpansion){
		if(!SampleFromLegendreExpansion(theta,phi,maxIterations)){
			//if sampling fails, fall back to isotropic
			std::cerr << "Warning: Legendre sampling failed, using isotropic." << std::endl;
			SampleIsotropic(theta,phi);
		}
	} else {
		SampleIsotropic(theta,phi);
	}
}

void KAngleSampler::SampleIsotropic(double& theta, double& phi){
	double u = mUniformDistribution(mRandomEngine);
	double v = mUniformDistribution(mRandomEngine);

	theta = std::acos(1.0 - 2.0*u)/DEGRAD;
	phi = 2.*M_PI*v/DEGRAD;
}

bool KAngleSampler::SampleFromLegendreExpansion(double& theta, double& phi, int maxIterations){
	//Acceptance-Rejection method
	//find the maximum value of the distribution
	int nSamples = 10000;
	double maxVal = 0.;
	for(int i=0; i<nSamples; i++){
		double costheta = 2. * mUniformDistribution(mRandomEngine) - 1.;
		maxVal = std::max(maxVal,EvaluateLegendreExpansion(costheta));
	}
	if(maxVal <= 0.) return false;//avoid division by 0

	for(int i=0; i<maxIterations; i++){
		double costheta = 2.*mUniformDistribution(mRandomEngine)-1.;
		double phi_rand = 2.*M_PI*mUniformDistribution(mRandomEngine);
		double pdfValue = EvaluateLegendreExpansion(costheta);
		double acceptanceProbability = pdfValue/maxVal;
		double randomY = mUniformDistribution(mRandomEngine);

		if(randomY < acceptanceProbability){
			theta = std::acos(costheta)/DEGRAD;
			phi = phi_rand/DEGRAD;
			return true;
		}
	}

	std::cerr << "Warning: Maximum iterations reached in angle sampling." << std::endl;
	return false;
}

double KAngleSampler::EvaluateLegendreExpansion(double cosTheta) const {
	double value = 0.;
	for(size_t l=0; l<mLegendreCoefficients.size(); l++){
		value += mLegendreCoefficients[l]*LegendreP(l,cosTheta);
	}
	return value;
}

double KAngleSampler::LegendreP(int l, double x) const {
	if(std::abs(x) > 1.0 + 1e-0){
		throw std::out_of_range("Argument of Legendre polynomial must be in [-1,1]");
	}
	if(l==0) return 1.;
	if(l==1) return x;
	double p_l_minus_2 = 1.;
	double p_l_minus_1 = x;
	for(int i=2; i<=l; i++){
		double pl = ((2.*i - 1.)*x*p_l_minus_1 - (i-1.)*p_l_minus_2)/i;
		p_l_minus_2 = p_l_minus_1;
		p_l_minus_1 = pl;
	}
	return p_l_minus_1;
}
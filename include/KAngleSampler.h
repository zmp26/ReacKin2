#ifndef KANGLESAMPLER_H
#define KANGLESAMPLER_H

#include <cmath>
#include <random>
#include <vector>

class KAngleSampler{
public:
	KAngleSampler();
	virtual ~KAngleSampler() = default;

	void Sample(double& theta, double& phi, int maxIterations=1000);
	void SampleIsotropic(double& theta, double& phi);
	void SetLegendreCoefficients(const std::vector<double>& coefficients);
	bool SampleFromLegendreExpansion(double& theta, double& phi, int maxIterations);

private:
	double LegendreP(int l, double x) const;
	double EvaluateLegendreExpansion(double cosTheta) const;

	std::mt19937 mRandomEngine;
	std::uniform_real_distribution<> mUniformDistribution;
	std::vector<double> mLegendreCoefficients;
	bool mUseLegendreExpansion = false;

	const double DEGRAD = M_PI/180.;
};

#endif
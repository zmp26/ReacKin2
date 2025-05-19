#ifndef KEVENTHANDLER_H
#define KEVENTHANDLER_H

#include <string>
#include <iostream>
#include <fstream>

#include "KReaction.h"
#include "KAngleSampler.h"

class KEventHandler{
public:
	KEventHandler();

	double GetBeamEnergy() { return mBeamEnergy; }
	double GetExEjectile() { return mExEjectile; }
	double GetExProduct() { return mExProduct; }
	KReaction* GetReaction() { return &mReaction; }

	void ConfigureReaction(int at, const char* symt,
		                   int ab, const char* symb,
		                   int aej, const char* symej,
		                   double beamEnergy, double exEjectile=0.,double exProduct=0.);

	void RunEvent_output(bool output=true);				//run single event
	std::string RunEvent();								//run single event, returns string of output instead of outputting to terminal
	void RunBatch(int n, bool output=false);		//run n events
	void RunBatch(int n, std::ofstream& outfile, bool header=false);		//run n events output to outfile

	void SetLegendreCoefficients(const std::vector<double>& coefficients);

private:
	KReaction mReaction;
	double mBeamEnergy;
	double mExEjectile;
	double mExProduct;
	KAngleSampler mAngleSampler;
};

#endif
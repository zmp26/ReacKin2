#ifndef KSIMULATIONMANAGER_H
#define KSIMULATIONMANAGER_H

#include <string>
#include <vector>
#include "KEventHandler.h"

class KSimulationManager {
public:
	KSimulationManager();

	//load reactions from a file with tab delimited entries
	bool LoadReactionsFile(const std::string& filename);

	//Run all simulations in the reactions file (no output to file)
	void RunAll(bool verbose=true);

	//Run all simulations in the reactions file (output to individual files)
	void RunAllToIndividualFiles(const std::string& outputDir, bool includeHeaders=true);

	//Run all simulations in the reactions file (output to single file)
	void RunAllToSingleFile(const std::string& outputFileName, bool includeHeaders=true);

	//run only the simulation at index
	void RunReaction(size_t index, bool verbose=true);

	//access to loaded configurations
	size_t GetNumReactions() const;
	const std::string& GetMassTablePath() const;

private:
	struct ReactionFileContents {
		int targetA;
		std::string targetSym;

		int beamA;
		std::string beamSym;

		int ejectileA;
		std::string ejectileSym;

		double beamEnergy = 0.;
		double exEjectile = 0.;
		double exProduct = 0.;

		int numEvents = 1;

		std::string outputFileName;
	};

	std::vector<ReactionFileContents> mReactions;
	std::string mMassTablePath;

	//parses one line or block from tab delimited file
	bool ParseConfigLine(const std::string& line, ReactionFileContents& contents, char delim='\t');
};

#endif
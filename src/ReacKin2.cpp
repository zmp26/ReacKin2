#include <fstream>
#include <chrono>
#include <iostream>
#include <system_error>
#include <filesystem>

#include "KEventHandler.h"
#include "KSimulationManager.h"

int main(){
	// std::ofstream outfile;
	// std::string outfilename = "test.out";
	// outfile.open(outfilename.data());

	// KEventHandler handler;

	// handler.ConfigureReaction(
	// 	197, "Au",		//target
	// 	4, "He",		//beam
	// 	4, "He",		//ejectile
	// 	5.				//beam energy in MeV
	// );

	// int numEvents = 1000000;

	// auto start = std::chrono::high_resolution_clock::now();
	// handler.RunBatch(numEvents,outfile,false);
	// auto stop = std::chrono::high_resolution_clock::now();
	// std::chrono::duration<double> elapsed = stop - start;

	// outfile.close();
	// std::cout << "Finished running " << numEvents << " events in " << elapsed.count() << " seconds." << std::endl;
	// std::cout << "Saved test output file: " << outfilename.data() << std::endl;
	// return 0;

	const std::string reactionsfile = "../data/reactions.input";

	KSimulationManager simman;

	if(!simman.LoadReactionsFile(reactionsfile)){
		std::cerr << "Error: Could not load reactions file " << reactionsfile << std::endl;
		return 1; //exit if laoding the file fails
	}

	std::cout << "Loaded " << simman.GetNumReactions() << " reactions from " << reactionsfile << std::endl;

	const std::string outputDir = "../output/";
	std::filesystem::create_directory(outputDir);

	simman.RunAllToIndividualFiles(outputDir,false);

	const std::string outputFileName = outputDir + "all_reactions_output.out";
	simman.RunAllToSingleFile(outputFileName,false);

	std::cout << "Simulation complete! Results are saved." << std::endl;

	return 0;
}
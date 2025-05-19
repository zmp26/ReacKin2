#include "KSimulationManager.h"
#include "KEventHandler.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

KSimulationManager::KSimulationManager(){
	//nothing for now
}

//parse a single line from the reactions file
bool KSimulationManager::ParseConfigLine(const std::string& line, ReactionFileContents& contents, char delim){
	std::istringstream ss(line);
	std::string token;

	try{
		std::getline(ss,token,delim); contents.targetA = std::stoi(token);
		std::getline(ss,token,delim); contents.targetSym = token;

		std::getline(ss,token,delim); contents.beamA = std::stoi(token);
		std::getline(ss,token,delim); contents.beamSym = token;

		std::getline(ss,token,delim); contents.ejectileA = std::stoi(token);
		std::getline(ss,token,delim); contents.ejectileSym = token;

		std::getline(ss,token,delim); contents.beamEnergy = std::stod(token);
		std::getline(ss,token,delim); contents.exEjectile = std::stod(token);
		std::getline(ss,token,delim); contents.exProduct = std::stod(token);
		std::getline(ss,token,delim); contents.numEvents = std::stoi(token);
		std::getline(ss,token,delim); contents.outputFileName = token;

		//extract legendre coefficients (example: "[a,b,c]")
		size_t start_pos = line.find('[');
		size_t end_pos = line.find(']');
		if(start_pos != std::string::npos && end_pos != std::string::npos && start_pos < end_pos){
			std::string coeff_str = line.substr(start_pos + 1, end_pos - start_pos - 1);
			std::stringstream coeff_ss(coeff_str);
			std::string coeff_token;
			while(std::getline(coeff_ss,coeff_token, ',')){
				try{
					contents.legendreCoefficients.push_back(std::stod(coeff_token));
				} catch(const std::invalid_argument& e){
					std::cerr << "Warning: Invalid Legendre coefficient: " << coeff_token << std::endl;
				} catch(const std::out_of_range& e){
					std::cerr << "Warning: Legendre coefficient out of range: " << coeff_token << std::endl;
				}
			}
		}

		return true;
	} catch(...){
		return false;
	}
}

//load and parse a reactions file containing one reaction per line
bool KSimulationManager::LoadReactionsFile(const std::string& filename){
	std::ifstream infile(filename);
	if(!infile.is_open()){
		std::cerr << "Error: Could not open reactions file " << filename << std::endl;
		return false;
	}

	std::string line;
	size_t linenum = 0;

	while(std::getline(infile,line)){
		linenum += 1;

		//skip comments and empty lines
		if(line.empty() || line[0] == '#' || line[0] == '/') continue;

		//get rid of pesky windows \r characters from windows text editor
		std::string result = line;
		if(!result.empty() && result[result.size()-1] == '\r'){
			result.erase(result.size() - 1);
		}

		ReactionFileContents contents;
		if(ParseConfigLine(result,contents)){
			mReactions.push_back(contents);
		} else {
			std::cerr << "Warning: failed to parse line " << linenum << ": " << result << std::endl;
		}
	}

	infile.close();
	return true;
}

//run all loaded reactions
void KSimulationManager::RunAll(bool verbose){
	for(size_t i=0; i<mReactions.size(); i++){
		RunReaction(i,verbose);
	}
}

void KSimulationManager::RunAllToIndividualFiles(const std::string& outputDir, bool includeHeaders){
	//make sure outputDir exists
	std::filesystem::create_directories(outputDir);

	for(size_t i=0; i<mReactions.size(); i++){
		const ReactionFileContents& contents = mReactions[i];

		//construct filename e.g. outputDir/reaction_0.txt
		std::ostringstream filename;
		filename << outputDir << "/" << contents.outputFileName;
		//filename << outputDir << "/reaction_" << i << ".out";

		std::ofstream outfile(filename.str());
		if(!outfile.is_open()){
			std::cerr << "Error: Could not open output file " << filename.str() << std::endl;
			continue;
		}

		KEventHandler handler;
		handler.ConfigureReaction(
			contents.targetA, contents.targetSym.c_str(),
			contents.beamA, contents.beamSym.c_str(),
			contents.ejectileA, contents.ejectileSym.c_str(),
			contents.beamEnergy, contents.exEjectile, contents.exProduct
		);
		handler.SetLegendreCoefficients(contents.legendreCoefficients);

		std::cout << "Writing reaction " << i << " to " << filename.str() << "...\n";
		handler.RunBatch(contents.numEvents,outfile,includeHeaders);

		outfile.close();
	}
}

void KSimulationManager::RunAllToSingleFile(const std::string& outputFileName, bool includeHeaders){
	std::ofstream outfile(outputFileName);
	if(!outfile.is_open()){
		std::cerr << "Error: Could not open " << outputFileName << " for writing.\n";
		return;
	}

	for(size_t i=0; i<mReactions.size(); i++){
		const ReactionFileContents& contents = mReactions[i];

		KEventHandler handler;
		handler.ConfigureReaction(
			contents.targetA, contents.targetSym.c_str(),
			contents.beamA, contents.beamSym.c_str(),
			contents.ejectileA, contents.ejectileSym.c_str(),
			contents.beamEnergy, contents.exEjectile, contents.exProduct
		);
		handler.SetLegendreCoefficients(contents.legendreCoefficients);

		//optionally prepend header for each reaction
		if(includeHeaders){
			outfile << "Reaction " << i << ": "
				    << contents.targetA << contents.targetSym << "("
				    << contents.beamA << contents.beamSym << ","
				    << contents.ejectileA << contents.ejectileSym << ")"
				    << handler.GetReaction()->GetProduct()->GetA() << handler.GetReaction()->GetProduct()->GetSymbol()
				    << std:: endl;
		}

		handler.RunBatch(contents.numEvents,outfile,includeHeaders);
	}

	outfile.close();
}

//ruin a specific reaction by index
void KSimulationManager::RunReaction(size_t index, bool verbose){
	if(index >= mReactions.size()){
		std::cerr << "Error: Invalid reaction index " << index << std::endl;
		return;
	}

	const ReactionFileContents& contents = mReactions[index];

	KEventHandler handler;
	handler.ConfigureReaction(
		contents.targetA, contents.targetSym.c_str(),
		contents.beamA, contents.beamSym.c_str(),
		contents.ejectileA, contents.ejectileSym.c_str(),
		contents.beamEnergy, contents.exEjectile, contents.exProduct
	);
	handler.SetLegendreCoefficients(contents.legendreCoefficients);

	if(verbose){
		std::cout << "Running reaction " << index+1 << "/" << mReactions.size() << ": "
		<< contents.targetA << contents.targetSym << "(" << contents.beamA << contents.beamSym
		<< "," << contents.ejectileA << contents.ejectileSym << ")" << handler.GetReaction()->GetProduct()->GetA()
		<< handler.GetReaction()->GetProduct()->GetSymbol() << std::endl;
	}

	handler.RunBatch(contents.numEvents,verbose);
}

//return how many reactions were loaded
size_t KSimulationManager::GetNumReactions() const {
	return mReactions.size();
}
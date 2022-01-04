#include "CompressionHandler.h"
#include "ZipFile.h"
#include <iostream>
#include <stdio.h>

std::string substring(std::string str, int start, int end) {
	//Create string to hold characters
	std::string newStr = "";

	//For each character in range
	for (int i = start; i < end; i++) {
		//Add character to new string
		newStr.push_back(str.at(i));
	}

	return newStr;
}

std::vector<std::string> parsePath(std::string path) {
	//Create vector to hold paths
	std::vector<std::string> parsedPath;
	//Create start position
	int startPos = 0;

	//For each character
	for (int i = 0; i < path.size(); ++i) {
		//If a '/' is met
		if (path[i] == '/') {
			//Add substring from last start position
			parsedPath.push_back(substring(path, startPos, i + 1));
			//Update start position to current '\' index
			startPos = i + 1;
		}
	}

	parsedPath.push_back(path.substr(startPos, path.size() - 1));

	return parsedPath;
}

std::string getArchiveNameFromParsedPath(std::vector<std::string> path) {
	//For each sub-path in path
	for (int i = 0; i < path.size(); ++i) {
		//If sub path is at least size of 6 and has extension ".bmna/"
		if (path[i].size() >= 6 && substring(path[i], path[i].size() - 6, path[i].size()) == ".bmna/") {
			//Remove the "/" from sub path
			path[i].pop_back();

			//Return sub-path
			return path[i];
		}
	}

	return "-1";
}

void deleteFile(std::string filename) {
	try {
		//Delete file
		remove(filename.c_str());
	}
	catch (std::exception& e) {
		//Output error
		std::cout << "An error occured during deletion of asset " << filename << ". Reason: " << e.what() << "\n";
	}
}

void compressFile(std::string archiveName, std::string filename) {
	try {
		//Compress file
		ZipFile::AddFile(archiveName, filename, LzmaMethod::Create());
	}
	catch (std::exception& e) {
		//Output error
		std::cout << "An error occured during compression of asset " << filename << ". Reason: " << e.what() << "\n";
	}
}

void decompressFile(std::string archiveName, std::string filename, std::string destPath) {
	try {
		//Decompress file
		ZipFile::ExtractFile(archiveName, filename, destPath);
	}
	catch (std::exception& e) {
		//Output error
		std::cout << "An error occured during extraction of asset " << filename << ". Reason: " << e.what() << "\n";

		//If archive name is "-1"
		if (substring(archiveName, archiveName.size() - 2, archiveName.size()) == "-1") {
			//Delete archive
			deleteFile(archiveName);
		}

		//Delete ghost file
		deleteFile(destPath);
	}
}
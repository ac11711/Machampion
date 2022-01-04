#pragma once
#include <string>
#include <vector>

//A file containing functions that deal with compression/decompression

std::string substring(std::string str, int start, int end);
std::vector<std::string> parsePath(std::string path);
std::string getArchiveNameFromParsedPath(std::vector<std::string> parsedPath);
void deleteFile(std::string filename);
void compressFile(std::string archiveName, std::string filename);
void decompressFile(std::string archiveName, std::string filename, std::string destPath);
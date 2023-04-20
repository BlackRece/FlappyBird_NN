#pragma once
#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonParser
{
public:
	template <typename t>
	static void LoadJson(std::vector<t>& structData, std::string inputfile);
	template <typename t>
	static void SaveJson(std::vector<t> structData, std::string inputfile);
	template <typename t>
	static void LoadJson(t& structData, std::string inputfile);
	template <typename t>
	static void SaveJson(t structData, std::string inputfile);
};

//Dont put these in the cpp, it will brake
template<typename t>
static void JsonParser::LoadJson(std::vector<t>& structData, std::string inputfile)
{
	try
	{
		std::ifstream jsonFile(inputfile);
		json jsonData = json::parse(jsonFile);
		structData = jsonData;
	}
	catch (const std::exception) {}
}

template<typename t>
static void JsonParser::SaveJson(std::vector<t> structData, std::string outputfile)
{
	try
	{
		json jsonData = structData;
		std::ofstream o(outputfile);
		o << jsonData.dump(2) << std::endl;
		//o << std::setw(4) << jsonData << std::endl;
	}
	catch (const std::exception) {}
}

template<typename t>
static void JsonParser::LoadJson(t& structData, std::string inputfile)
{
	try
	{
		std::ifstream jsonFile(inputfile);
		json jsonData = json::parse(jsonFile);
		structData = jsonData;
	}
	catch (const std::exception) {}
}

template<typename t>
static void JsonParser::SaveJson(t structData, std::string inputfile)
{
	try
	{
		json jsonData = structData;
		std::ofstream o(inputfile);
		o << jsonData.dump(2) << std::endl;
		//o << std::setw(4) << jsonData << std::endl;
	}
	catch (const std::exception) {}
}

/*
* example usage
* note: can be nested in another struct
struct ExampleJson
{
	float aFloat;
	int anInt;
	char aChar;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ExampleJson, aFloat, anInt, aChar);
*/
#endif
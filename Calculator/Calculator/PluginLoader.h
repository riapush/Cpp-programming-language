#pragma once
#include<iostream>
#include<map>
#include<Windows.h>

using unary = double(*)(double);
using binary = double(*)(double, double);

class PluginLoader {
	std::string path = "D:\\Git\\GitHub\\C---programming-language\\Calculator\\Calculator\\plugins\\";
	void loadDll();
	std::map<std::string, binary> binary_map;
	std::map<std::string, unary> unary_map;

public:
	PluginLoader();
	void addFunction(std::string const& name);
	double operation(double const& a, double const& b, std::string const& name);
	bool binary_contains(std::string const& name);
	bool unary_contains(std::string const& name);
};

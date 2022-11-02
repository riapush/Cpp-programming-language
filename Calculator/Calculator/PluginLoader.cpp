#include "PluginLoader.h"

PluginLoader::PluginLoader() {
	loadDll();
}

void PluginLoader::loadDll() {
	WIN32_FIND_DATAA find;

	HANDLE const hFind = FindFirstFileA((path+std::string("*.dll")).c_str(), &find);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string dllFileName(find.cFileName);
			addFunction(dllFileName);
		} while (NULL != FindNextFileA(hFind, &find));

		FindClose(hFind);
	}
	else { ;
        throw std::exception();
    }
    std::cout << "\n";
}

void PluginLoader::addFunction(std::string const& name) {
	HMODULE hm = LoadLibraryA((path+name).c_str());

	if (!hm)
		throw std::exception();

	std::string function = name.substr(0, name.find(".dll"));
    function.erase(0,3);
    std::cout << ", " << function;

	unary ufunc = (unary)GetProcAddress(hm, "unary");

	if (!ufunc) {
		binary bfunc = (binary)GetProcAddress(hm, "binary");
		if (!bfunc)
			throw std::exception();

		binary_map.insert(std::pair<std::string, binary>(function, bfunc));
	}
	else
		unary_map.insert(std::pair<std::string, unary>(function, ufunc));
}

double PluginLoader::operation(double const& a, double const& b, std::string const& name) {
	if (unary_map.contains(name))
		return unary_map[name](a);
	else if (binary_map.contains(name))
		return binary_map[name](a, b);
}

bool PluginLoader::binary_contains(std::string const& name) {
	return binary_map.contains(name);
}

bool PluginLoader::unary_contains(std::string const& name) {
	return unary_map.contains(name);
}
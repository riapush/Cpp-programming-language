//
// Created by Valer on 26.11.2022.
//

#include "wrapper.h"


int Wrapper::execute_func(const std::map<const std::string, int>& args) {
    std::vector<int> arg_vec;
    for (auto& pair : args) {
        if (_args.find(pair.first) == args.end())
            throw std::exception(); // no argument with this name
        _args.find(pair.first)->second = pair.second;
    }
    for (auto& pair : _args){
        arg_vec.push_back(pair.second);
    }
    int result;
    try {
        result = function(arg_vec);
    }
    catch (std::exception& ex){
        std::cout << ex.what();
    }
    return result;
}
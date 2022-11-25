//
// Created by Valer on 26.11.2022.
//
#pragma once
#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>
#include <map>
#include <vector>
#include <functional>

#ifndef ENGINE_WRAPPER_H
#define ENGINE_WRAPPER_H


class Wrapper {
    std::map<std::string, int> _args;
    std::function<int(std::vector<int>& argumentsVector)> function;
public:
    Wrapper() = delete; // default constructor is banned
    ~Wrapper() = default;

    template <typename Obj, typename... Args>
    Wrapper(Obj* obj, int(Obj::*command)(Args...), const std::map<const std::string, int>& args) {
        if(!obj || !command) {
            throw std::exception(); // nullptr instead of a method or object
        }

        for (auto arg:args){
            if (args.count(arg.first) > 1) { //
                _args.clear();
                throw std::exception(); // two arguments with the same name
            }
            _args.insert(arg);
        }

        function = [obj, command, this](std::vector<int>const& arg_vec) {
            if (sizeof...(Args) != arg_vec.size()) {
                throw std::exception(); // invalid number of arguments
            }
            return caller(obj, command, arg_vec, std::make_index_sequence<sizeof...(Args)>{});
        };
    }

    template<typename Obj, typename func, size_t... index>
    int caller(Obj* obj, func command,std::vector<int>const& arg_vec, std::index_sequence<index...>){
        return (obj->*command)(arg_vec[index]...);
    }

    int execute_func(const std::map<const std::string, int>& args);
};



#endif //ENGINE_WRAPPER_H

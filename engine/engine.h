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
#include "wrapper.h"

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H


class Engine {
private:
    std::map <const std::string, Wrapper*> commands;
public:
    void register_command(Wrapper* wrapper, const std::string& command);
    int execute(const std::string &command, const std::map<const std::string, int>& args);
};


#endif //ENGINE_ENGINE_H

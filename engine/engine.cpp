//
// Created by Valer on 26.11.2022.
//
#include <iostream>
#include "engine.h"

void Engine::register_command(Wrapper* wrapper, const std::string& command){
    if (!wrapper){
        throw std::exception(); // no command given to register
    }
    if (commands.find(command) != commands.end()){
        throw std::exception(); //command with this name already exists
    }
    else {
        commands.insert({command, wrapper});
    }
}

int Engine::execute(const std::string &command, const std::map<const std::string, int>& args) {
    auto it = commands.find(command);
    if (it == commands.end()){
        throw std::exception(); // no command with this name exists
    }
    Wrapper* wrapper = (*it).second;
    return wrapper->execute_func(args);
}
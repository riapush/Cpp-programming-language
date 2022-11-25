#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>
#include <map>
#include <vector>
#include <functional>

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

    int execute_func(const std::map<const std::string, int>& args){
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
};

class Engine {
private:
    std::map <const std::string, Wrapper*> commands;
public:
    void register_command(Wrapper* wrapper, const std::string& command);
    int execute(const std::string& command, const std::map<const std::string,int>& args);
};

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

class Test {
public:
    Test()=default;
    int f3(int a, int b){
        return a+b;
    }
};

int main() {
    Test* test = new Test();
    Wrapper wrapper(test,&Test::f3, {{"arg1", 0}, {"arg2", 0}});
    Engine engine;
    engine.register_command(&wrapper, "command1");
    std::cout << engine.execute("command1", { {"arg1", 4}, {"arg2", 5} }) << std::endl;
    return 0;
}

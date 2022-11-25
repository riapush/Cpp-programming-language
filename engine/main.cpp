#include <iostream>
#include <exception>
#include "engine.h"
#include "wrapper.h"

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

#include <cmath>

extern "C" __declspec(dllexport)
double unary(double x) {
    return cos(x);
}
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include "combine.hpp"

double rand_double(double x)
{
    return x + 20;
}

template<typename Engine>
auto createRandomFunction(Engine &engine)
{
    auto rand0 = calls::create([&](){
        std::uniform_real_distribution<> dist(0,1);
        return dist(engine);
    });

    auto rand1 = calls::create([&](int max) {
        std::uniform_int_distribution<> dist(0, max);
        return dist(engine);
    });

    auto rand2 = calls::create([&](int min, int max)->int {
        if(min > max)
            std::swap(min,max);

        std::uniform_int_distribution<> dist(min, max);
        return dist(engine);
    });

    auto double_rand = calls::create([&](double min, double max)-> double {
        std::uniform_real_distribution<> dist(min,max);
        return dist(engine);
    });

    return rand0 + rand1 + rand2 + double_rand + calls::create(rand_double);
}


int main()
{
    std::mt19937 engine(std::time(NULL));

    auto rand = createRandomFunction(engine);

    const auto endl = '\n';
    std::cout << rand(100) << endl;
    std::cout << rand(1, 100) << endl;
    std::cout << rand() << endl;
    std::cout << rand(double(20.4)) << endl;
    std::cout << rand(double(-100), double(100.0)) << endl;

    return 0;
}

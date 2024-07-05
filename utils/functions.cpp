#include "functions.hpp"
#include <random>

float random(const float range) { //0 -> HI
    std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_real_distribution num{ 0.f, range };
    return num(mt);

}
float random(const float min, const float range) { //LO -> HI
    std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_real_distribution num{ min, range };
    return num(mt);
}
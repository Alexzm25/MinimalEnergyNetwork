#pragma once
#include <iostream>

using namespace std;

class Sensor {
private:   
    float posX, posY;
    string name;

public:
    Sensor() : posX(0), posY(0), name("") {}
    Sensor(float x, float y, string n)
        : posX(x), posY(y), name(n) {
    }
    float getX() const { return posX; }
    float getY() const { return posY; }
    string getName() const { return name; }
};

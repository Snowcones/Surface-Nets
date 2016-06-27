//
//  main.cpp
//  Example Code
//

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

#include "graphics.hpp"
#include "surface.hpp"

float sphereRZ(float t, float z)
{
    return sqrtf(1 - z*z);
}

float sphereRPhi(float t, float phi)
{
    return 1;
}

float otherSphereRPhi(float t, float phi)
{
    return 2*cosf(t)*cosf(phi);
}

float catenoidR(float t, float z)
{
    return coshf(z);
}

int main(int argc, const char * argv[]) {
    
    static graphics g;
    Surface s;
    s.func = sphereRZ;
    s.parameterization = R_OF_Z_THETA;
    g.drawSurface(s);
    
    return 0;
}

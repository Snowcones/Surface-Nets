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

float offsetSphereRPhi(float t, float phi)
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
    s.func = offsetSphereRPhi;
    s.parameterization = R_OF_PHI_AND_THETA;
    g.drawSurface(s);
    
    return 0;
}

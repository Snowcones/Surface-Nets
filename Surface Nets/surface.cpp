//
//  surface.cpp
//  A class for managing a parametric surface
//


#include "surface.hpp"
#include <math.h>
#include <stdlib.h>

float Surface::dRdZ(float theta, float z)
{
    const float dz = .001;
    return (r(theta, z + dz) - r(theta, z - dz)) / (2 * dz);
}

float Surface::dRdPhi(float theta, float phi)
{
    const float dPhi = .001;
    return (r(theta, phi + dPhi) - r(theta, phi - dPhi)) / (2 * dPhi);
}

float Surface::dRdTheta(float theta, float v)
{
    const float dt = .001;
    return (r(theta + dt, v) - r(theta - dt, v)) / (2 * dt);

}

float Surface::arcLengthDZ(float theta, float z, float dZ)
{
    return dZ * sqrtf(1 + powf(dRdZ(theta, z), 2.0));
}

float Surface::arcLengthDPhi(float theta, float phi, float dPhi)
{
    return dPhi * sqrt(powf(r(theta, phi), 2.0) + powf(dRdPhi(theta, phi), 2.0));
}

float Surface::arcLengthDTheta(float theta, float v, float dTheta)
{
    if (parameterization == R_OF_Z_AND_THETA)
    {
        float z = v;
        return dTheta * sqrtf(powf(r(theta, z), 2.0) + powf(dRdTheta(theta, z), 2.0));
    }
    else if (parameterization == R_OF_PHI_AND_THETA)
    {
        float phi = v;
        return dTheta * sqrtf(powf(r(theta, phi) * cosf(phi), 2.0) + powf(dRdTheta(theta, phi), 2.0));
    }
    else
    {
        return 0;
    }
}

void Surface::createPlotList(float **segmentWidths, float **arcLengths, float **horizontalSpacing, float **verticalSpacing, int vSteps, int tSteps)
{
    if (vBoundsSet)
    {
        createPlotList(segmentWidths, arcLengths, horizontalSpacing, verticalSpacing, minV, maxV, vSteps, tSteps);
    }
    else
    {
        if (parameterization == R_OF_Z_AND_THETA)
        {
            createPlotList(segmentWidths, arcLengths, horizontalSpacing, verticalSpacing, -1.0, 1.0, vSteps, tSteps);
        }
        else if (parameterization == R_OF_PHI_AND_THETA)
        {
            createPlotList(segmentWidths, arcLengths, horizontalSpacing, verticalSpacing, -M_PI / 2, M_PI / 2, vSteps, tSteps);
        }
    }
}

float Surface::r(float t, float v)
{
    float r = func(t, v);
    if (r != r)
    {
        r = 0;
    }
    return r;
}

void Surface::createPlotList(float **segmentWidths, float **arcLengths, float **horizontalSpacing, float **verticalSpacing, float vStart, float vEnd, int vSteps, int tSteps)
{
    if (vSteps%2 == 1)
    {
        vSteps++;
        printf("Can't have an odd number (%d) of vertical steps, using %d vertical steps instead\n", vSteps-1, vSteps);
    }
    
    minArcLength = 0;
    maxArcLength = 0;
    
    *segmentWidths = (float*)malloc(sizeof(float) * 2 * (vSteps + 1) * tSteps);
    *arcLengths = (float*)malloc(sizeof(float) * (vSteps + 1) * tSteps);
    
    *horizontalSpacing = (float*)malloc(sizeof(float) * 2*tSteps);
    *verticalSpacing   = (float*)malloc(sizeof(float) * tSteps);
    
    for (int t=0; t<tSteps; t++)
    {
        float theta = 2 * M_PI / tSteps * t;
        float nextTheta = 2 * M_PI / tSteps * (t + 1);
        float dTheta = nextTheta - theta;
        
        float currLength = 0;
        float vStep = (vEnd - vStart) / vSteps;
        for (int s = 0; s < vSteps + 1; s++)
        {
            float v = vStep * s + vStart;
            (*segmentWidths)[(vSteps+1)*2*t + 2*s]     = arcLengthDTheta(theta, v, dTheta/2);
            (*segmentWidths)[(vSteps+1)*2*t + 2*s + 1] = arcLengthDTheta(nextTheta, v, dTheta/2);
            
            if (parameterization == R_OF_PHI_AND_THETA)
            {
                currLength += arcLengthDPhi(theta, v, vStep);
            }
            else if (parameterization == R_OF_Z_AND_THETA)
            {
                currLength += arcLengthDZ(theta, v, vStep);
            }
            
            if (fabsf(v) < vStep / 10)
            {
                (*verticalSpacing)[t] = currLength;
            }
            
            (*arcLengths)[(vSteps+1)*t + s] = currLength;
        }
        
        float arcLengthAtMiddle = (*arcLengths)[(vSteps+1)*t + vSteps/2];
        float arcLengthAtEnd = (*arcLengths)[(vSteps+1)*t + vSteps];
        float minLength = 0 - arcLengthAtMiddle;
        float maxLength = arcLengthAtEnd - arcLengthAtMiddle;
        
        if (minLength < minArcLength)
        {
            minArcLength = minLength;
        }
        if (maxArcLength > maxLength)
        {
            maxArcLength = maxLength;
        }
    }
    getSpacing(horizontalSpacing, *segmentWidths, *arcLengths, vSteps, tSteps);
}

void Surface::getSpacing(float** spacing, float *segmentWidths, float *arcLengths, int vSteps, int tSteps)
{
    maxCircumference = 0;
    for (int t=0; t<tSteps; t++)
    {
        float maxLeftWidth = 0;
        float maxRightWidth = 0;
        for (int v=0; v<vSteps + 1; v++)
        {
            float leftWidth  = segmentWidths[(vSteps+1)*2*t + 2*v];
            float rightWidth = segmentWidths[(vSteps+1)*2*t + 2*v + 1];
            if (leftWidth > maxLeftWidth)
            {
                maxLeftWidth = leftWidth;
            }
            if (rightWidth > maxRightWidth)
            {
                maxRightWidth = rightWidth;
            }
        }
        (*spacing)[2*t]     = maxLeftWidth;
        (*spacing)[2*t + 1] = maxRightWidth;
        
        maxCircumference += maxLeftWidth + maxRightWidth;
    }
}

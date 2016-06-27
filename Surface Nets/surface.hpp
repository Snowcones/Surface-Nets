//
//  surface.hpp
//  A class for managing a parametric surface
//

#ifndef surface_hpp
#define surface_hpp

typedef enum : int {
    R_OF_PHI_THETA,
    R_OF_Z_THETA,
} parameterizationType;

#include <stdio.h>

class Surface {
private:
    
    float dRdZ(float theta, float z);
    float dRdPhi(float theta, float phi);
    float dRdTheta(float theta, float z);
    float arcLengthDZ(float theta, float z, float dz);
    float arcLengthDPhi(float theta, float phi, float dPhi);
    float arcLengthDTheta(float theta, float phi, float dphi);
    void getSpacing(float** spacing, float* segmentWidths, float* arcLengths, int vSteps, int tSteps);
    float r(float t, float v);
    
public:
    float minArcLength;
    float maxArcLength;
    float maxCircumference;
    
    bool vBoundsSet;
    float minV;
    float maxV;
    
    parameterizationType parameterization = R_OF_Z_THETA;
    float (*func)(float, float) = NULL;
    
    void createPlotList(float** segmentWidths, float** arcLengths, float **horizontalSpacing, float **verticalSpacing, int vSteps, int tSteps);
    void createPlotList(float** segmentWidths, float** arcLengths, float **horizontalSpacing, float **verticalSpacing, float vStart, float vEnd, int vSteps, int tSteps);
};

#endif /* surface_hpp */

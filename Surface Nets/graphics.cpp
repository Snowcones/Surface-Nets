//
//  graphics.cpp
//  A class for drawing parametric surfaces
//

#include "graphics.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

const int screenWidth = 1440;
const int screenHeight = 900;

void graphics::drawSurface(Surface c)
{
    
    float* segmentWidths;
    float* arcLengths;
    float* horizontalSpacings;
    float* verticalSpacings;
    
    const int lattitudeSteps = 200;
    const int longitudeSteps = 30;
    
    c.createPlotList(&segmentWidths, &arcLengths, &horizontalSpacings, &verticalSpacings, lattitudeSteps, longitudeSteps);
    
    float netHeight = c.maxArcLength - c.minArcLength;
    float netWidth  = c.maxCircumference;
    
    float scaleFactor;
    if (netWidth/netHeight > (float)screenWidth/screenHeight)
    {
        //Net size will be limited by window width
        scaleFactor = screenWidth / netWidth;
    }
    else
    {
        //Net size will be limited by window height
        scaleFactor = screenHeight / netHeight;
    }
    
    std::vector<std::vector<sf::Vertex>> leftEdgeVec;
    std::vector<std::vector<sf::Vertex>> rightEdgeVec;
    
    float xToCenter = horizontalSpacings[0];
    for (int t=0; t<longitudeSteps; t++)
    {
        std::vector<sf::Vertex> leftEdge;
        std::vector<sf::Vertex> rightEdge;
        for (int v=0; v<lattitudeSteps + 1; v++)
        {
            float xOfLeftPoint  = segmentWidths[(lattitudeSteps+1)*2*t + 2*v];
            float xOfRightPoint = segmentWidths[(lattitudeSteps+1)*2*t + 2*v + 1];
            float yOfPoints = arcLengths[(lattitudeSteps+1)*t + v] - verticalSpacings[t];
            yOfPoints -= c.minArcLength;
            
            float leftPos  = xToCenter - xOfLeftPoint;
            float rightPos = xToCenter + xOfRightPoint;
            
            leftEdge.push_back(sf::Vertex(scaleFactor * sf::Vector2f(leftPos, yOfPoints), sf::Color::Red)); //Ugly syntax but it looks like it's the "SFML way" to do it
            rightEdge.push_back(sf::Vertex(scaleFactor * sf::Vector2f(rightPos, yOfPoints), sf::Color::Blue));
        }
        xToCenter += horizontalSpacings[2*t + 1];
        xToCenter += horizontalSpacings[2*((t+1)%lattitudeSteps)];
        
        leftEdgeVec.push_back(leftEdge);
        rightEdgeVec.push_back(rightEdge);
    }
    
    
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Curve Net!");
    window.setVerticalSyncEnabled(true);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::White);
        for (int i=0; i<leftEdgeVec.size(); i++)
        {
            window.draw(&leftEdgeVec[i][0], leftEdgeVec[i].size(), sf::LinesStrip);
            window.draw(&rightEdgeVec[i][0], rightEdgeVec[i].size(), sf::LinesStrip);
        }
        window.display();
    }
}
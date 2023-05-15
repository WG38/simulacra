#ifndef HELPERFUNC_H
#define HELPERFUNC_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

float Distance(float x1,float y1,float x2,float y2);

void keyPressTimeFreeze();

sf::Vertex* DrawLine(sf::Vector2f start, sf::Vector2f end, float thickness);


sf::Vector2f getClosestPoint(float x1, float y1, float x2, float y2,float a , float b );

int zoneCalculation(float x1, float y1, float x2, float y2,float a , float b);

float dist(float x1, float y1,float x2,float y2,float x3,float y3);


#endif
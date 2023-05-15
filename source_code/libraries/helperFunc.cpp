#include <chrono>
#include <math.h>
#include <cmath>
#include <thread>
#include <iostream>
#include "helperFunc.h"




float Distance(float x1,float y1,float x2,float y2) {
  float distance = pow(pow(x1-x2,2) + pow(y1-y2,2),0.5);
  return distance; 
}


void keyPressTimeFreeze() {
  std::chrono::milliseconds timespan(100); 
  std::this_thread::sleep_for(timespan);
}

sf::Vertex* DrawLine(sf::Vector2f start, sf::Vector2f end, float thickness)
{   
  sf::Vector2f dv = end - start;
  float dl = std::sqrt(dv.x * dv.x + dv.y * dv.y);
  sf::Vector2f uv = dv / dl;
  sf::Vector2f up = sf::Vector2f(-uv.x,uv.y);
  sf::Vector2f offset = up * (thickness / 2.f);

  sf::Vertex* vertexArray = new sf::Vertex[4];
  vertexArray[0] = sf::Vertex(start+offset,sf::Color(0,240,240,255));
  vertexArray[1] = sf::Vertex(end+offset,sf::Color(0,240,240,255));
  vertexArray[2] = sf::Vertex(end-offset,sf::Color(0,240,240,255));
  vertexArray[3] = sf::Vertex(start-offset,sf::Color(0,240,240,255));
  return vertexArray;

            
}
//(x1,y1) -> one node , (x2,y2) -> second node , (a,b) -> center of the blob
sf::Vector2f getClosestPoint(float x1, float y1, float x2, float y2,float a , float b ) {
  if (x1 == x2) { return sf::Vector2f(x1,b);}
  if (y1 == y2) { return sf::Vector2f(a,y1);}

  float m1 = (y2 - y1)/(x2 - x1);
  float m2 = -1/m1;
  float x = (m1*x1 - m2*a + b-y1) / (m1 - m2);
  float y = m2*(x-a)+b;
  return sf::Vector2f(x,y);

}
int zoneCalculation(float x1, float y1, float x2, float y2,float a , float b) {
  float f = (-x1 + y2)/(y1 - y2 + 0.00001)*a + (pow(x1,2) + pow(y1,2) - x1*y2 - y1*y2)/(y1 - y2 + 0.00001);
  float g = (-x2 + y1)/(y2 - y1 + 0.00001)*a + (pow(x2,2) + pow(y2,2) - x2*y1 - y2*y1)/(y2 - y1 + 0.00001);
  //std::cout << f << " " << g << " " << b << std::endl;
  if (b > f && b < g) { return 0;}
  else {return 1; }//std::cout << "YES";
}
float dist(float x1, float y1,float x2,float y2,float x3,float y3) {
  float px = x2-x1;
  float py = y2-y1;

  float norm = px*px + py*py;

  float u =  ((x3 - x1) * px + (y3 - y1) * py) / norm;

  if (u > 1){
    u = 1;
  }
  if (u < 0) {
    u = 0;
  }
  float x = x1 + u * px;
  float y = y1 + u * py;

  float dx = x - x3;
  float dy = y - y3;

  float dist = pow((dx*dx + dy*dy),0.5);
  return dist;
  //https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment/4165840#4165840

}
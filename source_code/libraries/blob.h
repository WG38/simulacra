#ifndef BLOB_H
#define BLOB_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Blob {
 public: // you need to add this to declare which elements can be accessed outside the class
  int ident = 0;
  bool isAnchor{false};
  bool isMagnetized{false};
  //std::vector<int> stickedIdents;
  //
  sf::Vector2f pos;
  sf::Vector2f oldPos;

  sf::Vector2f vel;
  sf::Vector2f acc;
  float m = 1;
  float radius{10};
  sf::Color color;
  std::vector<int> stickIdents;
  sf::Vector2f TEST;
};

class Stick {
 public:
  int ident = 0;
  std::vector<int> blobIdents;
  float length;

};

class Node {
  public:
    int ident = 0;
    sf::Vector2f pos; 
    
  
};
class NodeStruct {
  public:
    int nOfNodes;
    std::vector<Node> vecOfNodes;
};

/*
class BlobStickStructure {
  public:
    int nOfBlobs = 0;
    int nOfSticks = 0;
    std::vector<int> blobIdents;
    std::vector<int> stickIdents;
    bool isVisible{true};
};
*/

#endif
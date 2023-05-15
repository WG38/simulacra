#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <vector>
#include "blob.h"
#include <iostream>
#include <fstream>
#include "saveload.h"

std::vector<int> bIDsVec;
std::vector<int> sIDsVec;

void CreateStructure(int initialBlobId,std::vector<Blob>& bArray,std::vector<Stick>& sArray);
void saveStructure(std::vector<std::tuple<std::vector<int>,std::vector<int>>>& savingStructure,
std::vector<Blob>& bVec,std::vector<Stick>& sVec,int structureID);
std::tuple<std::vector<Blob>,std::vector<sf::CircleShape>,std::vector<Stick>> loadStructure(int nOfBlobs,int nOfSticks);


#endif
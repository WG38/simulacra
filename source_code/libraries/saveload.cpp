#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <tuple>
#include <vector>
#include <string.h>
#include <fstream>
#include <iostream>
#include "blob.h"
#include "saveload.h"


//save all the parameters. The arguments will be nOfBlobs and sticks,time state,container state,timestamp,magneticsState and of coruse
// vectors containing the blob,stick data and containerNode data 

void saveFile(int nBlobs,int nSticks,int nContainers,bool timeFreeze,bool mCheck,bool container,float currTime,
std::vector<Blob>& blobs,std::vector<Stick>& sticks,std::vector<NodeStruct> containers) {
 std::string outputStr;
 //load constants
 std::string nBStr = std::to_string(nBlobs) + ";\n";
 std::string nSStr = std::to_string(nSticks) + ";\n";
 std::string nCStr = std::to_string(nContainers) + ";\n";
 std::string TFStr = std::to_string(timeFreeze) + ";\n";
 std::string MCStr = std::to_string(mCheck) + ";\n";
 std::string CStr = std::to_string(container) + ";\n";
 std::string CFStr = std::to_string(currTime) + ";\n";
 //blobs
 std::string masterBlobStr = "";
 std::string masterStickStr = "";
 std::string masterContainerStr = "";
 
 for (int i=0;i<nBlobs;i++) {
  Blob cb = blobs[i];
  std::string currStr = "blob {" + std::to_string(cb.ident) + ";" + std::to_string(cb.isAnchor) + ";" + std::to_string(cb.isMagnetized) + 
  ";" + std::to_string(cb.radius) + ";" + std::to_string(cb.pos.x) + ";" + std::to_string(cb.pos.y) + ";" + std::to_string(cb.oldPos.x)
  + ";" + std::to_string(cb.oldPos.y) + ";" + std::to_string(cb.vel.x) + ";" + std::to_string(cb.vel.y) + ";" + std::to_string(cb.acc.x) + 
  ";" + std::to_string(cb.acc.y) + ";};\n";
  masterBlobStr.append(currStr);
 }
 
 for (int i=0;i<nSticks;i++) {
  Stick sb = sticks[i];
  std::string currStr = "stick {"+std::to_string(sb.blobIdents[0]) + ";"+std::to_string(sb.blobIdents[1]) + ";"+std::to_string(sb.length) + ";};\n";
  masterStickStr.append(currStr);
 }
 for (int i=0;i<nContainers;i++) {
  NodeStruct nb = containers[i];
  std::string currStr = "cont {"+std::to_string(nb.nOfNodes) + ";";
  
  for (int j=0;j<nb.nOfNodes;j++) {
    std::string loopStr = std::to_string(nb.vecOfNodes[j].ident)+ ";" + std::to_string(nb.vecOfNodes[j].pos.x) + ";"
    + std::to_string(nb.vecOfNodes[j].pos.y) + ";";
    currStr.append(loopStr);
    
  }
  currStr.append("};\n");
  masterContainerStr.append(currStr);
  
  
 }
 //add the containers
 std::cout << masterStickStr << std::endl;
 //+ masterBlobStr 
 std::string masterString = nBStr + nSStr + nCStr + TFStr + MCStr + CStr + CFStr + masterBlobStr +  masterStickStr + masterContainerStr;
 std::ofstream outfile ("sim_save.txt");
 outfile << masterString;

}

std::string updateStr(std::string currStr,std::size_t LSM) {
          std::string outStr = currStr.substr(LSM+1); //this may not work
          //std::cout << outStr << std::endl;
          return outStr;
}

std::tuple<int,int,int,bool,bool,bool,float,std::vector<Blob>,std::vector<sf::CircleShape>,std::vector<Stick>,std::vector<NodeStruct>> fileLoad() {
      std::fstream savefile;
      
      savefile.open("sim_save.txt",std::ios::in);
      if(savefile.is_open()) {
        //basic validity check, count number of lines. If less than 7 throw an exception
        int lineN = 0;
        std::string line;
        //while (std::getline(savefile,line)) ++lineN ;
        //if (lineN < 7) {std::cout << "Invalid File" << std::endl;}

        //actually load the stuff 
        lineN = 0;
        std::string line2;
        std::vector<std::string> outStrVec;
        while (std::getline(savefile,line2)) {
          //add all the lines in the file to a string vec
          
          
          outStrVec.resize(lineN+1);
          outStrVec[lineN] = line2;
          //std::cout << outStrVec[lineN] << std::endl;
          lineN++;

        }
        
        //assign new sim params
        int nBlobs = std::stoi(outStrVec[0].erase(outStrVec[0].length()-1,1));
        int nSticks = std::stoi(outStrVec[1].erase(outStrVec[1].length()-1,1));
        int nContainers = std::stoi(outStrVec[2].erase(outStrVec[2].length()-1,1));
        bool timeFrozen = std::stoi(outStrVec[3].erase(outStrVec[3].length()-1,1));
        bool magneticsOn = std::stoi(outStrVec[4].erase(outStrVec[4].length()-1,1));
        bool containerOn = std::stoi(outStrVec[5].erase(outStrVec[5].length()-1,1));
        float newTime = std::stof(outStrVec[6].erase(outStrVec[6].length()-1,1));
        
        //std::cout << nBlobs << std::endl;
        //load the blobs
        std::vector<Blob> newBlobVec(nBlobs);
        std::vector<sf::CircleShape> newblobShapeVec(nBlobs); 
        std::vector<Stick> newStickVec(nSticks);
        std::vector<NodeStruct> newContainerVec(nContainers);

        
        for (int i=0;i < nBlobs;i++) {
         
         Blob newBlob;
         std::string currStr = outStrVec[7+i];
         std::cout << i << " " << currStr << std::endl;
         std::size_t latestSemiPos = currStr.find(';');
         newBlob.ident = std::stoi(currStr.substr(6,latestSemiPos-6));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         //pack this into a neat function later 
         newBlob.isAnchor = std::stoi(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.isMagnetized = std::stoi(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');
         
         newBlob.radius = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.pos.x = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.pos.y = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.oldPos.x = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.oldPos.y = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.vel.x = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.vel.y = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.acc.x = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.acc.y = std::stof(currStr.substr(0,latestSemiPos));
         currStr = updateStr(currStr,latestSemiPos);
         latestSemiPos = currStr.find(';');

         newBlob.color = sf::Color(0,230,255,255);

         newBlobVec[i] = newBlob;
      
         sf::CircleShape shape(10.f);
         shape.setFillColor(newBlobVec[i].color);
         shape.setOrigin(10.f,10.f);
         shape.setRadius(newBlobVec[i].radius);
         newblobShapeVec[i] = shape;
         
         
         
         //omgomgomgomg omg omg omg omg ^_^

         
        }
        

        
        for (int i=0;i < nSticks;i++) {
          Stick newStick;
          std::string currStr = outStrVec[7+nBlobs+i];
          
          std::size_t latestSemiPos = currStr.find(';');
          newStick.blobIdents.resize(2);
          newStick.blobIdents[0] = std::stoi(currStr.substr(7,latestSemiPos-7)); //those numbers are the number of chars between the start
          //of the string and the first ;
          std::cout << std::stoi(currStr.substr(7,latestSemiPos-7)) << std::endl;
          currStr = updateStr(currStr,latestSemiPos);
          latestSemiPos = currStr.find(';');
          
          
          //pack this into a neat function later 
          newStick.blobIdents[1] = std::stoi(currStr.substr(0,latestSemiPos));
          currStr = updateStr(currStr,latestSemiPos);
          latestSemiPos = currStr.find(';');

          newStick.length = std::stoi(currStr.substr(0,latestSemiPos));
          currStr = updateStr(currStr,latestSemiPos);
          latestSemiPos = currStr.find(';');

          newStickVec[i] = newStick;
          
         
        }
        for (int i=0;i < nContainers;i++) {
          NodeStruct newContainer;
          std::string currStr = outStrVec[7+nBlobs+nSticks+i];
          std::size_t latestSemiPos = currStr.find(';');
          
          newContainer.nOfNodes = std::stoi(currStr.substr(6,latestSemiPos-6));
          
          
          newContainer.vecOfNodes.resize(newContainer.nOfNodes);
          //now we unpack all the nodes
          
          for (int j=0;j < newContainer.nOfNodes;j++) {
            Node newNode;
            currStr = updateStr(currStr,latestSemiPos);
            latestSemiPos = currStr.find(';');
            newNode.ident = std::stoi(currStr.substr(0,latestSemiPos));
            currStr = updateStr(currStr,latestSemiPos);
            latestSemiPos = currStr.find(';');
            newNode.pos.x = std::stof(currStr.substr(0,latestSemiPos));
            currStr = updateStr(currStr,latestSemiPos);
            latestSemiPos = currStr.find(';');
            newNode.pos.y = std::stof(currStr.substr(0,latestSemiPos));
            std::cout << newNode.pos.x << " " << newNode.pos.y << std::endl;
            newContainer.vecOfNodes[j] = newNode;
            
          }
          newContainerVec[i] = newContainer;
          
        }
        
        
        //std::tuple<int,int,bool,bool,bool,float,std::vector<Blob>,std::vector<Stick>>
        return std::make_tuple(nBlobs,nSticks,nContainers,timeFrozen,magneticsOn,containerOn,newTime,newBlobVec,newblobShapeVec,newStickVec,newContainerVec);



      }
      else {
        exit(0);
      }
}

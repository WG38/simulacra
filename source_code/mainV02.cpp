#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <random>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/System/String.hpp>
#include "libraries/blob.h"
#include "libraries/helperFunc.h"
#include "libraries/saveload.h"
#include <tuple>








int nOfBlobs = 0;
int nOfSticks = 0;
int nOfbsStructs = 0;
int nOfContainers = 0;
int const wSize = 1000; // window size
std::vector<Blob> blobVec(0);
std::vector<Stick> stickVec(0);
std::vector<int> bIDsVec;
std::vector<int> sIDsVec;

std::vector<NodeStruct> containerVec; // vector that stores all the nodeStructs 


std::vector<std::tuple<std::vector<int>,std::vector<int>>> bsStructVec;
//std::vector<BlobStickStructure> bsStructVec(0);

void Simulation(float stepSize,std::vector<Blob>& bArray,std::vector<Stick>& sArray,float gAcc,int radius,bool containerState,std::vector<NodeStruct>& vecOfContainers) {
 int r = radius;
 sf::Vector2f center(0.0,0.0);
 int substeps = 2;
 float dtCalc = pow((stepSize/substeps),2);
 //std::cout << "hello?"  << std::endl;
 for (int j=0;j < substeps;j++) {
 for (Blob& cb : bArray){
   //std::cout << cb.pos[0]  << std::endl;
   //std::cout << cb.oldPos[0]  << std::endl;
   //std::cout << "############"  << std::endl;
   
     
     //verlet integration
     if (!cb.isAnchor) {
      cb.vel = cb.pos - cb.oldPos;
     }
     else {
      cb.vel = sf::Vector2f(0,0);
     }
     
     
     
     
     cb.oldPos = cb.pos;
     //std::cout << blobVec[nOfBlobs-1].oldPos.y << blobVec[nOfBlobs-1].pos.y << std::endl;
     //std::cout << cb.oldPos.y << cb.pos.y << std::endl;
     
     //std::cout << cb.acc.x << " " << cb.acc.y << std::endl;
     
     cb.pos = cb.pos + cb.vel + cb.acc*dtCalc;
     
     
     


     //std::cout << "#################" << std::endl;

     //clear acceleration
     cb.acc = sf::Vector2f(0,0);
     //add gravity
     if (!cb.isAnchor) {
      cb.acc.y = -gAcc;
     }
     
     /*
     if (containerState) {
      //bounding circle
      sf::Vector2f toObj(0,0);
      toObj = center - cb.pos;
      float dist = Distance(cb.pos.x,cb.pos.y,0,0);

      if (dist > r - cb.radius) {
        sf::Vector2f n = toObj / dist;

        cb.pos = center - n*(r-cb.radius);
        //std::cout << "NEW POS: ";
        //std::cout << cb.pos[0]  << std::endl;

      }
     }
     */

     //collision detection for the new containers
     
     for (NodeStruct& NodeStruct : vecOfContainers) {
      //loop thru all the the nodeStructs and check for collisions with all the blob connections
      if (NodeStruct.nOfNodes > 1) 
      {
        for (int m=1;m<NodeStruct.nOfNodes;m++) {
        
        sf::Vector2f n1 = NodeStruct.vecOfNodes[m].pos;
        sf::Vector2f n2 = NodeStruct.vecOfNodes[m-1].pos;
        //float distance = std::abs( (n2.x - n1.x)*(n1.y - cb.pos.y) - (n1.x - cb.pos.x)*(n2.y - n1.y ) ) / 
        std::sqrt( std::pow((n2.x - n1.x),2) + std::pow((n2.y - n1.y),2) );
        //std::cout << distance << std::endl;
        
        //zone calculation
        //zoneCalculation(float x1, float y1, float x2, float y2,float a , float b)
        //int zoneN;
        //if (n1.x <= n2.x) {
          //zoneN = zoneCalculation(n1.x,n1.y,n2.x,n2.y,cb.pos.x,cb.pos.y);
          
        //}
        //else {
          //zoneN = zoneCalculation(n2.x,n2.y,n1.x,n1.y,cb.pos.x,cb.pos.y);
        //}
        float distance = dist(n1.x,n1.y,n2.x,n2.y,cb.pos.x,cb.pos.y);
        if (distance < cb.radius) { //&& zoneN == 1) {
          //add collision based on blob-blob collision
          sf::Vector2f pointOnLine = getClosestPoint(n1.x,n1.y,n2.x,n2.y,cb.pos.x,cb.pos.y);
          sf::Vector2f collAxis = cb.pos - pointOnLine;
          float dist2 = Distance(collAxis.x,collAxis.y,0,0);
          if (dist2 < cb.radius) {
            sf::Vector2f m = collAxis / dist2;
            float delta = (cb.radius) - dist2;
            cb.pos = cb.pos + 0.5f*delta*m;
          }
          

       }
      }
      }
      
     }
     

     //collision detection and magnetic attraction detection in one sweep loop!
     for (Blob& kb : bArray) {
      
      
      if (kb.ident != cb.ident && Distance(cb.pos.x,cb.pos.y,kb.pos.x,kb.pos.y) < cb.radius+kb.radius)  {
       // std::cout << "DOES HAPPEN" << std::endl;
        sf::Vector2f collAxis = cb.pos - kb.pos;
        
        float dist2 = Distance(collAxis.x,collAxis.y,0,0);
        if (dist2 < cb.radius + kb.radius) {
         sf::Vector2f m = collAxis / dist2;
         float delta = (cb.radius + kb.radius) - dist2;
         
         cb.pos = cb.pos + 0.5f*delta*m;

         kb.pos = kb.pos - 0.5f*delta*m;

        }
      }
      
      if (kb.ident != cb.ident && kb.isMagnetized && cb.isMagnetized) {
        //magnetic attraction basically (or gravitational lol)
        float theta = 6.2832;

        float angle1 = (std::atan2(cb.pos.y-kb.pos.y,cb.pos.x-kb.pos.y) + theta);
        angle1 = fmod(angle1,theta);
        //angle2 = (math.atan2(k.y-j.y,k.x - j.x) + 6.2832) % 6.2832
        float angle2 = (std::atan2(kb.pos.y-cb.pos.y,kb.pos.x-cb.pos.y) + theta);
        angle2 = fmod(angle2,theta);
        float gForce = 100/Distance(cb.pos.x,cb.pos.y,kb.pos.x,kb.pos.y); //30 is an arbitrary constant lol

        
        cb.acc.x += -gForce*std::cos(angle1);
        cb.acc.y += -gForce*std::sin(angle1);
        kb.acc.x += -gForce*std::cos(angle2);
        kb.acc.y += -gForce*std::sin(angle2);
        //std::cout << cb.ident << std::endl;
        //std::cout << cb.acc.x << cb.acc.y << std::endl << kb.acc.x << kb.acc.y << std::endl << "##########" << std::endl;

      }
     }

   
 }
 for (Stick& stick : sArray) {
  
    //update the positions of the points bounded by the stick
    int blob0 = stick.blobIdents[0];
    int blob1 = stick.blobIdents[1];
    float dx = bArray[blob1].pos.x - bArray[blob0].pos.x;
    float dy = bArray[blob1].pos.y - bArray[blob0].pos.y;
    float dist = std::sqrt(dx*dx + dy*dy);
    float difference = stick.length - dist;
    float percent = difference / dist / 2;
    float offX = dx * percent;
    float offY = dy * percent;
    bArray[blob0].pos.x -= offX*dtCalc;
    bArray[blob0].pos.y -= offY*dtCalc;

    bArray[blob1].pos.x += offX*dtCalc;
    bArray[blob1].pos.y += offY*dtCalc;
  
  

 }
 }
}



void CreateStructure(int initialBlobId,std::vector<Blob>& bArray,std::vector<Stick>& sArray) { //,std::vector<int> bIDArray,std::vector<int> sIDArray) {
  //iterate thru all the sticks the blob is connected to
  std::cout << "Now iterating through Blobs" << initialBlobId << " sticks" << std::endl;
  for (int& stickID : bArray[initialBlobId].stickIdents) {
    //get the id of the blob that the stick connects to. Check if the new id is the first or second blob in the id vector
    std::vector<int> bIdents = sArray[stickID].blobIdents;
    int newBlobId;
    if (bIdents[0] != initialBlobId ) {newBlobId = bIdents[0]; }
    else {newBlobId = bIdents[1]; }

    //check if the new blob and the new stick are in the bIDArray / sIDArray. If yes print out "The blob and stick are saved"
    //if no save them and iterate through all the sticks connected to the blob
    bool bExists = false;
    if (std::find(bIDsVec.begin(),bIDsVec.end(),newBlobId) != bIDsVec.end()) {
      bExists = true;
      std::cout << "Blob " << newBlobId << " is already saved" << std::endl;
    }
    else {
     //save the blob 
     bIDsVec.resize(bIDsVec.size()+1);
     bIDsVec[bIDsVec.size()-1] = newBlobId;
     std::cout << "Blob " << newBlobId << " wasnt saved. Saving." << std::endl;
    }
    if (std::find(sIDsVec.begin(),sIDsVec.end(),stickID) != sIDsVec.end())
    {
      std::cout << "Stick " << stickID << " is already saved" << std::endl;
    }
    else {
      sIDsVec.resize(sIDsVec.size()+1);
      sIDsVec[sIDsVec.size()-1] = stickID;
      std::cout << "Stick " << stickID << " wasnt saved. Saving." << std::endl;
    }

    if (bExists) {
      std::cout << "Blob " << newBlobId << " was already saved. No recursion required." << std::endl;
    }
    else {
      std::cout << "Blob " << newBlobId << " wasnt saved. Recursing." << std::endl;
      CreateStructure(newBlobId,bArray,sArray); //,bIDArray,sIDArray);
    }

    

    



    
  }
  
  

  
}

void saveStructure(std::vector<std::tuple<std::vector<int>,std::vector<int>>>& savingStructure,
std::vector<Blob>& bVec,std::vector<Stick>& sVec,int structureID) {
  
  std::string outputStr;
  std::string masterBlobStr = "";
  std::string masterStickStr = "";
  std::vector<int> unpackedBlobVec = std::get<0>(savingStructure[structureID]);
  std::vector<int> unpackedStickVec = std::get<1>(savingStructure[structureID]);
  std::string nBStr = std::to_string(unpackedBlobVec.size()) + ";\n";
  std::string nSStr = std::to_string(unpackedStickVec.size()) + ";\n";

  for (int i=0;i<unpackedBlobVec.size();i++) {
  Blob cb = bVec[i];
  
  //loading up all the stick vectors stored in the blob 
  std::string stickStorageStr = "";
  //masterBlobStr.append(cb.stickIdents.size()+";"); //get size of the stick storage vector (this is getting so convoluted)
  for (int i=0;i<cb.stickIdents.size();i++) {
    stickStorageStr.append(std::to_string(cb.stickIdents[i]) + ";");
  }
  std::string currStr = "blob {" + std::to_string(cb.ident) + ";" + std::to_string(cb.isAnchor) + ";" + std::to_string(cb.isMagnetized) + 
  ";" + std::to_string(cb.radius) + ";" + std::to_string(cb.pos.x) + ";" + std::to_string(cb.pos.y) + ";" + std::to_string(cb.oldPos.x)
  + ";" + std::to_string(cb.oldPos.y) + ";" + std::to_string(cb.vel.x) + ";" + std::to_string(cb.vel.y) + ";" + std::to_string(cb.acc.x) + 
  ";" + std::to_string(cb.acc.y) + ";" + std::to_string(cb.stickIdents.size()) + ";" + stickStorageStr +"};\n";
  masterBlobStr.append(currStr);
 }
 
 for (int i=0;i<unpackedStickVec.size();i++) {
  Stick sb = sVec[i];
  std::string currStr = "stick {"+std::to_string(sb.blobIdents[0]) + ";"+std::to_string(sb.blobIdents[1]) + ";"+std::to_string(sb.length) + ";"+std::to_string(sb.ident)+"};\n";
  
  masterStickStr.append(currStr);
 }
 outputStr = nBStr + nSStr + masterBlobStr + masterStickStr;
 std::ofstream outfile ("structure.txt");
 outfile << outputStr;
}

std::tuple<std::vector<Blob>,std::vector<sf::CircleShape>,std::vector<Stick>> loadStructure(int nOfBlobs,int nOfSticks) {
  
  

  std::fstream savefile;
  savefile.open("structure.txt",std::ios::in);
  if(savefile.is_open()) {
    int lineN = 0;
    std::string line;

    std::string line2;
    std::vector<std::string> outStrVec;
    while (std::getline(savefile,line2)) {
      outStrVec.resize(lineN+1);
      outStrVec[lineN] = line2;
      //std::cout << outStrVec[lineN] << std::endl;
      lineN++;


    }

    int nBlobs = std::stoi(outStrVec[0].erase(outStrVec[0].length()-1,1));
    int nSticks = std::stoi(outStrVec[1].erase(outStrVec[1].length()-1,1));
    //std::cout << "Are we even here?" << std::endl;
    std::vector<Blob> newBlobVec(nBlobs);
    std::vector<sf::CircleShape> newblobShapeVec(nBlobs); 
    std::vector<Stick> newStickVec(nSticks);

    for (int i=0;i < nBlobs;i++) {
         
         Blob newBlob;
         std::string currStr = outStrVec[2+i];
         //std::cout << i << " " << currStr << std::endl;
         std::size_t latestSemiPos = currStr.find(';');
         newBlob.ident = nOfBlobs + std::stoi(currStr.substr(12,latestSemiPos-12));
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

        /**/
        int nOfSticksInTheStickVector = std::stoi(currStr.substr(0,latestSemiPos));
        currStr = updateStr(currStr,latestSemiPos);
        latestSemiPos = currStr.find(';');
        
        for (int j=0;j < nOfSticksInTheStickVector;j++) {
          newBlob.stickIdents.resize(newBlob.stickIdents.size()+1);
          newBlob.stickIdents[newBlob.stickIdents.size()-1] = nOfSticks + std::stoi(currStr.substr(0,latestSemiPos));
          currStr = updateStr(currStr,latestSemiPos);
          latestSemiPos = currStr.find(';');
        }
        

         

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
          std::string currStr = outStrVec[2+nBlobs+i];
          
          std::size_t latestSemiPos = currStr.find(';');
          newStick.blobIdents.resize(2);
          newStick.blobIdents[0] = nOfBlobs + std::stoi(currStr.substr(7,latestSemiPos-7));
          //std::cout << std::stoi(currStr.substr(7,latestSemiPos-7)) << std::endl;
          currStr = updateStr(currStr,latestSemiPos);
          latestSemiPos = currStr.find(';');
          
          
          //pack this into a neat function later 
          newStick.blobIdents[1] = nOfBlobs + std::stoi(currStr.substr(0,latestSemiPos));
          currStr = updateStr(currStr,latestSemiPos);
          latestSemiPos = currStr.find(';');

          newStick.length = std::stoi(currStr.substr(0,latestSemiPos));
          currStr = updateStr(currStr,latestSemiPos);
          latestSemiPos = currStr.find(';');

          newStick.ident = nOfSticks + std::stoi(currStr.substr(0,latestSemiPos));

          newStickVec[i] = newStick;
          
         
        }

    return std::make_tuple(newBlobVec,newblobShapeVec,newStickVec);
  
  }
  else {
        exit(0);
      }


}


int main()
{
    //basic constants
    float g = -0.9; //zero for now
    float dt = 0.01;
    
    double time;
    int r = 300;
    int blobN = 0;

    bool timeFreeze = false;
    bool isInfoOn = false;
    bool magneticsCheck = false;
    bool isContainerON = true;
    bool doBlobsRender = true;
    bool containerCreatorMode = false;
    
    std::vector<int> stickInitializerVector(0);
    
    sf::Vector2f cameraPos = sf::Vector2f(0,0);
    
    //objects for render

    sf::RenderWindow window(sf::VideoMode(wSize, wSize), "Phys Engine V05");

    std::vector<sf::CircleShape> blobShapeVec(nOfBlobs); 

    sf::CircleShape boundingCircle(r);
    boundingCircle.setFillColor(sf::Color::White);
    boundingCircle.setOrigin(r,r);
    boundingCircle.setPosition(wSize/2,wSize/2);
    


    //inputs
    sf::Mouse mouse; // mouse 
    sf::Keyboard keyboard;

    
    //text consts
    sf::Font standardFont;
    standardFont.loadFromFile("Raleway-Light.ttf");
    int textSize = 25;


    //texts
    sf::Text timeText;
    timeText.setCharacterSize(textSize);
    timeText.setFont(standardFont);
    timeText.setPosition(0,100);

    sf::Text fpsText;
    fpsText.setCharacterSize(textSize);
    fpsText.setFont(standardFont);
    fpsText.setPosition(0,160);

    sf::Text blobInfoText;
    blobInfoText.setCharacterSize(textSize);
    blobInfoText.setFont(standardFont);
    blobInfoText.setPosition(0,240);

    sf::Text rInfoText;
    rInfoText.setCharacterSize(textSize);
    rInfoText.setFont(standardFont);
    rInfoText.setPosition(0,350);



    while (window.isOpen()) {
     //get start time
     auto startTime = std::chrono::high_resolution_clock::now();
     

     sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
     

     //simulate next step
     if (!timeFreeze) {
      Simulation(dt,blobVec,stickVec,g,r,isContainerON,containerVec);
      time += static_cast<double>(dt);
     
      sf::String tString("Time: " + std::to_string(time));
      timeText.setString(tString);
     }
     
     //

     
     
     
     
     //#####INPUTS######

     //move camera (primary key listeners)
     bool isWClicked = keyboard.isKeyPressed(sf::Keyboard::Key::W);
     bool isAClicked = keyboard.isKeyPressed(sf::Keyboard::Key::A);
     bool isSClicked = keyboard.isKeyPressed(sf::Keyboard::Key::S);
     bool isDClicked = keyboard.isKeyPressed(sf::Keyboard::Key::D);
    
    //secondary key listeners 
     bool isLeftClicked = mouse.isButtonPressed(sf::Mouse::Button::Left);
     bool isRightClicked = mouse.isButtonPressed(sf::Mouse::Button::Right);
     bool isTClicked = keyboard.isKeyPressed(sf::Keyboard::Key::T);
     bool isMClicked = keyboard.isKeyPressed(sf::Keyboard::Key::M);
     bool isRClicked = keyboard.isKeyPressed(sf::Keyboard::Key::R);
     bool isGClicked = keyboard.isKeyPressed(sf::Keyboard::Key::G);
     bool isCClicked = keyboard.isKeyPressed(sf::Keyboard::Key::C);
     bool isUClicked = keyboard.isKeyPressed(sf::Keyboard::Key::U);
     bool isShiftClicked = keyboard.isKeyPressed(sf::Keyboard::Key::RShift);
     bool isCtrlClicked = keyboard.isKeyPressed(sf::Keyboard::Key::RControl);
     bool isYClicked = keyboard.isKeyPressed(sf::Keyboard::Key::Y);
     bool isLShiftClicked = keyboard.isKeyPressed(sf::Keyboard::Key::LShift);
     bool isLCtrlClicked = keyboard.isKeyPressed(sf::Keyboard::Key::LControl);

     //move camera without 4 if statemets
     //cameraPos.y += isWClicked*0.5;
     //cameraPos.y -= isSClicked*0.5;
     //cameraPos.x += isDClicked*0.5;
     //cameraPos.x -= isAClicked*0.5;


     //save simstate, load simstate
     if (isShiftClicked) {
      saveFile(nOfBlobs,nOfSticks,nOfContainers,timeFreeze,magneticsCheck,isContainerON,time,blobVec,stickVec,containerVec); 
      keyPressTimeFreeze();
     }
     if (isCtrlClicked) {
      //return std::make_tuple(nBlobs,nSticks,timeFrozen,magneticsOn,containerOn,newTime,newBlobVec,newblobShapeVec,newStickVec);
      //
      std::tuple<int,int,int,bool,bool,bool,float,std::vector<Blob>,std::vector<sf::CircleShape>,std::vector<Stick>,std::vector<NodeStruct>> outputTuple = fileLoad();
      
      nOfBlobs = std::get<0>(outputTuple);
      nOfSticks = std::get<1>(outputTuple);
      nOfContainers = std::get<2>(outputTuple);
      timeFreeze = std::get<3>(outputTuple);
      magneticsCheck = std::get<4>(outputTuple);
      isContainerON = std::get<5>(outputTuple);
      time = std::get<6>(outputTuple);
      blobVec = std::get<7>(outputTuple);
      blobShapeVec = std::get<8>(outputTuple);
      stickVec = std::get<9>(outputTuple);
      containerVec = std::get<10>(outputTuple);
      
      keyPressTimeFreeze();
     }


     if (isLeftClicked && !containerCreatorMode) {
      sf::Vector2i mouseCoords = mouse.getPosition(window);
      Blob newBlob;

      newBlob.pos.x = mouseCoords.x - wSize/2;
      newBlob.pos.y = mouseCoords.y - wSize/2;
      newBlob.oldPos.x = newBlob.pos.x;
      newBlob.oldPos.y = newBlob.pos.y;
      newBlob.ident = nOfBlobs;
      newBlob.color = sf::Color(0,230,255,255);
      newBlob.isMagnetized = magneticsCheck;
      if (isAClicked) {
        newBlob.isAnchor = true;
      }
      

      blobVec.resize(nOfBlobs + 1);
      blobShapeVec.resize(nOfBlobs + 1);
      blobVec[nOfBlobs] = newBlob;

      
      sf::CircleShape shape(10.f);
      shape.setFillColor(blobVec[nOfBlobs].color);
      shape.setOrigin(10.f,10.f);
      shape.setRadius(blobVec[nOfBlobs].radius);
      blobShapeVec[nOfBlobs] = shape;
       
      nOfBlobs++;
      //std::cout << nOfBlobs << std::endl;
      
      keyPressTimeFreeze();

      
     

     }
     

     //add another node to a container 
     if (isLeftClicked && containerCreatorMode) {
      //create new node - the container will be stored as a series of nodes. Each node 
      // will contain its position and the nodes 2 nodes its connected to. The collision and rendering will be based on those nodes / vertexes
      sf::Vector2i mouseCoords = mouse.getPosition(window);
      Node newNode;
      //NodeStruct currContainer = containerVec[containerVec.size()-1];//get the newest container (the one in the process of being created)
      int currNodeStructIdent = containerVec.size()-1;
      newNode.pos.x = mouseCoords.x - wSize/2;
      newNode.pos.y = mouseCoords.y - wSize/2;
      newNode.ident = containerVec[currNodeStructIdent].nOfNodes;
      //actually no we will just iterate through a NodeStruct and check all of its members and access the pos data from that 
      //i think
      containerVec[currNodeStructIdent].vecOfNodes.resize(newNode.ident+1);
      containerVec[currNodeStructIdent].vecOfNodes[newNode.ident] = newNode; //store the new node in the thingy 

      containerVec[currNodeStructIdent].nOfNodes++;

      
      
      


      keyPressTimeFreeze();
     }
     // create a new container 
     if (isCClicked) {
      //if containerCreatorMode is false then create a new containerStruct and await LeftClicks
      if (!containerCreatorMode) {
        NodeStruct newNodeContainer;
        newNodeContainer.nOfNodes = 0;
        containerVec.resize(containerVec.size()+1);
        containerVec[containerVec.size()-1] = newNodeContainer; 
        nOfContainers++;
      }
      //if containerCreatorMode is true idk what bcs we already saved the new nodeContainer lolololol
      if (containerCreatorMode) {
        std::cout << containerVec[containerVec.size()-1].nOfNodes << std::endl;
      }

      //save the new container when containerCreatorMode is unclicked
      containerCreatorMode = !containerCreatorMode;
      std::cout << containerCreatorMode << std::endl;
      keyPressTimeFreeze();
     }
     
     if (isRightClicked) {

      
      sf::Vector2i mouseCoords = mouse.getPosition(window);

      for (int i=0;i<nOfBlobs;i++) {
        Blob currBlob = blobVec[i];
        //
        float distance = Distance(blobVec[i].pos.x,blobVec[i].pos.y,(mouseCoords.x - wSize/2),(mouseCoords.y - wSize/2));
        //checks if we clicked on a blob 
        if (distance < 15) {
          
          isInfoOn = true;
          blobN = i; 
          
          //window.draw(blobInfoText);
        }
      }

      keyPressTimeFreeze();
     }
     
     //freeze time
     
     if (isTClicked) {
      timeFreeze = !timeFreeze;
      keyPressTimeFreeze();
     }

     
     
     if (isRightClicked && isYClicked) {
        doBlobsRender = !doBlobsRender;
     }

     //this creates a structure methinks
     if (isRightClicked && isLShiftClicked) {
      bIDsVec.clear();
      sIDsVec.clear(); 
      bsStructVec.clear(); // for now we will only store one structure the one we want to save

      //std::tuple<std::vector<int>,std::vector<int>> structTuple = CreateStructure(blobN,blobVec,stickVec,bIDsVec,sIDsVec); 
      CreateStructure(blobN,blobVec,stickVec);
      std::cout << "#######################" << std::endl;
      for (int& bID : bIDsVec) {
        std::cout << "Blob: " << bID << std::endl;
      }
      for (int& bID : sIDsVec) {
        std::cout << "Stick: " << bID << std::endl;
      }
      //std::vector<std::tuple<std::vector<int>,std::vector<int>>>
      bsStructVec.resize(bsStructVec.size()+1);
      bsStructVec[bsStructVec.size()-1] = std::make_tuple(bIDsVec,sIDsVec);

      //saving goes here for now 
      saveStructure(bsStructVec,blobVec,stickVec,0);


      //start from here by exporting the output to a new struct vec
      

      keyPressTimeFreeze();
     }
     
     //this saves a structure
     if (isLCtrlClicked) {
      //std::tuple<std::vector<Blob>,std::vector<sf::CircleShape>,std::vector<Stick>> loadStructure()
      std::tuple<std::vector<Blob>,std::vector<sf::CircleShape>,std::vector<Stick>> structOutputTuple = loadStructure(nOfBlobs,nOfSticks);
      //return std::make_tuple(newBlobVec,newblobShapeVec,newStickVec);
      //std::get<0>(outputTuple);
      std::vector<Blob> outBVec = std::get<0>(structOutputTuple);
      std::vector<sf::CircleShape> outBCVec = std::get<1>(structOutputTuple);
      std::vector<Stick> outSVec = std::get<2>(structOutputTuple);
      std::cout << outBVec.size() << " " << outBCVec.size() << " " << outSVec.size() << std::endl;
      int specialNOfBlobs = nOfBlobs;
      for (Blob& cb : outBVec) {
        blobVec.resize(nOfBlobs+1);
        blobVec[nOfBlobs] = cb;
        
        nOfBlobs++;
      }
      for (sf::CircleShape& cBC : outBCVec) {
        blobShapeVec.resize(specialNOfBlobs+1);
        blobShapeVec[specialNOfBlobs] = cBC;
        specialNOfBlobs++;
      }
      for (Stick& cs : outSVec) {
        stickVec.resize(nOfSticks+1);
        stickVec[nOfSticks] = cs;
        nOfSticks++;
      }

      keyPressTimeFreeze();


     }  

     //move a particle
     if (isRightClicked && isMClicked) {
      sf::Vector2i mouseCoords = mouse.getPosition(window);

      blobVec[blobN].pos.x = mouseCoords.x - wSize/2;
      blobVec[blobN].pos.y = mouseCoords.y - wSize/2;
      blobVec[blobN].oldPos = blobVec[blobN].pos;

     }
     
     //change particle radius dynamically
     
     if (isRightClicked && isRClicked) {
        sf::Vector2i mouseCoords = mouse.getPosition(window);
        float distance = Distance(blobVec[blobN].pos.x,blobVec[blobN].pos.y,(mouseCoords.x - wSize/2),(mouseCoords.y - wSize/2));

        blobVec[blobN].radius += distance;
        blobShapeVec[blobN].setRadius(blobVec[blobN].radius);
        blobShapeVec[blobN].setOrigin(blobVec[blobN].radius,blobVec[blobN].radius);
        keyPressTimeFreeze();
     }

     //connect 2 particles with a STICK!
     
     if (isRightClicked && isUClicked) {
      if (stickInitializerVector.empty()) {
        stickInitializerVector.resize(1);
        stickInitializerVector[0] = blobVec[blobN].ident;

      }
      else {
        
        if (blobVec[blobN].ident != stickInitializerVector[0]){
          
          stickInitializerVector.resize(2);
          stickInitializerVector[1] = blobVec[blobN].ident;
          
          //initialize a stick
          Stick newStick;
          newStick.blobIdents.resize(2);
          newStick.ident = nOfSticks;
          newStick.blobIdents = stickInitializerVector;
          newStick.length = pow(pow(blobVec[stickInitializerVector[0]].pos.x - blobVec[stickInitializerVector[1]].pos.x,2) +
          pow(blobVec[stickInitializerVector[0]].pos.y - blobVec[stickInitializerVector[1]].pos.y,2),0.5);
          
          stickVec.resize(nOfSticks + 1);
          stickVec[nOfSticks] = newStick;
          nOfSticks++;
          std::cout <<  blobVec[stickInitializerVector[1]].stickIdents.size() << std::endl;
          //add the new stick to the library of sticks 
          blobVec[stickInitializerVector[0]].stickIdents.resize(blobVec[stickInitializerVector[0]].stickIdents.size()+1);
          
          blobVec[stickInitializerVector[1]].stickIdents.resize(blobVec[stickInitializerVector[1]].stickIdents.size()+1);
          
          blobVec[stickInitializerVector[0]].stickIdents[blobVec[stickInitializerVector[0]].stickIdents.size()-1] = newStick.ident;
          blobVec[stickInitializerVector[1]].stickIdents[blobVec[stickInitializerVector[1]].stickIdents.size()-1] = newStick.ident;
          
          
          
          


          
          std::cout << stickInitializerVector[0] << "  " << stickInitializerVector[1] << std::endl;
          //std::cout << bsStructVec[nOfbsStructs].blobIdents[0] << " " << bsStructVec[nOfbsStructs].blobIdents[1] << std::endl;
          stickInitializerVector.clear();
          
        }

       
        
      }
      keyPressTimeFreeze();
     }
     
     //magnetize/unmagnetize a particle (it will be attracted by other magnetized particles)
     
     if (isGClicked) {
      for (int i=0;i<nOfBlobs;i++) {
        blobVec[i].isMagnetized = !blobVec[i].isMagnetized; 
        
      }
      magneticsCheck = !magneticsCheck; // dont put this inside the for loop dummy it will flip flop and if the number is even
      // it will just end up the same way it was 
      
      
     }

     
     
     
     

     //#####INPUTS######
     

     window.clear();


     //render all the static objects
     


     
     window.draw(timeText);
     if (isInfoOn) {

      
      sf::String blobInfoString("Blob " + std::to_string(blobVec[blobN].ident) + " Info \n" +
          "Position: " + std::to_string(blobVec[blobN].pos.x) + " " + std::to_string(blobVec[blobN].pos.y) + "\n" +
          "Velocity: " + std::to_string(blobVec[blobN].vel.x) + " " + std::to_string(blobVec[blobN].vel.y) + "\n" +
          "Color: " + std::to_string(blobVec[blobN].color.r) + " " + std::to_string(blobVec[blobN].color.g) + " " + std::to_string(blobVec[blobN].color.b)
          );
      blobInfoText.setString(blobInfoString);


      window.draw(blobInfoText);

      sf::String rInfoString("Radius: " + std::to_string(blobVec[blobN].radius) + "\n" +
      "Is Magnetic? " + std::to_string(magneticsCheck));
      rInfoText.setString(rInfoString);

      window.draw(rInfoText);
      

    
     }

     //render blobs
     
      for (int i=0;i<nOfBlobs;i++)
      {
       Blob currBlob = blobVec[i];
       
       
       blobShapeVec[i].setPosition(blobVec[i].pos.x + wSize/2 - cameraPos.x,blobVec[i].pos.y + wSize/2 + cameraPos.y);
       
       //std::cout << blobShapeArray[i].getPosition().y << std::endl;
       if (doBlobsRender) {
       window.draw(blobShapeVec[i]);
       }
      }
     
     
     //render sticks
     for (int j=0;j<nOfSticks;j++) {
      
      int startIdent = stickVec[j].blobIdents[0];
      int endIdent = stickVec[j].blobIdents[1];
      sf::Vector2f start = blobShapeVec[startIdent].getPosition();
      sf::Vector2f end = blobShapeVec[endIdent].getPosition();
      float thick = 4;
      sf::Vertex* vertexA = DrawLine(start,end,thick);
      
      window.draw(vertexA,4,sf::PrimitiveType::Quads);
     
      

      delete[] vertexA;

     }

     //render containers
     for (int k=0; k<containerVec.size();k++) {
      //for all the lines in a container 
      NodeStruct currNodeStruct = containerVec[k];
      for (int o=1;o<currNodeStruct.nOfNodes;o++) { //starting from 1 
        //render all the connections between nodes
         sf::Vector2f start;
         sf::Vector2f end;
         start.x = currNodeStruct.vecOfNodes[o].pos.x + wSize/2 - cameraPos.x;
         start.y = currNodeStruct.vecOfNodes[o].pos.y + wSize/2 + cameraPos.y;
         end.x = currNodeStruct.vecOfNodes[o-1].pos.x + wSize/2 - cameraPos.x;
         end.y = currNodeStruct.vecOfNodes[o-1].pos.y + wSize/2 + cameraPos.y;
         float thick = 5;
         //std::cout << k << " " << o << std::endl;

         sf::Vertex* vertexA = DrawLine(start,end,thick);
         window.draw(vertexA,4,sf::PrimitiveType::Quads);
         delete[] vertexA;


     }
     }
    

     

     
     //measure fps
     auto endTime = std::chrono::high_resolution_clock::now();
     auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
     float fps = 1000000.f/frameDuration.count(); 
     
     sf::String fpsString("FPS: " + std::to_string(fps));
     fpsText.setString(fpsString);

     window.draw(fpsText);

     
     
     window.display();

    }

    
    
    

   

    return 0;
}










#include "structures.h"





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
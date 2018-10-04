/* 
 * File:   SubnetCalculator.cpp
 * Author: cleo
 * 
 * Created on March 26, 2014, 2:35 PM
 */

#include <SubnetCalculator.h>

SubnetCalculator::SubnetCalculator(std::string ip, std::string netmask) 
{            
    std::vector<int> octetsIP;
    std::vector<int> octetsNetmask;
    
    splitOctets(ip,octetsIP);
    splitOctets(netmask,octetsNetmask);

    // Print Initial User IP and Subnet Mask //
    std::vector<int> decimals;
    std::vector<int> decimalMask = toDecimal(octetsNetmask, decimals);
    
    firstUsableHost_ = getNetID(octetsIP, octetsNetmask);
    std::vector<int> decimalNetID = toDecimal(firstUsableHost_, decimals);
         
    int netInc = getIncrement(decimalMask, decimalNetID);

    /*int classResult = calcClass(octetsIP);
    int ipClass = calcIPClass(classResult);*/
        
    lastUsableHost_ = getNetIDRange(decimalNetID, netInc, decimalMask);

    //Quitamos las direcciones de RED y BROADCAST
    firstUsableHost_.at(firstUsableHost_.size()-1) += 1; 
    lastUsableHost_.at(lastUsableHost_.size()-1) -= 1; 
}

SubnetCalculator::~SubnetCalculator(){
}

void SubnetCalculator::splitOctets(std::string address, std::vector<int>& octets) {
    std::stringstream sip(address);
    std::string temp;
    
    octets.clear();									
    while (getline(sip,temp,'.'))						
        octets.push_back(atoi(temp.c_str()));			
}

// Turn Binary back to Decimal
std::vector<int> SubnetCalculator::toDecimal(std::vector<int> octets, std::vector<int>& decimals) {
    stringstream octStrm;
    decimals.clear();
    for(unsigned int j = 0; j < octets.size(); j++) {
        if (j>0) octStrm << '.';
        octStrm << octets[j];
    }

    std::string temp;
    while (getline(octStrm, temp, '.')) {
        decimals.push_back(atoi(temp.c_str()));
    }
    return decimals;
}

int SubnetCalculator::calcClass(std::vector<int> &octetsIP) {
    if (octetsIP[0] == 10) {
        return 1;	// Class A Private address blocks //
    }else if (octetsIP[0] == 172 && octetsIP[1] >= 16 && octetsIP[1] <= 31) {
        return 2;	// Class B Private address blocks //
    }else if (octetsIP[0] == 192 && octetsIP[1] == 168) {
        return 3;	// Class C Private address blocks //
    }else if (octetsIP[0] == 127) {
        return 4;	// Loopback Address Reserved address blocks //
    }else if (octetsIP[0] >= 0 && octetsIP[0] < 127) {
        return 5;
    }else if (octetsIP[0] > 127 && octetsIP[0] < 192) {
        return 6;
    }else if (octetsIP[0] > 191 && octetsIP[0] < 224) {
        return 7;
    }else if (octetsIP[0] > 223 && octetsIP[0] < 240) {
        return 8;
    }else if (octetsIP[0] > 239 && octetsIP[0] <= 255) {
        return 9;
    }else{
        return 0;	// Out of Range //
    }
}

int SubnetCalculator::calcIPClass(int calcClass) {
    switch (calcClass){
        case 1: /*IP Class: Private block, Class 'A'*/
            return 1;
        case 2: /*IP Class: Private block, Class 'B'*/
            return 2;
        case 3: /*IP Class: Private block, Class 'C'*/
            return  3;
        case 4: /*IP Class: Reserved block, System Loopback Address*/
            return  1;
        case 5: /*IP Class: A*/
            return 1;
        case 6: /*IP Class: B*/
            return  2;
        case 7: /*IP Class: C*/
            return 3;
        case 8: /*IP Class: D!! This is a reserved Class D Multicast IP Address Block */
            return 4;
        case 9: /* IP Class: E!! This is a reserved Class E Multicast IP Address Block */
            return 5;
        default : /*Not in Range */
            return 0;
    }
}

// Perform ANDing of IP and Subnet Mask to generate Network ID range //
std::vector<int> SubnetCalculator::getNetID(std::vector<int>& octetsIPBits, std::vector<int>& octetsNetmask) {
    std::vector<int> netID;
    for (unsigned int j=0; j < octetsIPBits.size(); j++){
        netID.push_back(octetsIPBits[j] & octetsNetmask[j]);
    }
    return netID;
}

// Get the network increment //
int SubnetCalculator::getIncrement(std::vector<int> decimalMask, std::vector<int> decimalNetID) {
    int increment = 0;
    for (unsigned int i=0; i<decimalMask.size(); i++){
        if (decimalMask[i] == 255){
            increment = 1;
        }else if(decimalMask[i] == 254){
            increment = 2;
            break;
        }else if(decimalMask[i] == 252){
            increment = 4;
            break;
        }else if(decimalMask[i] == 248){
            increment = 8;
            break;
        }else if(decimalMask[i] == 240){
            increment = 16;
            break;
        }else if(decimalMask[i] == 224){
            increment = 32;
            break;
        }else if(decimalMask[i] == 192){
            increment = 64;
            break;
        }else if(decimalMask[i] == 128){
            increment = 128;
            break;
        }
    }
    return increment;
}

// get network id range
std::vector<int> SubnetCalculator::getNetIDRange(std::vector<int>& decimalNetID, int& netInc, std::vector<int>& decimalMask) {
    std::vector<int> netIDEnd;
    for (unsigned int i=0; i<decimalNetID.size(); i++){
        if (decimalMask[i] == 255){
            netIDEnd.push_back(decimalNetID[i]);
        }else if (decimalMask[i] < 255 && decimalMask[i] > 0){
            netIDEnd.push_back( (decimalNetID[i] + netInc) - 1 );
        }else{
            netIDEnd.push_back(255);
        }
    }
    return netIDEnd;
}

long long SubnetCalculator::getSubnetSize() {
        
        int aux = lastUsableHost_[0] - firstUsableHost_[0];
        if (aux < 0) aux = 255 - aux;
        long long count = (aux * pow(255, 3));
        
        aux = lastUsableHost_[1] - firstUsableHost_[1];
        if (aux < 0) aux = 255 - aux;
        count += (aux * pow(255, 2));        

        aux = lastUsableHost_[2] - firstUsableHost_[2];
        if (aux < 0) aux = 255 - aux;
        count += (aux * pow(255, 1));        
        
        aux = lastUsableHost_[3] - firstUsableHost_[3];
        if (aux < 0) {
            if (count > 0)
                aux = 255 - aux;        
            else
                aux = 0;
        }
        count += aux;
        
        return count; 
    }  
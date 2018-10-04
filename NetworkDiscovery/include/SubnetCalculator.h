/* 
 * File:   SubnetCalculator.h
 * Author: cleo
 *
 * Created on March 26, 2014, 2:35 PM
 */

#ifndef SUBNETCALCULATOR_H
#define	SUBNETCALCULATOR_H

#include <Utils.h>

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <math.h>

using namespace std;

class SubnetCalculator {
public:
    SubnetCalculator(std::string ip, std::string netmask);
    virtual ~SubnetCalculator();
    
    std::vector<int> getFirstUsableHost() { return firstUsableHost_; }
    std::vector<int> getLastUsableHost() { return lastUsableHost_; }
    
    long long getSubnetSize();
    
private:
    void splitOctets(std::string address, std::vector<int>& octets);
    int calcClass(std::vector<int> &octetsIP);
    int calcIPClass(int calcClass);
    std::vector<int> getNetID(std::vector<int>& octetsIPBits, std::vector<int>& octetsNetmask);
    int getIncrement(std::vector<int> decimalMask, std::vector<int> decimalNetID);
    std::vector<int> getNetIDRange(std::vector<int>& decimalNetID, int& netInc, std::vector<int>& decimalMask);
    
    std::vector<int> toDecimal(std::vector<int> octets, std::vector<int>& decimals);
    
    std::vector<int> firstUsableHost_;
    std::vector<int> lastUsableHost_;
};

#endif	/* SUBNETCALCULATOR_H */


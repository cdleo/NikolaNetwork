/* 
 * File:   IPRangeCalculator.h
 * Author: cleo
 *
 * Created on March 28, 2014, 2:09 PM
 */

#ifndef IPRANGECALCULATOR_H
#define	IPRANGECALCULATOR_H

#include <ostream>
#include <iomanip> 
#include <vector>
#include <cmath>

#include <Utils.h>

class IPRangeCalculator {
public:
    IPRangeCalculator(std::vector<int> min, std::vector<int> max) {
        inferiorLimit = min;
        superiorLimit = max;
        reset();
    }
    
    virtual ~IPRangeCalculator() {};
    
    void reset() {
        currentValue = inferiorLimit;
    }
    
    void nextValue(std::string& nextIP) {
        
        currentValue[3]++;
        if (currentValue[3] > 255) {
            currentValue[3] = 0;
            currentValue[2]++;
            if (currentValue[2] > 255) {
                currentValue[2] = 0;
                currentValue[1]++;
                if (currentValue[1] > 255) {
                    currentValue[1] = 0;
                    currentValue[0]++;
                    if (currentValue[0] > 255) {
                        nextIP.clear();
                        return;
                    }
                }                
            }
        }
        
        if (isInRange()) 
            Utils::toString(currentValue, nextIP);
        else
            nextIP.clear();  
    }

private:
    bool isInRange() {    
        return (toNumber(currentValue) <= toNumber(superiorLimit));
    }    
    
    long long toNumber(std::vector<int> address) {
        std::ostringstream aux;
        for(unsigned int i = 0; i < address.size(); i++) {
            aux << std::setfill ('0') << std::setw(3) << address[i];
        }
        return atoll(aux.str().c_str());
    }
    
    std::vector<int> currentValue;
    std::vector<int> inferiorLimit;
    std::vector<int> superiorLimit;
    
};

#endif	/* IPRANGECALCULATOR_H */


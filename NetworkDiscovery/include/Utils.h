/* 
 * File:   Utils.h
 * Author: cleo
 *
 * Created on March 26, 2014, 12:10 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <vector>
#include <sstream>
#include <iostream>

#include <time.h>

class Utils {
public:
    
    static std::string getNowAsString(const char* format = "%Y%m%d%H%M%S") {
        char buffer[16];
        time_t rawtime;
        struct tm* timeinfo;

        time (&rawtime);
        timeinfo = localtime(&rawtime);
        strftime( buffer, sizeof(buffer), format, timeinfo);
        
        return buffer;
    }   

    static void toString(std::vector<int>& octets, std::string& result) {
        std::ostringstream octStrm;
        for(unsigned int j = 0; j < octets.size(); j++) {
            if (j>0) octStrm << '.';
            octStrm << octets[j];
        }
        result.assign( octStrm.str() );
    }    
    
    static void drawProgressBar(std::string legend, unsigned long long current,unsigned long long max){

        int barWidth = 70;
        float progress = float(current) / float(max);
        std::ostringstream oss;
        oss << "\r" << legend << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) oss << "=";
            else if (i == pos) oss << ">";
            else oss << " ";
        }
        oss << "] " << int(progress * 100.0) << " %";

        std::cout << oss.str() << std::flush;

        if (current == max) 
            std::cout << std::endl;
    }    

};

#endif	/* UTILS_H */


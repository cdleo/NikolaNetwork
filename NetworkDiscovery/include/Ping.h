/* 
 * File:   ping.h
 * Author: cleo
 *
 * Created on March 30, 2014, 3:34 PM
 */

#ifndef PING_H
#define PING_H

#include <string>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timex.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

typedef struct PingReply PingReply;

#define ERROR_MAXLENGTH 256
#define IP_HEADER_LENGTH 20

struct PingReply {
    PingReply() : icmp_type(0), icmp_code(0), msec(0.0) {
        error[0] = '\0';
    }
    
    PingReply( const PingReply& other ) {
        strncpy(error, other.error, ERROR_MAXLENGTH);
        icmp_type = other.icmp_type;
        icmp_code = other.icmp_code;
        msec = other.msec;
    }
    
    char error[ERROR_MAXLENGTH];
    unsigned short icmp_type;
    unsigned short icmp_code;
    double msec;
};

class Ping {
public:
    Ping(const char* destination, unsigned int timeout);
    PingReply Send();
  
private:
    unsigned short ip_checksum(void* vdata,size_t length);
    
    unsigned int id, seq, size, timeout;
    std::string destination;
};

#endif // PING_H 


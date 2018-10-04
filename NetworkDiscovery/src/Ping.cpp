/* 
 * File:   ping.cpp
 * Author: cleo
 * 
 * Created on March 30, 2014, 3:34 PM
 */

#include <Ping.h>
#include <sys/socket.h>
#include <string>

Ping::Ping(const char* destination, unsigned int timeout) {
    this->destination.assign(destination);
    this->timeout = timeout;
    this->id = getpid();
    this->seq = 0;
}

PingReply Ping::Send() {
    
    PingReply result;
            
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock==-1) {
        strncpy(result.error, "Unable to create socket", ERROR_MAXLENGTH);
        return result;
    }

    timeval tv_timeout;
    tv_timeout.tv_sec = 0;
    tv_timeout.tv_usec = timeout;
    int rc = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv_timeout, sizeof(timeval));
    if (rc==-1) {
        strncpy(result.error, "Unable to set socket options", ERROR_MAXLENGTH);
        close(sock);
        return result;
    }

    size = sizeof(icmphdr);
    char *send_buffer = (char*)malloc(size);
    if (!send_buffer) {
        strncpy(result.error,"malloc() failed. Unable to allocate memory.", ERROR_MAXLENGTH);
        close(sock);
        return result;
    }

    seq++;
    memset(send_buffer,0,size);
    struct icmphdr IcmpHeader;
    IcmpHeader.type = ICMP_ECHO;
    IcmpHeader.code = 0;
    IcmpHeader.checksum = 0;
    IcmpHeader.un.echo.id = htons(id);
    IcmpHeader.un.echo.sequence = htons(seq);
    IcmpHeader.checksum = this->ip_checksum(&IcmpHeader, sizeof(struct icmphdr));
    memcpy(send_buffer, &IcmpHeader, sizeof(struct icmphdr));

    struct sockaddr_in sa_dest;
    sa_dest.sin_family = AF_INET;
    sa_dest.sin_addr.s_addr = inet_addr(destination.c_str());
    sa_dest.sin_port = 0;
    rc = connect(sock,(struct sockaddr*)&sa_dest, sizeof(struct sockaddr_in));
    if (rc==-1) {
        strncpy(result.error,  "Unable to connect to host", ERROR_MAXLENGTH);
        free(send_buffer);
        close(sock);
        return result;
    }    
    rc = send(sock, send_buffer, size, 0);//, (struct sockaddr*)&sa_dest, sizeof(struct sockaddr_in));
    //rc = sendto(sock, send_buffer, size, 0, (struct sockaddr*)&sa_dest, sizeof(struct sockaddr_in));
    if (rc==-1) {
        strncpy(result.error, "Unable to Send data", ERROR_MAXLENGTH);
        free(send_buffer);
        close(sock);
        return result;
    }

    size = sizeof(icmphdr) + IP_HEADER_LENGTH;// ICMP header is after the IP Header which is 20 bytes long
    char *recv_buffer = (char*)malloc(size);
    if (!recv_buffer) {
        strncpy(result.error,"malloc() failed. Unable to allocate memory.", ERROR_MAXLENGTH);
        free(send_buffer);
        close(sock);
        return result;
    }
    
    //struct sockaddr_in sa_reply;
    //socklen_t sa_size = sizeof(struct sockaddr_in);
    //rc = recvfrom(sock, recv_buffer, size, 0, (struct sockaddr*)&sa_reply, &sa_size);
    rc = recv(sock, recv_buffer, size, 0);
    if (rc==-1) {
        strncpy(result.error, "Request timed out", ERROR_MAXLENGTH);
        free(recv_buffer);
        free(send_buffer);
        close(sock);
        return result;
    }
    //struct ntptimeval elapsed;
    //ntp_gettime(&elapsed);    
    //result.RAddr = inet_ntoa(sa_reply.sin_addr);
    //result.RAddr = inet_ntoa(sa_dest.sin_addr);

    struct icmphdr *IcmpReplyHeader = (struct icmphdr*)(recv_buffer+IP_HEADER_LENGTH); // ICMP header is after the IP Header which is 20 bytes long
    if (IcmpHeader.un.echo.id != IcmpReplyHeader->un.echo.id) { strncpy(result.error,"Request timed out", ERROR_MAXLENGTH);; } // If we capture an ICMP echo reply that's not for our PID, discard it
    if (IcmpHeader.un.echo.sequence != IcmpReplyHeader->un.echo.sequence) { strncpy(result.error,"Request timed out", ERROR_MAXLENGTH);; } // If we capture an old ICMP echo reply, discard it
    result.icmp_type = IcmpReplyHeader->type;
    result.icmp_code = IcmpReplyHeader->code;
    result.msec = 0;
    free(recv_buffer); free(send_buffer); close(sock);

    return result;
}

unsigned short Ping::ip_checksum(void* vdata,size_t length) {
    // Cast the data pointer to one that can be indexed.
    char* data=(char*)vdata;

    // Initialise the accumulator.
    uint32_t acc=0xffff;

    // Handle complete 16-bit blocks.
    size_t i;
    for (i=0;i+1<length;i+=2) {
        uint16_t word;
        memcpy(&word,data+i,2);
        acc+=ntohs(word);
        if (acc>0xffff) {
            acc-=0xffff;
        }
    }

    // Handle any partial block at the end of the data.
    if (length&1) {
        uint16_t word=0;
        memcpy(&word,data+length-1,1);
        acc+=ntohs(word);
        if (acc>0xffff) {
            acc-=0xffff;
        }
    }

    // Return the checksum in network byte order.
    return htons(~acc);
}


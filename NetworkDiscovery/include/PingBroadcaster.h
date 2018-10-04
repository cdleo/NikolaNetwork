/* 
 * File:   PingBroadcaster.h
 * Author: cleo
 *
 * Created on March 25, 2014, 4:04 PM
 */

#ifndef PINGER_H
#define	PINGER_H

#include <GlobalDefinitions.h>

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/signal.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include <vector>

#define icmphdr		icmp

#define	DEFDATALEN	56              /* (64 -8) default data length */
#define	MAXIPLEN	60
#define	MAXICMPLEN	76
#define	MAXPACKET	65468           /* (65536 - 60 - 8) max packet size */
#define	MAXWAIT		10		/* max seconds to wait for response */
#define	NROUTES		9		/* number of record route slots */

#define	A(bit)		rcvd_tbl[(bit)>>3]	/* identify byte in array */
#define	B(bit)		(1 << ((bit) & 0x07))	/* identify bit in byte */
#define	SET(bit)	(A(bit) |= B(bit))
#define	CLR(bit)	(A(bit) &= (~B(bit)))
#define	TST(bit)	(A(bit) & B(bit))

#define	F_FLOOD		0x001
#define	F_INTERVAL	0x002
#define	F_NUMERIC	0x004
#define	F_PINGFILLED	0x008
#define	F_QUIET		0x010
#define	F_RROUTE	0x020
#define	F_SO_DEBUG	0x040
#define	F_SO_DONTROUTE	0x080
#define	F_VERBOSE	0x100

#define MULTICAST_NOLOOP	0x001
#define MULTICAST_TTL		0x002
#define MULTICAST_IF		0x004

#if !defined(__GLIBC__) || (__GLIBC__ < 2)
#define icmp_type type
#define icmp_code code
#define icmp_cksum checksum
#define icmp_id un.echo.id
#define icmp_seq un.echo.sequence
#define icmp_gwaddr un.gateway
#endif /* __GLIBC__ */

#define ip_hl ihl
#define ip_v version
#define ip_tos tos
#define ip_len tot_len
#define ip_id id
#define ip_off frag_off
#define ip_ttl ttl
#define ip_p protocol
#define ip_sum check
#define ip_src saddr
#define ip_dst daddr

/*
 * MAX_DUP_CHK is the number of bits in received table, i.e. the maximum
 * number of received sequence numbers we can keep track of.  Change 128
 * to 8192 for complete accuracy...
 */
#define	MAX_DUP_CHK	(8 * 128)

class PingBroadcaster {
public:
    PingBroadcaster();
    virtual ~PingBroadcaster();

    void findNeighbors(std::string fromAddress, std::string destinationAddress);
    std::vector<std::string>& getNeighbors();
    
    void catcher(int ignore);
    static void catcher_st(int ignore);
    static void finish_st(int ignore);
    
private:
    /* protos */
    int in_cksum(u_short *addr, int len);
    void pinger(void);
    void fill(void *bp, char *patp);
    void pr_pack(char *buf, int cc, struct sockaddr_in *from);
    void tvsub(struct timeval *out, struct timeval *in);
    void pr_icmph(struct icmphdr *icp);
    void pr_retip(struct iphdr *ip);    
    void pr_iph(struct iphdr *ip);
    
    std::vector<std::string> vNeighbors_;
    
    char rcvd_tbl[MAX_DUP_CHK / 8];

    struct sockaddr whereto;	/* who to ping */
    u_char outpack[MAXPACKET];
    char BSPACE;		/* characters written for flood */
    char DOT;
    char *hostname;
    int ident;                  /* process id to identify our packets */

    /* counters */
    long npackets;		/* max packets to transmit */
    long nreceived;		/* # of packets we got back */
    long nrepeats;		/* number of duplicates */
    long ntransmitted;          /* sequence # for outbound packets = #sent */
    int interval;               /* interval between packets */

    /* timing */
    int timing;                 /* flag to do timing */
    long tmin;                  /* minimum round trip time */
    long tmax;                  /* maximum round trip time */
    u_long tsum;		/* sum of all times, for doing average */ 
    
    int mx_dup_ck;
    int options;                /* various options */
    int moptions;               /* multicast options */    
    int datalen;
    int s;			/* socket file descriptor */    

};

#endif	/* PINGER_H */


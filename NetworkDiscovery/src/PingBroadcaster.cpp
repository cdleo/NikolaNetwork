/*
 *			P I N G . C
 *
 * Using the InterNet Control Message Protocol (ICMP) "ECHO" facility,
 * measure round-trip-delays and packet loss across network paths.
 *
 * Author -
 *	Mike Muuss
 *	U. S. Army Ballistic Research Laboratory
 *	December, 1983
 *
 * Status -
 *	Public Domain.  Distribution Unlimited.
 * Bugs -
 *	More statistics could always be gathered.
 *	This program has to run SUID to ROOT to access the ICMP socket.
 */

#include <PingBroadcaster.h>

#include <iostream>

PingBroadcaster* this_;

void PingBroadcaster::catcher_st(int ignore) {
    this_->catcher(ignore);
}

void PingBroadcaster::finish_st(int ignore) {
    (void)ignore;
    //signal(SIGINT, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
}

PingBroadcaster::PingBroadcaster() { 
    this_ = this;
}

PingBroadcaster::~PingBroadcaster() {

}

std::vector<std::string>& PingBroadcaster::getNeighbors() {
    return vNeighbors_;
}

void PingBroadcaster::findNeighbors(std::string fromAddress, std::string destinationAddress)
{
    struct timeval timeout;
    struct hostent *hp;
    struct sockaddr_in *to;
    struct protoent *proto;
    struct in_addr ifaddr;
    int i;
    int fdmask, hold, packlen, preload;
    u_char *datap, *packet;
    u_char ttl, loop;

#ifdef IP_OPTIONS
    char rspace[3 + 4 * NROUTES + 1];	/* record route space */
#endif

    char *null = NULL;
    __environ = &null;
        
    mx_dup_ck = MAX_DUP_CHK; 
    datalen = DEFDATALEN; 
    BSPACE = '\b'; 
    DOT = '.'; 
    tmin = LONG_MAX; 
    tmax = 0; 
    options = 0; 
    moptions = 0;    
    
    vNeighbors_.clear();
    ntransmitted = 0;
    nreceived = 0;
    npackets = 1;
    
    /*
     * Pull this stuff up front so we can drop root if desired.
     */
    if (!(proto = getprotobyname("icmp"))) {
        syslog(LOG_WARNING, "ping: unknown protocol icmp.");
        return;
    }
    if ((s = socket(AF_INET, SOCK_RAW, proto->p_proto)) < 0) {
        if (errno==EPERM)
            syslog(LOG_WARNING, "ping: ping must run as root");
        else 
            syslog(LOG_WARNING, "ping: Error creando socket");
        return;
    }
    
#ifdef SAFE_TO_DROP_ROOT
    setuid(getuid());
#endif

    preload = 0;
    interval = 1;
    datap = &outpack[8 + sizeof(struct timeval)];

    moptions = 0; //MULTICAST_IF;
    if (!inet_aton(fromAddress.c_str(), &ifaddr))
        return;

    memset(&whereto, 0, sizeof(struct sockaddr));
    to = (struct sockaddr_in *)&whereto;
    to->sin_family = AF_INET;
    if (inet_aton(destinationAddress.c_str(), &to->sin_addr)) {
        hostname = const_cast<char*>(destinationAddress.c_str());

    } else {
        hp = gethostbyname(destinationAddress.c_str());
        if (!hp) return;

        to->sin_family = hp->h_addrtype;
        if (hp->h_length > (int)sizeof(to->sin_addr)) {
            hp->h_length = sizeof(to->sin_addr);
        }
        memcpy(&to->sin_addr, hp->h_addr, hp->h_length);

        char hnamebuf[MAXHOSTNAMELEN];
        strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
        hostname = hnamebuf;
    }

    if (options & F_FLOOD && options & F_INTERVAL) {
        syslog(LOG_WARNING, "ping: -f and -i incompatible options.");
        return;
    }

    if (datalen >= (int)sizeof(struct timeval)) /* can we time transfer */
            timing = 1;
    packlen = datalen + MAXIPLEN + MAXICMPLEN;
    packet = (u_char*) malloc((u_int)packlen);
    if (!packet) {
        syslog(LOG_WARNING, "ping: out of memory.");
        return;
    }
    if (!(options & F_PINGFILLED))
        for (i = 8; i < datalen; ++i)
            *datap++ = i;

    ident = getpid() & 0xFFFF;
    hold = 1;

    if (options & F_SO_DEBUG)
        setsockopt(s, SOL_SOCKET, SO_DEBUG, (char *)&hold, sizeof(hold));

    if (options & F_SO_DONTROUTE)
        setsockopt(s, SOL_SOCKET, SO_DONTROUTE, (char *)&hold,sizeof(hold));

    /* this is necessary for broadcast pings to work */
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&hold, sizeof(hold));

    /* record route option */
    if (options & F_RROUTE) {
#ifdef IP_OPTIONS
        memset(rspace, 0, sizeof(rspace));
        rspace[IPOPT_OPTVAL] = IPOPT_RR;
        rspace[IPOPT_OLEN] = sizeof(rspace)-1;
        rspace[IPOPT_OFFSET] = IPOPT_MINOFF;
        if (setsockopt(s, IPPROTO_IP, IP_OPTIONS, rspace, sizeof(rspace)) < 0) {
            syslog(LOG_WARNING, "ping: record route");
            return;
        }
#else
        syslog(LOG_WARNING, "ping: record route not available in this implementation.");
        return;
#endif /* IP_OPTIONS */
    }

    /*
     * When pinging the broadcast address, you can get a lot of answers.
     * Doing something so evil is useful if you are trying to stress the
     * ethernet, or just want to fill the arp cache to get some stuff for
     * /etc/ethers.
     */
    hold = 48 * 1024;
    setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&hold,sizeof(hold));

/*#if 0*/
    if (moptions & MULTICAST_NOLOOP) {
        if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP,&loop, 1) == -1) {
            syslog(LOG_WARNING, "can't disable multicast loopback");
            return;
        }
    }
    if (moptions & MULTICAST_TTL) {
        if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL,&ttl, 1) == -1) {
            syslog(LOG_WARNING, "can't set multicast time-to-live");
            return;
        }
    }
    if (moptions & MULTICAST_IF) {
        if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF,&ifaddr, sizeof(ifaddr)) == -1) {
            syslog(LOG_WARNING, "can't set multicast source interface");
            return;
        }
    }
/*#endif*/

    while (preload--)		/* fire off them quickies */
        pinger(); 
    
    if ((options & F_FLOOD) == 0)
        catcher(0);		/* start things going */    
    
    //for (;;) {
    //while (ntransmitted<=1){        /* Para que solo dé una vuelta!! sudo ./ping -i 1 -I 10.244.44.26 10.244.44.127 */
        struct sockaddr_in from;
        register int cc;
        size_t fromlen;

        if (options & F_FLOOD) {
            pinger();
            timeout.tv_sec = 0;
            timeout.tv_usec = 10000;
            fdmask = 1 << s;
            if (select(s + 1, (fd_set *)&fdmask, (fd_set *)NULL,(fd_set *)NULL, &timeout) < 1) {
                //continue;
            }
        }
        fromlen = sizeof(from);
        if ((cc = recvfrom(s, (char *)packet, packlen, 0,(struct sockaddr *)&from, (unsigned int*)&fromlen)) < 0) {
            syslog(LOG_WARNING, "ping: recvfrom");
            //continue;
        }
        pr_pack((char *)packet, cc, &from);
        //if (npackets && nreceived >= npackets)
        //    break;
        sleep(1);
    //}
    
    free(packet);
        
    signal(SIGALRM, SIG_IGN);
    
    return;
}

void PingBroadcaster::catcher(int ignore) {
    pinger();
    signal(SIGALRM, PingBroadcaster::catcher_st);
    if (!npackets || ntransmitted < npackets) {
        alarm((u_int)interval);
    } else {
        signal(SIGALRM, PingBroadcaster::finish_st);
        alarm((u_int)1);
    }
}

/*
 * pinger --
 * 	Compose and transmit an ICMP ECHO REQUEST packet.  The IP packet
 * will be added on by the kernel.  The ID field is our UNIX process ID,
 * and the sequence number is an ascending integer.  The first 8 bytes
 * of the data portion are used to hold a UNIX "timeval" struct in VAX
 * byte-order, to compute the round-trip time.
 */
void PingBroadcaster::pinger(void)
{
    register struct icmphdr *icp;
    register int cc;
    int i;

    icp = (struct icmphdr *)outpack;
    icp->icmp_type = ICMP_ECHO;
    icp->icmp_code = 0;
    icp->icmp_cksum = 0;
    icp->icmp_seq = ntransmitted++;
    icp->icmp_id = ident;			/* ID */

    CLR(icp->icmp_seq % mx_dup_ck);

    if (timing)
        gettimeofday((struct timeval *)&outpack[8],(struct timezone *)NULL);

    cc = datalen + 8;			/* skips ICMP portion */

    /* compute ICMP checksum here */
    icp->icmp_cksum = in_cksum((u_short *)icp, cc);

    i = sendto(s, (char *)outpack, cc, 0, &whereto, sizeof(struct sockaddr));

    if (i < 0 || i != cc)  {
        syslog(LOG_WARNING,"ping: wrote %s %d chars, ret=%d", hostname, cc, i);
    }
    if (!(options & F_QUIET) && options & F_FLOOD)
            (void)write(STDOUT_FILENO, &DOT, 1);
}

/*
 * pr_pack --
 *	Print out the packet, if it came from us.  This logic is necessary
 * because ALL readers of the ICMP socket get a copy of ALL ICMP packets
 * which arrive ('tis only fair).  This permits multiple copies of this
 * program to be run without having intermingled output (or statistics!).
 */
void PingBroadcaster::pr_pack(char *buf, int cc, struct sockaddr_in *from)
{
    register struct icmphdr *icp;
    register int i;
    register u_char *cp;
    /*#if 0*/
    register u_long l;
    register int j;
    int old_rrlen;
    char old_rr[MAX_IPOPTLEN];
    /*#endif*/
    struct iphdr *ip;
    struct timeval tv, *tp;
    long triptime = 0;
    int hlen;

    gettimeofday(&tv, (struct timezone *)NULL);

    /* Check the IP header */
    ip = (struct iphdr *)buf;
    hlen = ip->ip_hl << 2;
    if (cc < datalen + ICMP_MINLEN) {
        if (options & F_VERBOSE)
            syslog(LOG_WARNING, "ping: packet too short");
        return;
    }

    /* Now the ICMP part */
    cc -= hlen;
    icp = (struct icmphdr *)(buf + hlen);
    if (icp->icmp_type == ICMP_ECHOREPLY) {
        if (icp->icmp_id != ident)
                return;			/* 'Twas not our ECHO */
        ++nreceived;
        if (timing) {
    #ifndef icmp_data
            tp = (struct timeval *)(icp + 1);
    #else
            tp = (struct timeval *)icp->icmp_data;
    #endif
            tvsub(&tv, tp);
            triptime = tv.tv_sec * 10000 + (tv.tv_usec / 100);
            tsum += triptime;
            if (triptime < tmin)
                    tmin = triptime;
            if (triptime > tmax)
                    tmax = triptime;
        }

        if (TST(icp->icmp_seq % mx_dup_ck)) {
            ++nrepeats;
            --nreceived;
        } else {
            SET(icp->icmp_seq % mx_dup_ck);
        }

        if (options & F_QUIET)
            return;

        if (options & F_FLOOD)
            write(STDOUT_FILENO, &BSPACE, 1);
        else {
            vNeighbors_.push_back( std::string(inet_ntoa(*(struct in_addr *)&from->sin_addr.s_addr)) );

            /* No me interesa checkear los datos
    #ifndef icmp_data
            cp = ((u_char*)(icp + 1) + 8);
    #else
            cp = (u_char*)icp->icmp_data + 8;
    #endif
            dp = &outpack[8 + sizeof(struct timeval)];
            for (i = 8; i < datalen; ++i, ++cp, ++dp) {
                if (*cp != *dp) {
                    syslog(LOG_WARNING,"wrong data byte #%d should be 0x%x but was 0x%x",i, *dp, *cp);
                    cp = (u_char*)(icp + 1);
                    for (i = 8; i < datalen; ++i, ++cp) {
                        if ((i % 32) == 8)
                            syslog(LOG_WARNING,"\t");
                        syslog(LOG_WARNING,"%x ", *cp);
                    }
                    break;
                }
            }*/
        }
        
    } else {
        /* We've got something other than an ECHOREPLY */
        if (!(options & F_VERBOSE))
            return;
        pr_icmph(icp);
    }

    /*#if 0*/
    /* Display any IP options */
    cp = (u_char *)buf + sizeof(struct iphdr);

    for (; hlen > (int)sizeof(struct iphdr); --hlen, ++cp) {
        switch (*cp) {
            case IPOPT_EOL:
                hlen = 0;
                break;

            case IPOPT_LSRR:
                hlen -= 2;
                j = *++cp;
                ++cp;
                if (j > IPOPT_MINOFF){
                    for (;;)
                    {
                        l = *++cp;
                        l = (l<<8) + *++cp;
                        l = (l<<8) + *++cp;
                        l = (l<<8) + *++cp;
                        hlen -= 4;
                        j -= 4;
                        if (j <= IPOPT_MINOFF)
                            break;
                    }
                }
                break;

            case IPOPT_RR:
                    j = *++cp;		/* get length */
                    i = *++cp;		/* and pointer */
                    hlen -= 2;
                    if (i > j)
                            i = j;
                    i -= IPOPT_MINOFF;
                    if (i <= 0)
                            continue;
                    if (i == old_rrlen && cp == (u_char *)buf + sizeof(struct iphdr) + 2
                        && !memcmp((char *)cp, old_rr, i)
                        && !(options & F_FLOOD))
                    {
                        i = ((i + 3) / 4) * 4;
                        hlen -= i;
                        cp += i;
                        break;
                    }
                    old_rrlen = i;
                    memcpy(old_rr, cp, i);
                    for (;;) {
                        l = *++cp;
                        l = (l<<8) + *++cp;
                        l = (l<<8) + *++cp;
                        l = (l<<8) + *++cp;
                        hlen -= 4;
                        i -= 4;
                        if (i <= 0)
                            break;
                    }
                    break;

            case IPOPT_NOP:
                syslog(LOG_WARNING,"NOP");
                break;
            default:
                syslog(LOG_WARNING,"unknown option %x", *cp);
                break;
        }
    }
/*#endif*/
    if (!(options & F_FLOOD)) {
            (void)fflush(stdout);
    }
}

/*
 * in_cksum --
 *	Checksum routine for Internet Protocol family headers (C Version)
 */
int PingBroadcaster::in_cksum(u_short *addr, int len)
{
    register int nleft = len;
    register u_short *w = addr;
    register int sum = 0;
    u_short answer = 0;

    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1)  {
            sum += *w++;
            nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if (nleft == 1) {
            *(u_char *)(&answer) = *(u_char *)w ;
            sum += answer;
    }

    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
    sum += (sum >> 16);			/* add carry */
    answer = ~sum;				/* truncate to 16 bits */
    return(answer);
}

/*
 * tvsub --
 *	Subtract 2 timeval structs:  out = out - in.  Out is assumed to
 * be >= in.
 */
void PingBroadcaster::tvsub(register struct timeval *out, register struct timeval *in)
{
    if ((out->tv_usec -= in->tv_usec) < 0) {
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}

#ifdef notdef
char *ttab[] = {
	"Echo Reply",		/* ip + seq + udata */
	"Dest Unreachable",	/* net, host, proto, port, frag, sr + IP */
	"Source Quench",	/* IP */
	"Redirect",		/* redirect type, gateway, + IP  */
	"Echo",
	"Time Exceeded",	/* transit, frag reassem + IP */
	"Parameter Problem",	/* pointer + IP */
	"Timestamp",		/* id + seq + three timestamps */
	"Timestamp Reply",	/* " */
	"Info Request",		/* id + sq */
	"Info Reply"		/* " */
};
#endif

/*
 * pr_icmph --
 *	Print a descriptive string about an ICMP header.
 */
void PingBroadcaster::pr_icmph(struct icmphdr *icp)
{
    switch(icp->icmp_type) {
    case ICMP_ECHOREPLY:
        syslog(LOG_WARNING,"Echo Reply");
        /* XXX ID + Seq + Data */
        break;
        
    case ICMP_DEST_UNREACH:
        switch(icp->icmp_code) {
            case ICMP_NET_UNREACH:
                    syslog(LOG_WARNING,"Destination Net Unreachable");
                    break;
            case ICMP_HOST_UNREACH:
                    syslog(LOG_WARNING,"Destination Host Unreachable");
                    break;
            case ICMP_PROT_UNREACH:
                    syslog(LOG_WARNING,"Destination Protocol Unreachable");
                    break;
            case ICMP_PORT_UNREACH:
                    syslog(LOG_WARNING,"Destination Port Unreachable");
                    break;
            case ICMP_FRAG_NEEDED:
                    syslog(LOG_WARNING,"frag needed and DF set");
                    break;
            case ICMP_SR_FAILED:
                    syslog(LOG_WARNING,"Source Route Failed");
                    break;
            case ICMP_NET_UNKNOWN:
                    syslog(LOG_WARNING,"Network Unknown");
                    break;
            case ICMP_HOST_UNKNOWN:
                    syslog(LOG_WARNING,"Host Unknown");
                    break;
            case ICMP_HOST_ISOLATED:
                    syslog(LOG_WARNING,"Host Isolated");
                    break;
            case ICMP_NET_UNR_TOS:
                    printf("Destination Network Unreachable At This TOS");
                    break;
            case ICMP_HOST_UNR_TOS:
                    printf("Destination Host Unreachable At This TOS");
                    break;
    #ifdef ICMP_PKT_FILTERED
            case ICMP_PKT_FILTERED:
                    syslog(LOG_WARNING,"Packet Filtered");
                    break;
    #endif
    #ifdef ICMP_PREC_VIOLATION
            case ICMP_PREC_VIOLATION:
                    syslog(LOG_WARNING,"Precedence Violation");
                    break;
    #endif
    #ifdef ICMP_PREC_CUTOFF
            case ICMP_PREC_CUTOFF:
                    syslog(LOG_WARNING,"Precedence Cutoff");
                    break;
    #endif
            default:
                    syslog(LOG_WARNING,"Dest Unreachable, Unknown Code: %d",
                        icp->icmp_code);
                    break;
        }
            /* Print returned IP header information */
    #ifndef icmp_data
            pr_retip((struct iphdr *)(icp + 1));
    #else
            pr_retip((struct iphdr *)icp->icmp_data);
    #endif
            break;
            
    case ICMP_SOURCE_QUENCH:
        syslog(LOG_WARNING,"Source Quench");
    #ifndef icmp_data
        pr_retip((struct iphdr *)(icp + 1));
    #else
        pr_retip((struct iphdr *)icp->icmp_data);
    #endif
        break;
            
    case ICMP_REDIRECT:
        switch(icp->icmp_code) {
            case ICMP_REDIR_NET:
                    syslog(LOG_WARNING,"Redirect Network");
                    break;
            case ICMP_REDIR_HOST:
                    syslog(LOG_WARNING,"Redirect Host");
                    break;
            case ICMP_REDIR_NETTOS:
                    syslog(LOG_WARNING,"Redirect Type of Service and Network");
                    break;
            case ICMP_REDIR_HOSTTOS:
                    syslog(LOG_WARNING,"Redirect Type of Service and Host");
                    break;
            default:
                    syslog(LOG_WARNING,"Redirect, Bad Code: %d", icp->icmp_code);
                    break;
        }
        syslog(LOG_WARNING,"(New addr: %s)", inet_ntoa(icp->icmp_gwaddr));
    #ifndef icmp_data
        pr_retip((struct iphdr *)(icp + 1));
    #else
        pr_retip((struct iphdr *)icp->icmp_data);
    #endif
        break;
        
    case ICMP_ECHO:
        syslog(LOG_WARNING,"Echo Request");
        /* XXX ID + Seq + Data */
        break;
        
    case ICMP_TIME_EXCEEDED:
        switch(icp->icmp_code) {
            case ICMP_EXC_TTL:
                syslog(LOG_WARNING,"Time to live exceeded");
                break;
            case ICMP_EXC_FRAGTIME:
                syslog(LOG_WARNING,"Frag reassembly time exceeded");
                break;
            default:
                syslog(LOG_WARNING,"Time exceeded, Bad Code: %d",
                    icp->icmp_code);
                break;
        }
    #ifndef icmp_data
        pr_retip((struct iphdr *)(icp + 1));
    #else
        pr_retip((struct iphdr *)icp->icmp_data);
    #endif
        break;
        
    case ICMP_PARAMETERPROB:
        syslog(LOG_WARNING,"Parameter problem: IP address = %s",inet_ntoa (icp->icmp_gwaddr));
    #ifndef icmp_data
        pr_retip((struct iphdr *)(icp + 1));
    #else
        pr_retip((struct iphdr *)icp->icmp_data);
    #endif
        break;
        
    case ICMP_TIMESTAMP:
        syslog(LOG_WARNING,"Timestamp");
        /* XXX ID + Seq + 3 timestamps */
        break;
        
    case ICMP_TIMESTAMPREPLY:
        syslog(LOG_WARNING,"Timestamp Reply");
        /* XXX ID + Seq + 3 timestamps */
        break;
        
    case ICMP_INFO_REQUEST:
        syslog(LOG_WARNING,"Information Request");
        /* XXX ID + Seq */
        break;
        
    case ICMP_INFO_REPLY:
        syslog(LOG_WARNING,"Information Reply");
        /* XXX ID + Seq */
        break;
        
    #ifdef ICMP_MASKREQ
    case ICMP_MASKREQ:
        syslog(LOG_WARNING,"Address Mask Request");
        break;
    #endif
    #ifdef ICMP_MASKREPLY
    case ICMP_MASKREPLY:
        syslog(LOG_WARNING,"Address Mask Reply");
        break;
    #endif
    default:
        syslog(LOG_WARNING,"Bad ICMP type: %d", icp->icmp_type);
    }
}

/*
 * pr_iph --
 *	Print an IP header with options.
 */
void PingBroadcaster::pr_iph(struct iphdr *ip)
{
    int hlen;
    u_char *cp;

    hlen = ip->ip_hl << 2;
    cp = (u_char *)ip + 20;		/* point to options */

    syslog(LOG_WARNING,"Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst Data");
    syslog(LOG_WARNING," %1x  %1x  %02x %04x %04x",
        ip->ip_v, ip->ip_hl, ip->ip_tos, ip->ip_len, ip->ip_id);
    syslog(LOG_WARNING,"   %1x %04x", ((ip->ip_off) & 0xe000) >> 13,
        (ip->ip_off) & 0x1fff);
    syslog(LOG_WARNING,"  %02x  %02x %04x", ip->ip_ttl, ip->ip_p, ip->ip_sum);
    syslog(LOG_WARNING," %s ", inet_ntoa(*((struct in_addr *) &ip->ip_src)));
    syslog(LOG_WARNING," %s ", inet_ntoa(*((struct in_addr *) &ip->ip_dst)));
    /* dump and option bytes */
    while (hlen-- > 20) {
        syslog(LOG_WARNING,"%02x", *cp++);
    }
}

/*
 * pr_retip --
 *	Dump some info on a returned (via ICMP) IP packet.
 */
void PingBroadcaster::pr_retip(struct iphdr *ip)
{
    int hlen;
    u_char *cp;

    pr_iph(ip);
    hlen = ip->ip_hl << 2;
    cp = (u_char *)ip + hlen;

    if (ip->ip_p == 6)
            syslog(LOG_WARNING,"TCP: from port %u, to port %u (decimal)",
                (*cp * 256 + *(cp + 1)), (*(cp + 2) * 256 + *(cp + 3)));
    else if (ip->ip_p == 17)
            syslog(LOG_WARNING,"UDP: from port %u, to port %u (decimal)",
                    (*cp * 256 + *(cp + 1)), (*(cp + 2) * 256 + *(cp + 3)));
}

void PingBroadcaster::fill(void *bp1, char *patp)
{
    register int ii, jj, kk;
    int pat[16];
    char *cp, *bp = (char *)bp1;

    for (cp = patp; *cp; cp++)
        if (!isxdigit(*cp)) {
            syslog(LOG_WARNING, "ping: patterns must be specified as hex digits.");
        }
    ii = sscanf(patp,
        "%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x",
        &pat[0], &pat[1], &pat[2], &pat[3], &pat[4], &pat[5], &pat[6],
        &pat[7], &pat[8], &pat[9], &pat[10], &pat[11], &pat[12],
        &pat[13], &pat[14], &pat[15]);

    if (ii > 0)
            for (kk = 0; kk <= MAXPACKET - (8 + ii); kk += ii)
                    for (jj = 0; jj < ii; ++jj)
                            bp[jj + kk] = pat[jj];
    if (!(options & F_QUIET)) {
        syslog(LOG_WARNING,"PATTERN: 0x");
        for (jj = 0; jj < ii; ++jj)
            syslog(LOG_WARNING,"%02x", bp[jj] & 0xFF);
    }
}

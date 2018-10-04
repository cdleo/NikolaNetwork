/* 
 * File:   PingerThread.h
 * Author: cleo
 *
 * Created on March 30, 2014, 3:33 PM
 */

#ifndef PINGERTHREAD_H
#define	PINGERTHREAD_H

#include <threads/Thread.h>
#include <sockets/Ping.h>

class PingerThread: public Thread {
public:
    PingerThread(std::string remoteIP);
    virtual ~PingerThread();

    std::string getRemoteIP() { return remoteIP_; }
    
    bool isAlive() { return result_; }

    virtual void run();
        
private:
    Ping* p;
    
    std::string remoteIP_;
    bool        result_;
};


#endif	/* PINGERTHREAD_H */


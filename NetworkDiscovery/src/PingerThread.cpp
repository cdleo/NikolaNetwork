/* 
 * File:   PingerThread.cpp
 * Author: cleo
 * 
 * Created on March 30, 2014, 3:33 PM
 */

#include <PingerThread.h>

PingerThread::PingerThread(std::string remoteIP) 
: remoteIP_(remoteIP), result_(false) {
    p = new Ping( remoteIP_.c_str(), 250000);
}

PingerThread::~PingerThread() {
    if (p){
        delete p;
        p = NULL;
    }
}

void PingerThread::run() {
    PingReply pr = p->Send();
    result_ = (strlen(pr.error) == 0);
    return;
}


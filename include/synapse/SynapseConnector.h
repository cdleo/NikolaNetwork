/* 
 * File:   SynapseConnector.h
 * Author: cleo
 *
 * Created on July 4, 2014, 3:56 PM
 */

#ifndef SYNAPSECONNECTOR_H
#define	SYNAPSECONNECTOR_H

#include <queue>

#include <synapse/Packet.h>

#include <sockets/IEvents.h>
//#include <sockets/PeerSSL_Async.h>
#include <sockets/PeerINET_Async.h>

class SynapseConnector : public PeerINET_Async, public IEvents {
public:
    SynapseConnector(const char* certificateFilePath, const char* keyFilePath);
    virtual ~SynapseConnector();
   
    virtual bool connectToHost(const char* ip, const int port, const int timeoutSeconds);
    virtual int setFD(int fd);    
    virtual void disconnect();
    
    virtual SocketReturnCode read(Packet*& msg);
    virtual SocketReturnCode write(Packet& msg);
    
    std::string getID();
    void updateID(const char* ip, const int port);
    
    SocketReturnCode onRead_event();
    SocketReturnCode onWrite_event(); 
    void onError_event();
    void onHangUp_event();  
    
private:    
    bool initialize();
    void disconnectAsync();
    
    void buildID(const char* ip, const int port);
    
    static void* keepAlive(void* peer);
    
    Packet* createMsgFromHeader(Header& header);
    
    std::string         id_;
    pthread_t           keepAliveThread_;
    
    //pthread_mutex_t     mutex_;
    
    pthread_mutex_t     writeMutex_;
    std::string         writeBuffer_;
    
    pthread_mutex_t     readMutex_;
    std::queue<Packet*> readBuffer_;
    
};

#endif	/* SYNAPSECONNECTOR_H */


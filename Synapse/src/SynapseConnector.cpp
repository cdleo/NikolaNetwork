/* 
 * File:   SynapseConnector.cpp
 * Author: cleo
 * 
 * Created on July 4, 2014, 3:56 PM
 */

#include <synapse/SynapseConnector.h>
#include <synapse/Messages.h>

#include <sockets/Poller.h>
#include <threads/ScopedLock.h>

#include <sys/ioctl.h>
#include <linux/if.h>
#include <sstream>

void* disconnectAsyncHandler(void* arg){
    SynapseConnector* pThis = static_cast<SynapseConnector*>(arg);
    pThis->disconnect();
    pthread_exit(0); 
}

SynapseConnector::SynapseConnector(const char* certificateFilePath, const char* keyFilePath)
: PeerINET_Async(), id_(""), keepAliveThread_(0), writeBuffer_("") {
    //pthread_mutex_init(&mutex_, NULL);
    pthread_mutex_init(&readMutex_, NULL);
    pthread_mutex_init(&writeMutex_, NULL);
}

SynapseConnector::~SynapseConnector(){
    disconnect();
    pthread_mutex_destroy(&writeMutex_);
    pthread_mutex_destroy(&readMutex_);
    //pthread_mutex_destroy(&mutex_);
}

bool SynapseConnector::connectToHost(const char* ip, const int port, const int timeoutSeconds){
    
    //clean();
    if (PeerINET_Async::connectToHost(ip,port,timeoutSeconds)){
        buildID(ip, port);
        return initialize();
    }
    return false;    
}

int SynapseConnector::setFD(int fd){

    //clean();
    if (PeerINET_Async::setFD(fd) == SOCKET_OK_CODE){
        buildID(getRemoteIP().c_str(), getRemotePort());
        if (initialize())
            return SOCKET_OK_CODE;
    }
    return SOCKET_ERROR_CODE;  
}

void SynapseConnector::disconnectAsync(){
    pthread_t disconnectThread;
    pthread_create (&disconnectThread, NULL, disconnectAsyncHandler, this);  
}

void SynapseConnector::disconnect(){
    
    if (isConnected()) {        
        POLLER->rm_fd( getFD() );
        PeerINET_Async::disconnect();      
    } 
    
    if (keepAliveThread_ != 0) {
        pthread_join(keepAliveThread_, NULL);
        keepAliveThread_ = 0;
    }    
    
    pthread_mutex_lock(&readMutex_);
    while (readBuffer_.size() > 0){
        delete readBuffer_.front();
        readBuffer_.pop();            
    }
    pthread_mutex_unlock(&readMutex_);

    pthread_mutex_lock(&writeMutex_);
    writeBuffer_.clear();
    pthread_mutex_unlock(&writeMutex_);           
    
}

void SynapseConnector::updateID(const char* ip, const int port){
    buildID(ip, port);
}

void SynapseConnector::buildID(const char* ip, const int port){
    
    pthread_mutex_lock(&mutex_);
    
    std::ostringstream stream;
    stream << ip << ":" << port;
    id_.assign( stream.str() );
    
    pthread_mutex_unlock(&mutex_);
}

std::string SynapseConnector::getID() {
    ScopedLock lock(mutex_);
    return id_; 
}

bool SynapseConnector::initialize(){
    
    if (!POLLER->add_fd( getFD(), this ))
        return false;
    
    POLLER->set_pollin( getFD() );
    
    /*if ( pthread_create (&keepAliveThread_, NULL, keepAlive, this) != 0 ) {   
        LOG->LogMessage(LOG_WARNING, "SynapseConnector::bind(): Unable to create KeepAlive thread... (pthread_create())");
    }*/
    
    Request ident(CommandID_Ident);
    return (write(ident) == SocketReturnCode_OK);
}

void* SynapseConnector::keepAlive(void* peer) {
    
    SynapseConnector* pThis = reinterpret_cast<SynapseConnector*>(peer);
    int counter = 0;
    
    while (pThis->isConnected()) {
        counter++;
        if (counter > 60) {            //Chequeamos el estado x segundo, pero enviamos el ping x minuto
            Request ping(CommandID_Ping);
            if (pThis->write(ping) != SocketReturnCode_OK){
                pThis->disconnectAsync();
                pthread_exit(0); 
            }
            counter = 0;
        }
        sleep(1);
    }
    pthread_exit(0); 
}

SocketReturnCode SynapseConnector::read(Packet*& msg) {
    
    if (!isConnected()) return SocketReturnCode_Error;
    
    SocketReturnCode rc = SocketReturnCode_TimedOut;
    
    pthread_mutex_lock(&readMutex_);
    if (readBuffer_.size()) {
        msg = readBuffer_.front();
        readBuffer_.pop();
        rc = SocketReturnCode_OK;
    } 
    pthread_mutex_unlock(&readMutex_);    
    
    return rc;
}

SocketReturnCode SynapseConnector::write(Packet& msg) {
    
    if (!isConnected()) return SocketReturnCode_Error;
    
    pthread_mutex_lock(&writeMutex_);
    writeBuffer_.append( msg.encode() );
    POLLER->set_pollout( getFD() );
    pthread_mutex_unlock(&writeMutex_);
    return SocketReturnCode_OK;
}

SocketReturnCode SynapseConnector::onRead_event() {
        
    Header aux;
    SocketReturnCode rc = gRead(&aux, aux.getLength());
    if (rc != SocketReturnCode_OK)
        return rc;        

    Packet* msg = createMsgFromHeader(aux);
    if (!msg) return SocketReturnCode_InvalidData;    
    
    size_t bodySize = (msg->getLength() > sizeof(Header)) ? (msg->getLength() - sizeof(Header)) : 0;
    
    if (bodySize) {
        char bodyAux[bodySize];

        rc = gRead(bodyAux, bodySize);
        if (rc != SocketReturnCode_OK) {
            delete msg;
            return rc;
        }
        
        if (!msg->decode( bodyAux, bodySize )) {
            delete msg;
            return SocketReturnCode_InvalidData;
        }
    }
    
    pthread_mutex_lock(&readMutex_);
    readBuffer_.push( msg );
    pthread_mutex_unlock(&readMutex_);
    
    return rc;    
}

SocketReturnCode SynapseConnector::onWrite_event() {
    pthread_mutex_lock(&writeMutex_);
    SocketReturnCode result = gWrite( writeBuffer_.c_str(), writeBuffer_.length());
    writeBuffer_.clear();
    POLLER->reset_pollout( getFD() );
    pthread_mutex_unlock(&writeMutex_);
    
    if (result == SocketReturnCode_Error)
        disconnect();
    
    return result;
}

void SynapseConnector::onError_event(){
     disconnect();
}

void SynapseConnector::onHangUp_event(){
     disconnect();
}

Packet* SynapseConnector::createMsgFromHeader(Header& header) {
    
    if (header.getMsgType() == MsgType_Request) {
        switch(header.getCommandID()) {
            case CommandID_Ident:
            case CommandID_Services:
            case CommandID_Capabilities:
            case CommandID_Ping:
                return new Request(header);

            case CommandID_GetValue:
                switch(header.getSubCommandID()) {   
                   case SubCommandID_GetNetwork:
                        return new GetServiceData(header);
                        
                    case SubCommandID_GetUserHash:
                        return new UserValidation(header);

                    case SubCommandID_GetUserSecurityLevel:
                        return new UserSecurityLevelQuery(header);                        
                    
                    case SubCommandID_AnalogValue:
                        return new GetAnalogValue(header);

                    case SubCommandID_DigitalValue:
                        return new GetDigitalValue(header);                            

                    default:
                        return new GetValue(header);
                }     
                
            case CommandID_SetValue:
                switch(header.getSubCommandID()) {                                  
                    case SubCommandID_AnalogValue:
                        return new SetAnalogValue(header);

                    case SubCommandID_DigitalValue:
                        return new SetDigitalValue(header);                        
                    
                    case SubCommandID_Undefined:
                    default:
                        return new SetValue(header);
                }                 

            case CommandID_Subscribe:
                return new Subscribe(header);                

            case CommandID_Unsubscribe:
                return new Unsubscribe(header);                                             

            case CommandID_Revoke:
                return new RevokeSubscription(header);                
                
            default:
                break;
        }    
        
    } else if (header.getMsgType() == MsgType_Response) {
        switch(header.getCommandID()) {
            case CommandID_Ident:
                return new IdentResp(header);

            case CommandID_Services:
                return new ServicesResp(header);                
                
            case CommandID_Capabilities:
                return new CapabilitiesResp(header);

            case CommandID_Ping:
                return new Response(header);                                

            case CommandID_GetValue:
                switch(header.getSubCommandID()) {          
                    case SubCommandID_GetNetwork:
                        return new NetworkResp(header);

                    case SubCommandID_GetUserHash:
                        return new UserValidationResp(header);

                    case SubCommandID_GetUserSecurityLevel:
                        return new UserSecurityLevelQueryResp(header);                        
                        
                    case SubCommandID_AnalogValue:
                        return new GetAnalogValueResp(header);

                    case SubCommandID_DigitalValue:
                        return new GetDigitalValueResp(header);                              
                        
                    default:
                        return new GetValueResp(header);
                }            

            case CommandID_SetValue:
                switch(header.getSubCommandID()) {                                  
                    case SubCommandID_AnalogValue:
                    case SubCommandID_DigitalValue:                    
                    default:
                        return new SetValueResp(header);
                }                
                
            case CommandID_Subscribe:
                return new SubscribeResp(header);

            case CommandID_Unsubscribe:
                return new UnsubscribeResp(header);                                   
                
            case CommandID_Revoke:
                return new RevokeSubscriptionResp(header);                      
                
            case CommandID_Update:
                switch(header.getSubCommandID()) {          
                    case SubCommandID_GetNetwork:
                        return new NetworkUpdate(header);

                    case SubCommandID_AnalogValue:
                        return new AnalogValueUpdate(header);

                    case SubCommandID_DigitalValue:
                        return new DigitalValueUpdate(header);                        

                    case SubCommandID_GetUserRevoked:
                        return new RevokedUserUpdate(header);                        
                        
                    case SubCommandID_ResourcesUpdate:
                        return new ResourcesUpdate(header);                         
                        
                    default:
                        break;                   
                }                
                
            default:
                break;
        }        
    }
    
    LOG->LogMessage(LOG_WARNING, "SynapseConnector::createMsgFromHeader(): Invalid MsgType=%d CommandID=%d SubCommandID=%d", header.getMsgType(), header.getCommandID(), header.getSubCommandID());
    return NULL;
}


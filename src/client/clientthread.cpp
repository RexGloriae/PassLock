#include "clientthread.h"

ClientThread* ClientThread::m_inst = nullptr;

void ClientThread::start() {
    if (ClientThread::m_inst == nullptr) ClientThread::m_inst = new ClientThread;
    // Run Winsock client in a separate thread to avoid blocking the main thread
    std::thread clientThread(&ClientThread::run_client, ClientThread::m_inst);
    clientThread.detach();  // Detach the thread to let it run independently
}

ClientThread& ClientThread::get(){
    return *(ClientThread::m_inst);
}

void ClientThread::stop(){
    if(ClientThread::m_inst != nullptr){
        stopClient(ClientThread::m_inst->socket);
        delete ClientThread::m_inst;
    }
    ClientThread::m_inst = nullptr;
}

void ClientThread::run_client(){
    this->socket = startClient();
}

void ClientThread::sendPayload(const char* buff, int len) {
    send(*(this->socket), buff, len, 0);
}

void ClientThread::receiveFromServer(char* buff, int& len){
    int recvResult = recv(*(this->socket), buff, 1024, 0);
        if (recvResult > 0) {
            buff[recvResult] = '\0';  // Null-terminate the received data
            len = recvResult;
        }
}

void ClientThread::receiveFromServer(char* buff, int& len, int flag){
    int recvResult = recv(*(this->socket), buff, len, flag);
    if (recvResult > 0) {
        buff[recvResult] = '\0';  // Null-terminate the received data
        len = recvResult;
    }
}

void ClientThread::receiveFromServer(char* buff){
    int recvResult = recv(*(this->socket), buff, 1, 0);

}

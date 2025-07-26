#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <thread>
#include "client.h"
#include <string>

class ClientThread {
private:
    SOCKET* socket;

    void run_client();

    static ClientThread* m_inst;
    ClientThread() = default;
    ~ClientThread() = default;

public:
    ClientThread(const ClientThread& other) = delete;
    ClientThread(ClientThread&& other) = delete;
    static void start();
    static ClientThread& get();
    static void stop();
    void sendPayload(const char* buff, int len);
    void receiveFromServer(char* buff, int& len);
    void receiveFromServer(char* buff, int& len, int flag);
    void receiveFromServer(char* buff);
};

#endif // CLIENTTHREAD_H

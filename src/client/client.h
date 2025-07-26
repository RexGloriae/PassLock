#ifndef CLIENT_H
#define CLIENT_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"  // Server IP address (localhost for testing)
#define SERVER_PORT 54000      // Port to connect to

SOCKET* startClient();
void stopClient(SOCKET* socket);

#endif // CLIENT_H

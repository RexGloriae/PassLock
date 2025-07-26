#include "RegisterManager.h"
#include <QDebug>
#include "clientthread.h"
#include "packager.h"

RegisterManager::RegisterManager(QObject *parent) : QObject(parent) {}

// Login function that can be called from QML
void RegisterManager::reg(const QString &username, const QString &password, const QString &email)
{
    std::vector<char> payload =
        Packager::get().package_register_data(username.toStdString(),
                                              password.toStdString(),
                                              email.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
    char answ[32];
    int len = 32;
    ClientThread::get().receiveFromServer(answ, len);;
    if (answ[1] == 1) {
        emit registerResult(1);
    }
    else emit registerResult(0);
}

#include "DataChanger.h"
#include <QDebug>
#include "clientthread.h"
#include "packager.h"

DataChanger::DataChanger(QObject *parent) : QObject(parent) {}

Q_INVOKABLE void DataChanger::change_email(const QString& username, const QString& new_email) {
    std::vector<char> payload =
        Packager::get().package_email_change_data(username.toStdString(),
                                                  new_email.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
    char answ[32];
    int len = 32;
    ClientThread::get().receiveFromServer(answ, len);
    if(answ[1] == 1) {
        emit emailChangeResult(1);
    }
    else emit emailChangeResult(0);
}

Q_INVOKABLE void DataChanger::change_pass(const QString& username, const QString& new_pass) {
    std::vector<char> payload =
        Packager::get().package_pass_change_data(username.toStdString(),
                                                  new_pass.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
}

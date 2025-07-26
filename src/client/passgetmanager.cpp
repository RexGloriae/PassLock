#include "passgetmanager.h"

#include "packager.h"
#include "clientthread.h"
#include "crypto.h"
#include "interpreter.h"

#include <QDebug>

PassGetManager::PassGetManager(QObject *parent) : QObject(parent) {}

Q_INVOKABLE QVariantList PassGetManager::get(const QString& username) {
    QVariantList pass_list;
    QMap<QString, QString> pass;

    std::vector<char> payload = Packager::get().package_password_requests(username.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
    char answ[1064];
    int answ_len;
    ClientThread::get().receiveFromServer(answ);
    ClientThread::get().receiveFromServer(answ + 1);
    int num_of_passwords = answ[1];
    for (int i = 0; i < num_of_passwords; i++) {
        char password_payload[1064];
        char aux_frame_len[1];
        ClientThread::get().receiveFromServer(aux_frame_len);
        int frame_len = aux_frame_len[0];

        ClientThread::get().receiveFromServer(password_payload, frame_len, MSG_WAITALL);

        pass = Interpreter::get().retrieve_password_from_payload(password_payload);
        QVariantMap pass_map;
        pass_map["platform"] = pass["platform"];
        pass_map["username"] = pass["username"];
        pass_map["password"] = pass["password"];
        pass_list.append(pass_map);
    }

    return pass_list;
}

Q_INVOKABLE void PassGetManager::del(const QString& username, const QString& password) {
    std::vector<char> payload = Packager::get().package_pass_del(username.toStdString(), password.toStdString());

    ClientThread::get().sendPayload(payload.data(), payload.size());
}

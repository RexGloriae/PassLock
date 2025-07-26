#include "LoginManager.h"
#include <QDebug>
#include "clientthread.h"
#include "packager.h"
#include "interpreter.h"
#include "crypto.h"

LoginManager::LoginManager(QObject *parent) : QObject(parent) {}

// Login function that can be called from QML
void LoginManager::login(const QString &username, const QString &password)
{
    std::vector<char> payload =
        Packager::get().package_login_data(username.toStdString(),
                                           password.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
    char answ[32];
    int len = 32;
    ClientThread::get().receiveFromServer(answ, len);
    if (answ[1] == 1) {
        emit loginResult(1);
        payload = Packager::get().package_salt_request(username.toStdString());
        ClientThread::get().sendPayload(payload.data(), payload.size());
        char buff[1064];
        int buff_len = 1064;
        ClientThread::get().receiveFromServer(buff, buff_len);
        std::vector<char> salt =
            Interpreter::get().retrieve_salt_from_payload(buff, buff_len);
        Crypto::get().set_user_salt(salt);
        set_user(username);
    }
    else emit loginResult(0);
}

Q_INVOKABLE QString LoginManager::get_user() {
    return m_last_logged_user;
}

Q_INVOKABLE void LoginManager::logout() {
    set_user("None");
}

void LoginManager::set_user(const QString& username) {
    if (m_last_logged_user != username) {
        m_last_logged_user = username;
        emit userChanged();
    }
}

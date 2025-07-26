#include "passwordreset.h"

#include "packager.h"
#include "clientthread.h"

PasswordReset::PasswordReset(QObject *parent) : QObject(parent) {}

Q_INVOKABLE void PasswordReset::send_recovery_code_on_email(const QString& username) {
    std::vector<char> payload =
        Packager::get().package_recovery_code_request(username.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
    char answ[32];
    int len = 32;
    ClientThread::get().receiveFromServer(answ, len);
    if(answ[1] == 1) {
        emit codeSent(1);
    }
    else emit codeSent(0);
}

Q_INVOKABLE void PasswordReset::check_recovery_code(const QString& username, const QString& code) {
    std::vector<char> payload =
        Packager::get().package_recovery_code_check(username.toStdString(), code.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
    char answ[32];
    int len = 32;
    ClientThread::get().receiveFromServer(answ, len);
    if (answ[1] == 1) {
        emit codeOkay(1);
    }
    else emit codeOkay(0);
}

Q_INVOKABLE void PasswordReset::change_pass(const QString& username, const QString& new_pass) {
    std::vector<char> payload =
        Packager::get().package_pass_change_data(username.toStdString(),
                                                 new_pass.toStdString());
    ClientThread::get().sendPayload(payload.data(), payload.size());
}

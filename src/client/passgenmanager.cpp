#include "passgenmanager.h"
#include "packager.h"
#include "clientthread.h"
#include "crypto.h"

#include <QDebug>

PassGenManager::PassGenManager(QObject *parent) : QObject(parent) {}

Q_INVOKABLE void PassGenManager::gen(int len,
                                     bool special_ch_flag,
                                     bool digit_flag,
                                     bool capital_flag,
                                     bool non_capital_flag,
                                     const QString& logged_user,
                                     const QString& username,
                                     const QString& platform) {

    std::vector<char> payload =
        Packager::get().package_pass_gen_data(len,
                                              special_ch_flag,
                                              digit_flag,
                                              capital_flag,
                                              non_capital_flag,
                                              logged_user.toStdString(),
                                              username.toStdString(),
                                              platform.toStdString());

    ClientThread::get().sendPayload(payload.data(), payload.size());
    char answer[1064];
    int answ_len = 1064;
    ClientThread::get().receiveFromServer(answer, answ_len);
    int enc_len = answer[1];
    std::vector<char> enc_pass;
    int i = 2;
    while(enc_len--){
        enc_pass.push_back(answer[i++]);
    }
    std::vector<char> dec_pass =
        Crypto::get().aes_256_cbc_decr(enc_pass);
    QString string_pass;
    for(int i = 0; i < dec_pass.size(); i++) {
        string_pass.push_back(dec_pass[i]);
    }
    emit generatedPass(string_pass);
}

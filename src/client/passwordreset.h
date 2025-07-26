#ifndef PASSWORDRESET_H
#define PASSWORDRESET_H

#include <QObject>
#include <QString>

class PasswordReset : public QObject
{
    Q_OBJECT
public:
    explicit PasswordReset(QObject *parent = nullptr);

    Q_INVOKABLE void send_recovery_code_on_email(const QString& username);
    Q_INVOKABLE void check_recovery_code(const QString& username, const QString& code);
    Q_INVOKABLE void change_pass(const QString& username, const QString& pass);

signals:
    void codeSent(int res);
    void codeOkay(int res);
};

#endif // PASSWORDRESET_H

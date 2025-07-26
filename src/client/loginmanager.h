#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QObject>
#include <QString>

class LoginManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString user READ get_user NOTIFY userChanged)
private:
    QString m_last_logged_user;

public:
    explicit LoginManager(QObject *parent = nullptr);

    Q_INVOKABLE void login(const QString &username, const QString &password);
    Q_INVOKABLE QString get_user();
    void set_user(const QString& username);
    Q_INVOKABLE void logout();

signals:
    void loginResult(int res);
    void userChanged();
};

#endif // LOGINMANAGER_H

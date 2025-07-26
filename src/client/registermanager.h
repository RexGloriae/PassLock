#ifndef REGISTERMANAGER_H
#define REGISTERMANAGER_H

#include <QObject>
#include <QString>

class RegisterManager : public QObject
{
    Q_OBJECT

public:
    explicit RegisterManager(QObject *parent = nullptr);

    Q_INVOKABLE void reg(const QString &username, const QString &password, const QString &email);

signals:
    void registerResult(int res);
};

#endif // REGISTERMANAGER_H

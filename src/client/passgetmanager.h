#ifndef PASSGETMANAGER_H
#define PASSGETMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

class PassGetManager : public QObject
{
    Q_OBJECT
private:

public:
    explicit PassGetManager(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList get(const QString& username);
    Q_INVOKABLE void del(const QString& username, const QString& password);
};

#endif // PASSGETMANAGER_H

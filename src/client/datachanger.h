#ifndef DATACHANGER_H
#define DATACHANGER_H

#include <QObject>
#include <QString>

class DataChanger : public QObject
{
    Q_OBJECT

public:
    explicit DataChanger(QObject *parent = nullptr);

    Q_INVOKABLE void change_email(const QString& username, const QString& new_email);
    Q_INVOKABLE void change_pass(const QString& username, const QString& pass);

signals:
    void emailChangeResult(int res);
};

#endif // DATACHANGER_H

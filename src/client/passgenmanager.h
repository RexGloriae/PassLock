#ifndef PASSGENMANAGER_H
#define PASSGENMANAGER_H

#include <QObject>
#include <QString>

class PassGenManager : public QObject
{
    Q_OBJECT
private:

public:
    explicit PassGenManager(QObject *parent = nullptr);

    Q_INVOKABLE void gen(int len,
                         bool special_ch_flag,
                         bool digit_flag,
                         bool capital_flag,
                         bool non_capital_flag,
                         const QString& logged_user,
                         const QString& username,
                         const QString& platform);
signals:
    void generatedPass(const QString& pass);
};

#endif // PASSGENMANAGER_H

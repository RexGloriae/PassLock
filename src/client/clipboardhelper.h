// clipboardhelper.h
#pragma once
#include <QObject>
#include <QClipboard>
#include <QGuiApplication>

class ClipboardHelper : public QObject {
    Q_OBJECT
public:
    using QObject::QObject;

    Q_INVOKABLE void copyText(const QString& text);
};

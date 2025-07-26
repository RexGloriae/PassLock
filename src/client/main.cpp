#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QClipboard>
#include <QQmlContext>
#include "clientthread.h"
#include "loginmanager.h"
#include "crypto.h"
#include "registermanager.h"
#include "exception.h"
#include "packager.h"
#include "interpreter.h"
#include "passgenmanager.h"
#include "passgetmanager.h"
#include "clipboardhelper.h"
#include "datachanger.h"
#include "passwordreset.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/imgs/passlock.ico"));

    Packager::init();
    Crypto::init();
    Interpreter::init();
    ClientThread::start();

    qmlRegisterType<LoginManager>("com.login", 1, 0, "LoginManager");
    qmlRegisterType<RegisterManager>("com.register", 1, 0, "RegisterManager");
    qmlRegisterType<PassGenManager>("com.passgen", 1, 0, "PassGenManager");
    qmlRegisterType<PassGetManager>("com.passget", 1, 0, "PassGetManager");
    qmlRegisterType<ClipboardHelper>("com.clipboard", 1, 0, "ClipboardHelper");
    qmlRegisterType<DataChanger>("com.datachanger", 1, 0, "DataChanger");
    qmlRegisterType<PasswordReset>("com.passreset", 1, 0, "PasswordReset");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("clipboard", QGuiApplication::clipboard());
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Client", "Main");

    QObject::connect(&app, &QGuiApplication::aboutToQuit, [&]() {
        ClientThread::stop();
        Packager::kill();
        Crypto::kill();
        Interpreter::kill();
    });

    int result = app.exec();
    return result;
}

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Payload.h"

#include <vector>
#include <QString>
#include <QMap>

class Interpreter {
private:
    static Interpreter* m_inst;

    Interpreter() = default;
    ~Interpreter() = default;

public:
    Interpreter(const Interpreter& other) = delete;
    Interpreter(Interpreter&& other) = delete;

    static void         init();
    static Interpreter& get();
    static void         kill();

    std::vector<char> retrieve_salt_from_payload(const char* payload, int len);

    QMap<QString, QString> retrieve_password_from_payload(const char* payload);
};

#endif

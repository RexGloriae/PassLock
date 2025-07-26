#ifndef PACKAGER_H
#define PACKAGER_H

#include "Payload.h"

#include <vector>
#include <string>

class Packager
{
private:
    static Packager* m_inst;

    Packager() = default;
    ~Packager() = default;
public:
    Packager(const Packager& other) = delete;
    Packager(Packager&& other) = delete;

    static void init();
    static Packager& get();
    static void kill();

    std::vector<char> package_login_data(std::string username,
                                         std::string password);
    std::vector<char> package_register_data(std::string username,
                                            std::string password,
                                            std::string email);
    std::vector<char> package_salt_request(std::string username);
    std::vector<char> package_pass_gen_data(
        int len,
        bool special_ch_flag,
        bool digit_flag,
        bool capital_flag,
        bool non_capital_flag,
        const std::string& logged_user,
        const std::string& username,
        const std::string& platform);
    std::vector<char> package_password_requests(std::string username);
    std::vector<char> package_pass_del(std::string username, std::string password);
    std::vector<char> package_email_change_data(std::string username, std::string email);
    std::vector<char> package_pass_change_data(std::string username, std::string pass);
    std::vector<char> package_recovery_code_request(std::string username);
    std::vector<char> package_recovery_code_check(std::string username, std::string code);
};

#endif // PACKAGER_H

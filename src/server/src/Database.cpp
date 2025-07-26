#include "../include/Database.hxx"

#include "../include/Exception.hxx"

Database* Database::m_inst = nullptr;

void Database::init(std::string name) {
    if (Database::m_inst == nullptr) {
        Database::m_inst = new Database();
        Database::m_inst->m_db = nullptr;
    } else {
        return;
    }

    std::string file_path = Database::m_inst->create_path(name);

    if (sqlite3_open(file_path.c_str(), &(Database::m_inst->m_db)) !=
        SQLITE_OK) {
        Exception e("Error opening db: " + std::string(sqlite3_errmsg(
                                               Database::m_inst->m_db)),
                    0x01);
        Database::m_inst->m_db = nullptr;
        throw e;
    }

    Database::m_inst->createTables();
}
Database& Database::get() { return *(Database::m_inst); }
void      Database::kill() {
    if (Database::m_inst != nullptr) {
        if (Database::m_inst->m_db != nullptr) {
            sqlite3_close(Database::m_inst->m_db);
            Database::m_inst->m_db = nullptr;
        }
        delete Database::m_inst;
    }
    Database::m_inst = nullptr;
}

std::string Database::create_path(std::string file) {
    std::string path = this->m_path + file;
    return path;
}

#include <iostream>
void Database::executeQuery(const std::string& query) {
    char* errmsg;
    if (sqlite3_exec(
            this->m_db, query.c_str(), nullptr, nullptr, &errmsg) !=
        SQLITE_OK) {
        Exception e("SQL ERROR: " + std::string(errmsg), 0x01);
        sqlite3_free(errmsg);
        std::cout << e.what();
        throw e;
    }
}

void Database::createTables() {
    std::string t1 = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username BLOB UNIQUE NOT NULL,
            salt BLOB NOT NULL,
            password BLOB NOT NULL,
            email BLOB UNIQUE NOT NULL,
            recovery BLOB NOT NULL
        );
    )";

    std::string t2 = R"(
        CREATE TABLE IF NOT EXISTS passwords (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            platform BLOB NOT NULL,
            encrypted_password BLOB NOT NULL,
            associated_username BLOB NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        );
    )";

    this->executeQuery(t1);
    this->executeQuery(t2);
}

void Database::add_new_user(const std::vector<char>& username,
                            const std::vector<char>& pass_hash,
                            const std::vector<char>& salt,
                            const std::vector<char>& email,
                            const std::vector<char>& recovery) {
    std::string query =
        "INSERT INTO users (username, salt, password, email, recovery) "
        "VALUES (?, "
        "?, ?, "
        "?, ?);";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, salt.data(), salt.size(), SQLITE_STATIC);
    sqlite3_bind_blob(
        stmt, 3, pass_hash.data(), pass_hash.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 4, email.data(), email.size(), SQLITE_STATIC);
    sqlite3_bind_blob(
        stmt, 5, recovery.data(), recovery.size(), SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw Exception(
            "SQL Insert Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_finalize(stmt);
}

void Database::insert_new_password(
    const std::vector<char>& username,
    const std::vector<char>& associated_username,
    const std::vector<char>& platform,
    const std::vector<char>& pass) {
    std::string query = R"(
            INSERT INTO passwords (user_id, platform, associated_username, encrypted_password)
            VALUES ((SELECT id FROM users WHERE username = ?), ?, ?, ?);
        )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);
    sqlite3_bind_blob(
        stmt, 2, platform.data(), platform.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt,
                      3,
                      associated_username.data(),
                      associated_username.size(),
                      SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 4, pass.data(), pass.size(), SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        throw Exception(
            "SQL Insert Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_finalize(stmt);
}

std::vector<char> Database::get_password_hash(
    const std::vector<char>& username) {
    std::string   query = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);

    std::vector<char> encrypted_hash;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        encrypted_hash = std::vector<char>(
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)) +
                sqlite3_column_bytes(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return encrypted_hash;
}

std::vector<char> Database::get_user_salt(
    const std::vector<char>& username) {
    std::string   query = "SELECT salt FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);

    std::vector<char> salt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        salt = std::vector<char>(
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)) +
                sqlite3_column_bytes(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return salt;
}

std::vector<char> Database::get_user_email(
    const std::vector<char>& username) {
    std::string   query = "SELECT email FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);

    std::vector<char> email;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        email = std::vector<char>(
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)) +
                sqlite3_column_bytes(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return email;
}

std::vector<char> Database::get_user_recovery_code(
    const std::vector<char>& username) {
    std::string   query = "SELECT recovery FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);

    std::vector<char> recovery;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        recovery = std::vector<char>(
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)) +
                sqlite3_column_bytes(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return recovery;
}

void Database::add_recovery_code(const std::vector<char>& username,
                                 const std::vector<char>& code) {
    std::string query =
        "UPDATE users SET recovery = ? WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    // Bind the new recovery code
    sqlite3_bind_blob(stmt, 1, code.data(), code.size(), SQLITE_STATIC);

    // Bind the username
    sqlite3_bind_blob(
        stmt, 2, username.data(), username.size(), SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw Exception(
            "SQL Execution Error: " + std::string(sqlite3_errmsg(m_db)),
            0x02);
    }

    sqlite3_finalize(stmt);
}

void Database::delete_user(const std::vector<char>& username) {
    std::string query = "DELETE FROM users WHERE username = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        Exception e("SQL error: " + std::string(sqlite3_errmsg(m_db)),
                    0x01);
        throw e;
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Exception e(
            "Failed to delete user: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
        sqlite3_finalize(stmt);
        throw e;
    }

    sqlite3_finalize(stmt);
}

void Database::delete_password(const std::vector<char>& username,
                               const std::vector<char>& pass) {
    std::string query =
        "DELETE FROM passwords WHERE encrypted_password = ? AND user_id = "
        "(SELECT id FROM "
        "users WHERE username = ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        Exception e("SQL error: " + std::string(sqlite3_errmsg(m_db)),
                    0x01);
        throw e;
    }

    sqlite3_bind_blob(stmt, 1, pass.data(), pass.size(), SQLITE_STATIC);
    sqlite3_bind_blob(
        stmt, 2, username.data(), username.size(), SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        Exception e("Failed to delete password: " +
                        std::string(sqlite3_errmsg(m_db)),
                    0x01);
        sqlite3_finalize(stmt);
        throw e;
    }

    sqlite3_finalize(stmt);
}

std::vector<
    std::tuple<std::vector<char>, std::vector<char>, std::vector<char>>>
Database::get_all_passwords_for_user(const std::vector<char>& username) {
    std::string query = R"(
        SELECT platform, encrypted_password, associated_username 
        FROM passwords 
        WHERE user_id = (SELECT id FROM users WHERE username = ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        Exception e("SQL error: " + std::string(sqlite3_errmsg(m_db)),
                    0x01);
        throw e;
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);

    std::vector<std::tuple<std::vector<char>,
                           std::vector<char>,
                           std::vector<char>>>
        passwords;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const void*       platform_blob = sqlite3_column_blob(stmt, 0);
        int               platform_size = sqlite3_column_bytes(stmt, 0);
        std::vector<char> platform((char*)platform_blob,
                                   (char*)platform_blob + platform_size);

        const void* encrypted_password_blob = sqlite3_column_blob(stmt, 1);
        int encrypted_password_size = sqlite3_column_bytes(stmt, 1);
        std::vector<char> encrypted_password(
            (char*)encrypted_password_blob,
            (char*)encrypted_password_blob + encrypted_password_size);

        const void*       assoc_user_blob = sqlite3_column_blob(stmt, 2);
        int               assoc_user_size = sqlite3_column_bytes(stmt, 2);
        std::vector<char> assoc_user(
            (char*)assoc_user_blob,
            (char*)assoc_user_blob + assoc_user_size);

        passwords.push_back(
            std::make_tuple(platform, encrypted_password, assoc_user));
    }

    sqlite3_finalize(stmt);
    return passwords;
}

bool Database::is_collision(const std::vector<char>& username,
                            const std::vector<char>& platform,
                            const std::vector<char>& associated_username) {
    // Step 1: Get user_id from username
    std::string getUserIdQuery = "SELECT id FROM users WHERE username = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(
            m_db, getUserIdQuery.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception("SQL Prepare Error (get user id): " +
                            std::string(sqlite3_errmsg(m_db)),
                        0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, username.data(), username.size(), SQLITE_STATIC);

    int user_id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user_id = sqlite3_column_int(stmt, 0);
    } else {
        sqlite3_finalize(stmt);
        return false;  // Username not found
    }
    sqlite3_finalize(stmt);

    // Step 2: Check if password entry exists
    std::string checkQuery = R"(
SELECT 1 FROM passwords
WHERE user_id = ? AND platform = ? AND associated_username = ?
LIMIT 1;
)";

    if (sqlite3_prepare_v2(m_db, checkQuery.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception("SQL Prepare Error (check password): " +
                            std::string(sqlite3_errmsg(m_db)),
                        0x01);
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_blob(
        stmt, 2, platform.data(), platform.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt,
                      3,
                      associated_username.data(),
                      associated_username.size(),
                      SQLITE_STATIC);

    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return exists;
}

void Database::update_user_email(const std::vector<char>& username,
                                 const std::vector<char>& new_email) {
    std::string   query = "UPDATE users SET email = ? WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) !=
        SQLITE_OK) {
        throw Exception(
            "SQL Prepare Error: " + std::string(sqlite3_errmsg(m_db)),
            0x01);
    }

    sqlite3_bind_blob(
        stmt, 1, new_email.data(), new_email.size(), SQLITE_STATIC);

    // Bind the username
    sqlite3_bind_blob(
        stmt, 2, username.data(), username.size(), SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw Exception(
            "SQL Execution Error: " + std::string(sqlite3_errmsg(m_db)),
            0x02);
    }

    sqlite3_finalize(stmt);
}
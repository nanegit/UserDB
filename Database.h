#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "User.h"


class DatabaseManager {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    bool connected;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect(const std::wstring& connStr);
    void disconnect();
    bool isConnected() const;

    bool insertUser(const User& user);
    bool getUserByUsername(const std::string& username, User& user);
};

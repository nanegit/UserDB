// Database.cpp 
#include "Database.h"
#include "User.h"
#include <iostream>
#include <windows.h>
using namespace std;

std::wstring s2ws(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

//std::string ws2s(const std::wstring& wstr) {
//    if (wstr.empty()) return std::string();
//
//    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
//    if (size_needed == 0) return std::string();
//
//    std::string strTo(size_needed - 1, 0);
//    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size_needed, NULL, NULL);
//
//    return strTo;
//}


DatabaseManager::DatabaseManager() : connected(false) {
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect(const std::wstring& connStr) {
    SQLRETURN ret = SQLDriverConnectW(hDbc, NULL, (SQLWCHAR*)connStr.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (SQL_SUCCEEDED(ret)) {
        connected = true;
        std::wcout << L"Connection successful!" << std::endl;
        return true;
    }
    else {
        SQLWCHAR state[1024], message[1024];
        if (SQLGetDiagRecW(SQL_HANDLE_DBC, hDbc, 1, state, NULL, message, 1024, NULL) == SQL_SUCCESS) {
            std::wcout << L"Connection failed!" << std::endl;
            std::wcout << L"SQL State: " << state << std::endl;
            std::wcout << L"Message: " << message << std::endl;
        }
        connected = false;
        return false;
    }
}

bool DatabaseManager::insertUser(const User& user) {
    if (!connected) {
        std::cerr << "Not connected to the database!" << std::endl;
        return false;
    }

    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::wstring query = L"INSERT INTO Users (Firstname, Lastname, Email, Phone, BornDate, Username, Password, Address) "
        L"VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

    SQLRETURN ret = SQLPrepareW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        std::cerr << "Failed to prepare statement." << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }


    std::wstring fn = s2ws(user.getFirstname());
    std::wstring ln = s2ws(user.getLastname());
    std::wstring em = s2ws(user.getEmail());
    std::wstring ph = s2ws(user.getPhonenumber());
    std::wstring bd = s2ws(user.getBorndate());
    std::wstring un = s2ws(user.getUsername());
    std::wstring pw = s2ws(user.getPassword());
    std::wstring ad = s2ws(user.getAddress());

    SQLLEN len = SQL_NTS;
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)fn.c_str(), 0, &len);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)ln.c_str(), 0, &len);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)em.c_str(), 0, &len);
    SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)ph.c_str(), 0, &len);
    SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)bd.c_str(), 0, &len);
    SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)un.c_str(), 0, &len);
    SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)pw.c_str(), 0, &len);
    SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)ad.c_str(), 0, &len);

    ret = SQLExecute(hStmt);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    if (!SQL_SUCCEEDED(ret)) {
        std::wcerr << L"[ERROR] Failed to execute insert." << std::endl;
        return false;
    }

    return true;
}

bool DatabaseManager::getUserByUsername(const std::string& username, User& user) {
    if (!connected) return false;  

    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::wstring query = L"SELECT Firstname, Lastname, Email, Phone, BornDate, Username, Password, Address FROM Users WHERE Username = ?";
    SQLPrepareW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);  

    std::wstring wusername(username.begin(), username.end());
    SQLLEN len = SQL_NTS;

    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)wusername.c_str(), 0, &len);

    SQLRETURN ret = SQLExecute(hStmt);
    if (!SQL_SUCCEEDED(ret)) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }

    wchar_t fn[100] = { 0 }, ln[100] = { 0 }, em[100] = { 0 }, ph[50] = { 0 };
    wchar_t bd[50] = { 0 }, un[50] = { 0 }, pw[50] = { 0 }, ad[255] = { 0 }; 
    SQLLEN ind;

    if (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_WCHAR, fn, sizeof(fn), &ind);
        SQLGetData(hStmt, 2, SQL_C_WCHAR, ln, sizeof(ln), &ind);
        SQLGetData(hStmt, 3, SQL_C_WCHAR, em, sizeof(em), &ind);
        SQLGetData(hStmt, 4, SQL_C_WCHAR, ph, sizeof(ph), &ind);
        SQLGetData(hStmt, 5, SQL_C_WCHAR, bd, sizeof(bd), &ind);
        SQLGetData(hStmt, 6, SQL_C_WCHAR, un, sizeof(un), &ind);
        SQLGetData(hStmt, 7, SQL_C_WCHAR, pw, sizeof(pw), &ind);
        SQLGetData(hStmt, 8, SQL_C_WCHAR, ad, sizeof(ad), &ind);

        user.setFirstname(std::string(fn, fn + wcslen(fn)));
        user.setLastname(std::string(ln, ln + wcslen(ln)));
        user.setEmail(std::string(em, em + wcslen(em)));
        user.setPhonenumber(std::string(ph, ph + wcslen(ph)));
        user.setBorndate(std::string(bd, bd + wcslen(bd)));
        user.setUsername(std::string(un, un + wcslen(un)));
        user.setPassword(std::string(pw, pw + wcslen(pw)));
        user.setAddress(std::string(ad, ad + wcslen(ad)));

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return false;
}



void DatabaseManager::disconnect() {
    if (connected) {
        SQLDisconnect(hDbc);
        connected = false;
    }
    if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

bool DatabaseManager::isConnected() const {
    return connected;
}
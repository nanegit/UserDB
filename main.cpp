#include <iostream>
#include <string>
#include "User.h"
#include "Database.h"
using namespace std;

int main() {
    DatabaseManager db;

    wstring connStr = L"DRIVER={ODBC Driver 17 for SQL Server};"
        L"SERVER=DESKTOP-0HV19S3\\SQLEXPRESS;"
        L"DATABASE=UserDB;"
        L"Trusted_Connection=yes;";

    if (!db.connect(connStr)) {
        wcerr << L"[ERROR] Failed to connect to database." << endl;
        return 1;
    }

    string command;
    cout << "Enter command (insert/get): ";
    getline(cin, command);

    if (command == "insert") {
        User user;
        user.run();

        if (db.insertUser(user)) {
            cout << "User inserted successfully.\n";
        }
        else {
            cerr << "Failed to insert user.\n";
        }
    }
    else if (command == "get") {
        string username;
        cout << "Enter username to fetch: ";
        getline(cin, username);

        User user;
        if (db.getUserByUsername(username, user)) {
            cout << "User found:\n";
            user.display();

            string format;
            cout << "Export format (json/excel): ";
            getline(cin, format);

            if (format == "json") {
                user.exportToJSON(username + ".json");
            }
            else if (format == "excel") {
                user.exportToExcel(username + ".csv");
            }
            else {
                cout << "Unknown format.\n";
            }
        }
        else {
            cout << "User not found.\n";
        }
    }
    else {
        cout << "Invalid command.\n";
    }

    db.disconnect();
    return 0;
}

#include "User.h"
#include <iostream>
#include <regex>
#include <ctime>
#include <cstdio>
#include<fstream>

using namespace std;

User::User() : firstname(""), lastname(""), email(""), phonenumber(""),
borndate(""), username(""), password(""), address("") {
}

User::User(string fn, string ln, string em, string phone, string bd,
    string un, string pass, string addr) :
    firstname(fn), lastname(ln), email(em), phonenumber(phone),
    borndate(bd), username(un), password(pass), address(addr) {
}

bool User::isValidFirstname(const string& fn) {
    return !fn.empty() && regex_match(fn, regex("^[A-Za-z]+$"));
}

bool User::isValidLastname(const string& ln) {
    return !ln.empty() && regex_match(ln, regex("^[A-Za-z]+$"));
}

bool User::isValidEmail(const string& em) {
    return !em.empty() && regex_match(em, regex("^[\\w-\\.]+@[\\w-]+\\.[a-zA-Z]{2,}$"));
}

bool User::isValidPhonenumber(const string& phone) {
    return !phone.empty() && regex_match(phone, regex("^\\+?[0-9]{10,15}$"));
}

bool User::isValidBorndate(const string& bd) {
    if (bd.empty()) return false;
    if (!regex_match(bd, regex("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(19|20)\\d\\d$")))
        return false;

    int day, month, year;
    sscanf_s(bd.c_str(), "%d/%d/%d", &day, &month, &year);

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        daysInMonth[1] = 29;

    if (day > daysInMonth[month - 1])
        return false;

    return true;
}

bool User::isValidUsername(const string& un) {
    return !un.empty() && regex_match(un, regex("^[A-Za-z0-9_]{3,15}$"));
}

bool User::isValidPassword(const string& pass) {
    return !pass.empty() && pass.length() >= 6 &&
        regex_search(pass, regex("[A-Z]")) &&
        regex_search(pass, regex("[a-z]")) &&
        regex_search(pass, regex("[0-9]"));
}

bool User::isValidAddress(const string& addr) {
    return !addr.empty() && regex_match(addr, regex("^[A-Za-z0-9 ,.-]+$"));
}

int User::calculateAge() const {
    int day, month, year;
    sscanf_s(borndate.c_str(), "%d/%d/%d", &day, &month, &year);

    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);

    int currentYear = now.tm_year + 1900;
    int currentMonth = now.tm_mon + 1;
    int currentDay = now.tm_mday;

    int age = currentYear - year;
    if (currentMonth < month || (currentMonth == month && currentDay < day)) {
        age--;
    }
    return age;
}

// Getters
string User::getFirstname() const { return firstname; }
string User::getLastname() const { return lastname; }
string User::getEmail() const { return email; }
string User::getPhonenumber() const { return phonenumber; }
string User::getBorndate() const { return borndate; }
string User::getUsername() const { return username; }
string User::getPassword() const { return password; }
string User::getAddress() const { return address; }

// Setters
void User::setFirstname(const string& fn) { firstname = fn; }
void User::setLastname(const string& ln) { lastname = ln; }
void User::setEmail(const string& em) { email = em; }
void User::setPhonenumber(const string& phone) { phonenumber = phone; }
void User::setBorndate(const string& bd) { borndate = bd; }
void User::setUsername(const string& un) { username = un; }
void User::setPassword(const string& pass) { password = pass; }
void User::setAddress(const string& addr) { address = addr; }

// Run interactive input
void User::run() {
    while (true) {
        cout << "Please enter your details:\n";
        do {
            cout << "First Name: "; getline(cin, firstname);
        } while (!isValidFirstname(firstname));

        do {
            cout << "Last Name: "; getline(cin, lastname);
        } while (!isValidLastname(lastname));

        do {
            cout << "Email: "; getline(cin, email);
        } while (!isValidEmail(email));

        do {
            cout << "Phone Number: "; getline(cin, phonenumber);
        } while (!isValidPhonenumber(phonenumber));

        do {
            cout << "Born Date (DD/MM/YYYY): "; getline(cin, borndate);
        } while (!isValidBorndate(borndate));

        do {
            cout << "Username: "; getline(cin, username);
        } while (!isValidUsername(username));

        do {
            cout << "Password: "; getline(cin, password);
        } while (!isValidPassword(password));

        do {
            cout << "Address: "; getline(cin, address);
        } while (!isValidAddress(address));

        cout << "User Entered Successfully!" << endl;
        cout << "Full Name: " << firstname << " " << lastname << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phonenumber << endl;
        cout << "DOB: " << borndate << ", Age: " << calculateAge() << endl;
        cout << "Username: " << username << endl;
        cout << "Address: " << address << endl;

        string again;
        cout << "Add another user? (yes/no): ";
        getline(cin, again);
        if (again == "no" || again == "No") break;
    }
}

void User::exportToJSON(const std::string& filename) const {
    std::ofstream out(filename);
    out << "{\n";
    out << "  \"firstname\": \"" << firstname << "\",\n";
    out << "  \"lastname\": \"" << lastname << "\",\n";
    out << "  \"email\": \"" << email << "\",\n";
    out << "  \"phonenumber\": \"" << phonenumber << "\",\n";
    out << "  \"borndate\": \"" << borndate << "\",\n";
    out << "  \"username\": \"" << username << "\",\n";
    out << "  \"password\": \"" << password << "\",\n";
    out << "  \"address\": \"" << address << "\"\n";
    out << "}\n";
    out.close();
}

void User::exportToExcel(const std::string& filename) const {
    std::ofstream out(filename);
    out << "Firstname,Lastname,Email,Phone,BornDate,Username,Password,Address\n";
    out << firstname << "," << lastname << "," << email << "," << phonenumber << ","
        << borndate << "," << username << "," << password << "," << address << "\n";
    out.close();
}
void User::display() const {
    std::cout << "-----------------------------\n";
    std::cout << "First Name: " << firstname << "\n";
    std::cout << "Last Name: " << lastname << "\n";
    std::cout << "Email: " << email << "\n";
    std::cout << "Phone Number: " << phonenumber << "\n";
    std::cout << "Born Date: " << borndate << " (Age: " << calculateAge() << ")\n";
    std::cout << "Username: " << username << "\n";
    std::cout << "Password: " << password << "\n";
    std::cout << "Address: " << address << "\n";
    std::cout << "-----------------------------\n";
}


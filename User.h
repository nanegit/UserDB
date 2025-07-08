#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string firstname;
    std::string lastname;
    std::string email;
    std::string phonenumber;
    std::string borndate;
    std::string username;
    std::string password;
    std::string address;

    bool isValidFirstname(const std::string& fn);
    bool isValidLastname(const std::string& ln);
    bool isValidEmail(const std::string& em);
    bool isValidPhonenumber(const std::string& phone);
    bool isValidBorndate(const std::string& bd);
    bool isValidUsername(const std::string& un);
    bool isValidPassword(const std::string& pass);
    bool isValidAddress(const std::string& addr);
    int calculateAge() const;

public:
    User();
    User(std::string fn, std::string ln, std::string em, std::string phone,
        std::string bd, std::string un, std::string pass, std::string addr);

    void run();

    std::string getFirstname() const;
    std::string getLastname() const;
    std::string getEmail() const;
    std::string getPhonenumber() const;
    std::string getBorndate() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getAddress() const;

    void setFirstname(const std::string& fn);
    void setLastname(const std::string& ln);
    void setEmail(const std::string& em);
    void setPhonenumber(const std::string& phone);
    void setBorndate(const std::string& bd);
    void setUsername(const std::string& un);
    void setPassword(const std::string& pass);
    void setAddress(const std::string& addr);

    void exportToJSON(const std::string& filename) const;
    void exportToExcel(const std::string& filename) const;
    void display() const;

};

#endif

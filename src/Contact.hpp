#include "Address.hpp"
#include "PhoneNumber.hpp"

#include <string>
#include <list>

class Contact {
private:
    std::string name;
    std::string surname;
    std::list<PhoneNumber*>  phoneNumbers;
    std::string email;
    Address* address;

public:
    Contact(const std::string& name, const std::string& surname, const std::string& email = "", Address* address = nullptr);
   ~Contact() = default;

    std::string getName() const;
    std::string getEmail() const;
    Address* getAddress() const;

    std::string getFullName() const;
    bool hasAddress() const;
};

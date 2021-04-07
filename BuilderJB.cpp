// Joshua Bloch's Builder Pattern for simplifying the instantiation
// of objects with many constructor parameters. It is not the
// GOF (Gang of Four) builder pattern. 
//simplify the instantiation of objects with many constructor parameters or many optional parameters.
#include <iostream>
#include <string>

// Function meta object 
class UserData{
public:
        using ulong = unsigned long;
private:
        ulong          userID = 0;
        std::string    name;
        std::string    lastName;
        std::string    email;
        UserData() = default;
public:
        // Explicit is better than implicit 
        ~UserData() = default;

        auto show() -> void
        {
            std::cout << "\nUser{"
                      << "\n" << "  id        = " << userID
                      << "\n" << "  name      = " << name
                      << "\n" << "  last name = " << lastName
                      << "\n" << "  email     = " << email
                      << "\n" << "}"
                      << "\n";
        }

        // Allow builder class access UserData's private data
        //We can add getter/setter also
        friend class UserBuilder;
}; //--- EoF class UserData --- //

class UserBuilder{
private:
        //class UserData;
        UserData data{};
public:
        UserBuilder(){
                // _data = UserData();
        }       
        auto setID(ulong userID ) -> UserBuilder& {
           data.userID = userID;
           return *this;
        }
        auto setName(const std::string& name) -> UserBuilder& {
            data.name = name;
            return *this;
        }
        auto setLastName(const std::string& name) -> UserBuilder& {
            data.lastName = name;
            return *this;
        }
        auto setEmail(const std::string& email) -> UserBuilder& {
           data.email = email;
           return *this;
        }
        auto build() -> UserData {
           return this->data;
        }
}; //--- EoF class UserData::builder --- //

int main(){

    auto user1 =
            UserBuilder()
            .setID(1065)
            .setName("Rohit")
            .setLastName("Benake")
            .setEmail("rohitbenake@gmail.com")
            .build();
            
    auto user2 =
            UserBuilder()
            .setID(2001)
            .setName("Raksha")
            .setLastName("R")
            .setEmail("xyz@z.com")
            .build();

    user1.show();       
    user2.show();
    return EXIT_SUCCESS;
}
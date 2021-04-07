//Null-Object Pattern
 /*
 An object, called nulll object, which doesn't do anything and has empty methods implementing a required interface by the client code is used to convey the absense of an ordinary object instead of null or null pointer.

Example: in a database system, instead of returning null, null reference or null pointer for an not found employee object, an empty object, called null object, with an empty mehtods is returned. It has the advantage of avoiding null exception that can crash unexpectdely a program. Another problem of null or null pointers is that null bugs cannot be caught at compile-time and are also known to be hard to debug and trace.

Alternatives:
Throw an exception when there is the absense of an object. 
For instance, std::runtime_error("Error: record not found.").
Use optional type (Haskell's maybe) or C++17 Optional.
Return a pointer that is set to null for denoting the absence of an object. 
This pattern is widely used by many C and C++ codes. 
However it is prone for the infamous null pointer exception problems.

"A Null Object provides a surrogate for another object that shares the same interface but does nothing. 
Thus, the Null Object encapsulates the implementation decisions of how to do nothing and hides those details from its collaborators. – Bobby Woolf in [PLoP3]"

*/

#include <iostream>
#include <string>

// Interface 
class ICompany{
    public:
        virtual unsigned    getID() const       = 0;    
        virtual std::string getName() const     = 0;
        virtual void        showCompany() const = 0;   
        
        virtual bool        isNull() const = 0;

        virtual ~ICompany() = default;
};

class Company: public ICompany{
public:
        Company(unsigned tid, const std::string& name):
                name(name),
                id(tid){
        }

        unsigned getID()      const { return id; }

        std::string getName() const { return name; }

        void showCompany() const {
                std::cout << "Company is = " << name << "\n";
        }   

        bool isNull() const{ return false;}

        ~Company() = default;
private:
        std::string name;
        unsigned    id;    
};

// Null object 
// Returns this null object instead of returning a null pointer
// when a givne company is not found in the database system.
class NullCompany: public ICompany{
public:
        unsigned getID()      const { return 0; }
        std::string getName() const { return ""; }
        void showCompany()    const {}
        bool isNull() const{ return true;}      
};


ICompany* getCompany(){ return new NullCompany();}

int main(){
    
    std::cout << "Company is empty = "<<getCompany()->isNull();

    return EXIT_SUCCESS;
}

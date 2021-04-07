// Composite
 /*
 The composite pattern is a structural design pattern intruduced by GOF 
 which allows clients to deal with tree collection of objects 
 in the same way as a primitive objects.

Features and applications:
Allow client code to ignore differences between individual objects and collection (composite objects) and treat them uniformly.
Known cases:
    GUI Widgets - Java Swing GUI library allows to build a complex GUI out of individual UI elements such as buttons, menu, text box and panels which can have many UI child objects and other panels.
    AST - Abstract Syntax Trees
    XML - Nodes
Participants of Composite
    Component:
        Overview: Interface or abstract class which defines common operations shared by the primitive objects and composite objects.
    Leaf
        Overview:
            Represents a primitive object which doesn't contain any child object.
Composite
    Overview
        As the name implies, the composite object contains leaf children objects.
        Implements child operations in the component interface.
        Can add or remove leaf objects or composite object as children.
        The composite object forwards the messages related to child operations to the children elements.
    Methods:
        .Add(Component)
        .Remove(Component)
        .Clear()
*/

#include <iostream>
#include <string>
#include <memory>
#include <vector>

//Component interface:
//Defines primitive elements operations.
class IGraphic{
public:
     virtual auto type() const -> const std::string  = 0;
     virtual auto draw() -> void = 0;
     virtual auto rotate(double) -> void = 0;
     virtual ~IGraphic() = default;
};

//Convenient type aliases:
using cstring = const char*;
using GNode = std::shared_ptr<IGraphic>;

//Leaf, group aka composite object:
//Allows to performing an operation, which would be performed on a single primitive element, on all elements stored in the composite object.
class Group: public IGraphic{
private:
    std::vector<GNode> nodes;
    const std::string id;
    static constexpr cstring gtype = "Group";//Instead of #define prefer constexpr

public:
    Group(const std::string& id): id(id) {
        std::cout << " [TRACE] Create group = " << id << "\n";
    }

    ~Group(){
        std::cout << " [TRACE] Destroy group - id = << " << id << "\n";
    }

    auto begin() const -> decltype(nodes.begin()) {
        return nodes.begin();
    }

    auto end() const -> decltype(nodes.end()) {
        return nodes.end();
    }

    auto clear() -> void {
        nodes.clear();
    }

    auto size() -> size_t {
        return nodes.size();
    }

    auto add(GNode n) -> void {
        std::cout << " [TRACE] id = " << id
                  << "; Add object = " << n->type() << "\n";
       nodes.push_back(n);
    }

    //raw pointer version
    auto add(IGraphic* n) -> void {
        std::cout << " [TRACE] id = " << id
                  << " ; Add object = " << n->type() << "\n";
        nodes.push_back(std::shared_ptr<IGraphic>(n));
    }

    // Add stack-allocated object 
    auto addFromStack(IGraphic* n) -> void {
        std::cout << " [TRACE] id = " << id
                  << " ; Add object = " << n->type() << "\n";
        // Dummy deleter to avoid core dump by avoiding deleting
        // stack-allocated object or non-owned pointer.
        auto s = std::shared_ptr<IGraphic>(n, [](IGraphic*){ return ; });
        nodes.push_back(s);
    }   

    // Note: Template methods should always be in the header files
    template<class Node>
    auto addNew(const std::string& id) -> void {
        auto n = std::make_unique<Node>(id);
        std::cout << " [TRACE] id = " << id
                          << " ; Add object = " << n->type() << "\n";    
        nodes.push_back(std::move(n));//Just move the ownership of unique to shared pointer
    }

    // ======> Interface IGraphic <<=========//

    auto type() const -> const std::string override {
        return gtype;
    }

    auto draw() -> void override {
        std::cout << " [TRACE] Draw group - id =  " << id << "\n";
        for(const auto& obj: nodes)
            obj->draw();
    }
    auto rotate(double angle) -> void override {
        std::cout << " [TRACE] Rotate group - id = " << id << "\n";
        for(const auto& obj: nodes)
            obj->rotate(angle);
    }    
};

//Primitive element or node: Line
class Line: public IGraphic {
private:
    static constexpr cstring ltype = "Line";
    std::string id;
public:
    Line(const std::string& id): id{id} { }

    auto type() const -> const std::string override {
         return ltype;
    } 
    auto draw() -> void override {
         std::cout << " [TRACE] Draw line - id = " << id << "\n";
    } 
    auto rotate(double angle) -> void override {
         std::cout << " [TRACE] Rotate line ; id = " << id 
                   << "; angle = " << angle << "\n";
    }  
};

//Primitive element or node: Triangle
class Triangle: public IGraphic {
private:
    static constexpr cstring ttype = "Triangle";
    std::string _id;
public:
    Triangle(const std::string& id): _id{id} { }
    auto type() const -> const std::string override {
       return ttype;
    } 
    auto draw() -> void override {
        std::cout << " [TRACE] Draw triangle - id = " << _id << "\n";
    } 
    auto rotate(double angle) -> void override {
         std::cout << " [TRACE] Rotate triangle"
                   << " id = " << _id
                   << " angle = " << angle << "\n";
    }  
};

//Sample client code:
// Count total number of elements 
auto countElements(const Group& group) -> size_t {
    size_t n = 0;
    for(const auto& g: group){
       if(g->type() == "Group"){
               auto p = static_cast<Group*>(g.get());
               n += countElements(*p);          
       } else{ ++n; }
    }
    return n;
}


int main(){
    
    const char nl = '\n';
    std::cout << "=== Objects construction === " << nl;

    auto groupA = Group("groupA");
    groupA.add(new Triangle("triangleA1"));
    groupA.add(new Line("lineA1"));
    groupA.addNew<Line>("LineA2");

    auto groupB = std::make_shared<Group>("GroupB");
    groupB->add(new Triangle("triangleB1"));
    groupB->addNew<Triangle>("triangleB2");
    groupB->addNew<Line>("LineB1");
    groupB->addNew<Line>("LineB2");
    auto triangleB3 = Triangle("triangleB3");
    groupB->addFromStack(&triangleB3);
    groupA.add(groupB);

    std::cout << nl << "=== End of object construction === " << nl;
    std::cout << "Total of elements of groupA = " << countElements(groupA) << "\n";
    std::cout << "Total of elements of groupB = " << countElements(*groupB) << "\n";

    std::cout << nl << " [*] ==> Draw group B" << "\n";
    groupB->draw();

    std::cout << nl << " [*] ==> Rotate group B" << "\n";
    groupB->rotate(90);

    std::cout << nl << " [*] ==> Draw group A" << "\n";
    groupA.draw();

    std::cout << nl << " [*] ==> Rotate group A" << "\n";
    groupA.rotate(15);

    std::cout << nl << " [*] ==> Remove objects from group B" << "\n";
    groupB->clear();
    groupA.draw();

    std::cout << "=== End of Program ====" << "\n";

    return EXIT_SUCCESS;
}

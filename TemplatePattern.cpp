 //GOF - Template Pattern
 /*"Defines the skeleton of an algorithm in a method, 
    deferring some steps to subclasses. 
    Template Method lets subclasses redefine certain steps of an algorithm 
    without changing the algorithms structure."

Intent: Create an algorithm template that allows redefine some steps without changing its structure.    

The parent abstract class has four different types of methods:
Concrete methods: Methods implemented in the abstract class.
Abstract methods: Methods without implementation that must be implemented by subclasses.
Hook methods: Methods with default implementation that can be overriden by subclasses.
Template methods: Method that calls concrete methods, abstract methods or hook methods.
 
Participants:
Base Class: Defines an algorithm which calls primitive methods (aka hook methods) that will be defined by the derived classes.
Derived Class: Implements primitive methods (virtual methods or methods that can be overriden) defined in the base class.

Features:
The base class defines an algorithm stub, however some steps are required to be implemented by the derived class.
This pattern provides an inverse control structure, the algorithm defined in the base class which calls the primitive methods which are algorithm steps declared in the base class.
This pattern can be the base of an framework.
 */

#include <iostream>
#include <string>

class IntervalSummation{
public:
 // Algorithm or entry point which calls the derived class method.
 // This is the template method 
 double summation(int lower, int upper) const{
         double sum = 0;
         for(int i = lower; i <= upper; i++)
                 sum += this->stepFn(i);
         return sum; 
 }
protected:
    // Hook method or to be defined by the derived class
    virtual double stepFn(double x) const = 0 ;
};

class SumOfSquares: public IntervalSummation{
private:
    double stepFn(double x) const { return x * x; }
};

class SumOfCubes: public IntervalSummation{
private:
    double stepFn(double x) const { return x * x * x; }
};

void clientCode(const IntervalSummation& obj){
    std::cout << "Summation at [0, 15] = " << obj.summation(0, 15) << std::endl;
}

int main(){
    SumOfSquares sq;

    //(double) 385.00000
    std::cout << "sq.summation(0, 10) = " << sq.summation(0, 10) << std::endl;
    
    SumOfCubes sc;
    //(double) 3025.0000
    std::cout << "sc.summation(0, 10) = " << sc.summation(0, 10) << std::endl;

    clientCode(sq);
    //Summation at [0, 15] = 1240

    clientCode(sc);
    //Summation at [0, 15] = 14400

    return EXIT_SUCCESS;
}
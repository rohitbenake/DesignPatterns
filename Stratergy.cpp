//Strategy
//A behavioral design pattern from GOF which allows the client code to select and change an algorithm encapsulated as an object at runtime.
/*
Parts:
Context:
    Object that has a reference to an strategy objects and sets the strategy at runtime.
Resposibilities:
    Set strategy
    Change strategy
    Invoke strategy
IStrategy: (Algorithm interface)
    Strategt interface define the algorithm operations.
Concrete strategy.
    Strategies objects or implementation of IStrategy class.
*/

#include <iostream>
#include <string>
#include <memory>

//--------------Logging--------------------//
template <typename ...T> //variadic template
void LOGI(T&&... args) //rvalue ref
{
    std::cout<<"[INFO]";
    //warning: fold-expressions only available with '-std=c++17' or '-std=gnu++17'
    ((std::cout << args), ...); 
    std::cout<<'\n';
}

template <typename ...T> //variadic template
void LOGT(T&&... args) //rvalue ref
{
   // std::cerr<<"[TRACE]";
    //warning: fold-expressions only available with '-std=c++17' or '-std=gnu++17'
  //  ((std::cerr << args), ...); 
  //  std::cerr<<'\n';

    std::cout<<"[TRACE]";
    //warning: fold-expressions only available with '-std=c++17' or '-std=gnu++17'
    ((std::cout << args), ...); 
    std::cout<<'\n';
}
//-----------------------------------------//
// Strategy interface 
struct IStrategy{

  virtual ~IStrategy(){}

  // Essential: Algorithm encapsulated by strategy object 
  virtual auto compute(double x, double y) const -> double = 0; 
  
  // Optional: Provides strategy metadata 
  virtual auto name() const -> const std::string = 0;

  // Clone this object (Note: This is a virtual constructor)
  virtual auto clone() const -> IStrategy* = 0;

};

//Context Class:
//Selects and switch the strategy (aka algorithm).

class Context{
private: 
  std::unique_ptr<IStrategy> strategy;

public:
  Context()
    : strategy{nullptr} { 
        LOGT("Context => Constructed");
    }

  Context(IStrategy* s)
    : strategy{s} { 
        LOGT("Context => Constructed using *");
    }

  Context(const IStrategy& s)
    : strategy{s.clone()} {
        LOGT("Context => Constructed using clone");
     }

  auto setStrategy(IStrategy* s){
    LOGT("Context => setStrategy *");
    strategy.reset(s);// deletes managed object, acquires new pointer
  }

  auto setStrategy(const IStrategy& s){
    LOGT("Context => setStrategy const&");
    strategy.reset(s.clone());
  }  

  auto compute(double x, double y) -> void {
    if(nullptr == strategy){
        std::runtime_error("Error: strategy not set");
    }
      
    double result = strategy->compute(x, y);

    LOGI("strategy = ",strategy->name() ,
         " ","( x = " , x , 
         " ; ", "y = " , y," )",
         "Result = ",result
    );
  }

    ~Context(){
       LOGT("Context => Destructed");
   }
};

//Concrete strategies:
//add => algorithm which adds two numbers.
class AddStrategy: public IStrategy {
public:
  auto name() const -> const std::string override{
    return "add";
  }

  auto compute(double x, double y) const -> double override{
    return x + y;
  }

  auto clone() const -> IStrategy* {
    LOGT("AddStrategy => I was cloned");
    return new AddStrategy(*this);
  }
   
   ~AddStrategy(){
       LOGT("AddStrategy => Destructed");
   }
};

//Multiplication
struct MulStrategy: public IStrategy {
public:
  auto name() const -> const std::string override{
    return "mul";
  }  

  auto compute(double x, double y) const -> double override{
    return x + y;
  }
  auto clone() const -> IStrategy* {
    LOGT("MulStrategy => I was cloned");
    return new MulStrategy(*this);
  }  
};

//Linear Combination:
struct LinearCombStrategy: public IStrategy {
  double a, b, c;
  LinearCombStrategy(double a, double b, double c)
    : a(a), b(b), c(c)
  {
      LOGT("LinearCombStrategy => Constructed");
  }

  auto name() const -> const std::string override{
    return "Linear combination a * x + b * y + c";
  }

  auto compute(double x, double y) const -> double override{
    return a * x + b * y + c;
  }

  auto clone() const -> IStrategy* {
    LOGT("LinearCombStrategy => I was cloned");
    return new LinearCombStrategy(*this);
  }  

    ~LinearCombStrategy(){
       LOGT("LinearCombStrategy => Destructed");
   }
};

int main(){

    Context ctx;
    LOGI("==== Strategy = add ====");
    //Statergies are created on heap and the onership is maintained by context
    ctx.setStrategy(new AddStrategy);
    ctx.compute(3.0, 4.0);

    LOGI("==== Strategy = mul ====");
    ctx.setStrategy(new MulStrategy);
    ctx.compute(3.0, 4.0);

    LOGI("==== Strategy = Linear combination ====");
    ctx.setStrategy(new LinearCombStrategy(5, 3, 4));
    ctx.compute(3.0, 4.0);

    LOGI("==== Strategy = Linear combination [2] ====");
    auto comb1 = LinearCombStrategy(6.0, 5.0, 10.0);
    // Copy stack-allocated object comb1 using the virtual constructor
    ctx.setStrategy(comb1);
    ctx.compute(5.0, 3.0);  

    LOGI("==== Strategy = Linear combination [2] ====");  
    // Copy stack-allocated temporary object comb1 using the virtual constructor
    // clone
    ctx.setStrategy(LinearCombStrategy{6.0, 5.0, 10.0});
    ctx.compute(2.0, 6.0);

    return EXIT_SUCCESS;
}
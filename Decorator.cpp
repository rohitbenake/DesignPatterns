//Decorator :
//GOF: "Attach additional resposibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality." 
//extend a funcitonality of an object dynamically without deep inheritance or deep class hierarchies.
//Decorators are also known as wrappers due to it wrap itself around anohter object.
//The wrapping/enclosing object is called "decorator".

/*
	 +-------------------------------------+
     | <<Interface>> IComponent            |
     |-------------------------------------|
     |  +Operation()                       |
     +------------^----------------^-------+
                  |                |
           +------+                +
           |                       +-------+
           |                               |
+----------+-----------+      +-----------------------------+
|  Component           |      | Decorator                   |
|----------------------|      |-----------------------------|
|  +Operation()        |      | Variable: IComponen comp    |
|                      |      |                             |
+----------------------+      | +Decorator(IComopent comp)  |
                              |                             |
                              | +Operation(){               |
                              |   newBehavior();            |
                              |   comp.Operation();         |
                              |  }                          |
                              +-----------------------------+
*/

#include <iostream>
#include <string>
#include <memory>

template <typename ...T> //variadic template
void Log(T&&... args) //rvalue ref
{
    //warning: fold-expressions only available with '-std=c++17' or '-std=gnu++17'
    ((std::cout << args), ...); 
    std::cout<<'\n';
}

// Interface IShape (IComponent) 
struct IShape
{
  //optional:  -> return type
  virtual auto draw() -> void = 0;
  virtual auto description() -> std::string = 0;
  virtual ~IShape() = default;
};

//Concrete shape: Square
// Component class
class Square: public IShape
{
public:
   auto draw() -> void
   {
      Log("=> Draw square");
   }
   auto description() -> std::string 
   {
      return "square";
   }
};

// Component class 
class Triangle: public IShape
{
public:
    auto draw() -> void
    {
       Log(" => Draw tringle"); 
    }
    auto description() -> std::string
    {
       return "triangle";
    }
};

// Decorator 1 => Draw shape with color
class ColorDecorator: public IShape {
public:
    ColorDecorator(std::shared_ptr<IShape> shape)
       : m_shape(shape) { }

    auto draw() -> void
    {
        // Save color:   push() 
         Log("=> [ColorDecorator] Draw object with color blue");
        m_shape->draw();
        // Restore color: pop()
    }
    auto description() -> std::string
    {
        return  m_shape->description() + " ; color = " + m_color;
    }
    // Return a reference to itself (ColorDecorator&)
    auto setColor(const std::string& color) -> decltype(*this)& {
        m_color = color;
        return *this;
    }
private:
    // The decorator owns the decorated object 
    std::shared_ptr<IShape>  m_shape;
    std::string   m_color = "blue";
};

class PositionDecorator: public IShape {
public:
     PositionDecorator(std::shared_ptr<IShape> shape)
      : m_shape(shape){ }

     auto draw() -> void
     {
        // Save transformation matrix:   pushMatrix() 
        Log(" =>  [PositionDecorator] Draw object at x = ",m_x," ; y = ",m_y);
        m_shape->draw();
        // Restore transformation matrix: popMatrix()
     }
     auto description() -> std::string
     {
        return  m_shape->description() + " ; position x  = "
                + std::to_string(m_x) +  " , y = " + std::to_string(m_y);
     }

     auto setPosition(double x, double y) -> PositionDecorator& {
        m_x = x, m_y = y;
        return *this;
     }  
private:    
     // The decorator owns the decorated object 
     std::shared_ptr<IShape> m_shape;
     int m_x = 0, m_y = 0;
};

int main(){

    std::puts("\n ======>> Experiment 1 <<===========");
    auto shape = std::make_shared<ColorDecorator>(std::make_shared<Square>());
    shape->setColor("yellow");
    shape->draw();
    Log(shape->description());

    std::puts("\n ======>> Experiment 2 <<===========");
    auto observerd = std::shared_ptr<IShape>{nullptr};

    auto shapeWithColorAndPosition =
        [&observerd]{
            auto shape        = std::make_shared<Triangle>();
            observerd         = shape;
            auto shapeColored = std::make_shared<ColorDecorator>(shape);
            shapeColored->setColor("white");
            auto shapePos     = std::make_shared<PositionDecorator>(shapeColored);          
            return  shapePos;
        }();

    shapeWithColorAndPosition->setPosition(100, 20);
    shapeWithColorAndPosition->draw();

    Log("DESCRIPTION = ",shapeWithColorAndPosition->description());
    Log("[INFO] observed shape = ",observerd->description());

    return EXIT_SUCCESS;
}
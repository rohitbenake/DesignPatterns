// Observer-Observable
/*
    This implementation is based on classical observer-design pattern and 
    defines two interfaces IObservable, which all observable objects inherit from, and 
    IObserver, which all observer objects inherit from. 
    This sample application defines a counter observable object that notifies all observers 
    when its state changes. 
    The application also provides three observers, 
    a console observer which prints the console (terminal) and 
    two Qt5 Gui observers which displays that counter value.
*/

#include <iostream>
#include <iomanip>
#include <vector>

//#include <QtWidgets>
//#include <QApplication>
//#include <QSysInfo>

struct IObserver;

struct IObservable 
{
    /** Subscribe to observable notifications */
    virtual void addObserver(IObserver* obs) = 0;

    /** Notify all observers */
    virtual void notify() = 0; 
    virtual     ~IObservable() = default;
};

struct IObserver 
{
    virtual void update(IObservable* sender) = 0;
    virtual     ~IObserver() = default;
};

class BasicObservable: public IObservable 
{
    std::vector<IObserver*> m_observers{};
public:

    void addObserver(IObserver* obs) override 
    {
        m_observers.push_back(obs);
        obs->update(this);
    }

    /** Notify all observers */
    void notify() override  
    {
        for(const auto obs: m_observers){ obs->update(this); }
    }    
};

class CounterModel: public BasicObservable
{
    int m_counter = 0;
public: 

    void increment() 
    { 
        m_counter += 1;
        this->notify();
    }
    void decrement() 
    {
        m_counter -= 1;
        this->notify();
    }
    void reset()
    { 
        m_counter = 0;  
        this->notify();
    }

    void set(int n)
    {
        m_counter = n;
        this->notify();
    }

    int get() const { return m_counter; }
};

/** Concrete observer that prints the subject state in the console (terminal) */
class ConsoleView: public IObserver 
{
    /* override: IObserver::update() */
    void update(IObservable* sender) override 
    {
        /* Note: It can result in undefined behavior. */
        int cnt = static_cast<CounterModel*>(sender)->get();
        std::cout << " [CONSOLE] Counter state changed to = " << cnt << '\n';
    }
};


class FormView: public IObserver {
    //QWidget      m_win{};
    //QFormLayout* m_form;
   //QPushButton* m_btn_increment;
    //QPushButton* m_btn_decrement;
    //QLabel*      m_label;

    CounterModel& m_model;
public:

    FormView(CounterModel& model): m_model(model)
    {
        /*m_win.setWindowTitle("Form View");
        m_form = new QFormLayout(&m_win);
        m_btn_increment = new QPushButton("[+]");
        m_btn_decrement = new QPushButton("[-]");
        m_label         = new QLabel("0");

        m_form->addRow("Increment",     m_btn_increment);
        m_form->addRow("Decrement",     m_btn_decrement);
        m_form->addRow("Counter value", m_label);

        m_win.show(); 

        // ------ Setup event handlers ------------------- 

        QObject::connect(m_btn_decrement, &QPushButton::clicked, 
            [self = this]{ 
                self->m_model.decrement();
            });

        QObject::connect(m_btn_increment, &QPushButton::clicked, 
            [this](){ 
                m_model.increment(); 
            });
        */
    }

    /* override: IObserver::update() */
    void update(IObservable* sender) override 
    {
        int cnt = static_cast<CounterModel*>(sender)->get();
        //m_label->setText(QString::number(cnt)); 
        std::cout << " [QT GUI] Counter state changed to = " << cnt << '\n';
    }
};

class LabelView: public IObserver
{
    //QLabel m_label;
public:
    LabelView() 
    {   
        /*m_label.setWindowTitle("Label View");
        m_label.resize(400, 300);
        m_label.show();
        */
    }

    /* override: IObserver::update() */
    void update(IObservable* sender) override 
    {
        int cnt = static_cast<CounterModel*>(sender)->get();
        //m_label.setText(" [LABEL Observer] Counter value = " + QString::number(cnt));
    }

};


int main(int argc, char** argv)
{
    //QApplication app(argc, argv);

    CounterModel model;     
    ConsoleView  observerA{};
    FormView     observerB{model};
    LabelView    observerC{};

    model.addObserver(&observerA);
    model.addObserver(&observerB);
    model.addObserver(&observerC);

    std::cout << " -------------------------------- \n";
    //Simulate increment
    model.increment();

    return EXIT_SUCCESS;
}
// Observer-Observable
/*
The observer design pattern is a behavioral design pattern introduced by GOF(Gang-of-Four book), 
which defines a one-to-many dependency between a subject (a.k.a observable) and 
observers that are notified by the subject object whenever its state change.

This pattern is widely used in GUI graphical user interfaces for decoupling the application state and 
business logic from the graphical user interface. 
For instance, many GUI applications that provide Comboboxes, Listboxes or selection boxes widgets 
does not allow adding elements to those widgets directly. 
Instead, they provide observable objects, such as ListModel which notifies the subscribed, or attached widgets, about its changes. 
When any new element is added to a hypothetical ListModel object, it notifies the dependent widgets such as ListBoxes, Comboboxes and tables which update themselves. 
The benefit of this approach is that it allows changing the user interface widgets without modifying the application logic.

Also known as:
observer design pattern
publisher/subscriber
subject/observer

Use-cases and potential applications:
GUI - Graphical User Interfaces
Chart Applications
CAD - Computer Aided Design Software
Spreadsheets
Object-oriented callbacks
Event-driven systems
"Reactive programming" buzzword (ReactiveX)

Other Variants:
MVC - Model View Controller design pattern for GUIs
MVP - Model View Presenter design pattern for GUIs
"Reactive programming"
Two-way data bindings from .NET WPF (Windows Presentation Foundation)
Observable property from .NET
Observable variables from Java and Android
Signals and slots from Qt Framework

Known uses:
Java: java.util.Observer
.NET (C#): System.IObservable and System.IObserver
Java Swing
QT GUI C++ Framework

Parts:
Subject (aka Observable) - Keeps references to all observer objects and notifies them whenever its state changes.
Typical Methods:
    Subject.subscribe(Observer) => Register observer for further notifications.
    Subject.register(Observer) => Register observer for further notifications.
    Subject.attach(Observer) => Register observer for further notifications.
    Subject.detach(Observer) => Remove observer, unsubscribe from notifications.
    Subject.notify() => Notify all observers by calling the method .notify()
Observer - Object that subscribes to the subject notifications and are called by the subject whenever there is a state change.
Typical methods:
    .update()
    .notify()

Potential problems:
When notified, an observer object should complete its computation as fast as possible, otherwise it will block the remaining observers from being notified.
If the computation performed by any observer, when a notification arrives, blocks the current thread, all other remaining observers will not be able to receive notifications from the subject object.
Possible callback hell. Every observer is just a object-oriented callback.
Mechanisms to send data to observers:

Pull model. Observer executes Subject.getData() to get data from subject.
Push model. The subject pass its data as argument of observers' update method. for o in observers do o.update(subject.data)
*/

//Below example uses std::function<> callback which makes the code more loose coupled as any objects from any class and lambdas can be used as observers.

#include <iostream>
#include <iomanip>
#include <functional> 
#include <vector> 

//include <QtWidgets>
//#include <QApplication>
//#include <QSysInfo>

using ObserverCallback = std::function<void (void* sender)>;

struct IObservable 
{
    /** Subscribe to observable notifications */
    virtual void addObserver(ObserverCallback obs) = 0;

    /** Notify all observers */
    virtual void notify() = 0; 

    virtual     ~IObservable() = default;
};

class Observable: public IObservable 
{
    void* m_sender{nullptr};
    std::vector<ObserverCallback> m_observers{};
    
public:

    Observable(){ }
    Observable(void* sender): m_sender(sender){ }
    
    void addObserver(ObserverCallback callback) override 
    {
        m_observers.push_back(callback);
        std::cout << " Observable calling back the callback \n";
        callback(m_sender);//give the sender info to callback
    }

    /** Notify all observers */
    void notify() override  
    {
        for(const auto callback: m_observers){ callback(m_sender); }
    }    
};

class CounterModel
{
    int m_counter{0};
    Observable m_obs{this};
public: 
    void increment() { m_counter += 1; m_obs.notify(); }
    void decrement() { m_counter -= 1; m_obs.notify(); }
    int  get() const { return m_counter; }

    void onCounterChanged(ObserverCallback callback)
    {
        m_obs.addObserver(callback);
    }
};

class FormView {
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

    void update(void* sender)  
    {
        int cnt = reinterpret_cast<CounterModel*>(sender)->get();
        //m_label->setText(QString::number(cnt)); 
        std::cout << " [QT GUI] Counter state changed to = " << cnt << '\n';
    }
};

class LabelView
{
    //QLabel m_label;
public:
    LabelView() 
    {   
       /* m_label.setWindowTitle("Label View");
        m_label.resize(400, 300);
        m_label.show();
        */
    }

    void notify(void* sender)
    {
        int cnt = reinterpret_cast<CounterModel*>(sender)->get();
        //m_label.setText(" [LABEL Observer] Counter value = " + QString::number(cnt));
        std::cout << " [QT GUI] Counter state changed to = " << cnt << '\n';
    }

};


int main(int argc, char** argv)
{
    //QApplication app(argc, argv);

    CounterModel model;     
    
    //Passing lambda as callback
    model.onCounterChanged([](void* sender){
        int cnt = static_cast<CounterModel*>(sender)->get();
        std::cout << " [CONSOLE VIEW] Counter state changed to = " << cnt << '\n';
    });

    FormView   observerB{model};
    LabelView  observerC{};

    model.onCounterChanged([&](void* sender){
        std::cout << " [CONSOLE VIEW] updating observerB \n";
        observerB.update(sender);
    });
    
    model.onCounterChanged([&](void* sender){ 
        std::cout << " [CONSOLE VIEW] updating observerC \n";
        observerC.notify(sender); 
    });

    std::cout << " -------------------------------- \n";
    //Simulate increment
    model.increment();
    //return app.exec();
    return EXIT_SUCCESS;
}

//    SINGLETON CLASS:
//    Intent
//
//    Ensure a class has only one instance, and provide a global point of access to it.
//    Encapsulated "just-in-time initialization" or "initialization on first use".
//    Problem
//
//    Application needs one, and only one, instance of an object. Additionally, lazy initialization and global access are necessary.
//
//    Check list
//
//        - Define a private static attribute in the "single instance" class.
//        - Define a public static accessor function in the class.
//        - Do "lazy initialization" (creation on first use) in the accessor function.
//        - Define all constructors to be protected or private.
//        - Clients may only use the accessor function to manipulate the Singleton.
#include <iostream>
using namespace std;
class GlobalClass //singleton class
{
private:
    int m_value;
    static GlobalClass *s_instance; //private accessor through GlobalClass
    GlobalClass(int v = 0) // ctor private. no one else can create instances of this class.
    {
        m_value = v;
    }
public:
    int get_value()
    {
        return m_value;
    }
    void set_value(int v)
    {
        m_value = v;
    }
    static GlobalClass *instance() //accessor method also used in lazy instantiation.
    {
        if (!s_instance) //this object initialized only once.
            s_instance = new GlobalClass;
        return s_instance;
    }
};

// Allocating and initializing GlobalClass's
// static data member.  The pointer is being
// allocated - not the object inself.
GlobalClass *GlobalClass::s_instance = 0; //intializing the singleton pointer to nullptr;

void foo(void)
{
    GlobalClass::instance()->set_value(1);
    cout << "foo: global_ptr is " << GlobalClass::instance()->get_value() << '\n';
}

void bar(void)
{
    GlobalClass::instance()->set_value(2);
    cout << "bar: global_ptr is " << GlobalClass::instance()->get_value() << '\n';
}

int main()
{
    cout << "main: global_ptr is " << GlobalClass::instance()->get_value() << '\n';
    foo(); //instance() call here will initialize the singleton class.
    bar(); //instance() will only return referece to earlier instantiation of singleton class.
}

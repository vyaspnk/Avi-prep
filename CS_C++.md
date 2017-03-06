--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C++ Notes
============================================
[TOC]

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Keywords
--------------------------------------------
- Inline: Prefer inline over #define
- Access Keywords: Public, Private, Protected (inheritance).
- Initializer list in Constructor (can initialize even const)
- Copy and Move(C++11) Constructors
- this = self-referential
- Friend function
- auto (C++11) = auto is used to declare types that can be deduced from context and is no longer a storage specifier.
- static_assert (C++) = compile time assert combined with type_trace, helps enforce rules at compile-time. For ex, rule can check if the types assigned to template is valid.
- enum class = type-safe (better form of enumeration)
- lambda = unnamed function
- Function Objects (Functors) = Classes for common numerical operators defined in function.h (+, -, etc)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Typecast
--------------------------------------------
- static_cast
- dynamic_cast
- reinit_cast

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Simple yet useful features
--------------------------------------------
- Range command (introduced in C++11)
  - The below idiom can be used to iterate over a range of values. This is simpler than to use an iterator.
  - The expression (container) can be an array, a sequence container or a string. Conceptually something that has begin and end members.
  ```CPP
  for(declaration :  expression)
        statement
  for(auto &elem : array)             // auto infers the type and & gives a reference
        elem = elem + 2;                // mutate each element in the container    
  ```
- Square an entire vector (can also be used for scalar dot product of two vectors):
 ```CPP
  std::transform (v.begin(), v.end(),       // first source range
                  v.begin(),                // second source range
                  v.begin(),                // destination range
                  multiplies<int>());       // operation
 ```
- Array of functions
 ```CPP
 inline int add(int a, int b) { return a + b; }
 inline int sub(int a, int b) { return a - b; }
 inline int mul(int a, int b) { return a * b; }
 inline int div(int a, int b) { return a / b; }
 inline int mod(int a, int b) { return a % b; }
 int (*func_arr[])(int, int) = {&add, &sub, &mul, &div, &mod};
 for(auto i = 0; i < 10; ++i) cout << i << " : " << func_arr[i](10, i) << endl;
 ```

###Detect Arithmetic Overflows###
- To detect overflows either use [C/C++ built-ins](https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html) or [binary arithmetic as explained in Stackoverflow](http://stackoverflow.com/questions/199333/how-to-detect-integer-overflow-in-c-c)

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Pass by Reference
--------------------------------------------
- C is always pass by value. C++ has a new concept called pass by reference.
- References are typically implemented by the compiler as pointers. 
- The only real difference between pointers and references is that references have a cleaner but more restrictive syntax. This makes references easier and safer to use, but also less flexible. 
- This also means that pass by reference and pass by address are essentially identical in terms of efficiency.
```cpp
int swap(int &a, int &b) {
	int tmp = a;
	a = b;
	b = a;
}
```

###Restrict###
- The [restrict](http://cellperformance.beyond3d.com/articles/2006/05/demystifying-the-restrict-keyword.html) keyword can be considered an extension to strict aliasing. It says that for the lifetime of the pointer, only the pointer itself or a value directly derived from it (such as pointer + 1) will be used to access the object to which it points. This limits the effects of pointer aliasing, aiding optimizations.
- This [SIMD Performance tuning video](https://youtu.be/_OJmxi4-twY?list=PLGj2a3KTwhRb6LNVucPkwdpzg9OHd8jli&t=770) shows a good example of why restrict keyword helps in allowing compiler to vectorize instructions when using restrict.
- Note: One pointer is said to alias another pointer when both refer to the same location or object.
```CPP
  void vectorized_add(float *restrict c, float *restrict a, float *restrict b);
  //Restrict here is to signify that either a,b,c will be used directly or with index [] operator. There will not be any other pointer aliased to a,b or c.
```
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Useful Performance considerations
--------------------------------------------
| Technique                            | Purpose                                                   |
|:------------------------------------:|:----------------------------------------------------------|
| **Use Pre-increment operator (\+\+it insteadof it\+\+)** | Any kind of postfix operation on a non-integer type (such as iterator), strictly speaking, **requires a temporary copy** <br> While compiler MAY be able to eliminate this temporary as a part of optimization, it is easier to do it by hand. In post-increment case, old value has to be saved before the value is incremented which might be slower. |
| **Pass by const reference**          | This should be the default mechanism to avoid calling copy constructor (when passing directly by value) <br> Note that since C++11, due to **copy elision** (which is not mandated but is a de-facto standard by modern compilers), it is usually better performance-wise to pass classes/structs/collections by value **ONLY to create a copy of the relevant parameter within your function** |
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Standard Template Library (STL)
--------------------------------------------
- STL is a three legged stool sitting on top of Containers, Iterators and Algorithms
- Refer to this [quick reference sheet](./STL Quick Reference 1.29.pdf) for overview on containers, algorithms and iterators with example.

###Containers###
- ++**Sequence Containers**++
  - Array(C\+\+11 - fixed size), Vector, Deques, List, Forward_List (C\+\+11)
- ++**Associative Containers**++
  - Set, Map (RB-Tree based), Unordered_Map(C++11 - Hash based map)
- Notes about different containers:
  | Container name                               | Description                                                                                       |
  |:---------------------------------------------|:--------------------------------------------------------------------------------------------------|
  | **std::vector**                              | Contiguous memory (best performance). Almost like an array, but safer and can be expanded.  <br> Vectors are array based and are perfect as a stack. <br> To use a Vector as a queue which is either short lived or has a total upper bound (max elements known), use an index in the front of the vector to traverse the nodes and add new nodes in the end. |
  | **std::dequeue (Double Ended Queue)**        | Deque manages its elements with a **dynamic array (expanded and contracted both ends), provides random access, and has almost the same interface as a vector**. <br> Deque provides **Fast insertions and deletions at both the end and the beginning**. Inserting and deleting elements in the middle is relatively slow because all elements up to either of both ends may be moved to make room or to fill a gap. <br> Any insertion or deletion of elements other than at the beginning or end **invalidates all pointers, references, and iterators that refer to elements** of the deque. <br> Note: push_back, pop_back, etc operation modifies the dequeue container and thus invalidate loop iterators <br> [See differences between list and dequeue here](http://stackoverflow.com/questions/1436020/c-stl-containers-whats-the-difference-between-deque-and-list) |
  | **std::list**                                | Doubly linked-list with each node allocated separately. Therefore, traversal operations chase pointers to totally new memory (not cache efficient) <br> List manages its elements as a doubly linked list and does not provide random access. <br> In List, inserting and removing elements is fast at each position, and not only at one or both ends. <br> Recommended use case - Rarely traverse the list, but very frequently update the list. |
  | **std::set, std::map**                       | Linked list, oriented as a binary tree. This has all the downsides of linked list. Insert, Delete and re-balance (even with hinting) are traversal <br> std::set only stores key.  std::map stores key + associated value (eg, value can be a count of #occurrences of that key). <br> Note, in **std::map** keys must be unique. **std::multimap** allows duplicate keys |
  | **std::unordered_map**                       | Hash table with separate chaining. Each bucket is a linked list.  <br> An unordered_multimap allows multiple copies of the same mapping. <br> Note: only has Forward Iterators (so does not support O(1) lookup or backward iteration). |
  | **std::priority_queue**                      | A priority queue provides **constant time lookup for largest element**(by default), at the expense of logarithmic insertion and extraction. |
  - Stroustrup, in his book, "Programming Principles and Practice Using C++", talked about the rule of thumb of which container we should choose:
    - Use **vector** unless you have a good reason not to.
    - Use **map** if you need to look up based on a value (and if your key type has a reasonable and **efficient less-than operation**).
    - Use **unordered_map** if you need to do a lot of lookup in a large map and you don't need an ordered traversal (and if you can find a good hash function for your key type).
  - Refer to [C++ performance benchmarks](http://baptiste-wicht.com/posts/2012/12/cpp-benchmark-vector-list-deque.html) to understand vector vs list vs dequeue. Below is the summary:
    - std::list is very very slow to iterate through the collection due to its very poor spatial locality.
    - std::vector and std::deque perform always faster than std::list with very small data
    - std::list handles very well large elements
    - std::deque performs better than a std::vector for inserting at random positions (especially at the front, which is constant time)
    - std::deque and std::vector do not support very well data types with high cost of copy/assignment
- Priority_queue examples:
  ```C++
  std::priority_queue<int, std::vector<int>>                    maxheap;   //Max-Heap (default)
  std::priority_queue<int, std::vector<int>, std::greater<int>> minheap;   //Min-Heap
  ```
	
###Iterators###
- Iterators are navigators used to navigate through containers.
- STL Design principle - use the ++weakest iterator category++ that accommodates the most efficient algorithm.
  - eg. Quicksort requires random access, as a consequence it cannot be used on a non-random access container such as list.
- Below are the 5 iterator categories in STL ranked from weakest to strongest:
  | Iterator type       | Pass    | R/W     | Direction                   | Property                                                                 | Provider          |
  |:--------------------|:--------|:--------|:----------------------------|:-------------------------------------------------------------------------|:------------------|
  | **Input** (Weakest) | One-Pass | Read Only | Forward Moving           | Can only move in forward direction (one element at a time). The same input iterator cannot be used to pass through a sequence twice | istream |
  | **Output**          | One-Pass | Write Only | Forward Moving          | Can only move in forward direction (one element at a time). The same output iterator cannot be used to pass through a sequence twice | ostream |
  | **Forward**         | **Multi-Pass** | **Read+Write** | Forward Moving | Combines capability of input and output iterators and retains their position in the container (state). <br>The same iterator can be used to pass through sequence more than once (multi-pass algorithms)  | unordered_map  |
  | **Bidirectional**   | **Multi-Pass** | **Read+Write** | **Forward+Backward moving** | Combines capability of forward iterator with ability to move in reverse direction. Supports multi-pass algorithms      | list, <br> set and multiset <br> map and multimap |
  | **Random Access** (Strongest) | **Multi-Pass** | **Read+Write** | **Random Access** | Combines capability of a bi-directional iterator with ability to jump and directly access any element of the container.    |  Array <br> Vector <br> Deque |
- Below is an example of a function that uses iterator:
```CPP
  string FindAddr( const list<Employee>& emps,
                   const string& name )
  {
      list<Employee>::const_iterator end( emps.end() );
      for( list<Employee>::const_iterator i = emps.begin(); i != end; ++i )
      {
          if( i->name == name )
              return i->addr;
      }
      return "";
  }
```
- In the above function, instead of trying to write the find logic manually, we could have re-used STL find:
```CPP
  string FindAddr( const list<Employee>& emps, const string& name )
  {
      list<Employee>::const_iterator i( find(emps.begin(), emps.end(), name) );
      if( i != emps.end() )
          return i->addr;
      return "";
  }
```
- Below [example shows how to chose a random element](http://stackoverflow.com/questions/27024269/select-random-element-in-an-unordered-map-c) from a collection (note: if vector was used, then this is an O(1) operation)
```CPP
  std::unordered_map<std::string, Edge> edges;
  auto random_it = std::next(std::begin(edges), rand_between(0, edges.size()));
```

###Algorithms###
- Lower and Upper Bound (find example @ [vector_bounds_example.cc](https://github.com/prashrock/CPP/blob/master/random_qs/vector_bounds_example.cc)):
  | Lower Bound                                            | Upper Bound                                               |
  |:-------------------------------------------------------|:----------------------------------------------------------|
  | Lower bound returns an iterator pointing to the first element in the range [first,last) that is **not less** than value. <br> Note: If *it!=num, we have bigger#, so go back | Upper bound returns an iterator pointing to the first element in the range [first,last) that is **greater** than value. |
  | int lb = std::lower_bound(v.begin(), v.end(), find_val) - v.begin() | int ub = std::upper_bound(v.begin(), v.end(), find_val) - v.begin() |

  ```SHELL
  Below is example vector of length 9
  input:   1   1   2   3   4   8   9   9   9
  0  lb @ 0, ub @ 0
  1  lb @ 0, ub @ 2
  3  lb @ 3, ub @ 4
  5  lb @ 5, ub @ 5
  9  lb @ 6, ub @ 9
  10 lb @ 9, ub @ 9
  ```

###Lock###
- [Scoped Mutex (Lock Guard)](http://en.cppreference.com/w/cpp/thread/lock_guard)
  - When a lock guard object is created, it attempts to take ownership of mutex within the scope.
  - At end of scope, the mutex is released and the lock_guard is automatically destructed.
  ```CPP
  #include <mutex>
  int i = 0;
  std::mutex g_mutex;
  void safe_increment() {
      std::lock_guard<std::mutex> lock(g_mutex);
      ++i;
      // g_mutex is automatically released when lock goes out of scope
  }
  ```

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FAQs
--------------------------------------------
- Most of these FAQs have been taken from [C++ Interview Questions](http://alumni.cs.ucr.edu/~lyan/c++interviewquestions.pdf)

###Class member Scope###
| Type                       | About                                                |
|:---------------------------|:-----------------------------------------------------|
| **Public members**         | Public members are accessible from anywhere the class is visible. |
| **Private members**        | Private members are only accessible from the same class **or a friend (function or class)**. |
| **Protected members**      | Protected members are accessible from the same class, **derived classes, or a friend (function or class)**. |

###Constructor/Destructor###
- Purpose of [virtual destructor](http://www.programmerinterview.com/index.php/c-cplusplus/virtual-destructors/)
  ```CPP
  #include iostream.h
  class Base {
     public:
               Base(){ cout<<"Constructing Base"; }
      virtual ~Base(){ cout<<"Destroying Base"; }
  };
  class Derive: public Base {
     public:
       Derive(){ cout<<"Constructing Derive"; }	
      ~Derive(){ cout<<"Destroying Derive";}
  };
  void main() {
       Base *basePtr = new Derive();
       delete basePtr;
  }
  ```
  | Output without virtual destructor in base-class    | Code with virtual destructor in base-class      |
  |:---------------------------------------------------|:------------------------------------------------|
  | Constructing Base <br> Constructing Derive <br> Destroying Base | Constructing Base <br> Constructing Derive <br> **Destroying Derive** <br> Destroying Base |

- Common Questions
  | Question                                       | Answer                                          |
  |:-----------------------------------------------|:------------------------------------------------|
  | Can a Constructor be Virtual ?                 | The Constructor **cannot be virtual** as the constructor is responsible for creating an object and initializing it. It also creates vtable for virtual functions. It can't be delegated to any other object by virtual keyword means. |
  | Can a Destructor be Virtual ?                  | Yes. A destructor can be virtual as it is possible as at runtime depending on the type of object caller to call the proper destructor. Using virtual destructors, you can destroy objects without knowing their type - the correct destructor for the object is invoked using the virtual function mechanism. Note that destructors can also be declared as pure virtual functions for abstract classes. <br> If you destroy an object through a reference to a base class, and the base-class destructor is not virtual, the derived-class destructors are not executed, and the destruction might not be complete. |
  | Can a Constructor be Private ?                 | Yes. Private constructor means user cannot directly instantiate a class. Instead, use something like [Named Constructor Idiom](https://isocpp.org/wiki/faq/ctors#named-ctor-idiom)
  | Destructor overloading and explicit calling ?  | There can only be one destructor for each class (no overloading). It never takes any parameters, and it never returns anything. <br> Do not call destructor explicitly as it will be called at end of the block. Only exception is when using placement new.                     |
  | Can a Constructor throw an exception ?         | No. A Constructor can never throw an exception  |
  | What is assignment operator ?                  | Default assignment operator handles member to member (shallow copy) between two objects of the same class.                       |
  | Difference between Copy constructor and overloaded assignment operator | A copy constructor constructs a new object by using the content of the argument object. The copy constructor parameter must [always be passed by reference](http://stackoverflow.com/questions/2685854/why-should-the-copy-constructor-accept-its-parameter-by-reference-in-c)<br> An overloaded assignment operator assigns the contents of an existing object to another existing object of the same class. |
  | How can I handle a constructor that fails ?    | throw an exception. Constructors don't have a return type, so it's not possible to use return codes. The best way to signal constructor failure is therefore to throw an exception. |
  | How can I handle a destructor that fails ?     | Log, don't throw an exception. The C++ rule is that you must never throw an exception from a destructor that is being called during the "stack unwinding" process of another exception. |
  | What are some implicit member functions of a class ? | (a) default constructor <br>(b) copy constructor <br>(c) assignment operator <br>(d) default destructor <br>(e) address operator |
- **Placement new**
  - Used to place an object at a particular location in memory. This is done by supplying the place as a pointer parameter to the new part of a new expression:
  ```CPP
  char memory[sizeof(MyClass)];
  MyClass* f = new(memory) MyClass();
  // The pointers f and memory will be equal
  ```
  - Note1: You are taking sole responsibility (wrt alignment, size, etc) that the pointer you pass to "placement new" call.
  - Note2: You are also solely responsible for destructing the placed object. This can be done by explicitly calling the destructor
- **Destructor order**
  - In reverse order of construction: First constructed, last destructed.
  - This applies to objects in an array as well:
  ```CPP
  MyClass a[10]; //order for destructors will be a[9], a[8], ..., a[1], a[0]
  ```
- **Copy Elison**
  - Copy elision (or Copy omission) is a compiler optimization technique that avoids unnecessary copying of objects. Example:
  ```CPP
  B ob = "copy me"; //copy initialization
  //can be replaced as below to elide calling copy constructor
  B ob("copy me"); //direct initialization
  ```
- **When to use a virtual destructor?**
  - If your class has at least one virtual function, you should make a destructor for this class virtual. 
  - This will allow you to delete a dynamic object through a caller to a base class object.
  - If the destructor is non-virtual, then wrong destructor will be invoked during deletion of the dynamic object.

###Virtual Function###
- Common Questions
  | Question                                       | Answer                                          |
  |:-----------------------------------------------|:------------------------------------------------|
  | What is a Virtual function ?                   | When derived class overrides the base class method by redefining the same function, then if client wants to access redefined the method from derived class through a pointer from base class object, then you must define this function in base class as virtual function. |
  | What is a "Pure Virtual" function ?            | This construct should be used when one wants to ensure that all the derived classes implement the method defined as pure virtual in a base class. If not implemented, a compilation error will be thrown. |  
- **Virtual function implementation**:
  - Virtual functions are implemented using a **table of function pointers (called vtable)**.
  - There is one entry in the table per virtual function in the class. This table is created by the constructor of the class.
  - When a derived class is constructed, its base class is constructed _rst which creates the vtable.
  - If the derived class overrides any of the base classes virtual functions, those entries in the vtable are overwritten by the derived class constructor. This is why you should never call virtual functions from a constructor: because the vtable entries for the object may not have been set up by the derived class constructor yet, so you might end up calling base class implementations of those virtual functions.

###Inheritance and Polymorphism###
- Common Questions
  | Question                                       | Answer                                               |
  |:-----------------------------------------------|:-----------------------------------------------------|
  | What is inheritance ?                          | Inheritance allows one class to reuse the state and behavior of another class. The derived class inherits the properties and method implementations of the base class and extends it by overriding methods and adding additional properties and methods.               |
  | Go over the life cycle of a derived-class object ? | - Enough Space (data members of base + derived class) is allocated (on the stack or the heap) <br> - The base class constructor is called to initialize the data members inherited from the base class <br> - The derived class constructor is then called to initialize the data members added in the derived class <br> - When the object is destroyed (goes out of scope or is deleted) the derived class destructor is called on the object first <br> - Then the base class destructor is called on the object <br> - Finally the allocated space for the full object is reclaimed.                             |
  | What does a derived class inherit ?            | Inherits: Every data member defined in the parent class <br> Doesnt Inherit: The base class' constructors and destructor. The base class' assignment operator. The base class' friends       |
  | What is multiple inheritance ?                 | Multiple Inheritance is the process whereby a child can be derived from more than one parent class. <br> The advantage of multiple inheritance is that it allows a class to inherit the functionality of more than one base class thus allowing for modeling of complex relationships. <br> The disadvantage of multiple inheritance is that it can lead to a lot of confusion(ambiguity) when two base classes implement a method with the same name.                       |
  | What is Polymorphism ?                         | Polymorphism allows a client to treat different objects in the same way even if they were created from different classes and exhibit different behaviors. Base class objects pointer can invoke methods in derived class objects. You can also **achieve polymorphism in C++ by function overloading and operator overloading**. |
  | Can a friend call be inherited ?               | **friendship isnt inherited, transitive, or reciprocal** <br> - Just because I grant you friendship access to me doesnt automatically grant your kids access to me, doesnt automatically grant your friends access to me, and doesnt automatically grant me access to you. <br> Example of a friend is to use operator overloading to provide a friend left-shift operator.                                        |

###Memory Management and Pointers###
- Common Questions
  | Question                                       | Answer                                               |
  |:-----------------------------------------------|:-----------------------------------------------------|
  | Difference between malloc/free and new/delete ?| - Malloc/free allocate and de-allocate memory while new/delete create and destroy objects. In other words, malloc/free does not know about constructors and destructors. <br> - Delete calls the object's destructor and then releases heap memory allocated to the object. <br> - Note: If an array of objects were created with new Obj[n], then delete must be told that it is dealing with an array (e.g., delete []objs) |
  | Why prefer new instead of malloc ?             | - Invoke Constructor and Destructor automatically <br> - Type Safety: Malloc returns a void ptr whereas new returns pointer of the correct type <br> - Overridability: new is an operator that can be overriden by a class |
  | What happens when new does not succeed ?       | New does not return nullptr, instead it [throws an exception](http://stackoverflow.com/questions/26419786/why-doesnt-new-in-c-return-null-on-failure). To force new to return nullptr, use **std::nothrow** option when calling new         |
  | What is a dangling pointer ?        | A dangling pointer arises when you use the address of an object after its lifetime is over. |
  | Difference between pointer and reference       | A reference must always refer to some object and, therefore, must always be initialized; pointers do not have such restrictions. A pointer can be reassigned to point to different objects while a ref always refers to an object with which it was initialized.      |
  | What is a const pointer ?                      | - **const char *myPointer** is a non constant pointer to **constant data** <br> - **char *const myPointer** is a **constant pointer** to non constant data. |
  | Can a C++ program crash before main() ?        | C++ allows for dynamic initialization of global variables before main() is invoked. It is possible that initialization of global will invoke some function. If this function crashes the crash will occur before main() is entered.                                         |

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

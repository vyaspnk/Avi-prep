--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C Semantics
============================================
[TOC]

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Storage Class Specifier
--------------------------------------------
- A storage class defines the scope (visibility) and life-time of variables and/or functions within a program.
- In declaration syntax, Storage Class **precedes the type that they modify**. Below are the most common storage classes in C:
  | Storage Class       | Purpose                                                                                                                                                                      |
  |:--------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
  | **Auto**            | **Default storage class for all local variables**. Auto can only be used within functions.                                                                                   |
  | **Register**        | Define **local variables that should be stored in register instead of RAM**. This implies maximum size of variable = register size(4B on 32-bit CPU and 8B on 64-bit CPU)    |
  |                     | Address of (&) does not apply to registers. Application of this specifier is not guaranteed.                                                                                 |
  |                     | The only real purpose of "register" is to assert to compiler that an auto var is never addressed and can never alias with any pointer.                                       |
  | **Static**          | Keep a local variable in existence during life-time of program instead of create/destroy each time it comes into and goes out of scope.                                      |
  |                     | Making local variables static allows them to maintain their values between function calls.                                                                                   |
  |                     | On Global variable, static modifier causes that variable's scope to be restricted to the file in which it is declared.                                                       |
  | **Extern**          | The extern storage class is used to extend the visibility of a variable/function to ALL program files (.c and .h) which can see this line.                                   |
  |                     | Extern modifier is common when 2 or more files share the same global variable/function.                                                                                      |
  |                     | With Extern modifier, a variable cannot be initialized as extern only points the name to a previously defined storage location.                                              |
  | **__thread**        | Thread-local storage (TLS) is a mechanism by which variables are allocated such that there is one instance of var per extant thread.                                         |
  |                     | The __thread specifier may be used alone, with the extern or static specifiers, but with no other storage class specifier.                                                   |
  |                     | Using TLS will have slight overhead, as the address has to be determined at runtime with thread_id with a lookup table.                                                      |
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Type Specifier
--------------------------------------------
- Type specifiers indicate the type of the object or function being declared.
  | Type Specifier      | Purpose                                                                                                                                                                      |
  |:--------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
  | **Simple**          | A simple type specifier either specifies a (previously declared) user-defined type or a fundamental type (char, int, float, etc)                                             |
  | **Enumerated**      | An enumeration is a data type consisting of a set of values that are named integral constants.                                                                               |
  |                     | Enumerations are useful for variables that have a small number of possible values.                                                                                           |
  |                     | A named value in an enumeration is called an enumeration constant. Enumeration variables are treated by the compiler as integer types.                                       |
  |                     | Use "-qenum" Compile option to specify the size of Enumerated variables.			                                                                                           |
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Type Qualifiers
--------------------------------------------
- Type Qualifiers are keywords which are used to modify the properties of a variable.
- Const & Volatile are only meaningful with lvalue
  | Type Specifier      | Purpose                                                                                                                                                                      |
  |:--------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
  | **Const**           | Constants are like normal variables, but their values can't be modified by the program once they are defined.                                                                |
  |                     | They refer to fixed values. They are also called as literals.                                                                                                                |
  |                     | Note, A const value is not equivalent to "final" in Java. A const variable can be modified by another pointer (const int \* -> int \*)                                       |
  |                     | This is why const variables in C cannot be used to declare global arrays. However, a const function argument can be used to declare a function local array because by at the time of creation of local array, const fn argument will be a constant. |
  | **Volatile**        | A Variable whose values might change without any explicit assignment (=) by the program.                                                                                     |
  |                     | Volatile keyword only impacts (i.e. avoids) re-orderings performed by the compiler, not by the processor. Therefore, volatile might affect compiler optimizations. <br>Note, volatile variables are still subject to processor reordering.       |
  |                     | Volatile does not imply atomic. |
  |                     | Volatile is NOT required for critical section variables when using a mutex. It is more useful for accessing memory mapped I/O  |
  |                     | In order to avoid the compiler from optimizing out a load instruction, instead of using _volatile_, consider using **ACCESS_ONCE()**                                         |
- A volatile const variable cannot be modified by a process, but could be modified asynchronously by an external source.
- Read this [excellent linux kernel notes](https://www.kernel.org/doc/Documentation/volatile-considered-harmful.txt) about volatile for further info on volatile.

###Constant Pointer vs Pointer to Constant###
- Below table summarizes how the meaning of 'const' changes based on where its located:
  | Type                     | Code                        | Meaning                                                                                           |
  |:-------------------------|:----------------------------|:--------------------------------------------------------------------------------------------------|
  | Constant Pointer         | int *const ptr              | The location of the pointer (i.e., where it is pointing to) cannot be changed.                    |
  | Pointer to Constant      | const int \*ptr  <br> int const \*ptr   | This is a pointer to a constant. i.e., value cannot be changed                                    |
- The above example applies to both type of qualifiers - 'const' and 'volatile'
- Below are two common type of array + pointers:
```C
int *a[10];        // An array of 10 pointers to integers
int (*a)[10];      // A pointer to an array of 10 integers
```

###Array vs Pointer to array###
- Printing the [address of an array variable and the array variable itself](http://arjunsreedharan.org/post/69303442896/the-difference-between-arr-and-arr-how-to-find), both operations yield the same result:
  ```C
  int arr[5] = {1};
  printf("Address of arr  = %p\n", arr);
  printf("Address of &arr = %p\n", &arr);
  //Address of arr  = 0x244fdc4
  //Address of &arr = 0x244fdc4
  ```
- Incrementing arr and &arr by 1 have different properties:
  ```C
  int arr[5] = {1};
  printf("Address of arr+1  = %p\n", arr + 1);
  printf("Address of &arr+1 = %p\n", &arr + 1);
  //Address of arr  = 0x244fdc8
  //Address of &arr = 0x244fdd8
  ```
- With the above two examples we can infer that:
  | Property                 |  Array variable (arr)                           | Address of array variable (&arr)                |
  |:------------------------:|:------------------------------------------------|:------------------------------------------------|
  | Variable type            | **arr** has the type **int \***                 | **&arr** has the type **int (*)[size]**         |
  | Address                  | **arr** has the same address as **&arr**        | Refer to the first example in this section      |
  | Increment operation      | **(arr + 1)** points to the **second element of the array**. | **(&arr + 1)** points to the memory **address after the end of the array**.        |
  | Meaning                  | **arr** points to the first element of the array | **&arr** points to the entire array            |
- Since ==**&arr** points to the entire array where as **arr** points to the first element of the array==, we can [use **&arr** to find the size of an array directly](http://arjunsreedharan.org/post/69303442896/the-difference-between-arr-and-arr-how-to-find):
  ```C
  int n = *(&arr + 1) - arr;
  int n =  (&arr)[1]  - arr;  // since x[1] == *(x+1)
  ```
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Using void * for pointer arithmetic
--------------------------------------------
- Support for (void \*) for pointer arithmetic is compiler specific, but if sizeof(\*(void \*)) == 1, then pointer arithmetic will be correct.
- However, arithmetic on void * is illegal in both C and C++ (n1256:6.5.6-2, 6.2.5.1, 6.2.5-19)
  - **uintptr_t/intptr_t**: Any valid pointer can be converted to this type to do arithmetic.
  - **char \***: (char *) is convenient for arithmetic as char type is guaranteed to have a size of 1 Byte.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Setjmp() / longjmp()
--------------------------------------------
- C does not provide any primitives for handling exceptions.
- Instead, C provides 2 operations setjmp/longjmp for non-local control-flow aka non-local "goto".
  - **int setjmp(jmp_buf env)** - save state of registers including PC (current executing line) + signal mask into 'env' and returns 0 first time upon call.
  - **void longjmp(jmp_buf env, int val)** - Copies saved registers (SP, FP, PC) from 'env' to CPU, computes PC from return value of setjmp().  This operation translates to JMP instruction = i.e., longjmp does not return.
- With these 2 operations, normal stack unwinding does not happen.
  - Therefore, if function where setjmp was called returns, it is no longer safe to use longjmp as stack frame is invalidated when function returns.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Unary arithmetic
--------------------------------------------
- Below is the practical meaning of the two possible usages:
  | Usage          | Meaning                                                                       | Example           |
  |:---------------|:------------------------------------------------------------------------------|:------------------|
  | i++            | Increment the value of i, but return the original value that i held before being incremented. |  i = 1; j = i++;      |
  |                |                                                                               | (i is 2, j is 1)  |
  |++i             | Increment the value of i, and then return the incremented value (new value).  |  i = 1; j = ++i;  |
  |                |                                                                               | (i is 2, j is 2)  |
- Remember, when used within a **for** loop, either approaches produces the same result.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Arithmetic vs Logical Shift operator
--------------------------------------------
- Right Shift
  - Logical right shift by 1 bit shifts all the bits to the right and fills in the leftmost bit with a 0. 
  - Arithmetic right shift leaves the original value in the leftmost bit.
  - C/C++ 'usually' (compiler dependant) implement an arithmetic right-shift operation (>>). Java implements both arithmetic (>>) & logical (>>>) shifts.
- Left Shift 
  - Left shifts are mostly logical shifts.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Undefined behavior
--------------------------------------------
- Overflow of signed integers (say during addition) results in undefined behavior.
- Right shifting a -ve number typically retains 1's in the higher order bits (so that sign is retained), but this behavior is not required by C standard.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Arrays of zero-length
--------------------------------------------
- What is the difference between the below two structures in an x86_64 environment. What are the pros and cons of them ?
  ![Pointer to Array vs Array](./images_C_semantics/1_ptr_to_array_vs_array.png "Pointer to Array vs Array")
- Note, **C has an implicit cast from the array type to it's respective pointer**, just by taking the address of the beginning of the array. This offers a very good property of allowing arrays and pointers to be accessible through the same syntax 'data[i]'.
- Note, when used as a function argument, both syntaxes are equivalent (however, the [] make it clear that we are talking about an array of values and not about a single pointed object). To avoid this confusion, clang offers a compiler warning(-Wsizeof-array-argument)) to catch such cases.
| Context                    | Pointer to array                                                                                  | Array of zero-length                                                                              |
|:---------------------------|:--------------------------------------------------------------------------------------------------|:---------------------------------------------------------------------------------------------------|
| Structure Padding          | Since a pointer is 8B, 'data' has an alignment requirement of 8B causing a padding of 4B between 'len' and 'data'. | Since 'data' is a variable length character array (1B each), and since an array has alignment requirement of each individual object, 'data' only has to be aligned by 1B which means it can immediately follow 'len', no padding required |
| Size                       | Size of foo_t = 16B (4B for int + 4B of padding + 8B for data pointer | Size of bar_t = 4B. |
|                            | If malloc() is used for allocation, __ptmalloc2__ in glibc, requires 16B additional (8B per allocation) for malloc metadata | If malloc() is used for allocation, requires 8B additional for malloc metadata |
| Memory layout              | ![Pointer to Array mem layout](./images_C_semantics/2_ptr_to_array_mem_layout.png "Pointer to Array mem layout") | ![Array mem layout](./images_C_semantics/3_array_mem_layout.png "Array mem layout") |
| Allocation                 | 'data' can point to anywhere in memory (greater flexibility)        | 'data' has to immediately follow, i.e., the whole bar_t structure should be contiguous            |
|                            | 'data' can be re-allocated without any changes to foo_t structure   | 'data' cannot be re-allocated without re-allocating the whole bar_t structure, potentially changing its pointer |
| | Note, foo_t can still be allocated with a single allocation, however this will make reallocation harder (since the pointed data vector is not the allocated one and thus is not directly reallocable). This will slightly reduce the overhead for foo_t and improves the access performances | bar_t already has these features built-in |
|                            | ![Pointer to Array alloc](./images_C_semantics/4_ptr_to_array_alloc.png "Pointer to Array alloc") | ![Array alloc](./images_C_semantics/5_array_alloc.png "Array alloc") |
| Embed by value in another structure | Can easily be embedded in another containing structure since 'data' is standalone | Hard to embed by value in another structure (can only be kept at the end). |
|                            | ![Pointer to Array embed](./images_C_semantics/6_ptr_to_array_embed.png "Pointer to Array embed") | ![Array embed](./images_C_semantics/7_array_embed.png "Array embed") |
| Cache locality             | If 'data' is allocated independently, accessing 'data' involves two cache misses, once to access foo_t and next to access foo_t->data | Since 'data' immediately follows bar_t, 'data' need not be dereference separately |
|                            | ![Pointer to Array cache](./images_C_semantics/8_ptr_to_array_cache.png "Pointer to Array cache") | ![Array cache](./images_C_semantics/9_array_cache.png "Array cache") |
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Inlining Function
--------------------------------------------
- Using GCC option $\text{-Winline}$, causes compiler to warn when a function marked inline could not be substituted, and gives reason for failure.
- Easiest way to mark a function for inline is as below:
```CPP
  static inline int inc (int *a) {
      return (*a)++;
  }
```
- In non-optimized mode (O0), GCC does not inline any functions unless $\text{always_inline}$ attribute is used for the function
```CPP
  /* Prototype.  */
  inline void foo (const char) __attribute__((always_inline));
```
- [Stephen Hemminger follows Linux/Linus advice and resist the urge to add strong inlining](http://thread.gmane.org/gmane.comp.networking.dpdk.devel/25549/focus=25599):
  - The compiler does a good job of deciding to inline, and many times the reason it chooses for not inlining are quite good like:
    - The code is on an unlikely branch
    - Register pressure means inlining would mean the code would be worse
  - Therefore my rules are:
    - Only use inline for small functions. Let compiler decide on larger static funcs
    - Write code where most functions are static (localized scope) where compiler can decide
    - Reserve always inline for things that access hardware and would break if not inlined.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Compiler options (GNU GCC)
--------------------------------------------
| Option                     | Meaning                                                             |
|:--------------------------:|:--------------------------------------------------------------------|
| -fomit-frame-pointer       | With this option EBP is used as a general-purpose register in optimizations. <br> Some debuggers expect EBP to be used as a stack frame pointer, and cannot produce a stack backtrace unless this is so. So, debuging might not be straightforward with this option.<br> On X86 32-bit, where are there are only 8 registers ([including ESP](http://www.swansontec.com/sregisters.html)), not using one register as frame pointer saves [12.5% register space](http://stackoverflow.com/questions/14666665/trying-to-understand-gcc-option-fomit-frame-pointer) and 1-3 instructions on entry/exit. This is not that significant in x86-64 which has 16 registers <br> [This option is automatically enabled when you specify option -O1, -O2, or -O3](https://software.intel.com/en-us/node/522827). <br> To explicitly disable this option in O2,O3 OPT modes use **-fno-omit-frame-pointer** |
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
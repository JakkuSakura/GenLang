# INTRODUCTION TO GENLANG

GenLang(General Language) is a programming language. One can wirite his code in GenLang and it will be translated into C++(Maybe Java/C#/C/...).

Feature List:

- [ ] use a packages
- [ ] declare a variable
- [ ] variable assignment
- [ ] function declaration
- [ ] inner string support
- [ ] inner list support
- [ ] static dynamic type
- [ ] Garbage Collection
- [ ] strong STL
- [ ] less code
- [ ] less bugs

## Class System
use `class` to declare a class. A class could extend form just a base class, and it will be object(inner type is object) if it is not pointed out. Also a class could implement none or more interfaces.  



use `interface` to declare an interface. An interface could just contain none or more methods.

use `{key:val,...}` to declare a js-like object(inner type is map_object). use `o.key`/`o["key"]`/`o.get("key")` to get its val

use `[val1, val2, val3, ...]` to declare a python-like list(inner type is list). use `l[n]`/`l.get(n)` to fetch val-n.

use `struct` to declare a c-like object, with just c/c++ types in it.

each type has its MetaClass instance.

pass argument by val, for GenLang class, it's by refer actually.

## basic types
basic types are allocated in stack 


int uint                     `let i = 1;`       `let i = u1;`   
long ulong                   `let l = 666L;`    `let l = u666L;`   
short ushort                 `let s = 234s;`    `let s = u234s;`  
byte ubyte                   `let b = 122;`     `let b = u122;`  
char uchar(unicode-support)  `let c = 'h';`     `let c = u'字';`  
float double                 `let f = 123.0f;`  `let d = 123.0;`

each of these basic types has it wrapper const class like Int, uInt, Long, uLong, etc.. when it be included by class, it will be automacally boxed.

## c-array

type[n](traditional c array with just c/c++ types in it) `let a as int[5];`  
cannot return a c-array



## dynamic types
string ustring      `let s = "hello";`  `let s = u"hello";`  
array               `let a = array(int, 666);`  
list                `let l = ["hello"];`  
map_object          `let mb = {};`  




## use a package

```csharp
import basic_io;
```

## declare a variable

- when declaring a new variable, you have to choose either a type(and it wil be automatically inialized) or a default value
- as well, you can new an object like `let obj = object();`, use `let obj as object` to declere a pointer

```csharp
let a = 1;
let f = 1.0f;
let d = 1.0;
let l as long;
let o2 as object;

```

## change a val

use `=` to change a val

```csharp
b = 1;
b = 10.3; // warnning
b = 10.3 as int; // no warnning
b = int(10.3);

o = object(1, 2, 3);
o2 = o.clone();
```

## declare a function

```csharp
int foo(int a, int b)
{
    return a + b;
}
void foo(int a, int b)
{
    print(a + b);
    return;
}
```

## use a function

```csharp
foo();
print(a, a + b);

```
## xx

```csharp
let a = 18;
let foo = { name: "jack", age: a};
```

```java
Object *foo = gc.newObject();
foo->append("name", gc.newString("jack"));
foo->append("age", gc.newInt(18));
```

## inner functions

- id  == &x
- type == typeid(x)
- str == x.toString()
- etc...

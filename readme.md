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
foo a + b;
print a, a + b;

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

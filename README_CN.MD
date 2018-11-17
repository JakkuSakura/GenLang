# GenLang介绍
GenLang是一个基于c++的编译器，正处于设计和开发阶段。它为c++添加上了GC、动态类型支持和统一的OOP风格，兼有静态语言的运行效率和动态语言的编写效率。除此之外，它减少了c++的一些坑，比如封装了指针，来做到类似java的对象。在明明风格上，和python比较接近。
## 类型系统
GenLang的类型系统分为两大类，一类是OOP部分，所有OOP都有一个父亲，为object，另一类是传统的类型，由所有c++的基本类型组成。

object
    container
        container只内包含object的子类
        array       限定类型，固定长度的数组，有语法层面支持
        map_object  有语法层面支持
        list        有语法层面支持
    wrapper
        wrapper中的类型是对以下类型的简单封装，并且由编译器做到自动装箱和拆箱
        String      string
        Int         signed int
        Long        signed long long
        Short       signed short
        Byte        signed char
        uInt        unsigned int
        uLong       unsigned long long
        uShort      unsigned short
        uByte       unsigned char
        Struct      some struct
对于object，传参的方式是引用，对于基本类型，传参数的方式是传值。

## 元对象
每一种object都有对应的唯一的一个meta_object，自动注册到mate_manager中，里面有对象类型的string和meta_object的一一对应关系。meta_object存储了一些必要的信息，主要是内存分布和类型信息，用于垃圾回收。


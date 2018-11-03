#include <bits/stdc++.h>
#include "Basic.h"
#include "GC.h"
using namespace GenLang;
int main()
{
    GC gc;
    Object *obj = gc.newObject();
    List *lst = obj->put("hello", gc.newList());
    lst->put(gc.newString("I have a DREAM"));
    std::cout << obj->toString() << std::endl;
    std::cout << lst->toString() << std::endl;
    std::cout << gc.autoClean(NULL) << std::endl;
    std::cout << gc.autoClean(NULL) << std::endl;
    return 0;
}

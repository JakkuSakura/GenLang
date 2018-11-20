#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include "genlang/container.h"
#include "genlang/string.h"
#include "genlang/runtime_support.h"
#include "genlang/wrapper.h"
#include "genlang/utils.h"
namespace GenLang {
    list *split(const string &str);
    list *split(const String &str);
}

#endif //PROJECT_UTILS_H

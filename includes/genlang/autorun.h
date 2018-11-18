#ifndef GENLANG_AUTORUN_H
#define GENLANG_AUTORUN_H


struct autorun {
    autorun(void (*f)()) {
        f();
    }
};

#endif // GENLANG_AUTORUN_H

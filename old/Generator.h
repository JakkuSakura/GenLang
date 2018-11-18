//
// Created by Lenovo on 2018/11/9.
//

#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include <stdio.h>
#include "Scanner.h"
namespace GenLang{
class Generator {
    FILE *fout;
    List *program;
public:
    explicit Generator(FILE *file) {
        fout = file;
        program = NULL;
    }
    void receiveProgram(List *p) {
        program = p;
    }
    
    void generateCode() {

    }
};

}


#endif //PROJECT_GENERATOR_H

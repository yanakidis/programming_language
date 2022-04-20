#pragma once
#include "Execution.hpp"

class Interpretator {
    Parser pars;
    Executer E;
public:
    Interpretator (const char* program ): pars (program) {}
    void interpretation ();
};

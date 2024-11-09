/*
 * Phosphor main.cpp
 *
 * Testing main entry point
 */

#include <stdio.h>

#include "phosphor/phosphor.h"
#include "phosphor/core.h"

int main()
{
    Phosphor* engine = new Phosphor(800, 600);

    engine->init();
    engine->run();
    engine->shutdown();

    delete engine;
}


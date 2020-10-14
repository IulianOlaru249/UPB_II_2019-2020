#ifndef __XBOARD__H__
#define __XBOARD__H__

#define STR_BUFF 256

#include <stdio.h>
#include <iostream>

/* The engine can play as black, white, or neither(when in analysys mode) */
enum sides{Black, White, Neither};

extern int engine_side;

#define ENGINE_NAME "MadHorse"

using namespace std;

#endif

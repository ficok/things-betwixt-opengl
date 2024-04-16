#ifndef THINGSBETWIXT_ERRORS_H
#define THINGSBETWIXT_ERRORS_H

#include <iostream>
#include <glad/glad.h>

#define BREAK_IF_FALSE(x) if (!x) __builtin_trap()
#define assert(x, msg) do {if (!x) {std::cerr << msg << "\n"; BREAK_IF_FALSE(false); }} while (0)

#endif //THINGSBETWIXT_ERRORS_H

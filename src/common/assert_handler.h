#ifndef ASSERT_HANDLER_H

#include <stdint.h>

#define ASSERT(expression)                                                                         \
    do {                                                                                           \
        if (!(expression)) {                                                                       \
            assert_handler();                                                                      \
        }                                                                                          \
    } while (0)

void assert_handler();
#endif // ASSERT_HANDLER_H

#ifndef QRE1113_H
#define QRE1113_H
#include <stdint.h>

// Drivers for retrieving voltage output from the line sensors QRE1113

struct qre1113_voltages
{
    uint16_t front_left;
    uint16_t front_right;
    uint16_t back_left;
    uint16_t back_right;
};

void qre1113_init(void);
void qre1113_get_voltages(struct qre1113_voltages *voltages);

#endif // QRE1113_H

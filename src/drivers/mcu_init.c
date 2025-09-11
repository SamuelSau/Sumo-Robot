#include "drivers/mcu_init.h"
#include "drivers/io.h"
#include <msp430.h>

/* Watchdog is enabled by default and resets microcontroller repeatedly it not
 * explicitly stopped
 * */

static void watchdog_stop(void) { WDTCTL = WDTPW + WDTHOLD; }

void mcu_init(void) { watchdog_stop(); }

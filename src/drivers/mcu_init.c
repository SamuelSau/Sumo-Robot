#include "drivers/mcu_init.h"
#include "drivers/io.h"
#include "common/assert_handler.h"
#include <msp430.h>

/* Watchdog is enabled by default and resets microcontroller repeatedly it not
 * explicitly stopped
 * */

/* TI calibrates each unit during manufacturing and stores calibration value in memory to
 * achieve similar clock rate between different units. This is a sanity check if calibration
 * data still exists.
 * */

static void init_clocks()
{
    ASSERT(CALBC1_1MHZ != 0xFF && CALBC1_16MHZ != 0xFF);
    /* Configures internal oscillator (main clock) to run 16MHz.
       Clock will be used a reference for DCO.
    */
    BCSCTL1 |= CALBC1_16MHZ;
    // Sets 16MHz for digitally controller oscillator (DCO)
    DCOCTL |= CALDCO_16MHZ;

    /* Set DCO as source for
     * MCLK: Masters clock drives CPU and some peripherals
     * SMCLK: Sub system clock drives some peripherals
     * */
    // BCSCTL2 default
}

static void watchdog_stop(void) { WDTCTL = WDTPW + WDTHOLD; }

void mcu_init(void)
{
    watchdog_stop();
    init_clocks();
    io_init();
    _enable_interrupts(); // enable interrupts globally
}

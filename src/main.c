#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "drivers/led.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include <msp430.h>

/*

static void test_assert(void)
{
        test_setup();
        ASSERT(0);
}
*/

static void test_setup(void) { mcu_init(); }

static void test_blink_led(void)
{
    test_setup();
    led_init();
    led_state_e led_state = LED_STATE_OFF;
    while (1) {
        led_state = (led_state == LED_STATE_OFF) ? LED_STATE_ON : LED_STATE_OFF;
        led_set(LED_TEST, led_state);
        BUSY_WAIT_ms(1000); // 250ms
    }
}

/*
// Configure all pins as output and toggle them with a loop. Verify with logic analyzer.
static void test_launchpad_io_pins_output(void)
{
    test_setup();
    const struct io_config output_config = { .select = IO_SELECT_GPIO,
                                             .resistor = IO_RESISTOR_DISABLED,
                                             .dir = IO_DIR_OUTPUT,
                                             .out = IO_OUT_LOW };

    for (io_generic_e io = IO_10; io <= IO_27; io++) {
        io_configure(io, &output_config);
    }

    while (1) {
        for (io_generic_e io = IO_10; io <= IO_27; io++) {
            io_set_out(io, IO_OUT_HIGH);
            __delay_cycles(10000);
            io_set_out(io, IO_OUT_LOW);
        }
    }
}
*/
int main(void)
{
    test_blink_led();
    // test_assert();
    // test_launchpad_io_pins_output();
    return 0;
}

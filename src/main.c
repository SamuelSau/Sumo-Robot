#include <drivers/io.h>
#include <drivers/mcu_init.h>
#include <msp430.h>

static void test_setup(void) { mcu_init(); }

static void test_blink_led(void)
{
    test_setup();
    const struct io_config led_config = { .dir = IO_DIR_OUTPUT,
                                          .select = IO_SELECT_GPIO,
                                          .resistor = IO_RESISTOR_DISABLED,
                                          .out = IO_OUT_LOW };
    io_configure(IO_TEST_LED, &led_config);
    io_out_e out = IO_OUT_LOW;
    while (1) {
        out = (out == IO_OUT_LOW) ? IO_OUT_HIGH : IO_OUT_LOW;
        io_set_out(IO_TEST_LED, out);
        __delay_cycles(250000); // 250ms
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
    // test_launchpad_io_pins_output();
    return 0;
}

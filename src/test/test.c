#include "drivers/io.h"
#include "drivers/mcu_init.h"
#include "drivers/led.h"
#include "drivers/uart.h"
#include "drivers/ir_remote.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "common/trace.h"
#include <msp430.h>

static void test_assert(void);
static void test_setup(void);

SUPRESS_UNUSED
static void test_assert(void)
{
        test_setup();
        ASSERT(0);
}

SUPRESS_UNUSED
static void test_setup(void) 
{ 
	mcu_init();
}

SUPRESS_UNUSED
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


// Configure all pins as output and toggle them with a loop. Verify with logic analyzer.
SUPRESS_UNUSED
static void test_launchpad_io_pins_output(void)
{
#if defined(LAUNCHPAD)
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
            BUSY_WAIT_ms(10);
            io_set_out(io, IO_OUT_LOW);
        }
    }
#endif
}

SUPRESS_UNUSED
static void test_launchpad_io_pins_input(void)
{
#if defined(LAUNCHPAD)
    test_setup();
    led_init();
    const struct io_config input_config = {
        .select = IO_SELECT_GPIO,
        .resistor = IO_RESISTOR_ENABLED,
        .dir = IO_DIR_INPUT,
        .out = IO_OUT_HIGH // pull-up
    };

    // Configure all pins as input
    for (io_generic_e io = IO_10; io <= IO_27; io++) {
        io_configure(io, &input_config);
    }

    for (io_generic_e io = IO_10; io <= IO_27; io++) {
        if (io == (io_generic_e)IO_TEST_LED) {
            continue;
        }
        led_set(LED_TEST, LED_STATE_ON);
        // Wait for user to pull pin low
        while (io_get_input(io) == IO_IN_HIGH) {
            BUSY_WAIT_ms(100);
        }
        led_set(LED_TEST, LED_STATE_OFF);
        // Wait for user to disconnect
        while (io_get_input(io) == IO_IN_LOW) {
            BUSY_WAIT_ms(100);
        }
    }

    // Blink LED when test is done
    while (1) {
        led_set(LED_TEST, LED_STATE_ON);
        BUSY_WAIT_ms(500);
        led_set(LED_TEST, LED_STATE_OFF);
        BUSY_WAIT_ms(2000);
    }
#endif
}

SUPRESS_UNUSED
static void io_11_isr(void)
{
	led_set(LED_TEST, LED_STATE_ON);
}
static void io_20_isr(void)
{
	led_set(LED_TEST, LED_STATE_OFF);
}
SUPRESS_UNUSED
static void test_io_interrupt(void)
{
	test_setup();
	const struct io_config input_config = {
		.select = IO_SELECT_GPIO,
		.resistor = IO_RESISTOR_ENABLED,
		.dir = IO_DIR_INPUT,
		.out = IO_OUT_HIGH
	};
	io_configure(IO_11, &input_config);
	io_configure(IO_20, &input_config);
	led_init();
	io_configure_interrupt(IO_11, IO_TRIGGER_FALLING, io_11_isr);
	io_configure_interrupt(IO_20, IO_TRIGGER_FALLING, io_20_isr);
	io_enable_interrupt(IO_11);
	io_enable_interrupt(IO_20);
	while(1);	
}

SUPRESS_UNUSED
static void test_uart(void)
{
	test_setup();
	uart_init();
	while (1) {
		_putchar('S');
		_putchar('A');
		_putchar('M');
		_putchar('M');
		_putchar('Y');
		_putchar('S');
		_putchar('A');
		_putchar('U');
		_putchar('\n');
		BUSY_WAIT_ms(100);
	}
}

SUPRESS_UNUSED
static void test_trace(void)
{
	test_setup();
	trace_init();
	while (1) 
	{
		TRACE("SAMMY SAU %d", 2025);
		BUSY_WAIT_ms(100);
	}
}

SUPRESS_UNUSED
static void test_ir_remote(void)
{
	test_setup();
	trace_init();
	ir_remote_init();
	while (1)
	{
		TRACE("Command %s", ir_remote_cmd_to_string(ir_remote_get_cmd()));
		BUSY_WAIT_ms(250);
	}

}

int main()
{	TEST();
	ASSERT(0);			
}

#include "common/assert_handler.h"
#include "common/defines.h"
#include "drivers/uart.h"
#include "external/printf/printf.h"
#include <msp430.h>

#define BREAKPOINT __asm volatile("CLR.B R3");

// Text + Program counter + Null termination
#define ASSERT_STRING_MAX_SIZE (15u + 6u + 1u)

static void assert_trace(uint16_t program_counter)
{
    // TODO: Turn off motors "safe state"

    // UART Tx
    P1SEL |= BIT1;
    P1SEL2 |= BIT1;
    uart_init();
    char assert_string[ASSERT_STRING_MAX_SIZE];
    snprintf(assert_string, sizeof(assert_string), "ASSERT 0x%x\n", program_counter);
    uart_trace_assert(assert_string);
}

static void assert_blink_led(void)
{
    // Configure TEST LED pin on LAUNCHPAD
    P1SEL &= ~(BIT0);
    P1SEL2 &= ~(BIT0);
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);

    P2SEL &= ~(BIT6);
    P2SEL2 &= ~(BIT6);
    P2DIR |= BIT6;
    P2OUT &= ~(BIT6);

    while (1) {
        // Blink LED on both target in case wrong target was flashed
        P1OUT ^= BIT0;
        P2OUT ^= BIT6;
        BUSY_WAIT_ms(250);
    }
}

void assert_handler(uint16_t program_counter)
{
    BREAKPOINT
    assert_trace(program_counter);
    assert_blink_led();
}

#include "drivers/pwm.h"
#include "drivers/io.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include <stdbool.h>
#include <assert.h>
#include <msp430.h>

/* MSP430G2553 has no dedicated PWM, so use timer A0
 * to enumate hardware PWM. Each timer has 3 capture/compare
 * channels (CC) and first channel msut be used to set base period
 * (TA0CCR). The 2 other channels are used to output each an PWM and
 * duty cycle set for timer value (TAOCCR1 and TAOCCR2). The CC outputs
 * are muxed to corresponding IO pins (io.h)
 *
 * Set base frequency to 20000 Hz b.c with SMCLK of 16 MHz it
 * gives base period of 100 ticks, meaning duty cycle percent
 * is equivalent to TA0CCRx. 20 kHz for stable motor behavior.
 * */

#define PWM_TIMER_FREQ_HZ (SMCLK / TIMER_DIVIDER_ID3)
#define PWM_PERIOD_FREQ_HZ (20000)
#define PWM_PERIOD_TICKS (PWM_TIMER_FREQ_HZ / PWM_PERIOD_FREQ_HZ)
static_assert(PWM_PERIOD_TICKS == 100, "Expect 100 ticks per period");

#define PWM_TA0CCR0 (PWM_PERIOD_TICKS - 1)

struct pwm_channel_cfg
{
    bool enabled;
    volatile unsigned int *const cct1; // counting mode
    volatile unsigned int *const ccr; // set value for capture & compare register (duty cycle)
};

static struct pwm_channel_cfg pwm_cfgs[] = {

    [PWM_TB6612FNG_LEFT] = { .enabled = false, .cct1 = &TA0CCTL1, .ccr = &TA0CCR1 },
    [PWM_TB6612FNG_RIGHT] = { .enabled = false, .cct1 = &TA0CCTL2, .ccr = &TA0CCR2 },
};

static bool pwm_all_channels_disabled(void)
{
    for (uint8_t ch = 0; ch < ARRAY_SIZE(pwm_cfgs); ch++) {
        if (pwm_cfgs[ch].enabled) {
            return false;
        }
    }
    return true;
}

static bool pwm_enabled = false;
static void pwm_enable(bool enable)
{
    /* MC_0: Stop
     * MC_1 : Count to TACCR0
     */
    if (pwm_enabled != enable) {
        TA0CTL = (TA0CTL & ~TIMER_MC_MASK) + TACLR + (enable ? MC_1 : MC_0);
    }
}

static inline uint8_t pwm_scale_duty_cycle(uint8_t duty_cycle_percent)
{
    return duty_cycle_percent == 1 ? duty_cycle_percent : duty_cycle_percent * 3 / 4;
}

static void pwm_channel_enable(pwm_e pwm, bool enable)
{
    if (pwm_cfgs[pwm].enabled != enable) {
        *pwm_cfgs[pwm].cct1 = enable ? OUTMOD_7 : OUTMOD_0;
        pwm_cfgs[pwm].enabled = enable;
    }
    if (enable) {
        pwm_enable(true);
    } else if (pwm_all_channels_disabled()) {
        pwm_enable(false);
    }
}

void pwm_set_duty_cycle(pwm_e pwm, uint8_t duty_cycle_percent)
{
    ASSERT(duty_cycle_percent <= 100);
#if defined(LAUNCHPAD)

    if (pwm == PWM_TB6612FNG_RIGHT) {
        return;
    }
#endif
    const bool enable = duty_cycle_percent > 0;
    if (enable) {
        *pwm_cfgs[pwm].ccr = pwm_scale_duty_cycle(duty_cycle_percent);
    }
    pwm_channel_enable(pwm, enable);
}

static const struct io_config pwm_io_config = {
    .select = IO_SELECT_ALT1,
    .resistor = IO_RESISTOR_DISABLED,
    .dir = IO_DIR_OUTPUT,
    .out = IO_OUT_LOW,
};

static bool initialized = false;
void pwm_init(void)
{
    ASSERT(!initialized);
    struct io_config current_config;
    io_get_current_config(IO_PWM_MOTORS_LEFT, &current_config);
    ASSERT(io_config_compare(&current_config, &pwm_io_config));
#if defined(NSUMO)
    io_get_current_config(IO_PWM_MOTORS_RIGHT, &current_config);
    ASSERT(io_config_compare(&current_config, &pwm_io_config));
#endif
    /* TASSEL_2: Clock source SMCLK
     * ID_3: Input divider /8
     * MC_0: Stopped
     *
     * */
    TA0CTL = TASSEL_2 + ID_3 + MC_0;

    // Set period

    TA0CCR0 = PWM_TA0CCR0;

    initialized = true;
}

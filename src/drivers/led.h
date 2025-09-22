#ifndef LED_H

// Simple driver to test for LED

typedef enum {
    LED_TEST,
} led_e;

typedef enum {
    LED_STATE_OFF,
    LED_STATE_ON
} led_state_e;

void led_init(void);
void led_set(led_e led, led_state_e state);

#endif // LED_H

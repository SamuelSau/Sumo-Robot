#ifndef UART_H
#define UART_H

void uart_init(void);
void _putchar(char c);

void uart_init_assert(void);
void uart_trace_assert(const char *string);

#endif // UART_H

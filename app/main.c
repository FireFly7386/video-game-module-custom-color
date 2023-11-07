#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/sync.h>
#include <hardware/vreg.h>
#include <pico/sem.h>
#include "frame.h"

#define LED_PIN 25

const frame_t start = {
    .data =
        {

            0xfe, 0x01, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x55, 0x55, 0xfd, 0x01,
            0xff, 0xfe, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
            0xa8, 0xa8, 0xa8, 0xa8, 0x88, 0xa8, 0x88, 0xa8, 0x88, 0xa8, 0x88, 0xa8, 0x88, 0x28,
            0xa8, 0x28, 0xa8, 0x28, 0xa8, 0x28, 0xa8, 0x28, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8,
            0xa8, 0xa8, 0xa8, 0xac, 0xa6, 0xa3, 0x91, 0x49, 0x25, 0x15, 0x95, 0xc5, 0xd5, 0xd5,
            0xc5, 0xd5, 0xc5, 0xd5, 0xc5, 0xd1, 0xc5, 0xd5, 0xd1, 0xd5, 0xd5, 0xd1, 0xd5, 0xd1,
            0xd5, 0xd1, 0xd5, 0x91, 0x25, 0x49, 0x91, 0xa3, 0x26, 0xac, 0xa8, 0x28, 0xa8, 0xfe,
            0x01, 0xf9, 0x05, 0xf5, 0xf5, 0xf5, 0xf5, 0xf5, 0xf5, 0xf5, 0xf5, 0xf5, 0xf5, 0xf5,
            0xf5, 0xf5, 0xf5, 0xf5, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0xf9, 0x91, 0xf1, 0x01,
            0xff, 0xfe, 0x0f, 0x18, 0x1b, 0x1b, 0x1b, 0x19, 0x19, 0x1b, 0x1b, 0x19, 0x19, 0x19,
            0x19, 0x18, 0x1f, 0x0f, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x03, 0x01, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x0f, 0x18, 0x19, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a,
            0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x19, 0x18,
            0x18, 0x18, 0x1f, 0x0f, 0x00, 0x00, 0x80, 0x86, 0x98, 0xe0, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xe0,
            0x98, 0x86, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xa0, 0xa0, 0xc0,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20,
            0x10, 0x08, 0x08, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04,
            0x04, 0x04, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x04, 0x04, 0xfc,
            0x04, 0x04, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0xfc, 0x40, 0xa0, 0x10, 0x00, 0xe0,
            0x50, 0x50, 0x60, 0x00, 0x00, 0x00, 0x10, 0xfc, 0x10, 0x00, 0xfc, 0x10, 0x10, 0xe0,
            0x00, 0xe0, 0x50, 0x50, 0x60, 0x00, 0x00, 0x00, 0xf0, 0x10, 0x10, 0x00, 0xe0, 0x50,
            0x50, 0x60, 0x00, 0xe0, 0x10, 0x10, 0xfc, 0x00, 0x00, 0x00, 0xf0, 0x10, 0x10, 0xe0,
            0x00, 0xf4, 0x00, 0xfc, 0x00, 0xfc, 0x00, 0x00, 0x00, 0xff, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0xf0, 0x8e, 0x81,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xd0, 0xd0, 0xd0,
            0xd0, 0x50, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0e, 0xf0, 0x80, 0x80,
            0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x21, 0x20, 0x20, 0x20, 0x20, 0xa1, 0xe1, 0xa1, 0xe0, 0xa1, 0x20, 0x20, 0x61,
            0xa0, 0x20, 0x21, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x20, 0x20, 0x21, 0xa0,
            0xa0, 0xa1, 0x20, 0x20, 0x21, 0x21, 0xe0, 0xe0, 0xa0, 0x20, 0x21, 0x20, 0x20, 0x20,
            0x20, 0x21, 0x21, 0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x20, 0x60, 0xa0, 0x27, 0x21,
            0x21, 0x20, 0x20, 0x21, 0x20, 0x21, 0x20, 0x21, 0x20, 0x60, 0x60, 0x63, 0x64, 0x68,
            0x70, 0xa0, 0x80, 0x80, 0x80, 0x80, 0x87, 0x8c, 0x0e, 0x1e, 0x1e, 0xbc, 0xfc, 0x78,
            0x78, 0x30, 0x31, 0x21, 0x22, 0x42, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x01, 0x02, 0x06,
            0x07, 0x07, 0x05, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
            0xff, 0xff, 0xff, 0xff, 0x03, 0x01, 0x01, 0x01, 0x02, 0xfc, 0xbc, 0xbc, 0xbc, 0xbc,
            0xbc, 0xbc, 0xbc, 0xbc, 0xb4, 0xb2, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7, 0xb7,
            0xb7, 0xb2, 0xb4, 0xbd, 0xbd, 0xbd, 0xbe, 0xbe, 0xbe, 0xbe, 0xbe, 0xbe, 0xbd, 0xbd,
            0xbd, 0xbc, 0xbc, 0xbd, 0xbe, 0xbc, 0xbc, 0xbc, 0xbc, 0xbf, 0x3f, 0x3c, 0xdc, 0xdc,
            0xec, 0xec, 0xfc, 0xf4, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x01, 0x02, 0x04, 0x04, 0x08, 0x88, 0x88, 0x90, 0x50, 0x50, 0x50, 0x20, 0x20, 0x20,
            0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x07, 0x09, 0x09, 0x11, 0x31,
            0x31, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x70, 0x30, 0x30, 0x10, 0x10, 0x01,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x10, 0x08, 0x04, 0x04,
            0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x06, 0x0e, 0x1e, 0xfc, 0xf8, 0xff, 0xdf, 0xdf,
            0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf,
            0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf,
            0xdf, 0xdf, 0xdf, 0x1f, 0xef, 0xef, 0xf7, 0xfb, 0xfb, 0xfd, 0xfe, 0xfe, 0xff, 0x00,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x02, 0x32, 0x5a, 0x9a,
            0x1a, 0x1a, 0x15, 0x15, 0x19, 0x15, 0x1d, 0x12, 0x12, 0x1c, 0x14, 0x16, 0x1d, 0x13,
            0x12, 0x1c, 0x16, 0x16, 0x1e, 0x14, 0x14, 0x18, 0x18, 0x90, 0xd0, 0xf0, 0xf0, 0xf0,
            0xf0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x83, 0x00, 0x00, 0x00,
            0x00, 0x00, 0xc0, 0xf0, 0x38, 0x1c, 0x0e, 0x06, 0x07, 0x03, 0x03, 0x03, 0x03, 0xff,
            0xff, 0xff, 0xf3, 0x61, 0x61, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xeb,
            0xe3, 0xeb, 0xe3, 0xeb, 0xe3, 0xe3, 0xeb, 0xeb, 0xe3, 0xe3, 0xeb, 0xeb, 0xe3, 0xff,
            0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
            0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0x08, 0x08,
            0x08, 0x08, 0x09, 0x0a, 0x0a, 0x0c, 0x0c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
            0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x0c, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f,
            0x0f, 0x0f, 0x0f, 0x0e, 0x0c, 0x0c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
            0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x0c, 0x0e, 0x1e, 0x1f, 0xff, 0x01, 0x02,
            0x04, 0x04, 0x04, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0xff,

        },
};

frame_t noise = {0};

int main() {
    vreg_set_voltage(frame_get_voltage());
    sleep_ms(10);
    set_sys_clock_khz(frame_get_clock(), true);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    frame_init();
    frame_parse_data(&start);

    // Initialise UART 0
    uart_init(uart0, 230400);

    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    uart_puts(uart0, "Hello world!");

    while(1) {
        // fill frame with noise
        {
            uint8_t* data = (uint8_t*)noise.data;
            for(size_t i = 0; i < 1024; i++) {
                data[i] = rand() % 256;
            }
        }

        frame_parse_data(&start);
        // frame_parse_data(&noise);
    }

    // while(1) {
    //     uint8_t c = uart_getc(uart0);
    //     if(c == 1) {
    //         c = uart_getc(uart0);
    //         if(c == 2) {
    //             c = uart_getc(uart0);
    //             if(c == 3) {
    //                 c = uart_getc(uart0);
    //                 if(c == 4) {
    //                     uart_read_blocking(uart0, data, 128 * 64 / 8);
    //                     parse_data();
    //                 } else {
    //                     continue;
    //                 }
    //             } else {
    //                 continue;
    //             }
    //         } else {
    //             continue;
    //         }
    //     } else {
    //         continue;
    //     }
    // }

    while(1) __wfe();
    __builtin_unreachable();
}
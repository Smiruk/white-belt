#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "driver/uart.h"

const bool HIGH = 1;
const bool LOW = 0;
bool print_status = HIGH;

const uint8_t button_pin = GPIO_NUM_25;

const uint8_t red_led_pin = GPIO_NUM_27;
const uint8_t blue_led_pin = GPIO_NUM_26;
const uint8_t green_led_pin = GPIO_NUM_32;

static volatile uint8_t led_state = 0;
static volatile uint8_t counter_started = false;

const double BUTTON_ISR_TIMEOUT_MS = 200;
const double BUTTON_ISR_TIMEOUT_S = BUTTON_ISR_TIMEOUT_MS / 1000;

const int TIMER_DIVIDER = 16;  //  Hardware timer clock divider
const int TIMER_SCALE = TIMER_BASE_CLK / TIMER_DIVIDER;  // convert counter value to seconds
const double TIMER_INTERVAL0_SEC = 0.1; // sample test interval for the first timer
const bool TEST_WITH_RELOAD = 1; // testing will be done with auto reload

// Чтение терминала
const int BUF_SIZE = 1024;
const uint8_t uart_port_num = 1;

const uint8_t TXD0 = GPIO_NUM_35;
const uint8_t RXD0 = GPIO_NUM_34;
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

static void echo_task(void *arg)
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(uart_port_num, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(uart_port_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_port_num, TXD0, RXD0, ECHO_TEST_RTS, ECHO_TEST_CTS));
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(uart_port_num, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        // Write data back to the UART
        uart_write_bytes(uart_port_num, (const char *) data, len);
    }
}

// конец чтения терминала

typedef struct {
    int type;  // the type of timer's event
    int timer_group;
    int timer_idx;
    uint64_t timer_counter_value;
} timer_event_t;

bool scan_button (int button) {
    return gpio_get_level(button);
}

bool get_nth_bit(uint8_t value, uint8_t pin) {
    return (bool)(value & (1 << pin));
}


int set_led(int led_pin, bool color_led_state) {
    gpio_set_level(led_pin, color_led_state);
    return color_led_state;
}

void delay_ms(int ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void setup_led(int led_pin) {
    gpio_pad_select_gpio(led_pin);
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(led_pin, LOW);
}

void IRAM_ATTR timer_group0_isr(void *para)
{
    timer_spinlock_take(TIMER_GROUP_0);
    int timer_idx = (int) para;

    uint32_t timer_intr = timer_group_get_intr_status_in_isr(TIMER_GROUP_0);
    uint64_t timer_counter_value = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, timer_idx);

    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);

    led_state += 1;

    counter_started = false;
    print_status = HIGH;

    timer_spinlock_give(TIMER_GROUP_0);
}

static void example_tg0_timer_init(int timer_idx, double timer_interval_sec)
{
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = false,
    };
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr,
                       (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
}

static void IRAM_ATTR increment_counter(void* arg) {
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);

    if (!counter_started) {
        example_tg0_timer_init(TIMER_0, BUTTON_ISR_TIMEOUT_S);
        timer_start(TIMER_GROUP_0, TIMER_0);

        counter_started = true;
    }
}

void setup_button(int button_pin) {
    gpio_pad_select_gpio(button_pin);
    gpio_set_direction(button_pin, GPIO_PULLDOWN_ONLY);

    gpio_set_intr_type(button_pin, GPIO_INTR_POSEDGE);
    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(button_pin, increment_counter, (void*) button_pin);
}

void setup() {
    setup_button(button_pin);

    setup_led(red_led_pin);
    setup_led(blue_led_pin);
    setup_led(green_led_pin);

    example_tg0_timer_init(TIMER_0, BUTTON_ISR_TIMEOUT_S);
}

void app_main()
{
    setup();

    // TODO: print only if led_counter has changed
    while (1) {
        if (print_status) {
            printf("leds state = (%d, %d, %d)\r\n", 
            set_led(red_led_pin, get_nth_bit(led_state, 2)),
            set_led(blue_led_pin, get_nth_bit(led_state, 1)),
            set_led(green_led_pin, get_nth_bit(led_state, 0)));
            print_status = LOW;
        }
        else {
            // xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);
            delay_ms(10);
        }
    }
}

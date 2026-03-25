#include "ln882h_pwm_hal.h"
#include "hal/hal_adv_timer.h"
#include "hal/hal_gpio.h"
#include "hal/hal_clock.h"
#include "hal/hal_common.h"
#include <string.h>
#include <stdio.h>

static uint32_t get_timer_base(uint8_t ch) {
    switch (ch) {
        case 0: return ADV_TIMER_0_BASE;
        case 1: return ADV_TIMER_1_BASE;
        case 2: return ADV_TIMER_2_BASE;
        case 3: return ADV_TIMER_3_BASE;
        case 4: return ADV_TIMER_4_BASE;
        case 5: return ADV_TIMER_5_BASE;
        default: return ADV_TIMER_0_BASE;
    }
}

void ln882h_pwm_init(uint8_t port, uint8_t pin_num, uint8_t channel, uint32_t frequency) {
    uint32_t gpio_base = (port == 0) ? GPIOA_BASE : GPIOB_BASE;
    gpio_pin_t gpio_pin = (gpio_pin_t)(1 << pin_num);

    uint32_t reg_base = get_timer_base(channel);

    adv_tim_init_t_def adv_tim_init;
    memset(&adv_tim_init, 0, sizeof(adv_tim_init));

    if (frequency >= 10000) {
        adv_tim_init.adv_tim_clk_div = 0;
        adv_tim_init.adv_tim_load_value =
            (uint32_t)(hal_clock_get_apb0_clk() * 1.0 / frequency - 2);
    } else {
        adv_tim_init.adv_tim_clk_div =
            (uint32_t)(hal_clock_get_apb0_clk() / 1000000) - 1;
        adv_tim_init.adv_tim_load_value =
            1000000 / frequency - 2;
    }

    adv_tim_init.adv_tim_cmp_a_value = 0;
    adv_tim_init.adv_tim_dead_gap_value = 0;
    adv_tim_init.adv_tim_dead_en = ADV_TIMER_DEAD_DIS;
    adv_tim_init.adv_tim_cnt_mode = ADV_TIMER_CNT_MODE_INC;
    adv_tim_init.adv_tim_cha_inv_en = ADV_TIMER_CHA_INV_EN;

    hal_adv_tim_init(reg_base, &adv_tim_init);

    hal_gpio_pin_afio_select(gpio_base, gpio_pin,
        (afio_function_t)(ADV_TIMER_PWM0 + channel * 2));
    hal_gpio_pin_afio_en(gpio_base, gpio_pin, HAL_ENABLE);

}

void ln882h_pwm_set_duty(uint8_t channel, float duty) {
    uint32_t reg_base = get_timer_base(channel);
    uint32_t load = hal_adv_tim_get_load_value(reg_base) + 2;
    uint32_t compare = (uint32_t)(load * duty);
    hal_adv_tim_set_comp_a(reg_base, compare);
}

void ln882h_pwm_enable(uint8_t channel) {
    uint32_t reg_base = get_timer_base(channel);
    hal_adv_tim_a_en(reg_base, HAL_ENABLE);
}

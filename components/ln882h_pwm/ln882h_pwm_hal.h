#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void ln882h_pwm_init(uint8_t port, uint8_t pin_num, uint8_t channel, uint32_t frequency);
void ln882h_pwm_set_duty(uint8_t channel, float duty);
void ln882h_pwm_enable(uint8_t channel);

#ifdef __cplusplus
}
#endif

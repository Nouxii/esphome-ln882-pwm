#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "ln882h_pwm_hal.h"

namespace esphome {
namespace ln882h_pwm {

class LN882HPWM : public output::FloatOutput, public Component {
 public:
  void set_port(uint8_t port) { this->port_ = port; }
  void set_pin_num(uint8_t pin) { this->pin_num_ = pin; }
  void set_timer_channel(uint8_t ch) { this->channel_ = ch; }
  void set_frequency(uint32_t freq) { this->frequency_ = freq; }

void setup() override {
    ln882h_pwm_init(this->port_, this->pin_num_, this->channel_, this->frequency_);
    ln882h_pwm_enable(this->channel_);
  }

  void write_state(float state) override {
    ESP_LOGD("ln882h_pwm", "write_state: %.2f", state);
    ln882h_pwm_set_duty(this->channel_, state);
  }

  float get_setup_priority() const override { return setup_priority::HARDWARE; }

 protected:
  uint8_t port_{0};
  uint8_t pin_num_{0};
  uint8_t channel_{0};
  uint32_t frequency_{1000};
};

}  // namespace ln882h_pwm
}  // namespace esphome

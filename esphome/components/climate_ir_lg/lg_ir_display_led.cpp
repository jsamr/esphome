#include "lg_ir_display_led.h"
#include "climate_ir_lg.h"
#include "esphome/components/light/light_state.h"

namespace esphome {
namespace climate_ir_lg {

static const uint32_t MIN_RECEIVE_DURATION_MS = 250;

light::LightTraits LgIrDisplayLed::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::ON_OFF});
  return traits;
}

void LgIrDisplayLed::setup_state(light::LightState *state) { this->state_ = state; }

void LgIrDisplayLed::write_state(light::LightState *state) {
  bool new_state;
  state->current_values_as_binary(&new_state);

  if (this->receiving_) {
    // State update triggered by on_receive_display_led_toggle; don't re-transmit.
    return;
  }

  if (new_state != this->assumed_state_) {
    this->assumed_state_ = new_state;
    this->parent_->send_display_led_toggle();
  }
}

void LgIrDisplayLed::on_receive_display_led_toggle() {
  if (this->state_ == nullptr)
    return;
  // Debounce in RX+TX setups
  if (millis() - this->last_received_ < MIN_RECEIVE_DURATION_MS)
    return;
  this->last_received_ = millis();
  this->set_assumed_state(!this->assumed_state_);
}

void LgIrDisplayLed::set_assumed_state(bool state) {
  this->receiving_ = true;
  if (state) {
    this->state_->turn_on().perform();
  } else {
    this->state_->turn_off().perform();
  }
  this->assumed_state_ = state;
  this->receiving_ = false;
}

}  // namespace climate_ir_lg
}  // namespace esphome

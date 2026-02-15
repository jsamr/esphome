#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace climate_ir_lg {

class LgIrClimate;

class LgIrDisplayLed : public light::LightOutput, public Parented<LgIrClimate> {
 public:
  light::LightTraits get_traits() override;
  void setup_state(light::LightState *state) override;
  void write_state(light::LightState *state) override;

  /// Called by the parent climate when a LIGHT_ON_OFF IR code is received.
  void on_receive_display_led_toggle();
  void set_assumed_state(bool state);

 protected:
  light::LightState *state_{nullptr};
  bool assumed_state_{false};
  bool receiving_{false};
};

}  // namespace climate_ir_lg
}  // namespace esphome

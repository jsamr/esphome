#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

#include <cinttypes>

#include "esphome/components/select/select.h"

namespace esphome {
namespace climate_ir_lg {

class LgIrDisplayLed;

// Temperature
const uint8_t TEMP_MIN = 18;  // Celsius
const uint8_t TEMP_MAX = 30;  // Celsius

class LgIrClimate : public climate_ir::ClimateIR {
 public:
  LgIrClimate()
      : climate_ir::ClimateIR(TEMP_MIN, TEMP_MAX, 1.0f, true, true,
                              {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM,
                               climate::CLIMATE_FAN_HIGH},
                              {climate::CLIMATE_SWING_OFF, climate::CLIMATE_SWING_VERTICAL}) {}

  void setup() override;

  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override {
    this->send_swing_cmd_ = call.get_swing_mode().has_value();
    // swing resets after unit powered off, except when alternative_mode_ is set
    if (call.get_mode().has_value() && *call.get_mode() == climate::CLIMATE_MODE_OFF && !(this->alternative_mode_))
      this->swing_mode = climate::CLIMATE_SWING_OFF;
    climate_ir::ClimateIR::control(call);
  }
  void set_alternative_mode(bool value) { this->alternative_mode_ = value; }
  void set_header_high(uint32_t header_high) { this->header_high_ = header_high; }
  void set_header_low(uint32_t header_low) { this->header_low_ = header_low; }
  void set_bit_high(uint32_t bit_high) { this->bit_high_ = bit_high; }
  void set_bit_one_low(uint32_t bit_one_low) { this->bit_one_low_ = bit_one_low; }
  void set_bit_zero_low(uint32_t bit_zero_low) { this->bit_zero_low_ = bit_zero_low; }

  /// Send a fixed vertical louver position (index 0-5, mapping to VERT_FIX_1 through VERT_FIX_6).
  void send_louver_fixed_vertical_position(size_t index);

  /// Transmit the LIGHT_ON_OFF IR code to toggle the display LED.
  void send_display_led_toggle();

  void set_display_led(LgIrDisplayLed *display_led) { this->display_led_ = display_led; }

  SUB_SELECT(louver_fixed_vertical_position)

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
  /// Handle received IR Buffer
  bool on_receive(remote_base::RemoteReceiveData data) override;

  bool send_swing_cmd_{false};

  void calc_checksum_(uint32_t &value);
  void transmit_(uint32_t value);
  void sync_display_led_();
  /// Sync the display LED state and publish the climate state.
  void publish_state_();

  bool alternative_mode_{false};
  uint32_t header_high_;
  uint32_t header_low_;
  uint32_t bit_high_;
  uint32_t bit_one_low_;
  uint32_t bit_zero_low_;

  climate::ClimateMode mode_before_{climate::CLIMATE_MODE_OFF};
  LgIrDisplayLed *display_led_{nullptr};
};

}  // namespace climate_ir_lg
}  // namespace esphome

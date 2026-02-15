#include "lg_ir_louver_vertical_select.h"
#include "climate_ir_lg.h"

namespace esphome {
namespace climate_ir_lg {

void LgIrLouverVerticalSelect::control(size_t index) {
  this->publish_state(index);
  this->parent_->send_louver_fixed_vertical_position(index);
}

}  // namespace climate_ir_lg
}  // namespace esphome

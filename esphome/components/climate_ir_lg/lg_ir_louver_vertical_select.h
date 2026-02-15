#pragma once

#include "esphome/components/select/select.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace climate_ir_lg {

class LgIrClimate;

class LgIrLouverVerticalSelect : public select::Select, public Parented<LgIrClimate> {
 protected:
  void control(size_t index) override;
};

}  // namespace climate_ir_lg
}  // namespace esphome

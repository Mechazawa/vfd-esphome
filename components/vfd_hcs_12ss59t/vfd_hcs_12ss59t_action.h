
#pragma once

#include "esphome/components/vfd_hcs_12ss59t/vfd_hcs_12ss59t.h"
#include "esphome/core/automation.h"
#include <string>

namespace esphome {
namespace vfd_hcs_12ss59t {

class VFDWriteAction : public esphome::Action<void> {
 public:
  VFDWriteAction(VFDHCS12SS59T *parent, std::string text) : parent_(parent), text_(std::move(text)) {}
  void play(esphome::ActionInfo &info) override { parent_->write(text_.c_str()); }

 protected:
  VFDHCS12SS59T *parent_;
  std::string text_;
};

class VFDClearAction : public esphome::Action<void> {
 public:
  explicit VFDClearAction(VFDHCS12SS59T *parent) : parent_(parent) {}
  void play(esphome::ActionInfo &info) override { parent_->clear(); }

 protected:
  VFDHCS12SS59T *parent_;
};

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome

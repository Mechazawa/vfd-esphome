#pragma once

#include "vfd_hcs_12ss59t.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace vfd_hcs_12ss59t {

class VFDWriteAction : public Action {
 public:
  VFDWriteAction(VFDHCS12SS59T *parent, std::string text) : parent_(parent), text_(std::move(text)) {}
  void play(ActionInfo &info) override { parent_->write(text_.c_str()); }

 protected:
  VFDHCS12SS59T *parent_;
  std::string text_;
};

class VFDClearAction : public Action {
 public:
  explicit VFDClearAction(VFDHCS12SS59T *parent) : parent_(parent) {}
  void play(ActionInfo &info) override { parent_->clear(); }

 protected:
  VFDHCS12SS59T *parent_;
};

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome

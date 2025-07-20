
#pragma once


#include "esphome/core/automation.h"
#include <string>

namespace esphome {
namespace vfd_hcs_12ss59t {

class VFDHCS12SS59T;  // Forward declaration

class VFDWriteAction : public esphome::Action<> {
 public:
  VFDWriteAction(VFDHCS12SS59T *parent, std::string text) : parent_(parent), text_(std::move(text)) {}
  void play() override { parent_->write(text_.c_str()); }

 protected:
  VFDHCS12SS59T *parent_;
  std::string text_;
};

class VFDClearAction : public esphome::Action<> {
 public:
  explicit VFDClearAction(VFDHCS12SS59T *parent) : parent_(parent) {}
  void play() override { parent_->clear(); }

 protected:
  VFDHCS12SS59T *parent_;
};

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome

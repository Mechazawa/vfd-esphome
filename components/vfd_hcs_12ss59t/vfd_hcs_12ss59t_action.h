
#pragma once


#include "esphome/core/automation.h"
#include <string>

namespace esphome {
namespace vfd_hcs_12ss59t {

class VFDHCS12SS59T;  // Forward declaration

class VFDWriteAction : public esphome::Action<> {
 public:
  VFDWriteAction(VFDHCS12SS59T *parent, std::string text);
  void play() override;

 protected:
  VFDHCS12SS59T *parent_;
  std::string text_;
};

class VFDClearAction : public esphome::Action<> {
 public:
  explicit VFDClearAction(VFDHCS12SS59T *parent);
  void play() override;

 protected:
  VFDHCS12SS59T *parent_;
};

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome

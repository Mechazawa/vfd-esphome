#include "vfd_hcs_12ss59t_action.h"
#include "vfd_hcs_12ss59t.h"

namespace esphome {
namespace vfd_hcs_12ss59t {

VFDWriteAction::VFDWriteAction(VFDHCS12SS59T *parent, std::string text)
    : parent_(parent), text_(std::move(text)) {}

void VFDWriteAction::play() {
  parent_->write(text_.c_str());
}

VFDClearAction::VFDClearAction(VFDHCS12SS59T *parent)
    : parent_(parent) {}

void VFDClearAction::play() {
  parent_->clear();
}

}  // namespace vfd_hcs_12ss59t
}  // namespace esphome

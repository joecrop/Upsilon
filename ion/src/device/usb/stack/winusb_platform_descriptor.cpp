#include "winusb_platform_descriptor.h"

namespace Ion {
namespace USB {
namespace Device {

constexpr uint8_t WinUSBPlatformDescriptor::k_winUSBUUID[];

void WinUSBPlatformDescriptor::push(Channel * c) const {
  PlatformDeviceCapabilityDescriptor::push(c);
  c->push(m_dwWindowsVersion);
  c->push(m_wMSOSDescriptorSetTotalLength);
  c->push(m_bMS_VendorCode);
  c->push(m_bAltEnumCode);
}

uint8_t WinUSBPlatformDescriptor::bLength() const {
  return PlatformDeviceCapabilityDescriptor::bLength() + sizeof(uint32_t) + sizeof(uint16_t) + 2*sizeof(uint8_t);
}

}
}
}

#ifndef SN_NET_TYPES_H
#define SN_NET_TYPES_H

#include "core/common/types.h"

// An IPv4 mapped to IPv6. See: RFC 4038 Section 4.2
struct IPv4Mapped {
  u16 mZeros[5];
  u16 mFFFF;
  u32 mIPv4;
};

/// Defining a address structure that works with both ipv4 and ipv6 in
/// a network that only supports either ivp4 or IPv6
/// Check out RFC 4038 For more detail
struct IPAddr {

  /// @brief Set current address to 0
  void SetZero();

  /// @brief Set IPv6
  void SetIPv6(const u8 *ipv6, u16 port);

  /// @brief Set the host byte order (little or big indian) ipv4 address
  /// as mapped IPv4
  void SetIPv4(u32 ipv4, u16 port);

  /// @brief Return true if current address is mapped IPv4
  b8 IsIPv4();

  /// @brief Return true if current address is localhost
  b8 IsLocalHost();

  u16 mPort;
  union {
    u8 mIPv6[16];
    IPv4Mapped mIPv4;
  };
};

#endif // !SN_NET_TYPES_H

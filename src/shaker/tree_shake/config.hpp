#pragma once

namespace shaker {

/// Tree-shake aggressiveness (set from --shake-strength before running):
///   0 = remove unreachable free functions only
///   1 = + unreachable global variables
///   2 = + unreachable classes / structs / unions  (default)
///   3 = + unreachable typedefs / enums / using-declarations
inline unsigned ShakeStrength = 2;

} // namespace shaker

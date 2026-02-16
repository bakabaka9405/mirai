#pragma once

namespace shaker {

/// @brief Tree-shake 裁剪强度。
/// @details 在运行前通过 `--shake-strength` 设置：
/// - `0`：仅移除不可达的自由函数。
/// - `1`：在 `0` 基础上，额外移除不可达全局变量。
/// - `2`：在 `1` 基础上，额外移除不可达类成员/成员函数（默认）。
/// - `3`：在 `2` 基础上，额外移除不可达类型别名/枚举/using 声明。
inline unsigned ShakeStrength = 2;

} // namespace shaker

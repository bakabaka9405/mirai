#pragma once
#ifdef MR_DEBUG
#define mr_noexcept
#else
#define mr_noexcept noexcept
#endif
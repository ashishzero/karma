#pragma once

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
void do_pre_build_steps(const char* dir, const char* out);
#else
#define do_pre_build_steps(...)
#endif

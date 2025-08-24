// File implements system monitor that scan and save system data during benchmark execution
// For example core temperature and CPU busyness

// TODO Create separated process which will record machine data during benchmark
// For example we need use signals to start and stop recordering
// Maybe there is shared memory between processes that contain system data.
// Dumper may use this information to dump monitoring result.
// BUG There may be problem with synchronizing machine clock and benchmark events

// To implement CPU busyness read /proc/stat
// To implement CPU temperature scanning read /sys/class/thermal/thermal_zone*/temp

// Alternative use lm-sensors (as a C++ library)
#pragma once


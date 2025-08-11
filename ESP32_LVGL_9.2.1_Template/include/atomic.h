
// atomic.h - dummy atomic support for PlatformIO Arduino ESP32

#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdint.h>

#define ATOMIC_SET(p, v) (*(p) = (v))
#define ATOMIC_GET(p) (*(p))
#define ATOMIC_INC(p) ((*(p))++)
#define ATOMIC_DEC(p) ((*(p))--)

#endif  // ATOMIC_H

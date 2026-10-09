#include "types.h"
#include "rnc/ui/vendor/vendor_capture.h"

struct CaptureBoundsAdjustment capture_bounds_adjustments[6] __attribute__((section(".data"))) = {{0.01f, 0.01f, 0.009f, 0.01f}, {0.01f, 0.03f, 0.03f, 0.03f}, {0.03f, 0.0f, 0.0f, 0.0f}, {0.02f, 0.06f, 0.04f, 0.035f}, {0.07f, 0.05f, 0.03f, 0.03f}, {0.03f, 0.06f, 0.02f, 0.03f}};

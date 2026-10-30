/* Moby class 0xA7: one of the two flames of pack item 3 (the thruster pack), lit while the hero jumps, long-jumps or lands, and the function that spawns it. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/rendering/packets/transform_scaled_vertex_batch.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/globals.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

#define BEAM_QUAD_YZ                                                                       \
    {                                                                                      \
        {0.0f, -2.0f, 2.0f, 1.0f}, {0.0f, -2.0f, -2.0f, 1.0f}, {0.0f, 2.0f, 2.0f, 1.0f}, { \
            0.0f, 2.0f, -2.0f, 1.0f                                                        \
        }                                                                                  \
    }

#define BEAM_QUAD_UVS                               \
    {                                               \
        {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, { \
            1.0f, 1.0f                              \
        }                                           \
    }

#define BEAM_QUAD_XZ                                                                       \
    {                                                                                      \
        {-2.0f, 0.0f, 2.0f, 1.0f}, {-2.0f, 0.0f, -2.0f, 1.0f}, {2.0f, 0.0f, 2.0f, 1.0f}, { \
            2.0f, 0.0f, -2.0f, 1.0f                                                        \
        }                                                                                  \
    }

#define RIBBON_UVS                                                                                 \
    {                                                                                              \
        {0.625f, 0.027055f}, {0.654509f, 0.027055f}, {0.5f, 0.0f}, {0.625f, 0.027055f},            \
            {0.5f, 0.0f}, {0.547746f, 0.027055f}, {0.5f, 0.0f}, {0.452254f, 0.027055f},            \
            {0.5f, 0.0f}, {0.375f, 0.027055f}, {0.5f, 0.0f}, {0.345491f, 0.027055f}, {0.5f, 0.0f}, \
            {0.375f, 0.027055f}, {0.5f, 0.0f}, {0.452254f, 0.027055f}, {0.5f, 0.0f},               \
            {0.547746f, 0.027055f}, {0.625f, 0.027055f}, {0.547746f, 0.027055f},                   \
            {0.737764f, 0.105573f}, {0.547746f, 0.027055f}, {0.590818f, 0.105573f},                \
            {0.452254f, 0.027055f}, {0.409182f, 0.105573f}, {0.375f, 0.027055f},                   \
            {0.262236f, 0.105573f}, {0.345491f, 0.027055f}, {0.206107f, 0.105573f},                \
            {0.375f, 0.027055f}, {0.262236f, 0.105573f}, {0.452254f, 0.027055f},                   \
            {0.409182f, 0.105573f}, {0.547746f, 0.027055f}, {0.590818f, 0.105573f},                \
            {0.625f, 0.027055f}, {0.737764f, 0.105573f}, {0.654509f, 0.027055f},                   \
            {0.793893f, 0.105573f}, {0.625f, 0.027055f}, {0.793893f, 0.105573f},                   \
            {0.737764f, 0.105573f}, {0.904509f, 0.227867f}, {0.737764f, 0.105573f},                \
            {0.827254f, 0.227867f}, {0.590818f, 0.105573f}, {0.625f, 0.227867f},                   \
            {0.409182f, 0.105573f}, {0.375f, 0.227867f}, {0.262236f, 0.105573f},                   \
            {0.172746f, 0.227867f}, {0.206107f, 0.105573f}, {0.095491f, 0.227867f},                \
            {0.262236f, 0.105573f}, {0.172746f, 0.227867f}, {0.409182f, 0.105573f},                \
            {0.375f, 0.227867f}, {0.590818f, 0.105573f}, {0.625f, 0.227867f},                      \
            {0.737764f, 0.105573f}, {0.827254f, 0.227867f}, {0.793893f, 0.105573f},                \
            {0.827254f, 0.227867f}, {0.904509f, 0.227867f}, {0.88471f, 0.381966f},                 \
            {0.904509f, 0.227867f}, {0.975528f, 0.381966f}, {0.827254f, 0.227867f},                \
            {0.88471f, 0.381966f}, {0.625f, 0.227867f}, {0.646946f, 0.381966f},                    \
            {0.375f, 0.227867f}, {0.353054f, 0.381966f}, {0.172746f, 0.227867f},                   \
            {0.11529f, 0.381966f}, {0.095491f, 0.227867f}, {0.024472f, 0.381966f},                 \
            {0.172746f, 0.227867f}, {0.11529f, 0.381966f}, {0.375f, 0.227867f},                    \
            {0.353054f, 0.381966f}, {0.625f, 0.227867f}, {0.646946f, 0.381966f},                   \
            {0.827254f, 0.227867f}, {0.646946f, 0.381966f}, {0.88471f, 0.381966f},                 \
            {0.654509f, 0.552786f}, {0.88471f, 0.381966f}, {0.904509f, 0.552786f},                 \
            {0.975528f, 0.381966f}, {1.0f, 0.552786f}, {0.88471f, 0.381966f},                      \
            {0.904508f, 0.552786f}, {0.646946f, 0.381966f}, {0.654508f, 0.552786f},                \
            {0.353054f, 0.381966f}, {0.345491f, 0.552786f}, {0.11529f, 0.381966f},                 \
            {0.095491f, 0.552786f}, {0.024472f, 0.381966f}, {0.0f, 0.552786f},                     \
            {0.11529f, 0.381966f}, {0.095491f, 0.552786f}, {0.353054f, 0.381966f},                 \
            {0.345491f, 0.552786f}, {0.646946f, 0.381966f}, {0.345491f, 0.552786f},                \
            {0.654509f, 0.552786f}, {0.353054f, 0.723607f}, {0.654509f, 0.552786f},                \
            {0.646946f, 0.723607f}, {0.904509f, 0.552786f}, {0.88471f, 0.723607f},                 \
            {1.0f, 0.552786f}, {0.975528f, 0.723607f}, {0.904508f, 0.552786f},                     \
            {0.88471f, 0.723607f}, {0.654508f, 0.552786f}, {0.646946f, 0.723607f},                 \
            {0.345491f, 0.552786f}, {0.353054f, 0.723607f}, {0.095491f, 0.552786f},                \
            {0.11529f, 0.723607f}, {0.0f, 0.552786f}, {0.024472f, 0.723607f},                      \
            {0.095491f, 0.552786f}, {0.11529f, 0.723607f}, {0.345491f, 0.552786f},                 \
            {0.11529f, 0.723607f}, {0.353054f, 0.723607f}, {0.172746f, 0.877706f},                 \
            {0.375f, 0.877706f}, {0.262236f, 1.0f}, {0.375f, 0.877706f}, {0.409182f, 1.0f},        \
            {0.375f, 0.877706f}, {0.590818f, 1.0f}, {0.375f, 0.877706f}, {0.625f, 0.877706f},      \
            {0.353054f, 0.723607f}, {0.625f, 0.877706f}, {0.646946f, 0.723607f},                   \
            {0.625f, 0.877706f}, {0.88471f, 0.723607f}, {0.625f, 0.877706f},                       \
            {0.827254f, 0.877706f}, {0.590818f, 1.0f}, {0.827254f, 0.877706f}, {0.737764f, 1.0f},  \
            {0.827254f, 0.877706f}, {0.793893f, 1.0f}, {0.827254f, 0.877706f},                     \
            {0.904509f, 0.877706f}, {0.88471f, 0.723607f}, {0.904509f, 0.877706f},                 \
            {0.975528f, 0.723607f}, {0.904509f, 0.877706f}, {0.88471f, 0.723607f},                 \
            {0.904509f, 0.877706f}, {0.827254f, 0.877706f}, {0.793893f, 1.0f},                     \
            {0.827254f, 0.877706f}, {0.737764f, 1.0f}, {0.827254f, 0.877706f}, {0.590818f, 1.0f},  \
            {0.827254f, 0.877706f}, {0.625f, 0.877706f}, {0.88471f, 0.723607f},                    \
            {0.625f, 0.877706f}, {0.646946f, 0.723607f}, {0.625f, 0.877706f},                      \
            {0.353054f, 0.723607f}, {0.625f, 0.877706f}, {0.375f, 0.877706f}, {0.590818f, 1.0f},   \
            {0.375f, 0.877706f}, {0.409182f, 1.0f}, {0.375f, 0.877706f}, {0.262236f, 1.0f},        \
            {0.375f, 0.877706f}, {0.172746f, 0.877706f}, {0.353054f, 0.723607f},                   \
            {0.172746f, 0.877706f}, {0.11529f, 0.723607f}, {0.095491f, 0.877706f},                 \
            {0.024472f, 0.723607f}, {0.095491f, 0.877706f}, {0.11529f, 0.723607f},                 \
            {0.095491f, 0.877706f}, {0.172746f, 0.877706f}, {0.095491f, 0.877706f},                \
            {0.262236f, 1.0f}, {0.095491f, 0.877706f}, {0.206107f, 1.0f}, {0.172746f, 0.877706f},  \
        {                                                                                          \
            0.262236f, 1.0f                                                                        \
        }                                                                                          \
    }

#define RIBBON_POINTS                                                                              \
    {                                                                                              \
        {-1.389244f, -1.912131f, -0.901867f, 1.0f}, {0.0f, -2.363524f, -0.901867f, 1.0f},          \
            {0.0f, 0.0f, 0.0f, 1.0f}, {1.389244f, -1.912131f, -0.901867f, 1.0f},                   \
            {0.0f, 0.0f, 0.0f, 1.0f}, {2.247845f, -0.730369f, -0.901867f, 1.0f},                   \
            {0.0f, 0.0f, 0.0f, 1.0f}, {2.247845f, 0.730369f, -0.901867f, 1.0f},                    \
            {0.0f, 0.0f, 0.0f, 1.0f}, {1.389244f, 1.912131f, -0.901867f, 1.0f},                    \
            {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 2.363524f, -0.901867f, 1.0f},                         \
            {0.0f, 0.0f, 0.0f, 1.0f}, {-1.389244f, 1.912131f, -0.901867f, 1.0f},                   \
            {0.0f, 0.0f, 0.0f, 1.0f}, {-2.247844f, 0.730369f, -0.901867f, 1.0f},                   \
            {0.0f, 0.0f, 0.0f, 1.0f}, {-2.247845f, -0.730369f, -0.901867f, 1.0f},                  \
            {-1.389244f, -1.912131f, -0.901867f, 1.0f},                                            \
            {-2.247845f, -0.730369f, -0.901867f, 1.0f}, {-2.6425f, -3.637089f, -3.519189f, 1.0f},  \
            {-2.247845f, -0.730369f, -0.901867f, 1.0f},                                            \
            {-4.275654f, -1.389244f, -3.519189f, 1.0f}, {-2.247844f, 0.730369f, -0.901867f, 1.0f}, \
            {-4.275654f, 1.389245f, -3.519189f, 1.0f}, {-1.389244f, 1.912131f, -0.901867f, 1.0f},  \
            {-2.642499f, 3.637089f, -3.519189f, 1.0f}, {0.0f, 2.363524f, -0.901867f, 1.0f},        \
            {0.0f, 4.495689f, -3.519189f, 1.0f}, {1.389244f, 1.912131f, -0.901867f, 1.0f},         \
            {2.6425f, 3.637089f, -3.519189f, 1.0f}, {2.247845f, 0.730369f, -0.901867f, 1.0f},      \
            {4.275654f, 1.389244f, -3.519189f, 1.0f}, {2.247845f, -0.730369f, -0.901867f, 1.0f},   \
            {4.275654f, -1.389244f, -3.519189f, 1.0f}, {1.389244f, -1.912131f, -0.901867f, 1.0f},  \
            {2.6425f, -3.637089f, -3.519189f, 1.0f}, {0.0f, -2.363524f, -0.901867f, 1.0f},         \
            {0.0f, -4.495689f, -3.519189f, 1.0f}, {-1.389244f, -1.912131f, -0.901867f, 1.0f},      \
            {0.0f, -4.495689f, -3.519189f, 1.0f}, {-2.6425f, -3.637089f, -3.519189f, 1.0f},        \
            {0.0f, -6.187785f, -7.595764f, 1.0f}, {-2.6425f, -3.637089f, -3.519189f, 1.0f},        \
            {-3.637089f, -5.006023f, -7.595764f, 1.0f},                                            \
            {-4.275654f, -1.389244f, -3.519189f, 1.0f}, {-5.884933f, -1.91213f, -7.595764f, 1.0f}, \
            {-4.275654f, 1.389245f, -3.519189f, 1.0f}, {-5.884933f, 1.912131f, -7.595764f, 1.0f},  \
            {-2.642499f, 3.637089f, -3.519189f, 1.0f}, {-3.637089f, 5.006023f, -7.595764f, 1.0f},  \
            {0.0f, 4.495689f, -3.519189f, 1.0f}, {0.0f, 6.187785f, -7.595764f, 1.0f},              \
            {2.6425f, 3.637089f, -3.519189f, 1.0f}, {3.637089f, 5.006023f, -7.595764f, 1.0f},      \
            {4.275654f, 1.389244f, -3.519189f, 1.0f}, {5.884933f, 1.912131f, -7.595764f, 1.0f},    \
            {4.275654f, -1.389244f, -3.519189f, 1.0f}, {5.884933f, -1.912131f, -7.595764f, 1.0f},  \
            {2.6425f, -3.637089f, -3.519189f, 1.0f}, {3.637089f, -5.006023f, -7.595764f, 1.0f},    \
            {0.0f, -4.495689f, -3.519189f, 1.0f}, {3.637089f, -5.006023f, -7.595764f, 1.0f},       \
            {0.0f, -6.187785f, -7.595764f, 1.0f}, {4.275654f, -5.884933f, -12.732548f, 1.0f},      \
            {0.0f, -6.187785f, -7.595764f, 1.0f}, {0.0f, -7.274178f, -12.732548f, 1.0f},           \
            {-3.637089f, -5.006023f, -7.595764f, 1.0f},                                            \
            {-4.275655f, -5.884933f, -12.732548f, 1.0f},                                           \
            {-5.884933f, -1.91213f, -7.595764f, 1.0f},                                             \
            {-6.918154f, -2.247844f, -12.732548f, 1.0f},                                           \
            {-5.884933f, 1.912131f, -7.595764f, 1.0f}, {-6.918154f, 2.247845f, -12.732548f, 1.0f}, \
            {-3.637089f, 5.006023f, -7.595764f, 1.0f}, {-4.275654f, 5.884933f, -12.732548f, 1.0f}, \
            {0.0f, 6.187785f, -7.595764f, 1.0f}, {0.0f, 7.274178f, -12.732548f, 1.0f},             \
            {3.637089f, 5.006023f, -7.595764f, 1.0f}, {4.275655f, 5.884933f, -12.732548f, 1.0f},   \
            {5.884933f, 1.912131f, -7.595764f, 1.0f}, {6.918154f, 2.247845f, -12.732548f, 1.0f},   \
            {5.884933f, -1.912131f, -7.595764f, 1.0f}, {6.918154f, -2.247845f, -12.732548f, 1.0f}, \
            {3.637089f, -5.006023f, -7.595764f, 1.0f}, {6.918154f, -2.247845f, -12.732548f, 1.0f}, \
            {4.275654f, -5.884933f, -12.732548f, 1.0f},                                            \
            {7.274178f, -2.363524f, -18.426716f, 1.0f},                                            \
            {4.275654f, -5.884933f, -12.732548f, 1.0f},                                            \
            {4.495689f, -6.187785f, -18.426716f, 1.0f}, {0.0f, -7.274178f, -12.732548f, 1.0f},     \
            {0.0f, -7.648523f, -18.426716f, 1.0f}, {-4.275655f, -5.884933f, -12.732548f, 1.0f},    \
            {-4.495689f, -6.187785f, -18.426716f, 1.0f},                                           \
            {-6.918154f, -2.247844f, -12.732548f, 1.0f},                                           \
            {-7.274178f, -2.363523f, -18.426716f, 1.0f},                                           \
            {-6.918154f, 2.247845f, -12.732548f, 1.0f},                                            \
            {-7.274178f, 2.363524f, -18.426716f, 1.0f},                                            \
            {-4.275654f, 5.884933f, -12.732548f, 1.0f},                                            \
            {-4.495688f, 6.187785f, -18.426716f, 1.0f}, {0.0f, 7.274178f, -12.732548f, 1.0f},      \
            {0.0f, 7.648523f, -18.426716f, 1.0f}, {4.275655f, 5.884933f, -12.732548f, 1.0f},       \
            {4.495689f, 6.187785f, -18.426716f, 1.0f}, {6.918154f, 2.247845f, -12.732548f, 1.0f},  \
            {7.274178f, 2.363524f, -18.426716f, 1.0f}, {6.918154f, -2.247845f, -12.732548f, 1.0f}, \
            {7.274178f, 2.363524f, -18.426716f, 1.0f}, {7.274178f, -2.363524f, -18.426716f, 1.0f}, \
            {6.918154f, 2.247845f, -24.120884f, 1.0f}, {7.274178f, -2.363524f, -18.426716f, 1.0f}, \
            {6.918154f, -2.247845f, -24.120884f, 1.0f},                                            \
            {4.495689f, -6.187785f, -18.426716f, 1.0f},                                            \
            {4.275654f, -5.884933f, -24.120884f, 1.0f}, {0.0f, -7.648523f, -18.426716f, 1.0f},     \
            {0.0f, -7.274178f, -24.120884f, 1.0f}, {-4.495689f, -6.187785f, -18.426716f, 1.0f},    \
            {-4.275655f, -5.884933f, -24.120884f, 1.0f},                                           \
            {-7.274178f, -2.363523f, -18.426716f, 1.0f},                                           \
            {-6.918154f, -2.247844f, -24.120884f, 1.0f},                                           \
            {-7.274178f, 2.363524f, -18.426716f, 1.0f},                                            \
            {-6.918154f, 2.247845f, -24.120884f, 1.0f},                                            \
            {-4.495688f, 6.187785f, -18.426716f, 1.0f},                                            \
            {-4.275654f, 5.884933f, -24.120884f, 1.0f}, {0.0f, 7.648523f, -18.426716f, 1.0f},      \
            {0.0f, 7.274178f, -24.120884f, 1.0f}, {4.495689f, 6.187785f, -18.426716f, 1.0f},       \
            {4.275655f, 5.884933f, -24.120884f, 1.0f}, {7.274178f, 2.363524f, -18.426716f, 1.0f},  \
            {4.275655f, 5.884933f, -24.120884f, 1.0f}, {6.918154f, 2.247845f, -24.120884f, 1.0f},  \
            {3.637089f, 5.006023f, -29.257668f, 1.0f}, {5.884933f, 1.912131f, -29.257668f, 1.0f},  \
            {2.6425f, 3.637089f, -33.334244f, 1.0f}, {5.884933f, 1.912131f, -29.257668f, 1.0f},    \
            {4.275654f, 1.389244f, -33.334244f, 1.0f}, {5.884933f, 1.912131f, -29.257668f, 1.0f},  \
            {4.275654f, -1.389244f, -33.334244f, 1.0f}, {5.884933f, 1.912131f, -29.257668f, 1.0f}, \
            {5.884933f, -1.912131f, -29.257668f, 1.0f}, {6.918154f, 2.247845f, -24.120884f, 1.0f}, \
            {5.884933f, -1.912131f, -29.257668f, 1.0f},                                            \
            {6.918154f, -2.247845f, -24.120884f, 1.0f},                                            \
            {5.884933f, -1.912131f, -29.257668f, 1.0f},                                            \
            {4.275654f, -5.884933f, -24.120884f, 1.0f},                                            \
            {5.884933f, -1.912131f, -29.257668f, 1.0f},                                            \
            {3.637089f, -5.006023f, -29.257668f, 1.0f},                                            \
            {4.275654f, -1.389244f, -33.334244f, 1.0f},                                            \
            {3.637089f, -5.006023f, -29.257668f, 1.0f}, {2.6425f, -3.637089f, -33.334244f, 1.0f},  \
            {3.637089f, -5.006023f, -29.257668f, 1.0f}, {0.0f, -4.495689f, -33.334244f, 1.0f},     \
            {3.637089f, -5.006023f, -29.257668f, 1.0f}, {0.0f, -6.187785f, -29.257668f, 1.0f},     \
            {4.275654f, -5.884933f, -24.120884f, 1.0f}, {0.0f, -6.187785f, -29.257668f, 1.0f},     \
            {0.0f, -7.274178f, -24.120884f, 1.0f}, {0.0f, -6.187785f, -29.257668f, 1.0f},          \
            {-4.275655f, -5.884933f, -24.120884f, 1.0f}, {0.0f, -6.187785f, -29.257668f, 1.0f},    \
            {-3.637089f, -5.006023f, -29.257668f, 1.0f}, {0.0f, -4.495689f, -33.334244f, 1.0f},    \
            {-3.637089f, -5.006023f, -29.257668f, 1.0f},                                           \
            {-2.6425f, -3.637089f, -33.334244f, 1.0f},                                             \
            {-3.637089f, -5.006023f, -29.257668f, 1.0f},                                           \
            {-4.275654f, -1.389244f, -33.334244f, 1.0f},                                           \
            {-3.637089f, -5.006023f, -29.257668f, 1.0f},                                           \
            {-5.884933f, -1.91213f, -29.257668f, 1.0f},                                            \
            {-4.275655f, -5.884933f, -24.120884f, 1.0f},                                           \
            {-5.884933f, -1.91213f, -29.257668f, 1.0f},                                            \
            {-6.918154f, -2.247844f, -24.120884f, 1.0f},                                           \
            {-5.884933f, -1.91213f, -29.257668f, 1.0f},                                            \
            {-6.918154f, 2.247845f, -24.120884f, 1.0f},                                            \
            {-5.884933f, -1.91213f, -29.257668f, 1.0f},                                            \
            {-5.884933f, 1.912131f, -29.257668f, 1.0f},                                            \
            {-4.275654f, -1.389244f, -33.334244f, 1.0f},                                           \
            {-5.884933f, 1.912131f, -29.257668f, 1.0f},                                            \
            {-4.275654f, 1.389245f, -33.334244f, 1.0f},                                            \
            {-5.884933f, 1.912131f, -29.257668f, 1.0f},                                            \
            {-2.642499f, 3.637089f, -33.334244f, 1.0f},                                            \
            {-5.884933f, 1.912131f, -29.257668f, 1.0f},                                            \
            {-3.637089f, 5.006023f, -29.257668f, 1.0f},                                            \
            {-6.918154f, 2.247845f, -24.120884f, 1.0f},                                            \
            {-3.637089f, 5.006023f, -29.257668f, 1.0f},                                            \
            {-4.275654f, 5.884933f, -24.120884f, 1.0f}, {0.0f, 6.187785f, -29.257668f, 1.0f},      \
            {0.0f, 7.274178f, -24.120884f, 1.0f}, {0.0f, 6.187785f, -29.257668f, 1.0f},            \
            {4.275655f, 5.884933f, -24.120884f, 1.0f}, {0.0f, 6.187785f, -29.257668f, 1.0f},       \
            {3.637089f, 5.006023f, -29.257668f, 1.0f}, {0.0f, 6.187785f, -29.257668f, 1.0f},       \
            {2.6425f, 3.637089f, -33.334244f, 1.0f}, {0.0f, 6.187785f, -29.257668f, 1.0f},         \
            {0.0f, 4.495689f, -33.334244f, 1.0f}, {-3.637089f, 5.006023f, -29.257668f, 1.0f}, {    \
            -2.642499f, 3.637089f, -33.334244f, 1.0f                                               \
        }                                                                                          \
    }

#define RIBBON_COLORS                                                                              \
    {                                                                                              \
        0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,        \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x80808080, 0x80808080, 0x20808080, 0x80808080, 0x80808080,    \
            0x80808080, 0x20808080, 0x80808080, 0x80808080, 0x80808080, 0x20808080, 0x80808080,    \
            0x80808080, 0x80808080, 0x20808080, 0x80808080, 0x80808080, 0x80808080, 0x20808080,    \
            0x80808080, 0x20808080, 0x80808080, 0x8808080, 0x80808080, 0x20808080, 0x20808080,     \
            0x8808080, 0x80808080, 0x20808080, 0x20808080, 0x8808080, 0x80808080, 0x20808080,      \
            0x20808080, 0x8808080, 0x80808080, 0x20808080, 0x20808080, 0x8808080, 0x80808080,      \
            0x20808080, 0x20808080, 0x20808080, 0x8808080, 0x808080, 0x808080, 0x808080, 0x808080, \
            0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x8808080, 0x808080, 0x20808080,     \
            0x808080, 0x8808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080,       \
            0x808080, 0x808080, 0x808080, 0x8808080, 0x808080, 0x20808080, 0x808080, 0x8808080,    \
            0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080,        \
            0x808080, 0x8808080, 0x808080, 0x20808080, 0x808080, 0x8808080, 0x808080, 0x808080,    \
            0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x8808080,       \
            0x808080, 0x20808080, 0x808080, 0x8808080, 0x808080, 0x20808080, 0x808080, 0x808080,   \
            0x808080, 0x808080, 0x808080, 0x808080, 0x808080, 0x808080                             \
    }

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A_2b3628;

typedef int Q_2b3628 __attribute__((mode(TI)));

typedef struct {
    float         pos[4][4];
    unsigned int  col[4];
    float         uv[4][2];
    unsigned long tag[4];
} Quad;

typedef struct {
    int a, b;
} Ids;

typedef struct {
    float x, y, z;
} P3;

typedef struct {
    float u, v;
} UV;

typedef struct Child_2b3628 {
    char  pad0[0xA6];
    short cls;
} Child_2b3628;

typedef struct Data_2b3628 {
    int           f0;
    int           f4;
    Child_2b3628 *child;
    char          padC[0x14];
    float         f20;
    char          pad24[8];
    float         f2C;
} Data_2b3628;

typedef struct {
    f32 f[4];
} __attribute__((aligned(16))) V4;

typedef struct N {
    s32 f00;
    u8  p04[8];
    s32 f0C;
    V4  v10;
} N;

typedef struct X {
    u8 p00[0x10];
    V4 v10;
    u8 p20[0x58];
    N *f78;
    u8 p7C[0x44];
    V4 vC0;
} X;

/* D_0013F350, the shared Level-00 state block; only the offsets the listing
   names are declared. */
/* the per-object block reached through o+0x78: 0x20..0x2C are s32 counters and
   0x30..0x3C are f32 rates (the case-1/3 bodies divide five constants by
   scale_game_frames(2) / scale_game_frames(8) and store the results there). */
typedef struct {
    s32  i0;
    char pad4[4];
    s32  i8;
    s32  iC;
    char pad10[0x10];
    f32  f20;
    f32  f24;
    f32  f28;
    f32  f2C;
    f32  f30;
    f32  f34;
    f32  f38;
    f32  f3C;
} V_4198;

typedef struct {
    char    pad00[0x18];
    f32     f18;
    char    pad1C[4];
    u8      state;
    char    pad21[0xF];
    u8      u30;
    char    pad31[3];
    u16     u34;
    char    pad36[0x42];
    V_4198 *v;
} O_4198;

extern long  get_effect_texture(int) __asm__("FUN_001f44b8");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void  draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern void  font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void  func_L00_001FDE48_2b3628(int, void *, void *, void *, int) __asm__("FUN_L00_001fde98");
extern f32   random_angle_radians(void) __asm__("FUN_00213308");
extern u8   *spawn_burst(V4 *, V4 *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");
extern s32   random_integer_below(s32) __asm__("FUN_00213260");
extern u8   *spawn_puff(V4 *, f32, f32, f32, f32, s32, V4 *, u32) __asm__("FUN_L00_0026d000");
extern char *create_moby(int);
extern void  FUN_L00_002b3628_4198(void) __asm__("FUN_L00_002b3628");
extern s32   FUN_L00_0020d498_4198(s32) __asm__("FUN_L00_0020d498");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void  FUN_L00_002b3c50_4198(void *) __asm__("FUN_L00_002b3c50");

extern Ids D_L00_00161488 NOT_SDA;
extern Ids D_L00_00161490 NOT_SDA;
extern s32                D_L00_0015F5CC;

float D_L00_001DA9E0[4][4] = BEAM_QUAD_YZ;
float D_L01_001DAE60[4][4] = BEAM_QUAD_YZ;
float D_L02_001CF460[4][4] = BEAM_QUAD_YZ;
float D_L03_001CEF60[4][4] = BEAM_QUAD_YZ;
float D_L04_001CEFE0[4][4] = BEAM_QUAD_YZ;
float D_L05_001CF300[4][4] = BEAM_QUAD_YZ;
float D_L06_001CF5E0[4][4] = BEAM_QUAD_YZ;
float D_L07_001CEEE0[4][4] = BEAM_QUAD_YZ;
float D_L08_001CF660[4][4] = BEAM_QUAD_YZ;
float D_L09_001CEFE0[4][4] = BEAM_QUAD_YZ;
float D_L10_001CF2E0[4][4] = BEAM_QUAD_YZ;
float D_L11_001CF860[4][4] = BEAM_QUAD_YZ;
float D_L12_001E1F00[4][4] = BEAM_QUAD_YZ;
float D_L13_001CF770[4][4] = BEAM_QUAD_YZ;
float D_L14_001DBE20[4][4] = BEAM_QUAD_YZ;
float D_L15_001CF8E0[4][4] = BEAM_QUAD_YZ;
float D_L16_001CF2E0[4][4] = BEAM_QUAD_YZ;
float D_L17_001CFB60[4][4] = BEAM_QUAD_YZ;
float D_L18_001CF860[4][4] = BEAM_QUAD_YZ;
float D_L00_001DAA60[4][2] = BEAM_QUAD_UVS;
float D_L01_001DAEE0[4][2] = BEAM_QUAD_UVS;
float D_L02_001CF4E0[4][2] = BEAM_QUAD_UVS;
float D_L03_001CEFE0[4][2] = BEAM_QUAD_UVS;
float D_L04_001CF060[4][2] = BEAM_QUAD_UVS;
float D_L05_001CF380[4][2] = BEAM_QUAD_UVS;
float D_L06_001CF660[4][2] = BEAM_QUAD_UVS;
float D_L07_001CEF60[4][2] = BEAM_QUAD_UVS;
float D_L08_001CF6E0[4][2] = BEAM_QUAD_UVS;
float D_L09_001CF060[4][2] = BEAM_QUAD_UVS;
float D_L10_001CF360[4][2] = BEAM_QUAD_UVS;
float D_L11_001CF8E0[4][2] = BEAM_QUAD_UVS;
float D_L12_001E1F80[4][2] = BEAM_QUAD_UVS;
float D_L13_001CF7F0[4][2] = BEAM_QUAD_UVS;
float D_L14_001DBEA0[4][2] = BEAM_QUAD_UVS;
float D_L15_001CF960[4][2] = BEAM_QUAD_UVS;
float D_L16_001CF360[4][2] = BEAM_QUAD_UVS;
float D_L17_001CFBE0[4][2] = BEAM_QUAD_UVS;
float D_L18_001CF8E0[4][2] = BEAM_QUAD_UVS;
float D_L00_001DAA20[4][4] = BEAM_QUAD_XZ;
float D_L01_001DAEA0[4][4] = BEAM_QUAD_XZ;
float D_L02_001CF4A0[4][4] = BEAM_QUAD_XZ;
float D_L03_001CEFA0[4][4] = BEAM_QUAD_XZ;
float D_L04_001CF020[4][4] = BEAM_QUAD_XZ;
float D_L05_001CF340[4][4] = BEAM_QUAD_XZ;
float D_L06_001CF620[4][4] = BEAM_QUAD_XZ;
float D_L07_001CEF20[4][4] = BEAM_QUAD_XZ;
float D_L08_001CF6A0[4][4] = BEAM_QUAD_XZ;
float D_L09_001CF020[4][4] = BEAM_QUAD_XZ;
float D_L10_001CF320[4][4] = BEAM_QUAD_XZ;
float D_L11_001CF8A0[4][4] = BEAM_QUAD_XZ;
float D_L12_001E1F40[4][4] = BEAM_QUAD_XZ;
float D_L13_001CF7B0[4][4] = BEAM_QUAD_XZ;
float D_L14_001DBE60[4][4] = BEAM_QUAD_XZ;
float D_L15_001CF920[4][4] = BEAM_QUAD_XZ;
float D_L16_001CF320[4][4] = BEAM_QUAD_XZ;
float D_L17_001CFBA0[4][4] = BEAM_QUAD_XZ;
float D_L18_001CF8A0[4][4] = BEAM_QUAD_XZ;
UV    D_L00_001DA0B0[196] = RIBBON_UVS;
UV    D_L01_001DA530[196] = RIBBON_UVS;
UV    D_L02_001CEB30[196] = RIBBON_UVS;
UV    D_L03_001CE630[196] = RIBBON_UVS;
UV    D_L04_001CE6B0[196] = RIBBON_UVS;
UV    D_L05_001CE9D0[196] = RIBBON_UVS;
UV    D_L06_001CECB0[196] = RIBBON_UVS;
UV    D_L07_001CE5B0[196] = RIBBON_UVS;
UV    D_L08_001CED30[196] = RIBBON_UVS;
UV    D_L09_001CE6B0[196] = RIBBON_UVS;
UV    D_L10_001CE9B0[196] = RIBBON_UVS;
UV    D_L11_001CEF30[196] = RIBBON_UVS;
UV    D_L12_001E15D0[196] = RIBBON_UVS;
UV    D_L13_001CEE40[196] = RIBBON_UVS;
UV    D_L14_001DB4F0[196] = RIBBON_UVS;
UV    D_L15_001CEFB0[196] = RIBBON_UVS;
UV    D_L16_001CE9B0[196] = RIBBON_UVS;
UV    D_L17_001CF230[196] = RIBBON_UVS;
UV    D_L18_001CEF30[196] = RIBBON_UVS;
float D_L00_001D9470[196][4] = RIBBON_POINTS;
float D_L01_001D98F0[196][4] = RIBBON_POINTS;
float D_L02_001CDEF0[196][4] = RIBBON_POINTS;
float D_L03_001CD9F0[196][4] = RIBBON_POINTS;
float D_L04_001CDA70[196][4] = RIBBON_POINTS;
float D_L05_001CDD90[196][4] = RIBBON_POINTS;
float D_L06_001CE070[196][4] = RIBBON_POINTS;
float D_L07_001CD970[196][4] = RIBBON_POINTS;
float D_L08_001CE0F0[196][4] = RIBBON_POINTS;
float D_L09_001CDA70[196][4] = RIBBON_POINTS;
float D_L10_001CDD70[196][4] = RIBBON_POINTS;
float D_L11_001CE2F0[196][4] = RIBBON_POINTS;
float D_L12_001E0990[196][4] = RIBBON_POINTS;
float D_L13_001CE200[196][4] = RIBBON_POINTS;
float D_L14_001DA8B0[196][4] = RIBBON_POINTS;
float D_L15_001CE370[196][4] = RIBBON_POINTS;
float D_L16_001CDD70[196][4] = RIBBON_POINTS;
float D_L17_001CE5F0[196][4] = RIBBON_POINTS;
float D_L18_001CE2F0[196][4] = RIBBON_POINTS;
P3    D_L00_001DAA80[196] = {0};

P3 D_L01_001DAF00[196] = {0};

P3 D_L02_001CF500[196] = {0};

P3 D_L03_001CF000[196] = {0};

P3 D_L04_001CF080[196] = {0};

P3 D_L05_001CF3A0[196] = {0};

P3 D_L06_001CF680[196] = {0};

P3 D_L07_001CEF80[196] = {0};

P3 D_L08_001CF700[196] = {0};

P3 D_L09_001CF080[196] = {0};

P3 D_L10_001CF380[196] = {0};

P3 D_L11_001CF900[196] = {0};

P3 D_L12_001E1FA0[196] = {0};

P3 D_L13_001CF810[196] = {0};

P3 D_L14_001DBEC0[196] = {0};

P3 D_L15_001CF980[196] = {0};

P3 D_L16_001CF380[196] = {0};

P3 D_L17_001CFC00[196] = {0};

P3 D_L18_001CF900[196] = {0};

int D_L00_001DA6D0[196] = RIBBON_COLORS;
int D_L01_001DAB50[196] = RIBBON_COLORS;
int D_L02_001CF150[196] = RIBBON_COLORS;
int D_L03_001CEC50[196] = RIBBON_COLORS;
int D_L04_001CECD0[196] = RIBBON_COLORS;
int D_L05_001CEFF0[196] = RIBBON_COLORS;
int D_L06_001CF2D0[196] = RIBBON_COLORS;
int D_L07_001CEBD0[196] = RIBBON_COLORS;
int D_L08_001CF350[196] = RIBBON_COLORS;
int D_L09_001CECD0[196] = RIBBON_COLORS;
int D_L10_001CEFD0[196] = RIBBON_COLORS;
int D_L11_001CF550[196] = RIBBON_COLORS;
int D_L12_001E1BF0[196] = RIBBON_COLORS;
int D_L13_001CF460[196] = RIBBON_COLORS;
int D_L14_001DBB10[196] = RIBBON_COLORS;
int D_L15_001CF5D0[196] = RIBBON_COLORS;
int D_L16_001CEFD0[196] = RIBBON_COLORS;
int D_L17_001CF850[196] = RIBBON_COLORS;
int D_L18_001CF550[196] = RIBBON_COLORS;
Ids D_L01_001614B0 = {0, 1};

Ids D_L01_001614B8 = {2, 3};

Ids D_L02_00161458 = {0, 1};

Ids D_L02_00161460 = {2, 3};

Ids D_L03_00161468 = {0, 1};

Ids D_L03_00161470 = {2, 3};

Ids D_L04_00161458 = {0, 1};

Ids D_L04_00161460 = {2, 3};

Ids D_L05_001614A8 = {0, 1};

Ids D_L05_001614B0 = {2, 3};

Ids D_L06_00161480 = {0, 1};

Ids D_L06_00161488 = {2, 3};

Ids D_L07_00161458 = {0, 1};

Ids D_L07_00161460 = {2, 3};

Ids D_L08_001614A8 = {0, 1};

Ids D_L08_001614B0 = {2, 3};

Ids D_L09_00161510 = {0, 1};

Ids D_L09_00161518 = {2, 3};

Ids D_L10_00161468 = {0, 1};

Ids D_L10_00161470 = {2, 3};

Ids D_L11_001614E8 = {0, 1};

Ids D_L11_001614F0 = {2, 3};

Ids D_L12_00161468 = {0, 1};

Ids D_L12_00161470 = {2, 3};

Ids D_L13_00161538 = {0, 1};

Ids D_L13_00161540 = {2, 3};

Ids D_L14_00161670 = {0, 1};

Ids D_L14_00161678 = {2, 3};

Ids D_L15_00161610 = {0, 1};

Ids D_L15_00161618 = {2, 3};

Ids D_L16_00161518 = {0, 1};

Ids D_L16_00161520 = {2, 3};

Ids D_L17_00161568 = {0, 1};

Ids D_L17_00161570 = {2, 3};

Ids D_L18_001614F8 = {0, 1};

Ids D_L18_00161500 = {2, 3};

/* Draws the moby's beam: orients the moby along its child's two joints, draws three quads and a ribbon of 196 points. */
void draw_joint_aligned_beam_effect(struct Moby *beam_moby) __asm__("FUN_L00_002b3628");

void draw_joint_aligned_beam_effect(struct Moby *beam_moby) {
    Quad          pk;
    Vec4A_2b3628  mtx[4];
    Vec4A_2b3628  out[2];
    Ids           ids;
    Vec4A_2b3628  up;
    Vec4A_2b3628  beam_quad_transform[4];
    Vec4A_2b3628  idm[4];
    Vec4A_2b3628  billboard_view_direction;
    Vec4A_2b3628  billboard_side_axis;
    Vec4A_2b3628  billboard_up_axis;
    Vec4A_2b3628  billboard_origin;
    Vec4A_2b3628  ribbon_vertex_position;
    Data_2b3628  *data = (Data_2b3628 *)beam_moby->pvars;
    Child_2b3628 *child = data->child;
    int           i;

    if (child == 0 || child->cls != 0x260) {
        return;
    }
    pk.tag[2] = 0xFF9000000260;
    pk.tag[3] = 0x8000000048;
    pk.tag[0] = 0;
    if (data->f4 != 0) {
        ids = D_L00_00161488;
        transform_scaled_vertex_batch(child, 2, &ids, out);
    } else {
        ids = D_L00_00161490;
        transform_scaled_vertex_batch(child, 2, &ids, out);
    }
    *(Q_2b3628 *)&up = 0;
    up.y = 1.0f;
    fast_vec_sub(&mtx[2], &out[0], &out[1]);
    fast_vec_normalize(&mtx[2], &mtx[2], 1.0f);
    mtx[2].w = 0.0f;
    fast_vec_cross(&mtx[0], &up, &mtx[2]);
    fast_vec_normalize(&mtx[0], &mtx[0], 1.0f);
    mtx[0].w = 0.0f;
    fast_vec_cross(&mtx[1], &mtx[2], &mtx[0]);
    mtx[1].w = 0.0f;
    qcopy(&mtx[3], &out[1]);
    mtx[3].w = 1.0f;
    qcopy(&beam_moby->rot_row0, &mtx[0]);
    qcopy(&beam_moby->rot_row1, &mtx[1]);
    qcopy(&beam_moby->rot_row2, &mtx[2]);
    qcopy(&beam_moby->pos, &mtx[3]);
    set_identity_matrix(idm);
    idm[3].w = 1.0f;
    idm[3].z = -0.075f;
    idm[3].x = 0.0f;
    idm[3].y = 0.0f;
    multiply_matrix(beam_quad_transform, mtx, idm);
    pk.tag[1] = get_effect_texture(0xC);
    for (i = 0; i < 4; i++) {
        pk.col[i] = 0x80808080;
        if (i == 0 || i == 2) {
            fast_vec_scale(pk.pos[i], D_L00_001DA9E0[i], data->f2C);
        } else {
            float *v = pk.pos[i];
            fast_vec_scale(v, D_L00_001DA9E0[i], data->f2C);
            v[2] -= random_float_between(0.0f, 0.1f);
        }
        pk.uv[i][0] = D_L00_001DAA60[i][0];
        pk.uv[i][1] = D_L00_001DAA60[i][1];
    }
    draw_geometry_quad(&pk, beam_quad_transform, 0);
    for (i = 0; i < 4; i++) {
        if (i == 0 || i == 2) {
            fast_vec_scale(pk.pos[i], D_L00_001DAA20[i], data->f2C);
        } else {
            float *v = pk.pos[i];
            fast_vec_scale(v, D_L00_001DAA20[i], data->f2C);
            v[2] -= random_float_between(0.0f, 0.1f);
        }
    }
    draw_geometry_quad(&pk, beam_quad_transform, 0);
    pk.tag[1] = get_effect_texture(0xB);
    fast_vec_scale(&billboard_origin, &mtx[2], -0.13f);
    fast_vec_add(&billboard_origin, &billboard_origin, &mtx[3]);
    fast_vec_sub(&billboard_view_direction, &g_debug_camera, &billboard_origin);
    fast_vec_normalize(&billboard_view_direction, &billboard_view_direction, 1.0f);
    fast_vec_cross(&billboard_side_axis, &billboard_view_direction, &hero.gravity_dir);
    fast_vec_normalize(&billboard_side_axis, &billboard_side_axis, -1.0f);
    fast_vec_cross(&billboard_up_axis, &billboard_side_axis, &billboard_view_direction);
    pk.col[0] = pk.col[1] = pk.col[2] = pk.col[3] = 0x7F2020FF;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(pk.pos[i], D_L00_001DA9E0[i], 0.1f);
        transform_vector(pk.pos[i], pk.pos[i], &billboard_view_direction);
    }
    draw_geometry_quad(&pk, 0, 0);
    vu1_add_g_sregister(6, get_effect_texture(0xA));
    vu1_add_g_sregister(0x42, pk.tag[3]);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260);
    vu1_add_g_sregister(0x4A, 0);
    font_queue_vu_state();
    for (i = 0; i < 0xC4; i++) {
        fast_vec_scale(&ribbon_vertex_position, D_L00_001D9470[i], data->f20);
        transform_vector(&ribbon_vertex_position, &ribbon_vertex_position, mtx);
        D_L00_001DAA80[i].x = ribbon_vertex_position.x;
        D_L00_001DAA80[i].y = ribbon_vertex_position.y;
        D_L00_001DAA80[i].z = ribbon_vertex_position.z;
        D_L00_001DA0B0[i].v -= 0.025f;
        if (D_L00_001DA0B0[i].v < -7.0f) {
            D_L00_001DA0B0[i].v += 7.0f;
        }
    }
    func_L00_001FDE48_2b3628(0x94, D_L00_001DAA80, D_L00_001DA6D0, D_L00_001DA0B0, 1);
    func_L00_001FDE48_2b3628(
        0x32, &D_L00_001DAA80[0x92], &D_L00_001DA6D0[0x92], &D_L00_001DA0B0[0x92], 1);
}

void emit_beam_trail_particles(X *x) __asm__("FUN_L00_002b3c50");

void emit_beam_trail_particles(X *x) {
    V4  effect_input_and_position_offset;
    V4  emission_position;
    V4  previous_position_delta;
    V4  trail_sample_position;
    N  *trail_state;
    f32 trail_sample_spacing;
    f32 effect_random_upper_bound;
    u32 col;
    s32 i;
    s32 trail_sample_limit;
    s32 hero_state;
    u8 *spawned_effect;

    trail_state = x->f78;
    effect_input_and_position_offset.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    effect_input_and_position_offset.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    effect_input_and_position_offset.f[2] = 0.0f;
    effect_input_and_position_offset.f[2] = -random_float_between(0.01f, 0.03f);
    transform_vector_by_basis(
        &effect_input_and_position_offset, &effect_input_and_position_offset, &x->vC0);
    qcopy(&emission_position, &x->v10);
    if (hero.state.current == 0x81) {
        if (D_L00_0015F5CC % 3 == 0) {
            spawn_burst(&emission_position,
                        &effect_input_and_position_offset,
                        0x4F007FFF,
                        0x1FFFFFFF,
                        scale_game_frames(10),
                        1,
                        10000.0f);
        }
    } else {
        spawn_burst(&emission_position,
                    &effect_input_and_position_offset,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    scale_game_frames(10),
                    1,
                    10000.0f);
    }
    if (trail_state->f0C) {
        trail_sample_spacing = fast_vec_dist(&trail_state->v10, &emission_position) * 0.5f;
        fast_vec_sub(&previous_position_delta, &trail_state->v10, &emission_position);
        for (i = 1; i < 2; i++) {
            fast_vec_normalize(
                &trail_sample_position, &previous_position_delta, trail_sample_spacing * (f32)i);
            fast_vec_add(&trail_sample_position, &emission_position, &trail_sample_position);
            spawn_burst(&trail_sample_position,
                        &effect_input_and_position_offset,
                        0x4F007FFF,
                        0x1FFFFFFF,
                        scale_game_frames(10),
                        1,
                        10000.0f);
        }
    }
    trail_state->f00 = random_integer_below(scale_game_frames(10));
    effect_random_upper_bound = 1.075f;
    col = 0x808080;
    effect_input_and_position_offset.f[0] = 0.0f;
    effect_input_and_position_offset.f[1] = 0.0f;
    effect_input_and_position_offset.f[2] = -0.3f;
    effect_input_and_position_offset.f[3] = 1.0f;
    transform_vector_by_basis(
        &effect_input_and_position_offset, &effect_input_and_position_offset, &x->vC0);
    fast_vec_add(&emission_position, &x->v10, &effect_input_and_position_offset);
    effect_input_and_position_offset.f[3] = 1.0f;
    effect_input_and_position_offset.f[2] = -0.075f;
    effect_input_and_position_offset.f[0] = 0.0f;
    effect_input_and_position_offset.f[1] = 0.0f;
    transform_vector_by_basis(
        &effect_input_and_position_offset, &effect_input_and_position_offset, &x->vC0);
    effect_input_and_position_offset.f[2] = 0.025f;
    if (hero.state.current == HERO_STATE_LAND) {
        col = 0xA0A0A0;
        effect_random_upper_bound = 1.0165f;
        effect_input_and_position_offset.f[2] -= frame_time * 5.0f;
    }
    if (hero.state.current == 0x81) {
        col = 0xA0A0A0;
        effect_random_upper_bound = 1.037f;
        effect_input_and_position_offset.f[2] -= frame_time * 3.0f;
    }
    spawned_effect = spawn_puff(&emission_position,
                                0.05f,
                                1.01f,
                                effect_random_upper_bound,
                                30000.0f,
                                6,
                                &effect_input_and_position_offset,
                                col);
    if (spawned_effect)
        spawned_effect[3] = 0x44;
    if (trail_state->f0C) {
        trail_sample_spacing = fast_vec_dist(&trail_state->v10, &emission_position) * 0.16667f;
        fast_vec_sub(&previous_position_delta, &trail_state->v10, &emission_position);
        hero_state = hero.state.current;
        trail_sample_limit = 4;
        if (hero_state == 8)
            trail_sample_limit = 2;
        if (hero_state == 0x81)
            trail_sample_limit = 1;
        for (i = 1; i < trail_sample_limit; i++) {
            fast_vec_normalize(
                &trail_sample_position, &previous_position_delta, trail_sample_spacing * (f32)i);
            fast_vec_add(&trail_sample_position, &emission_position, &trail_sample_position);
            spawned_effect = spawn_puff(&trail_sample_position,
                                        0.05f,
                                        1.01f,
                                        effect_random_upper_bound,
                                        30000.0f,
                                        6,
                                        &effect_input_and_position_offset,
                                        col);
            if (spawned_effect)
                spawned_effect[3] = 0x44;
        }
    }
    qcopy(&trail_state->v10, &emission_position);
    trail_state->f0C = 1;
}

/* Creates a class 0xA7 moby carrying a in its data; sets bytes 0x30/0x31, 0x32 and state 0. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B5428), where it is exact; names translated to the US level program. */
char *spawn_thruster_pack_flame(int instance_index) __asm__("FUN_L00_002b4138");

char *spawn_thruster_pack_flame(int instance_index) {
    char *spawned_moby = create_moby(0xA7);
    if (spawned_moby != 0) {
        int *moby_vars = ((int *)MOBY(spawned_moby)->pvars);
        moby_vars[1] = instance_index;
        moby_vars[0] = 0;
        moby_vars[2] = 0;
        moby_vars[3] = 0;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x7E;
        MOBY(spawned_moby)->visible = 0;
        MOBY(spawned_moby)->state = 0;
    }
    return spawned_moby;
}

void update_thruster_pack_flame(void *moby) __asm__("FUN_L00_002b4198");

void update_thruster_pack_flame(void *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_WAIT_FOR_BEAM = 1,
        STATE_BEAM_GROW = 2,
        STATE_BEAM_ACTIVE = 3,
        STATE_BEAM_SHRINK = 4
    };
    O_4198 *beam_moby = (O_4198 *)moby;
    V_4198 *v = beam_moby->v;
    s32     flag = 0;
    s32     hero_state;
    s32     lim;

    if (hero.items[3].item_id == 3) {
        if (beam_moby->state == 1 || beam_moby->state == 3) {
            hero_state = hero.state.current;
            if ((u32)(hero_state - 0xB) < 4 ||
                (hero_state == 0x1C &&
                 ((u8 *)((void *)hero.moby))[0x52] == ((u8 *)((void *)hero.moby))[0x53] &&
                 hero.anim_frame >= 6.0f && hero.anim_frame <= 16.0f)) {
                if (!hero.past_jump_apex)
                    flag = 1;
            }
            if (hero.state.current == HERO_STATE_PACK_LONG_JUMP) {
                if (hero.state_timer < scale_game_frames(0x2C)) {
                    flag = 1;
                }
            }
            if (hero.state.current == 0x22) {
                if (hero.state_timer < scale_game_frames(0xF) ||
                    (scale_game_frames(0x21) < hero.state_timer && hero.air_frames.s != 0)) {
                    flag = 1;
                }
            }
            if (hero.state.current == HERO_STATE_LAND || hero.state.current == 0x81) {
                flag = 1;
            }
        }
    }
    switch (beam_moby->state) {
    case STATE_INITIALIZE:
        beam_moby->f18 += 0.5f;
        beam_moby->state = 1;
        beam_moby->u30 = 0xFF;
        beam_moby->u34 |= 1;
        break;
    case STATE_WAIT_FOR_BEAM:
        if (FUN_L00_0020d498_4198(3) != 3) {
            mark_moby_for_removal(beam_moby);
        } else if (flag != 0) {
            beam_moby->state = 2;
            v->i0 = scale_game_frames(2);
            v->f30 = 0.0125f / scale_game_frames(2);
            v->f34 = 0.2f / scale_game_frames(2);
            v->f38 = 0.05f / scale_game_frames(2);
            v->f3C = 0.025f / scale_game_frames(2);
            v->f20 = 0.0f;
            v->f24 = 0.0f;
            v->f28 = 0.0f;
            v->f2C = 0.0f;
        }
        break;
    case STATE_BEAM_GROW:
        v->f20 += v->f30;
        v->f24 += v->f34;
        v->f28 += v->f38;
        v->f2C += v->f3C;
        v->i8 = ((s32)hero.items[3].moby);
        add_draw_callback(FUN_L00_002b3628_4198, beam_moby);
        if (fast_dec_timer(v) != 0) {
            beam_moby->state = 3;
            v->f20 = 0.0125f;
            v->f24 = 0.2f;
            v->f28 = 0.05f;
            v->iC = 0;
            v->f2C = 0.025f;
        }
        break;
    case STATE_BEAM_ACTIVE:
        v->i8 = ((s32)hero.items[3].moby);
        add_draw_callback(FUN_L00_002b3628_4198, beam_moby);
        if (flag == 0) {
            beam_moby->state = 4;
            v->i0 = scale_game_frames(8);
            v->f30 = 0.0125f / scale_game_frames(8);
            v->f34 = 0.2f / scale_game_frames(8);
            v->f38 = 0.05f / scale_game_frames(8);
            v->f3C = 0.025f / scale_game_frames(8);
        } else {
            v->f20 = 0.0125f;
            FUN_L00_002b3c50_4198(beam_moby);
        }
        break;
    case STATE_BEAM_SHRINK:
        if (fast_dec_timer(v) != 0) {
            beam_moby->state = 1;
        } else {
            v->f20 -= v->f30;
            v->f24 -= v->f34;
            v->f28 -= v->f38;
            v->f2C -= v->f3C;
            v->i8 = ((s32)hero.items[3].moby);
            add_draw_callback(FUN_L00_002b3628_4198, beam_moby);
        }
        break;
    }
}

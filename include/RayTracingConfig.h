#ifndef RAY_TRACING_CONFIG_H
#define RAY_TRACING_CONFIG_H

//#define RESOLUTION_144P
//#define RESOLUTION_240P
//#define RESOLUTION_360P
//#define RESOLUTION_480P
//#define RESOLUTION_720P

#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720

#if defined RESOLUTION_144P
    #define RESOLUTION_H        256
    #define RESOLUTION_V        144
#elif defined RESOLUTION_240P
    #define RESOLUTION_H        352
    #define RESOLUTION_V        240
#elif defined RESOLUTION_720P
    #define RESOLUTION_H        1280
    #define RESOLUTION_V        720
#elif defined RESOLUTION_360P
    #define RESOLUTION_H        480
    #define RESOLUTION_V        360
#elif defined RESOLUTION_480P
    #define RESOLUTION_H        852
    #define RESOLUTION_V        480
#else
    #define RESOLUTION_H        150
    #define RESOLUTION_V        100
#endif

#define EPSILON             0.0001
#define LINEAR_VELOCITY     1.5
#define ANGULAR_VELOCITY    0.1
#define REFLECTION_DEPTH    3

#endif

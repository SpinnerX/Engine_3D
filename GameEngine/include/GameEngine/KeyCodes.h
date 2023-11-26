#pragma once

// REN stands for RENDERER
// Since we are calling this GameEngine for right now, this is what we're going with

// From glfw3.h
/* The unknown key */
#define RENDER_KEY_UNKNOWN            -1

/* Printable keys */
#define RENDER_KEY_SPACE              32
#define RENDER_KEY_APOSTROPHE         39  /* ' */
#define RENDER_KEY_COMMA              44  /* , */
#define RENDER_KEY_MINUS              45  /* - */
#define RENDER_KEY_PERIOD             46  /* . */
#define RENDER_KEY_SLASH              47  /* / */
#define RENDER_KEY_0                  48
#define RENDER_KEY_1                  49
#define RENDER_KEY_2                  50
#define RENDER_KEY_3                  51
#define RENDER_KEY_4                  52
#define RENDER_KEY_5                  53
#define RENDER_KEY_6                  54
#define RENDER_KEY_7                  55
#define RENDER_KEY_8                  56
#define RENDER_KEY_9                  57
#define RENDER_KEY_SEMICOLON          59  /* ; */
#define RENDER_KEY_EQUAL              61  /* = */
#define RENDER_KEY_A                  65
#define RENDER_KEY_B                  66
#define RENDER_KEY_C                  67
#define RENDER_KEY_D                  68
#define RENDER_KEY_E                  69
#define RENDER_KEY_F                  70
#define RENDER_KEY_G                  71
#define RENDER_KEY_H                  72
#define RENDER_KEY_I                  73
#define RENDER_KEY_J                  74
#define RENDER_KEY_K                  75
#define RENDER_KEY_L                  76
#define RENDER_KEY_M                  77
#define RENDER_KEY_N                  78
#define RENDER_KEY_O                  79
#define RENDER_KEY_P                  80
#define RENDER_KEY_Q                  81
#define RENDER_KEY_R                  82
#define RENDER_KEY_S                  83
#define RENDER_KEY_T                  84
#define RENDER_KEY_U                  85
#define RENDER_KEY_V                  86
#define RENDER_KEY_W                  87
#define RENDER_KEY_X                  88
#define RENDER_KEY_Y                  89
#define RENDER_KEY_Z                  90
#define RENDER_KEY_LEFT_BRACKET       91  /* [ */
#define RENDER_KEY_BACKSLASH          92  /* \ */
#define RENDER_KEY_RIGHT_BRACKET      93  /* ] */
#define RENDER_KEY_GRAVE_ACCENT       96  /* ` */
#define RENDER_KEY_WORLD_1            161 /* non-US #1 */
#define RENDER_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define RENDER_KEY_ESCAPE             256
#define RENDER_KEY_ENTER              257
#define RENDER_KEY_TAB                258
#define RENDER_KEY_BACKSPACE          259
#define RENDER_KEY_INSERT             260
#define RENDER_KEY_DELETE             261
#define RENDER_KEY_RIGHT              262
#define RENDER_KEY_LEFT               263
#define RENDER_KEY_DOWN               264
#define RENDER_KEY_UP                 265
#define RENDER_KEY_PAGE_UP            266
#define RENDER_KEY_PAGE_DOWN          267
#define RENDER_KEY_HOME               268
#define RENDER_KEY_END                269
#define RENDER_KEY_CAPS_LOCK          280
#define RENDER_KEY_SCROLL_LOCK        281
#define RENDER_KEY_NUM_LOCK           282
#define RENDER_KEY_PRINT_SCREEN       283
#define RENDER_KEY_PAUSE              284
#define RENDER_KEY_F1                 290
#define RENDER_KEY_F2                 291
#define RENDER_KEY_F3                 292
#define RENDER_KEY_F4                 293
#define RENDER_KEY_F5                 294
#define RENDER_KEY_F6                 295
#define RENDER_KEY_F7                 296
#define RENDER_KEY_F8                 297
#define RENDER_KEY_F9                 298
#define RENDER_KEY_F10                299
#define RENDER_KEY_F11                300
#define RENDER_KEY_F12                301
#define RENDER_KEY_F13                302
#define RENDER_KEY_F14                303
#define RENDER_KEY_F15                304
#define RENDER_KEY_F16                305
#define RENDER_KEY_F17                306
#define RENDER_KEY_F18                307
#define RENDER_KEY_F19                308
#define RENDER_KEY_F20                309
#define RENDER_KEY_F21                310
#define RENDER_KEY_F22                311
#define RENDER_KEY_F23                312
#define RENDER_KEY_F24                313
#define RENDER_KEY_F25                314
#define RENDER_KEY_KP_0               320
#define RENDER_KEY_KP_1               321
#define RENDER_KEY_KP_2               322
#define RENDER_KEY_KP_3               323
#define RENDER_KEY_KP_4               324
#define RENDER_KEY_KP_5               325
#define RENDER_KEY_KP_6               326
#define RENDER_KEY_KP_7               327
#define RENDER_KEY_KP_8               328
#define RENDER_KEY_KP_9               329
#define RENDER_KEY_KP_DECIMAL         330
#define RENDER_KEY_KP_DIVIDE          331
#define RENDER_KEY_KP_MULTIPLY        332
#define RENDER_KEY_KP_SUBTRACT        333
#define RENDER_KEY_KP_ADD             334
#define RENDER_KEY_KP_ENTER           335
#define RENDER_KEY_KP_EQUAL           336
#define RENDER_KEY_LEFT_SHIFT         340
#define RENDER_KEY_LEFT_CONTROL       341
#define RENDER_KEY_LEFT_ALT           342
#define RENDER_KEY_LEFT_SUPER         343
#define RENDER_KEY_RIGHT_SHIFT        344
#define RENDER_KEY_RIGHT_CONTROL      345
#define RENDER_KEY_RIGHT_ALT          346
#define RENDER_KEY_RIGHT_SUPER        347
#define RENDER_KEY_MENU               348
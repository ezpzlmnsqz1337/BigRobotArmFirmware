#ifndef CONFIG_H
#define CONFIG_H

namespace Config
{
// default motor steps per revolution (without microstepping) NEMA aliexpress china motors
static const long M1_STEPS_PER_REVOLUTION = 200;
static const long M2_STEPS_PER_REVOLUTION = 200;
static const long M3_STEPS_PER_REVOLUTION = 200;
static const long M4_STEPS_PER_REVOLUTION = 200;
static const long M5_STEPS_PER_REVOLUTION = 200;
} // namespace Config

typedef enum Microstepping
{
  HALF_STEPPING = 2,
  MICRO_4 = 4,
  MICRO_8 = 8,
  MICRO_16 = 16
};

typedef enum Motors
{
  M1_STEP = 54,
  M1_DIR = 55,
  M1_ENABLE = 38,
  M2_STEP = 60,
  M2_DIR = 61,
  M2_ENABLE = 56,
  M3_STEP = 46,
  M3_DIR = 48,
  M3_ENABLE = 62,
  M4_STEP = 26,
  M4_DIR = 28,
  M4_ENABLE = 24,
  M5_STEP = 36,
  M5_DIR = 34,
  M5_ENABLE = 30
};

#endif

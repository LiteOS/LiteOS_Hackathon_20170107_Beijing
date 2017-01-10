#ifndef __JSON_PARSER_H
#define __JSON_PARSER_H
#include "stdint.h"
#define AIR_T_LIMITS    0
#define AIR_H_LIMITS    1
#define SOIL_T_LIMITS   2
#define SOIL_H_LIMITS   3
#define AIR_CO2_LIMITS  4
#define AIR_L_LIMITS    5

uint8_t json_parse(void);

#endif

#ifndef __PMAP_H
#define __PMAP_H

#include "stm32f10x.h"

#define PMAP_MAX_WIDTH 5
#define PMAP_MAX_HEIGHT 5

typedef enum 
{
	UPWARD,
	DOWNWARD,
	LEFTWARD,
	RIGHTWARD,
	STOP
} PMap_Dir_Enum_TypeDef;

typedef struct 
{
	PMap_Dir_Enum_TypeDef dir;
	float dist;
} PMap_Cmd_TypeDef;


void              PMap_Init   (void);
PMap_Cmd_TypeDef  getPMapCmd  (uint8_t id);

#endif

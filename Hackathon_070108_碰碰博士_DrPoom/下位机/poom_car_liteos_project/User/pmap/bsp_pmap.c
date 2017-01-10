#include "bsp_pmap.h"

static PMap_Cmd_TypeDef trackCmd[20];

static void PMap_Track_Config (void)
{
	trackCmd[0].dir = UPWARD;
	trackCmd[0].dist = 0.3;
	
	trackCmd[1].dir = UPWARD;
	trackCmd[1].dist = 0.3;

	trackCmd[2].dir = RIGHTWARD;
	trackCmd[2].dist = 0.3;
	
	trackCmd[3].dir = RIGHTWARD;
	trackCmd[3].dist = 0.3;

	trackCmd[4].dir = DOWNWARD;
	trackCmd[4].dist = 0.3;

	trackCmd[5].dir = LEFTWARD;
	trackCmd[5].dist = 0.3;

	trackCmd[6].dir = DOWNWARD;
	trackCmd[6].dist = 0.3;
	
	trackCmd[7].dir = RIGHTWARD;
	trackCmd[7].dist = 0.3;
	
	trackCmd[8].dir = RIGHTWARD;
	trackCmd[8].dist = 0.3;
	
	trackCmd[9].dir = UPWARD;
	trackCmd[9].dist = 0.3;
	
	trackCmd[10].dir = RIGHTWARD;
	trackCmd[10].dist = 0.3;
	
	trackCmd[11].dir = UPWARD;
	trackCmd[11].dist = 0.3;
	
	trackCmd[12].dir = LEFTWARD;
	trackCmd[12].dist = 0.3;

}

void PMap_Init (void)
{
	PMap_Track_Config();
}

PMap_Cmd_TypeDef  getPMapCmd (uint8_t id)
{
	return trackCmd[id];
}

#pragma once

enum HD_STATUS
{
  HD_STATUS_INACTIVE = 0,
  HD_STATUS_ACTIVE
};

void HumanDetectorInit(void);
int HumanDetectGetStatus(void);
#pragma once

// Commands define
#define CMD_REPORT_LIGHT_OUT                                                0x00
#define CMD_REPORT_LIGHT_IN                                                 0x01
#define CMD_REPORT_GESTURE                                                  0x02
#define CMD_REPORT_HD                                                       0x03
#define CMD_REPORT_ALL                                                      0x07
#define CMD_CTRL_LIGHT_OUT                                                  0x10

// Payload parameters
#define CMD_PAYLOAD_LIGHT_OUT_MAX                                            999
#define CMD_PAYLOAD_LIGHT_IN_MAX                                             255
#define CMD_PAYLOAD_HD_INACTIVE                                             0x00
#define CMD_PAYLOAD_HD_ACTIVE                                               0x01
#define CMD_PAYLOAD_GESTURE_UP                                              0x00
#define CMD_PAYLOAD_GESTURE_DOWN                                            0x00
#define CMD_PAYLOAD_GESTURE_LEFT                                            0x00
#define CMD_PAYLOAD_GESTURE_RIGHT                                           0x00
#define CMD_PAYLOAD_REPORT_LEN                                                 4
#define CMD_PAYLOAD_REPORT_INDEX_LIGHT_OUT                                     0
#define CMD_PAYLOAD_REPORT_INDEX_LIGHT_IN                                      2
#define CMD_PAYLOAD_REPORT_INDEX_HD                                            3

void CmdProcess(uint8_t CMD, uint8_t* Data);
void CmdSend(uint8_t CMD, int Len, uint8_t* Data);
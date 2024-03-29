/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SensorHubOpCodeExtrator.h"

static struct iic_unit eCmdInitArray[] = {
  // ps
  {
    .operate = 0x02, .addr = 0x01, .val = 0x36, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x02, .val = 0x20, .mask = 0xFF,
  },
  {
	.operate = 0x02, .addr = 0x03, .val = 0x5C, .mask = 0xFF,
  },
  // als
  /*{
    .operate = 0x02, .addr = 0x04, .val = 0x22, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x05, .val = 0x01, .mask = 0xFF,
  },
  // interrupt settings
  {
    .operate = 0x02, .addr = 0x19, .val = 0x15, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x1A, .val = 0x11, .mask = 0xFF,
  },*/
};

static struct iic_unit eCmdEnableArray[] = {
  {
    .operate = 0x02, .addr = 0x00, .val = 0x01, .mask = 0x01,
  },  
  {
    .operate = 0x04, .addr = 0x00, .val = 0x00, .mask = 0x0A,
  },
};

static struct iic_unit eCmdDisableArray[] = {
  {
    .operate = 0x02, .addr = 0x00, .val = 0x00, .mask = 0x01,
  },
};

static struct iic_unit eCmdGetRawDataArray[] = {
  {
    .operate = 0x01, .addr = 0x08, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x01, .addr = 0x09, .val = 0x00, .mask = 0xFF,
  },
};

static struct iic_unit eCmdGetFifoDataArray[] = {
};

static struct iic_unit eCmdGetStatusArray[] = {
  {
    .operate = 0x01, .addr = 0x07, .val = 0x01, .mask = 0x01,
  },
};

static struct iic_unit eCmdSetStatusArray[] = {
};

static struct iic_unit eCmdSetSelftestArray[] = {
};

static struct iic_unit eCmdSetOffsetArray[] = {
};

static struct iic_unit eCmdSetRateArray[] = {
  {
    // ltr578 measure rate 6.125ms
    .operate = 0x02, .addr = 0x03, .val = 0x01, .mask = 0x07,
  },
  {
    // ltr578 measure rate 25ms
    .operate = 0x02, .addr = 0x03, .val = 0x03, .mask = 0x07,
  },
  {
    // ltr578 measure rate 50ms
    .operate = 0x02, .addr = 0x03, .val = 0x04, .mask = 0x07,
  },
  {
    // ltr578 measure rate 100ms
    .operate = 0x02, .addr = 0x03, .val = 0x05, .mask = 0x07,
  },
  {
    .operate = 0x04, .addr = 0x00, .val = 0x00, .mask = 0x64,
  },
};

static struct iic_unit eCmdSetModeArray[] = {
};

static struct iic_unit eCmdSetSettingArray[] = {
};

static struct iic_unit eCmdCheckIdArray[] = {
  {
    .operate = 0x03, .addr = 0x06, .val = 0xB1, .mask = 0xFF,
  },
};

static struct iic_unit eCmdScanSlaveArray[] = {
};

static sensor_info_t sensorInfoConfigArray[] = {
  {
    .Interface      = 0,
    .Interface_Freq = 400,
    .ISR_Mode       = 0,
    .GPIO_ISR_Num   = 0,
    .Slave_Addr     = 0xA6,
    .Chip_Id        = 0xB1,
    .Resolution     = 1.0f,
    .Range          = 0,
    .Postion        = 0,
    .Vendor         = 0x0,
  },
};

static struct opcode_unit sSensorOpcode[eCmdTotal] = {
  {
    .eCmd = CMD_HWINIT,
    .nIIcUnits = sizeof(eCmdInitArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdInitArray,
  },
  {
    .eCmd = CMD_ENABLE,
    .nIIcUnits = sizeof(eCmdEnableArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdEnableArray,
  },
  {
    .eCmd = CMD_DISABLE,
    .nIIcUnits = sizeof(eCmdDisableArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdDisableArray,
  },

  {
    .eCmd = CMD_GET_BYPASS,
    .nIIcUnits = sizeof(eCmdGetRawDataArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdGetRawDataArray,
  },
  {
    .eCmd = CMD_GET_FIFO,
    .nIIcUnits = sizeof(eCmdGetFifoDataArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdGetFifoDataArray,
  },
  {
    .eCmd = CMD_GET_STATUS,
    .nIIcUnits = sizeof(eCmdGetStatusArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdGetStatusArray,
  },
  {
    .eCmd = CMD_SET_STATUS,
    .nIIcUnits = sizeof(eCmdSetStatusArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdSetStatusArray,
  },
  {
    .eCmd = CMD_SET_SELFTEST,
    .nIIcUnits = sizeof(eCmdSetSelftestArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdSetSelftestArray,
  },
  {
    .eCmd = CMD_SET_OFFSET,
    .nIIcUnits = sizeof(eCmdSetOffsetArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdSetOffsetArray,
  },
  {
    .eCmd = CMD_SET_RATE,
    .nIIcUnits = sizeof(eCmdSetRateArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdSetRateArray,
  },
  {
    .eCmd = CMD_SET_MODE,
    .nIIcUnits = sizeof(eCmdSetModeArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdSetModeArray,
  },
  {
    .eCmd = CMD_SET_SETTING,
    .nIIcUnits = sizeof(eCmdSetSettingArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdSetSettingArray,
  },
  {
    .eCmd = CMD_CHECK_ID,
    .nIIcUnits = sizeof(eCmdCheckIdArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdCheckIdArray,
  },
  {
    .eCmd = CMD_SCANSALVE,
    .nIIcUnits = sizeof(eCmdScanSlaveArray) / sizeof(iic_unit_t),
    .pIIcUnits = eCmdScanSlaveArray,
  },
};

static prox_cali_t eProxCaliArray[] = {
  {
    .collect_num = 7,
    .ps_threshold_high = 250,
    .ps_threshold_low = 200,
    .dyna_cali = 1000,
    .dyna_cali_add = 20,
    .noise_threshold = 350,
    .noise_high_add = 70,
    .noise_low_add = 20,
    .ps_threshold_high_def = 250,
    .ps_threshold_low_def = 200,
    .dyna_cali_threahold = 150,
    .dyna_cali_reduce = 15,
    .value_threshold = 350,
    .noise_reference = 190,
  },
};

void SensorHubOpcodeRegisterProximity_ltr578als(void) {
  SensorHubOpcodeRegister("proximity_ltr578als", sensorInfoConfigArray, sSensorOpcode, (uint8_t*)eProxCaliArray, sizeof(eProxCaliArray));
  return;
}

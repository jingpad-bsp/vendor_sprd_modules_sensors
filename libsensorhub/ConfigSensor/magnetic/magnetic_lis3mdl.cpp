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
  {
    .operate = 0x02, .addr = 0x20, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x21, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x22, .val = 0x03, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x23, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x24, .val = 0x00, .mask = 0xFF,
  },
};

static struct iic_unit eCmdEnableArray[] = {
  {
    .operate = 0x02, .addr = 0x20, .val = 0x6C, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x21, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x22, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x23, .val = 0x0C, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x24, .val = 0x40, .mask = 0xFF,
  },
};

static struct iic_unit eCmdDisableArray[] = {
  {
    .operate = 0x02, .addr = 0x20, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x21, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x22, .val = 0x03, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x23, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x02, .addr = 0x24, .val = 0x00, .mask = 0xFF,
  },
};

static struct iic_unit eCmdGetRawDataArray[] = {
  {
    .operate = 0x01, .addr = 0x28, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x01, .addr = 0x29, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x01, .addr = 0x2A, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x01, .addr = 0x2B, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x01, .addr = 0x2C, .val = 0x00, .mask = 0xFF,
  },
  {
    .operate = 0x01, .addr = 0x2D, .val = 0x00, .mask = 0xFF,
  },
};

static struct iic_unit eCmdGetFifoDataArray[] = {
};

static struct iic_unit eCmdGetStatusArray[] = {
  {
    .operate = 0x03, .addr = 0x27, .val = 0x00, .mask = 0x00,
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
    .operate = 0x02, .addr = 0x20, .val = 0x1C, .mask = 0x1C,
  },
  {
    .operate = 0x02, .addr = 0x20, .val = 0x1C, .mask = 0x1C,
  },
  {
    .operate = 0x02, .addr = 0x20, .val = 0x14, .mask = 0x1C,
  },
  {
    .operate = 0x02, .addr = 0x20, .val = 0x0C, .mask = 0x1C,
  },
};

static struct iic_unit eCmdSetModeArray[] = {
};

static struct iic_unit eCmdSetSettingArray[] = {
};

static struct iic_unit eCmdCheckIdArray[] = {
  {
    .operate = 0x03, .addr = 0x0F, .val = 0x3D, .mask = 0xFF,
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
    .Slave_Addr     = 0x3C,
    .Chip_Id        = 0x3D,
    .Resolution     = 0.014616f,
    .Range          = 4,
    .Postion        = 4,
    .Vendor         = ST,
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

static int eMagSoftCaliArray[36] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
};


void SensorHubOpcodeRegisterMagnetic_lis3mdl(void) {
  SensorHubOpcodeRegister("magnetic_lis3mdl", sensorInfoConfigArray, sSensorOpcode, (uint8_t*)eMagSoftCaliArray, sizeof(eMagSoftCaliArray));
  return;
}

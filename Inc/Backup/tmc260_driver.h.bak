#ifndef TMC260_DRIVER_H
#define TMC260_DRIVER_H

#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "gpio.h"

#define NUMBER_OF_BYTES_PER_TRANSMISSION ((uint8_t) 1)
#define TIMEOUT_FOR_TRANSMUSSION ((uint8_t) 1)

/**
  * The enumeration of registers in tmc260 device
  */
typedef enum{TMC260_DRVCTRL, TMC260_CHOPCONF, TMC260_SMARTEN, TMC260_SGSCONF, TMC260_DRVCONF, TMC260_NUMBER_OF_REGISTERS}tmc260_register;

/**
  * The enumeration of bytes in following type 'Register'
  */
typedef enum{TMC260_BYTE_0, TMC260_BYTE_1, TMC260_BYTE_2, TMC260_NUMBER_OF_BYTES_IN_REGISTER}tmc260_byteOfRegister;

/**
 * The array 'Register' represents bits of a tmc260 device register as bytes
 */
typedef uint8_t Register[TMC260_NUMBER_OF_BYTES_IN_REGISTER];

/**
  * Union to represent the driver control register (DRVCTRL) of a tmc260 device as bitfield
  */
typedef union tmc260_DriverControlRegister_{
    Register bytes;
    struct{
        uint32_t currentB                                   : 8;
        uint32_t polarityB                                  : 1;
        uint32_t currentA                                   : 8;
        uint32_t polarityA                                  : 1;
        uint32_t registerAdressBit_1                        : 1;
        uint32_t registerAdressBit_2                        : 1;
        uint32_t                                            : 4; /* anonymous data structure padding */
    }bits_SPI;
    struct{
        uint32_t microstepResolution                        : 4;
        uint32_t reserved_1                                 : 4;
        uint32_t enableDoubleEdge                           : 1;
        uint32_t enableInterpolation                        : 1;
        uint32_t reserved_2                                 : 8;
        uint32_t registerAdressBit_1                        : 1;
        uint32_t registerAdressBit_2                        : 1;
        uint32_t                                            : 4; /* anonymous data structure padding */
    }bits_StepDir;
}tmc260_DriverControlRegister;

/**
  * Union to represent the chopper control register (CHOPCONF) of a tmc260 device as bitfield
  */
typedef union tmc260_ChopperControlRegister_{
    Register bytes;
    struct{
        uint32_t offTimeOrMosfetDisable                     : 4;
        uint32_t hysteresisStartOrFastDecayTime             : 3;
        uint32_t hysteresisEndOrSineWaveOffset              : 4;
        uint32_t hysteresisDecrementIntervalOrFastDecayMode : 2;
        uint32_t randomToffTime                             : 1;
        uint32_t chopperMode                                : 1;
        uint32_t blankingTime                               : 2;
        uint32_t registerAdressBit_1                        : 1;
        uint32_t registerAdressBit_2                        : 1;
        uint32_t registerAdressBit_3                        : 1;
        uint32_t                                            : 4; /* anonymous data structure padding */
    }bits;
}tmc260_ChopperControlRegister;

/**
  * Union to represent the coolStep control register (SMARTEN) of a tmc260 device as bitfield
  */
typedef union CoolStepControlRegister_{
    Register bytes;
    struct{
        uint32_t lowerCoolStepThresholdOrCoolStepDisable    : 4;
        uint32_t reserved_1                                 : 1;
        uint32_t currentIncrementSize                       : 2;
        uint32_t reserved_2                                 : 1;
        uint32_t upperCoolStepThresholdAsOffsetFromLower    : 4;
        uint32_t reserved_3                                 : 1;
        uint32_t currentDecrementSpeed                      : 2;
        uint32_t minimumCoolStepCurrent                     : 1;
        uint32_t reserved_4                                 : 1;
        uint32_t registerAdressBit_1                        : 1;
        uint32_t registerAdressBit_2                        : 1;
        uint32_t registerAdressBit_3                        : 1;
        uint32_t                                            : 4; /* anonymous data structure padding */
    }bits;
}tmc260_CoolStepControlRegister;

/**
  * Union to represent the stallGuard2 control register (SGCSCONF) of a tmc260 device as bitfield
  */
typedef union tmc260_StallGuard2ControlRegister_{
    Register bytes;
    struct{
        uint32_t currentScale                               : 5;
        uint32_t reserved_1                                 : 1;
        uint32_t reserved_2                                 : 1;
        uint32_t reserved_3                                 : 1;
        uint32_t stallGuard2ThresholdValue                  : 7;
        uint32_t reserved_4                                 : 1;
        uint32_t stallGuard2FilterEnable                    : 1;
        uint32_t registerAdressBit_1                        : 1;
        uint32_t registerAdressBit_2                        : 1;
        uint32_t registerAdressBit_3                        : 1;
        uint32_t                                            : 4; /* anonymous data structure padding */
    }bits;
}tmc260_StallGuard2ControlRegister;

/**
  * Union to represent the driver configuration register (DRVCONF) of a tmc260 device as bitfield
  */
typedef union tmc260_DriverConfigurationRegister_{
    Register bytes;
    struct{
        uint32_t reserved_1                                 : 1;
        uint32_t reserved_2                                 : 1;
        uint32_t reserved_3                                 : 1;
        uint32_t reserved_4                                 : 1;
        uint32_t selectValueforReadOut                      : 2;
        uint32_t senseResistorVoltageBasedCurrentScallung   : 1;
        uint32_t stepDirInterfaceDisable                    : 1;
        uint32_t shortToGndDetectionTimer                   : 2;
        uint32_t shortToGndProtectionDisable                : 1;
        uint32_t reserved_5                                 : 1;
        uint32_t slopeControlLowSide                        : 2;
        uint32_t slopeControlHighSide                       : 2;
        uint32_t reservedTestMode                           : 1;
        uint32_t registerAdressBit_1                        : 1;
        uint32_t registerAdressBit_2                        : 1;
        uint32_t registerAdressBit_3                        : 1;
        uint32_t                                            : 4; /* anonymous data structure padding */
    }bits;
}tmc260_DriverConfigurationRegister;

/**
  * Union to represent the read response of a tmc260 device as bitfield
  */
typedef union tmc260_ReadResponse_{
    Register bytes;
    struct{
        uint32_t stallGuard2Status                          : 1;
        uint32_t overtemperatureShutdown                    : 1;
        uint32_t overtemperatureWarning                     : 1;
        uint32_t shortToGndDetectionOnHighSideTransistors   : 2;
        uint32_t openLoadIndicator                          : 2;
        uint32_t standstillIndicator                        : 1;
        union{
            struct{
                uint16_t reserved_1                          : 1;
                uint16_t reserved_2                          : 1;
                uint16_t mstep_value                         : 9;
                uint16_t mstep_direction                     : 1;
                uint32_t                                     : 4; /* anonymous data structure padding */
            }rdsel_00;
            struct{
                uint16_t reserved_1                          : 1;
                uint16_t reserved_2                          : 1;
                uint16_t stallGuard2Value9_0                 : 10;
                uint32_t                                     : 4; /* anonymous data structure padding */
            }rdsel_01;
            struct{
                uint16_t reserved_1                          : 1;
                uint16_t reserved_2                          : 1;
                uint16_t coolStepValue4_0                    : 5;
                uint16_t stallGuard2Value9_5                 : 5;
                uint32_t                                     : 4; /* anonymous data structure padding */
            }rdsel_10;
        };
    }bits;
}tmc260_ReadResponse;

/**
  * Structure to represent the registers of a tmc260 device as set of bitfields
  */
typedef struct tmc260_RegisterSet_{
    tmc260_DriverControlRegister           driverControlRegister;
    tmc260_ChopperControlRegister          chopperControlRegister;
    tmc260_CoolStepControlRegister         coolStepControlRegister;
    tmc260_StallGuard2ControlRegister      stallGuard2ControlRegister;
    tmc260_DriverConfigurationRegister     driverConfigurationRegister;
    tmc260_ReadResponse                    readResponse;
    int8_t (* sendAndReceive)(struct tmc260_RegisterSet_ * const, tmc260_register);
}tmc260_RegisterSet;

/**
 * @brief   TMC260_Registers_init
 * @param   device      pointer to a uninitialized structure
 * @return  exitStatus  EXIT_SUCCESS or EXIT_FAILURE
 */
int8_t tmc260_Registers_init(tmc260_RegisterSet * device);

#endif // TMC260_DRIVER_H

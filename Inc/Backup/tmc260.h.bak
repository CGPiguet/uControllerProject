#ifndef TMC260
#define TMC260

#include <stdint.h>

/**
 * Opaque type 'tmc260_RegiserSet' to hide information of tmc260_driver module
 */
typedef struct tmc260_RegisterSet_ tmc260_RegisterSet;

/**
  * Enumeration of avaiable parameters to configure a tmc260 device
  */
typedef enum{TMC260_MICROSTEP_RESOLUTION, TMC260_CURRENT_SCALE, TMC260_NUMBER_OF_PARAMETERS}tmc260_parameter;

/**
  * Structure as limited interface to the tmc260_driver module
  */
typedef struct tmc260_Device_{
    tmc260_RegisterSet  * registerSet;
    int8_t (* setParameter)(struct tmc260_Device_ * const, tmc260_parameter, uint32_t);
    int8_t (* sendParameter)(struct tmc260_Device_ * const);
}tmc260_Device;

/**
 * @brief   tmc260_init
 * @param   device  pointer to a uninitialized structure
 * @return  exitStatus  EXIT_SUCCESS or EXIT_FAILURE
 */
int8_t tmc260_init(tmc260_Device * const device);

#endif // TMC260

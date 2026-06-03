#ifndef BSP_MOTOR_H
#define BSP_MOTOR_H

#include <stdint.h>

#define BSP_MOTOR_PWM_FREQ_HZ             (20000U)
#define BSP_MOTOR_DUTY_MAX                (9500U)
#define BSP_MOTOR_START_DUTY_MIN          (3000U)
#define BSP_MOTOR_START_DUTY_MAX          (4000U)
#define BSP_MOTOR_NORMAL_DUTY_MIN         (4000U)
#define BSP_MOTOR_NORMAL_DUTY_MAX         (8000U)
#define BSP_MOTOR_PROTECT_REVERSE_DELAY_MS (80U)

typedef enum {
    BSP_MOTOR_DIR_STOP = 0,
    BSP_MOTOR_DIR_FORWARD,
    BSP_MOTOR_DIR_REVERSE
} bsp_motor_direction_t;

void bsp_motor_init(void);
void bsp_motor_forward(uint16_t duty);
void bsp_motor_reverse(uint16_t duty);
void bsp_motor_stop(void);
void bsp_motor_protect_reverse(uint16_t reverse_duty, uint32_t reverse_time_ms);
bsp_motor_direction_t bsp_motor_get_direction(void);
uint16_t bsp_motor_get_duty(void);

#endif /* BSP_MOTOR_H */

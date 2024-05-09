#ifndef _BULLET_HATCH_TASK_H_
#define _BULLET_HATCH_TASK_H_

#include "public.h"
	typedef struct
{
  float hatch_angle_ref;
  float hatch_angle_fdb;
  float hatch_speed_ref;
  float hatch_speed_fdb;
  int16_t bullet_hatch_motor_input;
} hatch_ref_and_fdb_t;

typedef enum
{
	INIT						= 0,
  CLOSE          = 1,
	OPEN           = 2,
} bullet_hatch_mode_e;

typedef struct
{
	u8 bullet_hatch_init_flag;
	u8 bullet_hatch_lock;
	hatch_ref_and_fdb_t hatch_ref_and_fdb;
	bullet_hatch_mode_e bullet_hatch_mode;
	
	pid_t pid_hatch_speed_init;
	pid_t pid_hatch_angle_init;
	pid_t pid_hatch_speed;
	pid_t pid_hatch_angle;
	
}bullet_hatch_t;


extern bullet_hatch_t bullet_hatch;
void bullet_hatch_param_init(void);
void bullet_hatch_task(void);
void hatch_state_mode_switch(void);

#endif
#pragma once

// domain specific language

#define GET_POS(entity) monitor.GetComponent<ComponentPosition>(entity)->pos
#define GET_STEPS_CUR(entity) monitor.GetComponent<ComponentMovement>(entity)->steps_cur
#define GET_STEPS_MAX(entity) monitor.GetComponent<ComponentMovement>(entity)->steps_max
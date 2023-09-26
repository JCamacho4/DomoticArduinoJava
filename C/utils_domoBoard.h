/*
 * utils_domoBoard.h
 *
 *  Created on: 10/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

#ifndef UTILS_DOMOBOARD_H_
#define UTILS_DOMOBOARD_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"Arduino.h"
#include	"./DomoBoard/domoBoard.h"
#include	"./DomoBoard/ModbusDomoboard.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define 	ON   				HIGH
#define     OFF					LOW

static unsigned long startTime = 0;
static int estado = STOP;
static int porcentaje = 0;
static int totalDuration = 7200;


static unsigned short int tiempoTotalEspera = 5;
static unsigned short int tiempoProteccionMotor = 2;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
void Interruptor(void *Sensor);
void Pulsado_Soltado(void *Sensor);
void conmutador(void *Sensor);

void mbInterruptor(void *mbSensor);
void mbConmutador(void *mbSensor);
void mbInterruptorTemporizado(void *Sensor);
void Calc_Temperatura(void *mbSensor);
void interruptor_SRC(void *Sensor);
void trigger_Level(void *Sensor);


void mbPersiana();
void mbBotonBajar(void *mbSensor);
void mbBotonSubir(void *mbSensor);

void mbBotonGaraje(void *mbSensor);
void mbGaraje();

void tiempoPersiana();

void AccionesTemporizadas(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern QueueList<TpmbSensor>		SensoresTemporizados;


#endif /* UTILS_DOMOBOARD_H_ */

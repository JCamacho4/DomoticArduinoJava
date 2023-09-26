/*
 * domoBoard.h
 *
 *  Created on: 09/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

#ifndef DOMOBOARD_H_
#define DOMOBOARD_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"Arduino.h"
#include	"../QueueList.h"

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef void (*TNotifyEvent)(void *);

typedef enum
{
	S_DIGITAL,
	S_ANALOGICO
}teSensor;

typedef struct
{
	byte	pin;
	bool	estado;
}tsActuator, *tpsActuator;

typedef 	QueueList<tpsActuator>		TManagerActuators;

typedef struct
{
	byte					pin;			//Pin asignado al sensor
	int	    				valor;			//Valor leido
	int						valor_Df;		//Valor Sensor por defecto
	bool					Activo;			//Activar/Desactivas sensor
	teSensor				eSensor;		//Tipos de sensor ANAL�GICO/DIGITAL
	int						Aux;			//Variables para ser usadas en la gesti�n del sensor
	String					name;
	TNotifyEvent			SensorEvent;
	TManagerActuators		managerActuators;
}tsSensor, *ptsSensor;

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define 	BUTTON1_P  				3   	   	// Pulsador 1
#define 	BUTTON2_P  				10        	// Pulsador 2
#define 	RELE_P					4			// RELE 1
#define		TRIAC_P					7			// ON/OFF Solid State Triac
#define 	BTN_OPT_P		    	11 	    	// Entrada Optocoplada
#define		EN_485					12			// Bit to enable RS485 Conmunications
#define     PIR_P					13			// Sensor de Movimiento

#define		PHOTORES_P				A0			// Photo resistor SRC
#define		TEMPSEN_P				A2			// Sensor de temperatura
#define		POT1_P					A3			// Potenci�metro 1
#define		POT2_P					A4			// Potenci�metro 2
#define		PHOTOTTOR_P				A5			// Sensor de luminosidad (PhotoTransistor)

#define		PERSIANA_ONOFF_P		5			// Activar persiana
#define		PERSIANA_SENTIDO_P		6			// Sentido de la persiana

/****************************************************************************/
/***        Exported Class                                                ***/
/****************************************************************************/
class DomoBoard
{
private:


public:

	DomoBoard(); 						//Constructor
	virtual ~DomoBoard(){};

	//Sensores Digitales
	tsSensor		BOTON1;
	tsSensor		BOTON2;
	tsSensor 		BTN_OPT;
	tsSensor		PIR_MOV;

	//Sensores Anal�gicos
	tsSensor		POT1;				// Potenci�metro 1
	tsSensor		POT2;				// Potenci�metro 2
	tsSensor		PHOTORES;			// Photoresistor SRC
	tsSensor		TEMPSEN;			//Sensor de temperatura
	tsSensor		PHOTOTTOR;			//Foto transistor

	//Salidas
	tsActuator		RELE;
	tsActuator		TRIAC;

		// persiana
	tsActuator 		PERSIANA_ONOFF;
	tsActuator		PERSIANA_SENTIDO;

	void 	leerAllSensor(void);
	void 	leerSensor(ptsSensor Sensor);
	static 	void	setActuator(tsActuator *Actuator, bool val);
	static 	void 	manageSensorActuators(TManagerActuators *managerActuators, bool val);
	virtual void    clear_Actuators();
};

extern DomoBoard domoboard;


#endif /* DOMOBOARD_H_ */

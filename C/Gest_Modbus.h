/*
 * Gest_Modbus.h
 *
 *  Created on: 16/05/2014
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

#ifndef GEST_MODBUS_H_
#define GEST_MODBUS_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include 	"./ModbusSlave/ModbusSlave.h"
#include    "./DomoBoard/ModbusDomoboard.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
//Configuraci�n de la conexi�n MODBUS
#define	ADDR_SLAVE			1		//Direcci�n Dispositivo Esclavo
#define	SERIAL_BPS			115200	//Velocidad Comunicaci�n serie
#define	SERIAL_PARITY		'n'		//Paridad comunicaci�n serie
#define	TX_EN_PIN			0		//Pin usado para la transmisi�n RS485; 0 No usado

// La cofiguraci�n de domoboard usando ModBus, ser� almacenada en la EEPROM.
// Las direccioes de los valores a almacenar se indican acontinuaci�n

#define	ADDR_SELPRACT		0		//Comando de Configuraci�n actual de DomoBoad
#define	ADDR_ACTPIR			1		//Activa/Desactiva PIR
#define ADDR_TIEMPO_PIR_1	2		//Tiempo PIR Activo 1
#define ADDR_TIEMPO_PIR_2	3		//Tiempo PIR Activo 2
#define ADDR_ACTSRC			4		//Activa/desactiva SRC (Photorresistencia)
#define	ADDR_SRC_HL_1		5		//Nivel Alto Activaci�n SRC
#define	ADDR_SRC_HL_2		6		//Nivel Alto Activaci�n SRC
#define	ADDR_SRC_LL_1		7		//Nivel Bajo Activaci�n SRC
#define	ADDR_SRC_LL_2		8		//Nivel Bajo Activaci�n SRC
#define ADDR_ACTTOR			9       //Activa/desactiva Phototransistor
#define ADDR_TTOR_LL_1		10		//Nivel Bajo Activaci�n Trigger ttor
#define ADDR_TTOR_LL_2		11		//Nivel Bajo Activaci�n Trigger ttor


/****************************************************************************/
/***        DEFINICI�N DE FUNCIONES    **************************************/
/****************************************************************************/
void Init_RTU_Modbus();
void RTU_ModBus();
void load_Config();



#endif /* GEST_MODBUS_H_ */

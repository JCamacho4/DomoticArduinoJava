/*
 * utils_domoBoard.cpp
 *
 *  Created on: 10/03/2015
 *      Author: jctejero
 *
 *  Modified on: 16/03/2022
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "utils_domoBoard.h"
#include "HardwareSerial.h"
#include "debuglog.h"
#include "Gest_Modbus.h"


#include	"./DomoBoard/ModbusDomoboard.h"


/******************************************************************/
/***                      Variables Locales                     ***/
/******************************************************************/
//Lista de sensores temporizados
QueueList<TpmbSensor>		SensoresTemporizados;


/*============================================*/
/*		 		  INTERRUPTOR                 */
/*============================================*/
void Interruptor(void *Sensor)
{
	if(((ptsSensor)Sensor)->valor_Df == ((ptsSensor)Sensor)->valor){
		Serial.print(((ptsSensor)Sensor)->name);
		Serial.print(" : Interruptor --> ");
		if(((ptsSensor)Sensor)->Aux == OFF){
			((ptsSensor)Sensor)->Aux = ON;
			Serial.println("ON");
		}else{
			((ptsSensor)Sensor)->Aux = OFF;
			Serial.println("OFF");
		}
	}

	DomoBoard::manageSensorActuators(&((ptsSensor)Sensor)->managerActuators, ((ptsSensor)Sensor)->Aux);
}

/*============================================*/
/*		 		    PULSADOR                  */
/*============================================*/
void Pulsado_Soltado(void *Sensor){
	ptsSensor sensor = reinterpret_cast<ptsSensor>(Sensor);

	Serial.print(sensor->name);
	if(sensor->valor_Df == sensor->valor){
		Serial.println(" --> Soltado");
	}else{
		Serial.println(" --> Pulsado");
	}

	DomoBoard::manageSensorActuators(&(sensor->managerActuators), !(sensor->valor_Df == sensor->valor));
}

/*============================================*/
/*		     	   CONMUTADOR                 */
/*============================================*/
void conmutador(void *Sensor)
{
	static int valor = OFF;

	ptsSensor sensor = reinterpret_cast<ptsSensor>(Sensor);

	if(sensor->valor_Df == sensor->valor){
		Serial.print("Conmutador --> ");
		if(valor == OFF){
			valor = ON;
			Serial.println("ON");
		}else{
			valor = OFF;
			Serial.println("OFF");
		}

		//Actualiza Actuadores
		//DomoBoard::setActuator(&domoboard.RELE, valor);

		DomoBoard::manageSensorActuators(&(sensor->managerActuators), valor);
	}
}

/*============================================*/
/*		      INTERRUPTOR MODBUS              */
/*============================================*/
void mbInterruptor(void *mbSensor)
{
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	if(sensor->Sensor->valor_Df == sensor->Sensor->valor){
		mbDomoboard.manager_mbActuators(&(sensor->mbActuators), TOGGLE);
	}
}

/*============================================*/
/*		 	   CONMUTADOR MODBUS              */
/*============================================*/
void mbConmutador(void *mbSensor)
{
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	if(sensor->Sensor->valor_Df == sensor->Sensor->valor){
		//Estado del sensor ha cambiado
		mbDomoboard.manager_mbActuators(&(sensor->mbActuators), TOGGLE);
	}
}

/*========================================================*/
/*		      INTERRUPTOR TEMPORIZADO MODBUS              */
/*========================================================*/

/*
 * Este sensor ser� activado mediante Sensor (Variable de control) y aunque se vuelva inactivo,
 * el interruptor, permanecer� activo durante el tiempo que indique el temporizador.
 * Si durante el tiempo que el interruptor est� activo, la variable de control permanece inactiva,
 * o se activa de nuevo, el tiempo que el interruptor permanece activo se
 * ir� actualizando, de tal forma que el tiempo que el interruptor permanece ativo,
 * siempre se cuenta desde la �ltima vez que el sensor se activo.
 *
 */

void	mbInterruptorTemporizado(void *Sensor){

	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	if(sensor->Sensor->valor_Df != sensor->Sensor->valor){
		sensor->asyncWait->restart();

		mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)ON);

	}else{
		if(!sensor->asyncWait->isWaiting() && !sensor->asyncWait->isVerified()){

			DEBUGLNF("ASYNWAIT TERMINADO");

			sensor->asyncWait->setVerified();
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)OFF);
		}
	}

}

/*========================================================*/
/*		      			PERSIANA			             */
/*========================================================*/

void tiempoPersiana() {
	int porcentajeDeLaIteracion;
	unsigned long currentTime = millis();
	int porcentajeTotal;
	if (estado == SUBIENDO) {
		// Porcentaje de tiempo transcurrido en la iteracion
		porcentajeDeLaIteracion = (currentTime - startTime) * 100 / totalDuration;

		// Si se supera el limite, se queda a 100
		porcentajeTotal =
				porcentaje + porcentajeDeLaIteracion >= 100 ?
						100 : porcentaje + porcentajeDeLaIteracion;

		// Se redondea hacia arriba o hacia abajo
		Iregs[MB_PER_TIEMPO] = porcentajeTotal % 5 <= 2 ?
				porcentajeTotal - (porcentajeTotal % 5) :
				porcentajeTotal+ (5 - (porcentajeTotal % 5));

	} else if (estado == BAJANDO) {
		// Porcentaje de tiempo transcurrido en la iteracion
		porcentajeDeLaIteracion = (currentTime - startTime) * 100 / totalDuration;

		// Si se supera el limite, se queda a 100
		porcentajeTotal =
				porcentaje - porcentajeDeLaIteracion < 0 ?
						0 : porcentaje - porcentajeDeLaIteracion;

		// Se redondea hacia arriba o hacia abajo
		Iregs[MB_PER_TIEMPO] = porcentajeTotal % 5 <= 2 ?
				porcentajeTotal - (porcentajeTotal % 5) :
				porcentajeTotal + (5 - (porcentajeTotal % 5));
	}
}


void mbPersiana() {
	int boton_subir = Cregs[MB_PER_SUBIR];
	int boton_bajar = Cregs[MB_PER_BAJAR];

	switch (estado) {
	case STOP:
		if ((boton_subir == 1 && boton_bajar == 0)) {
			estado = SUBIENDO;
			startTime = millis();
			mbDomoboard.cambioEstadoPersiana(estado);
		} else if (boton_subir == 0 && boton_bajar == 1) {
			estado = BAJANDO;
			startTime = millis();
			mbDomoboard.cambioEstadoPersiana(estado);
		}
		break;

	case SUBIENDO:
		if (boton_subir == 0 && boton_bajar == 0) {
			porcentaje = Iregs[MB_PER_TIEMPO];
			estado = STOP;
			mbDomoboard.cambioEstadoPersiana(estado);
		}

		if (boton_subir == 0 && boton_bajar == 1) {
			porcentaje = Iregs[MB_PER_TIEMPO];
			estado = STOP_2;
			mbDomoboard.cambioEstadoPersiana(estado);
		}
		break;

	case BAJANDO:
		if (boton_subir == 0 && boton_bajar == 0) {
			porcentaje = Iregs[MB_PER_TIEMPO];
			estado = STOP;
			mbDomoboard.cambioEstadoPersiana(estado);
		}

		if (boton_subir == 1 && boton_bajar == 0) {
			porcentaje = Iregs[MB_PER_TIEMPO];
			estado = STOP_2;
			mbDomoboard.cambioEstadoPersiana(estado);
		}
		break;

	case STOP_2:
		if (boton_subir == 0 && boton_bajar == 0) {
			estado = STOP;
			mbDomoboard.cambioEstadoPersiana(estado);
		}
		break;
	}

	Iregs[MB_PER_ESTADO] = estado;

}

void mbBotonSubir(void *mbSensor)
{
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	Cregs[MB_PER_SUBIR] = sensor->Sensor->valor;

	mbPersiana();
}

void mbBotonBajar(void *mbSensor)
{
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	Cregs[MB_PER_BAJAR] = sensor->Sensor->valor;

	mbPersiana();
}


/*========================================================*/
/*		      			GARAJE	  			             */
/*========================================================*/

void mbGaraje(){
	static AsyncWait asyncWait;


	switch (estado) {
	case A_STOP:
		if (Cregs[MB_GAR_BOTON] == ON) {
			Iregs[MB_PER_TIEMPO] = 0;
			estado = B_SUBIENDO;
			startTime = millis();
			mbDomoboard.cambioEstadoPersiana(SUBIENDO);
		}
		break;

	case B_SUBIENDO:
		if (Iregs[MB_PER_TIEMPO] == 100) {
			porcentaje = 100;
			estado = C_STOP;
			mbDomoboard.cambioEstadoPersiana(STOP);
			asyncWait.startWaiting(&tiempoTotalEspera);
		}
		break;

	case C_STOP:
		if (Cregs[MB_GAR_BOTON] == ON
				|| Iregs[MB_PHOTOTTOR] < Aregs[MB_TTOR_LL]) {
			asyncWait.restart();
		}
		if (!asyncWait.isWaiting() && !asyncWait.isVerified()) {
			asyncWait.setVerified();
			startTime = millis();
			mbDomoboard.cambioEstadoPersiana(BAJANDO);
			estado = D_BAJANDO;

		}
		break;

	case D_BAJANDO:
		if (Cregs[MB_GAR_BOTON] == ON
				|| Iregs[MB_PHOTOTTOR] < Aregs[MB_TTOR_LL]) {
			porcentaje = Iregs[MB_PER_TIEMPO];
			estado = E_STOP;
			mbDomoboard.cambioEstadoPersiana(STOP);
			asyncWait.startWaiting(&tiempoProteccionMotor);
		}
		if (Iregs[MB_PER_TIEMPO] == 0) {
			estado = A_STOP;
			porcentaje = 0;
			mbDomoboard.cambioEstadoPersiana(STOP);
		}
		break;

	case E_STOP:
		if (!asyncWait.isWaiting()) {
			estado = B_SUBIENDO;
			startTime = millis();
			mbDomoboard.cambioEstadoPersiana(SUBIENDO);
		}
		break;
	default:
		break;
	}

	Iregs[MB_PER_ESTADO] = estado;
}

void mbBotonGaraje(void *mbSensor){
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(mbSensor);

	Cregs[MB_GAR_BOTON] = sensor->Sensor->valor;
}




/*****************************************************************************************/
/***********************************  Leer Temperatura ***********************************/
/*****************************************************************************************/
//mas/menos 1 - 2�C de exactitud para el TMP36, por lo que escribimos valores decimales
void Calc_Temperatura(void *Sensor){
	float valTMP;
	int   temp;

	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	valTMP = sensor->Sensor->valor*.004882812;	//Conviere resultado convertidor a voltios

	valTMP = (valTMP - .5)*100;          //Convierte Temperatura de 10 mV por grado con 500 mV de Offset


	temp = (valTMP - (int)valTMP)*100;

    if(temp < 50) temp = 0;
    //else if ((25 < temp)&&(temp < 75)) temp = 5;
    //else if ((75 < temp)&&(temp <= 99)){
    else{
    	temp = 0;
    	valTMP = (int)valTMP +1;
    }

	*(sensor->mbRegs) = (((int)valTMP & 0xff) << 8) | (temp & 0xff);

#ifdef DEBUG_TEMP
    static float vT = 0;

    if(vT != valTMP){
    	vT = valTMP;
    	/*
    	DEBUGF("Temperatura = " );
    	Serial.print((int)valTMP,DEC);
    	Serial.print(".");
    	Serial.println(temp,DEC);
    	*/
    	DEBUGLOG("Temperatura = %d.%d\n", (int)valTMP, temp);

    //	Serial.print("Temperatura F -> ");
    //	Serial.println(Sensor->Regs_App[Sensor->MBReg_App],BIN);
    }
#endif
}

/*============================================*/
/*			  INTERRUPTOR LUMINOSIDAD         */
/*============================================*/
/*
 * Interruptor por nivel de luminosidad. funcionar� con una histeresis, es decir,
 * Si el interruptor est� desactivado, se activar� cuando alcance el "highlevel".
 *
 * Si el interruptor est� ativado, se desactivar� cuando alcanze el "lowlevel".
 */
void	interruptor_SRC(void *Sensor){
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	int state = sensor->Sensor->Aux;

	switch(state){
	case 1:
		if(sensor->Sensor->valor <= (int)*(sensor->ctrlLevelPtr.lInf)){
			state = 2;
			//Estado del sensor ha cambiado
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)ON);
		}

		break;

	case 2:
		if(sensor->Sensor->valor >= (int)*(sensor->ctrlLevelPtr.lSup)){
			state = 1;
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)OFF);
		}

		break;

	default:
		state = 2;
		mbDomoboard.set_coilRegister(MB_TRIAC, OFF);
	}

	sensor->Sensor->Aux = state;
}

/*============================================*/
/*			  TRIGGER WITH LUMINOSIDAD        */
/*============================================*/
/*
 * tRIGGER por nivel de luminosidad. Dispara un evento cuando el nivel del sensor
 * se encuentra por debajo de un determinado nivel
 */
void	trigger_Level(void *Sensor){
	TpmbSensor sensor = reinterpret_cast<TpmbSensor>(Sensor);

	uint16_t limInfe = *(sensor->ctrlLevelPtr.lInf);

	if(((uint16_t) sensor->Sensor->valor <= limInfe)){
		if(sensor->Sensor->Aux == 0){
			mbDomoboard.manager_mbActuators(&(sensor->mbActuators), (TStateDigitalDev)ON);
			sensor->Sensor->Aux = 1;
		}
	} else sensor->Sensor->Aux = 0;
}

/*====================================================================================
 * Llamada regular usada para gestionar los temporizadores usados por la aplicaci�n. =
 * es llamada cada LOOP_TIME (10 ms)
 *====================================================================================*/
void AccionesTemporizadas(void){

	TpmbSensor pmbSensor;

	for(int n=0; n<SensoresTemporizados.count(); n++){

		pmbSensor = SensoresTemporizados.peek(n);

		pmbSensor->mbSensorEvent(pmbSensor);
	}
}





package Base_COM_Serie;

public class MB_Registers {
	
	//Defininición de comando de configuración
	public static final int P01_PULSADORES 		= 0x11;
	public static final int P01_INTERRUPTOR		= 0x12;
	public static final int P01_CONMUTADOR		= 0x13;
	public static final int P03_CONMUTADOR		= 0x32;
	public static final int P04_PULSADORES		= 0x41;
	public static final int P04_INTERRUPTOR		= 0x42;
	public static final int P04_CONMUTADOR		= 0x43;
	public static final int PRACTICAS_MODBUS	= 0x50;
	public static final int P05_INTERRUPTOR		= 0x54;
	public static final int P06_INTERRUPTOR		= 0x62;
	public static final int P07_PIR				= 0x70;
	public static final int P09_ALL				= 0x90;
	public static final int P10_PERSIANA		= 0x91;
	public static final int P10_GARAJE			= 0x92;

	
	
	
	//Selección configuración Práctica
	public static final int SELPRACT[] = {P01_PULSADORES, P01_INTERRUPTOR, P01_CONMUTADOR, 
										P03_CONMUTADOR, P04_PULSADORES, P04_INTERRUPTOR, P04_CONMUTADOR, P05_INTERRUPTOR, 
										P06_INTERRUPTOR, P07_PIR, P09_ALL, P10_PERSIANA, P10_GARAJE};
	
	public static int PRACTICE_SELECTED = 0x00;

	//Analog Output Holding Registers
	public enum MB_Analog_Output_Holding {
				
		//Discrete Output Coils
		MB_PRACT(0),			// Registro para indicar la práctica con la que trabajamos
		MB_TMP_PIR(1),			// Registro para controlar el tiempo activo del sensor PIR (Segundos)
		MB_SRC_HL(2),			// Registro para controlar el nivel superior de activación SRC
		MB_SRC_LL(3),			// Registro para controlar el nivel Inferior de activación SRC
		MB_TTOR_LL(4),			// Registro para controlar el nivel de detección del phototransistor
		MB_AREGS(5);
				
		int reg;
				
		MB_Analog_Output_Holding(int rg){
			reg = rg;
		}
				
		public int getReg(){
			return reg;
		}
				
		public void setReg(int rg) {
			reg = rg;
		}
	}
	
	public enum MB_Analog_Input_Register {
		
		//Discrete Output Coils
		MB_POT1(0),
		MB_POT2(1),
		MB_PHOTORES(2),				// Foto Resistencia
		MB_TEMPSEN(3),				// Sensor de Temperatura
		MB_PHOTOTTOR(4),			// Photo Transistor
		
		MB_PER_ESTADO(5),
		MB_PER_TIEMPO(6),
		
		MB_I_AREGS(7);
		
		int reg;
		
		MB_Analog_Input_Register(int rg){
			reg = rg;
		}
		
		public int getReg(){
			return reg;
		}
		
		public void setReg(int rg) {
			reg = rg;
		}
		
		public int getDefaultValue() {
			switch(this) {
			case MB_POT1:
				return 0;
			case MB_POT2:
				return 1;
			default:
				return -1;
			}
		}
	}

	
	public enum MB_Discrete_Output_Coils {
		
		//Discrete Output Coils
		MB_RELE(0),
		MB_TRIAC(1),
		MB_ACTPIR(2),					//Salida virtual para Activar/Desactivar PIR
		MB_ACTSRC(3),					//Salida virtual para Activar/Desactivar SRC --> PhotoResistor
		MB_ACTTOR(4),					//Salida virtual para Activar/Desactivar Trigger --> PhotoTransistor

		MB_PERSIANA_ONOFF(5),			// Actuador encargado de encender o apagar el motor de la persiana
		MB_PERSIANA_SENTIDO(6),		// Actuador encargado de poner el sentido con el que irá la persiana

		MB_PER_SUBIR(7),				// Esto sería el sensor virtual con el que se pondrá parejo Java/Cpp
		MB_PER_BAJAR(8),				// Esto sería el sensor virtual con el que se pondrá parejo Java/Cpp
		
		MB_GAR_BOTON(9),

		MB_O_COILS(5);				// DE MOMENTO NO NECESITO LEERLO TODO, ASI MAS EFECIENTE
		
		int reg;
		
		MB_Discrete_Output_Coils(int rg){
			reg = rg;
		}
		
		public int getReg(){
			return reg;
		}
		
		public void setReg(int rg) {
			reg = rg;
		}
	}
	
	public enum MB_Discrete_Input_Contacts {
		
		//Discrete Output Coils
		MB_BTN1(0),
		MB_BTN2(1),
		MB_OPT(2),
		MB_PIR(3),			//Sensor de movimiento
		MB_I_REGS(4);
		
		int reg;
		
		MB_Discrete_Input_Contacts(int rg){
			reg = rg;
		}
		
		public int getReg(){
			return reg;
		}
		
		public void setReg(int rg) {
			reg = rg;
		}
		
		public int getDefaultValue() {
			switch(this) {
			case MB_BTN1:
				return 0;
			case MB_BTN2:
				return 0;
			case MB_OPT:
				return 1;
			default:
				return -1;
			}
		}
	}
	
	public enum TSwitchState { 
		OFF, 
		ON,  
		TOGGLE;
		
		public static String ToNumberString(TSwitchState x) {
	        switch(x) {
	        case OFF:
	            return "0";
	        case ON:
	        	return "1";
	        case TOGGLE:
	        	return "2";
	        default:
	        	return "0";
	        }
	    }
		
		public static int ToNumber(TSwitchState x) {
	        switch(x) {
	        case OFF:
	            return 0;
	        case ON:
	        	return 1;
	        case TOGGLE:
	        	return 2;
	        default:
	        	return 0;
	        }
	    }
		
	}
}

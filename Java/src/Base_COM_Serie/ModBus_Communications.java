package Base_COM_Serie;

import Base_COM_Serie.MB_Registers.TSwitchState;
import CommTransport.CommTransport;
import Utilidades.threadpool.DefaultExecutorSupplier;
import modbus.Const_Modbus;
import modbus.ModBusEvent;
import modbus.Modbus;

public class ModBus_Communications {

	public static void writeCoil(int SlaveAddress, int ModBusRegister, TSwitchState coilState,
			CommTransport sn_Transport) {

		String[] args = { Integer.toString(SlaveAddress), String.valueOf(Const_Modbus.WRITE_COIL),
				Integer.toString(ModBusRegister), TSwitchState.ToNumberString(coilState) };

		// Iniciamos Comunicación
		InitModbusComunication(args, sn_Transport, null);
	}
	
	public static void writeSingleRegister(int SlaveAddress, int ModBusRegister, String val, CommTransport sn_Transport) {
		
		String[] args = {Integer.toString(SlaveAddress),String.valueOf(Const_Modbus.WRITE_SINGLE_REGISTER), 
				Integer.toString(ModBusRegister), val};
		
		// Iniciamos Comunicación
		InitModbusComunication(args, sn_Transport, null);
	}
	
	public static void readMultipleRegisters(int SlaveAddress, int firstRegister, int numRegisters, CommTransport sn_Transport, IModBusResponse modBusResponse,
			int[]... reg) {
		
		
		String[] args = {Integer.toString(SlaveAddress),String.valueOf(Const_Modbus.READ_MULTIPLE_REGISTERS), 
				Integer.toString(firstRegister), Integer.toString(numRegisters)};
		
		// Iniciamos Comunicación
		InitModbusComunication(args, sn_Transport, modBusResponse, reg);
		
	}
	
	
	
	

	public static void InitModbusComunication(String[] args, CommTransport sCon, IModBusResponse modBusResponse,
			int[]... reg) {

		// Iniciamos Comunicación
				
		DefaultExecutorSupplier.getInstance().forBackgroundTasks().execute(new Runnable() {
			
			@Override
			public void run() {
				initCommunications();
			}
			
			public synchronized void initCommunications() {
				Modbus.InitComunication(args, sCon, reg);

				if (modBusResponse != null) {
					ModBusEvent e = new ModBusEvent(reg[0]);
					e.set_Args(args);

					modBusResponse.onModBusResponse(e);
				}
		    }
		});
	}	
}

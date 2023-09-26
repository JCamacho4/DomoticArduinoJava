package Base_COM_Serie;

import modbus.ModBusEvent;

public interface IModBusResponse {
	
	void onModBusResponse(final ModBusEvent mbe);

}

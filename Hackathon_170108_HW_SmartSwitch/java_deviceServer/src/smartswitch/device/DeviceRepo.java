package smartswitch.device;

import java.util.HashMap;
import java.util.Map;

/**
 * a (K,V) set to store the connected Device
 * @author fang
 *
 */
public class DeviceRepo{
	private static DeviceRepo _Instance = null;
	Map<String, Device> mMap = null;
	
	private DeviceRepo() {
		if(mMap == null)
			mMap = new HashMap<>();
	}
	public static synchronized DeviceRepo getInstance(){
		if(_Instance == null)
			_Instance = new DeviceRepo();
		return _Instance;
	}
	
	public synchronized void add(Device device) {
		if(device == null)
			return;
		mMap.put(device.getId(), device);
	}
	
	public synchronized Device remove(String deviceID){
		if(null == deviceID)
			return null;
		return mMap.remove(deviceID);
	}
	
	public Device findById(String deviceID){
		if(null == deviceID)
			return null;
		return mMap.get(deviceID);
	}
}

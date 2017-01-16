package smartswitch.device;

import smartswitch.device.DeviceMsg;
import smartswitch.device.DeviceService;

public class Test {
	public static void main(String[] args) throws InterruptedException {
		
		//test DeviceMsg Decode function
//		DeviceMsg deviceMsg = new DeviceMsg();
//		String msg = "hdu12345&pm25:100,temperature:14,humidity:24";
//		if(!deviceMsg.decode(msg))
//			System.out.println("failed decode");
//		System.out.println("decode success");
//		deviceMsg.toString();
		DeviceService.getInstance().startSmartSwitchService();
		while(true)
			Thread.sleep(5000);
	}
}

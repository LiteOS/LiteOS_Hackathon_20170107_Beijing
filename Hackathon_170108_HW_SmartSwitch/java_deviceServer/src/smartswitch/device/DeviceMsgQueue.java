package smartswitch.device;

import java.util.concurrent.LinkedBlockingQueue;

public class DeviceMsgQueue {
	private static int mCapacity;//max ele number of mQueue
	private LinkedBlockingQueue<DeviceMsg> mQueue = null;
	private static DeviceMsgQueue _instance;
	private DeviceMsgQueue(){
		if(mQueue == null){
			this.mQueue = new LinkedBlockingQueue<DeviceMsg>();
		}
		mCapacity = 255;
	}
	public static synchronized DeviceMsgQueue getInstance(){
		if(_instance != null)
			return _instance;
		return new DeviceMsgQueue();
	}
	
	public static DeviceMsgQueue getInstance(int capacity){
		mCapacity = capacity;
		return getInstance();
	}
	
	public void add(DeviceMsg msg){
		if(mQueue.size() > mCapacity)
			return;
		mQueue.add(msg);
	}
	
	public void put(DeviceMsg msg){
		if(mQueue.size() > mCapacity)
			return;
		try {
			mQueue.put(msg);
			System.out.println("add MsqQueue success");
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public DeviceMsg take(){
		//if(mQueue.size() == 0)
		//	return null;
		try {
			return mQueue.take();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	public DeviceMsg top(){
		if(mQueue.isEmpty())
			return null;
		else
			return mQueue.peek();
	}
	
	public DeviceMsg pop(){
		if(mQueue.isEmpty())
			return null;
		return mQueue.remove();
	}
	
	public int size(){
		return mQueue.size();
	}
}

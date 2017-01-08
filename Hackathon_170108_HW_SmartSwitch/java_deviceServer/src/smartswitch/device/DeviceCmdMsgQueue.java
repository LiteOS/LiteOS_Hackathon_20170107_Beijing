package smartswitch.device;

import java.util.concurrent.LinkedBlockingQueue;

public class DeviceCmdMsgQueue {
	private static DeviceCmdMsgQueue _Instance = null;
	private LinkedBlockingQueue<DeviceCmdMsg> mQueue=null; 
	private static int mCapacity;//max ele number of mQueue
	
	private DeviceCmdMsgQueue() {
		if(mQueue == null)
			mQueue = new LinkedBlockingQueue<>();
		mCapacity = 255;
	}
	
	public synchronized static DeviceCmdMsgQueue getInstance(){
		if(_Instance == null)
			_Instance = new DeviceCmdMsgQueue();
		return _Instance;
	}
	
	public static DeviceCmdMsgQueue getInstance(int capacity){
		mCapacity = capacity;
		return getInstance();
	}
	
	public void add(DeviceCmdMsg msg){
		if(mQueue.size() > mCapacity)
			return;
		mQueue.add(msg);
	}
	
	public void put(DeviceCmdMsg msg){
		//if(mQueue.size() > mCapacity)
		//	return;
		try {
			mQueue.put(msg);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public DeviceCmdMsg take(){
		//if(mQueue.size() > mCapacity)
		//	return null;
		try {
			return mQueue.take();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	public DeviceCmdMsg top(){
		if(mQueue.isEmpty())
			return null;
		else
			return mQueue.peek();
	}
	
	public DeviceCmdMsg pop(){
		if(mQueue.isEmpty())
			return null;
		return mQueue.remove();
	}
	
	public int size(){
		return mQueue.size();
	}
}

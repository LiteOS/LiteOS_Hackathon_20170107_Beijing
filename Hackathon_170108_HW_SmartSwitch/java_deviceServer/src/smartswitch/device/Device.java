package smartswitch.device;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import fang.io.ConnectCrashHandler;
import fang.io.ReadWriter;
import fang.io.StringMsgHandler;

/**
 * Device class
 * Device Object add itself into the DeviceRepo and 
 * 				 send DeviceMsg into the DeviceMsgQueue
 * 				 throug the StringMsgHandler
 * @author Administrator
 *
 */
public class Device implements StringMsgHandler,ConnectCrashHandler{
	private Socket mSocket = null;
	private DeviceMsg mDeviceMsg = null;
	private ReadWriter mReadWriter = null;
	private StringMsgHandler mMsgHandler=null;
	private ConnectCrashHandler mCrashHandler = null;
	private String id = null;
	
	//DeviceMsgQueue
	private DeviceMsgQueue msgQueue = null;
	//DeviceRepo
	private DeviceRepo mRepo = null;
	
	public Device(Socket socket) {
		this.mSocket = socket;
		setRecMsgHandler(this);
		setCrashHandler(this);
		msgQueue = DeviceMsgQueue.getInstance();
		mRepo = DeviceRepo.getInstance();
	}
	
	/**
	 * set the StringMsgHandler
	 * if the param of handler is null,the Object's hander is itself
	 * @param handler
	 */
	public void setRecMsgHandler(StringMsgHandler handler){
		if(handler == null)
			return;
		this.mMsgHandler = handler;
	}
	
	public void setCrashHandler(ConnectCrashHandler crashHandler) {
		mCrashHandler = crashHandler;
	}

	public void init(){
		InputStream inStream;
		OutputStream outStream;
		try{
		 inStream = mSocket.getInputStream();
		 outStream = mSocket.getOutputStream();
		 mReadWriter = new ReadWriter(inStream, outStream,this.mMsgHandler,this.mCrashHandler);
		 System.out.println("device init start");
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	public String getId(){
		return id;
	}
	
	/**
	 * send cmd to the device
	 * @param cmd
	 */
	public void send(String cmd){
		mReadWriter.write(cmd);
	}

	@Override
	public void HandleStringMsg(String msg) {
		DeviceMsg deviceMsg = new DeviceMsg();
		if(deviceMsg.decode(msg)){
			id = deviceMsg.getId();
			msgQueue.put(deviceMsg);
			System.out.println(deviceMsg.toString());
//			if(null == DeviceRepo.getInstance().findById(id)){
//				mRepo.add(this);
//				System.out.println("device:"+id+" success added into repo");
//			}
			mRepo.add(this);//will off-line reconnect
			System.out.println("device:"+id+" success added into repo");
		}
	}
	
	@Override
	public void HandleCrash(String CrashMsg) {
		if(this.id != null){
			mRepo.remove(this.id);
			System.out.println("device:"+id+" off-line");
		}
	}
}
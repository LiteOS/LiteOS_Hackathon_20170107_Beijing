package fang.io;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class ReadWriter implements CloseableInterface{
	WriteThread writeThread = null;
	ReadThread readThread = null;
	List<CloseableInterface> ThreadList = new LinkedList<CloseableInterface>();
	StringMsgHandler mReceiveMsgHandler = null;
	
	volatile boolean isActive = false;
	volatile boolean crashed = false;
	
	public ReadWriter(InputStream in) {
		readThread = new ReadThread(in);
		ThreadList.add(readThread);
		readThread.start();
	}
	
	public ReadWriter(InputStream in,StringMsgHandler receiveHandler){
		readThread = new ReadThread(in);
		readThread.setMsgHandler(receiveHandler);
	}
	
	public void setReceiveMsgHandler(StringMsgHandler recMsgHandler) {
		mReceiveMsgHandler = recMsgHandler;
		readThread.setMsgHandler(recMsgHandler);
	}
	
	public ReadWriter(OutputStream out) {
		writeThread = new WriteThread(out);
		ThreadList.add(writeThread);
		writeThread.start();
	}
	
	public ReadWriter(InputStream in,OutputStream out) {
		readThread = new ReadThread(in);
		writeThread = new WriteThread(out);
		ThreadList.add(readThread);ThreadList.add(writeThread);
		readThread.start();writeThread.start();
	}
	
	public ReadWriter(InputStream in,OutputStream out,StringMsgHandler recMsgHandler,ConnectCrashHandler crashHandler) {
		readThread = new ReadThread(in);
		if(null != recMsgHandler)
			this.setReceiveMsgHandler(recMsgHandler);
		writeThread = new WriteThread(out);
		//add crashed handler
		if(null != crashHandler){
			writeThread.setCrashHandler(crashHandler);
			readThread.setCrashed(crashed);
		}
		ThreadList.add(readThread);ThreadList.add(writeThread);
		readThread.start();writeThread.start();
	}
	
	public void write(String msg) {
		if(writeThread.isAlive())
			writeThread.write(msg);
	}
	
	public void writeLine(String msg){
		String newDate = msg+"\r\n";
		write(newDate);
	}
	
	@Override
	public void close() {
		for (CloseableInterface closeableInterface : ThreadList) {
			if(closeableInterface != null)
				closeableInterface.close();
		}
	}
	
	public boolean isCrashed(){
		if(null == writeThread && null == readThread)
			return false;
		else if(null == writeThread && null != readThread)
			return readThread.isCrashed();
		else if(null != writeThread && null == readThread)
			return writeThread.isCrashed();
		else {
			return writeThread.isCrashed()||readThread.isCrashed();
		}
	}
}

class WriteThread extends Thread implements CloseableInterface{
	Queue<String> MsgSendingQueue = new LinkedBlockingQueue<>(255);
	OutputStream outputStream;
	volatile boolean stop=false;
	volatile boolean crashed = false;
	ConnectCrashHandler mCrashHandler = null;
	private String CrashMsg = "crashed";
	
	public WriteThread(OutputStream out) {
		outputStream = out;
	}
	
	public WriteThread(OutputStream out,ConnectCrashHandler crashHandler) {
		this(out);
		this.mCrashHandler = crashHandler;
	}
	
	public void write(String msg) {
		synchronized (MsgSendingQueue) {
			MsgSendingQueue.add(msg);
			MsgSendingQueue.notifyAll();
		}
	}
	
	@Override
	public void close() {
		stop = true;
	}
	
	@Override
	public void run() {
		stop = false;
		while(!stop){
			synchronized (MsgSendingQueue) {
				try{
				if(MsgSendingQueue.isEmpty())
					MsgSendingQueue.wait();
				else {
					outputStream.write(MsgSendingQueue.remove().getBytes());
				}
				}catch(IOException e){
					setCrashed(true);
					if(mCrashHandler != null)
						mCrashHandler.HandleCrash(CrashMsg);
				}
				catch(Exception e){
					this.close();
					break;
				}
			}
		}
	}
	
	
	public void setCrashHandler(ConnectCrashHandler crashHandler) {
		mCrashHandler = crashHandler;
	}

	public boolean isCrashed() {
		return crashed;
	}

	public void setCrashed(boolean crashed) {
		this.crashed = crashed;
	}	
}

/**
 * TCP client thread for receive and display msg
 * @author fang
 *
 */
class ReadThread extends Thread implements CloseableInterface{
	InputStream inputStream=null;
	volatile boolean stop=false;
	volatile StringMsgHandler mMsgHandler = null;
	volatile boolean crashed = false;
	ConnectCrashHandler mCrashHandler = null;
	private String CrashMsg = "crashed";
	
	public ReadThread(InputStream in) {
		inputStream = in;
	}
	
	public ReadThread(InputStream in,ConnectCrashHandler crashHandler) {
		this(in);
		this.mCrashHandler = crashHandler;
	}
	
	public void setMsgHandler(StringMsgHandler msgHandler) {
		mMsgHandler = msgHandler;
	}
	
	@Override
	public void close() {
		stop = true;
	}
	
	@Override
	public void run() {
		stop = false;
		BufferedReader bf=new BufferedReader(new InputStreamReader(inputStream));
		while(!stop){
			try {
				if(bf == null){
					System.err.println("buffer reader is null");
					break;
				}
				String msg = bf.readLine();
				if(mMsgHandler != null){
					mMsgHandler.HandleStringMsg(msg);
				}else{
					if(msg !=null && !msg.isEmpty())
						System.out.println(msg);//display the msg
				}
			} catch (IOException e) {
				stop = true;
				setCrashed(true);
				if(mCrashHandler != null)
					mCrashHandler.HandleCrash(CrashMsg);
			}
		}
	}
	
	public void setCrashHandler(ConnectCrashHandler crashHandler) {
		mCrashHandler = crashHandler;
	}
	
	public boolean isCrashed() {
		return crashed;
	}

	public void setCrashed(boolean crashed) {
		this.crashed = crashed;
	}
}


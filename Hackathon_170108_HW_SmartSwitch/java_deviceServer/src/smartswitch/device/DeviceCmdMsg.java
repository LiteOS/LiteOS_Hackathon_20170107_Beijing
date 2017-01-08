package smartswitch.device;

/**
 * Command Message of the Device
 * @author Administrator
 *
 */
public class DeviceCmdMsg {
	private String id,cmd;
	public DeviceCmdMsg() {
		id = null;cmd = null;
	}
	
	public DeviceCmdMsg(String id,String cmd){
		this.id = id;this.cmd = cmd;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getCmd() {
		return cmd;
	}

	public void setCmd(String cmd) {
		this.cmd = cmd;
	}
}

package smartswitch.device;

public class DeviceCmd {
    //cmd type define
    public static final int CMD_Close = 0;
    public static final int CMD_Open = 1;
    public static final int CMD_Timer = 2;
    public static final int CMD_Unknow = 255;
    //cmd content
    public static final String OpenCmd = "open";
    public static final String CloseCmd = "close";
    public static final String TimerCmd = "timer";

    /**
     * get type of cmd
     * @param cmd
     * @return
     */
    public static int type(String cmd){
        int type = DeviceCmd.CMD_Unknow;
        if(CloseCmd.equals(cmd))
            return DeviceCmd.CMD_Close;
        else if(OpenCmd.equals(cmd))
            return DeviceCmd.CMD_Open;
        else if(TimerCmd.equals(cmd.substring(0, 5)))
            return DeviceCmd.CMD_Timer;
        return type;
    }

    private DeviceCmd(){}
}

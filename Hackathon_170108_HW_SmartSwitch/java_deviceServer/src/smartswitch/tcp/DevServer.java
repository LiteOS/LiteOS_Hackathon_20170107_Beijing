package smartswitch.tcp;


import smartswitch.device.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;


public class DevServer {

    private DeviceMsgQueue queue;
    private DeviceCmdMsgQueue cmdQueue;

    public static void main(String[] args) throws InterruptedException {
        new DevServer().start();
    }


    public void start() throws InterruptedException {

        DeviceService.getInstance().startSmartSwitchService();
        queue = DeviceMsgQueue.getInstance();
        cmdQueue = DeviceCmdMsgQueue.getInstance();

        new Thread(new HttpServer()).start();

        while(true) {

            //模拟添加到队列
            //DeviceMsg req_msg = new DeviceMsg();
            //req_msg.decode("deviceID=123&pm=233&humidity=32&temperature=12&status=1");
            //queue.put(req_msg);
            //Thread.sleep(1000);


            //从队列中获取数据
            DeviceMsg msg = queue.take();
            //发送到web端
            String url = "http://192.168.0.101:3000/saveDevices";
            String req_args = msg.toString();
            HttpClient httpClient = new HttpClient();
            System.out.println(httpClient.post(url, req_args));

            //Thread.sleep(2000);
        }
    }

    class HttpServer implements Runnable {
        private int port = 6666;

        @Override
        public void run() {

            while(true) {
                PrintWriter writer = null;
                try(ServerSocket server = new ServerSocket(port);
                    Socket socket = server.accept();
                    BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {
                    writer = new PrintWriter(socket.getOutputStream());

                    //获取web端发来的控制消息
                    String cmdMsg = in.readLine();
                    System.out.println("Client:" + cmdMsg);

                    //控制消息解析
                    String[] pars = cmdMsg.split("&");
                    String id = "";
                    String scmd = "";
                    for(String kvs : pars) {
                        String[] kv = kvs.split("=");
                        if(kv[0].contains("deviceID")) {
                            id = kv[1];
                        }
                        if(kv[0].contains("status")) {
                            if(kv[1].equals("0")) {
                                scmd = "close";
                            } else if(kv[1].equals("1")) {
                                scmd = "open";
                            } else {
                                scmd = kv[1];
                            }
                        }
                    }

                    //消息添加到控制队列
                    DeviceCmdMsg cmd = new DeviceCmdMsg(id, scmd);
                    cmdQueue.put(cmd);

                    //向web端返回成功
                    writer.println("{\"status\":\"1\",\"msg\":\"success\"}");
                    writer.flush();

                } catch(IOException e) {
                    writer.println("{\"status\":\"0\",\"msg\":\"failed\"}");
                    writer.flush();
                }
            }
        }
    }
}



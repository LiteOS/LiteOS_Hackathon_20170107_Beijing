package smartswitch.tcp;

import java.io.*;
import java.net.Socket;
import java.nio.CharBuffer;


public class DevClient {

    private String host = "192.168.0.105";
    private int port = 8989;

    /**
     * 数据发送线程
     */
    class SendThread implements Runnable {
        private Socket socket;

        public SendThread(Socket socket) {
            this.socket = socket;
        }

        public void run() {
            while(true) {
                try {
                    PrintWriter pw = new PrintWriter(new OutputStreamWriter(
                            socket.getOutputStream()));
                    Thread.sleep(3000);
                    pw.write("deviceID=123&pm=233&humidity=32&temperature=12&status=1");
                    pw.flush();

                } catch(Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * 数据接收线程
     */
    class ReceiveThread implements Runnable {
        private Socket socket;

        public ReceiveThread(Socket socket) {
            this.socket = socket;
        }

        public void run() {
            while(true) {
                try {
                    Reader reader = new InputStreamReader(
                            socket.getInputStream());
                    CharBuffer charBuffer = CharBuffer.allocate(8192);
                    @SuppressWarnings("unused")
                    int charIndex = -1;
                    while((charIndex = reader.read(charBuffer)) != -1) {
                        charBuffer.flip();
                        System.out.println("client--> " + charBuffer.toString());
                    }
                } catch(IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void start() {
        try {
            Socket socket = new Socket(host, port);// 创建Socket
            new Thread(new SendThread(socket)).start();// 启动读线程
            //new Thread(new ReceiveThread(socket)).start();// 启动收线程
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        new DevClient().start();
    }

}
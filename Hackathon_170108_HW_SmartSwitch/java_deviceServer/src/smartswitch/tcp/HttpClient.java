package smartswitch.tcp;

import java.io.*;
import java.net.URL;
import java.net.URLConnection;

public class HttpClient {
    public static void main(String[] args) {
        HttpClient httpClient = new HttpClient();
        String url = "http://192.168.0.101:3000/saveDevices";
        //String args = genArgs(deviceMsg);
        String req_args = "deviceID=123&pm=233&temperature=12&humidity=32&status=1";

        String s = httpClient.post(url, req_args);
        System.out.println(s);
    }

    public static String post(String reqURL, String args) {
        try {

            URL url = new URL(reqURL);
            URLConnection connection = url.openConnection();
            connection.setDoOutput(true);
            OutputStreamWriter out = new OutputStreamWriter(connection.getOutputStream(), "UTF-8");
            out.write(args); // 向页面传递数据。post的关键所在！
            out.flush();
            out.close();
            // 一旦发送成功，用以下方法就可以得到服务器的回应：
            String sCurrentLine;
            String sTotalString = "";
            InputStream urlStream = connection.getInputStream();
            // 传说中的三层包装阿！
            BufferedReader reader = new BufferedReader(new InputStreamReader(urlStream));
            while((sCurrentLine = reader.readLine()) != null) {
                sTotalString += sCurrentLine + "\r\n";
            }
            return sTotalString;
        } catch(IOException e) {
            System.out.println(e);
        }
        return "\r\n";
    }


}

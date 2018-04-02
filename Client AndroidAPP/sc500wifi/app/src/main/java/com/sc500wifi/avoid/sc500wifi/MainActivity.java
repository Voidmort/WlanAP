package com.sc500wifi.avoid.sc500wifi;

import android.os.StrictMode;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import java.io.IOException;
import java.net.Socket;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.EditText;

import java.io.*;

public class MainActivity extends AppCompatActivity {

    //private String serverName = "192.168.1.176";//目标IP
    private String serverName = "192.168.42.1";//目标IP
    private int port = 666;//端口

    private EditText ssid, password;
    private TextView resurt;
    private String mseeages;
    @Override

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn = (Button)findViewById(R.id.button);

        ssid = (EditText)findViewById(R.id.editText1);
        password = (EditText)findViewById(R.id.editText2);
        resurt = (TextView)findViewById(R.id.textView2);

        //主线程通信
        StrictMode.ThreadPolicy policy=new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        btn .setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try
                {
                    System.out.println("连接到主机：" + serverName + " ，端口号：" + port);
                    Socket client = new Socket(serverName, port);
                    System.out.println("远程主机地址：" + client.getRemoteSocketAddress());

                    OutputStream outToServer = client.getOutputStream();
                    DataOutputStream out = new DataOutputStream(outToServer);

                    //发送的字符串
                    mseeages = ssid.getText().toString() + "\n" + password.getText().toString();

                    out.write(mseeages.getBytes("utf-8"));

                    InputStream inFromServer = client.getInputStream();
                    DataInputStream in = new DataInputStream(inFromServer);

                    String restr = in.readLine();
                    System.out.println("服务器响应： " + restr);
                    resurt.setText(restr);
                    client.close();

                }catch(IOException e)
                {
                    e.printStackTrace();
                }
           }
       });
    }
}

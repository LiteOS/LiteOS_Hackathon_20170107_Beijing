package com.piaoxue.weixins;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Shader;
import android.graphics.SweepGradient;
import android.graphics.drawable.AnimationDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.text.method.ScrollingMovementMethod;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.LinearInterpolator;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener {
	Context context;
	private final static int REQUEST_CONNECT_DEVICE = 1; // 宏定义查询设备句柄
	
	//private String SERVER_HOST_IP = "192.168.4.1";
	//private int SERVER_HOST_PORT = 8080;
	private String IP_PORT="192.168.31.140:8080";
	//存储 IP 地址  
	SharedPreferences preferences;
	SharedPreferences.Editor editor;
	private Socket socket;
	private PrintStream output;
	
	private int mInitialColor;//初始颜色
    private OnColorChangedListener mListener;
	
	private Button startButton;	
	private EditText IPText;
	private Button sendButtonClient;
	
	private EditText editMsgText, editMsgTextCilent;
	
	private boolean isConnecting = false;
	private Thread mThreadClient = null;	
	private Socket mSocketClient = null;
	static BufferedReader mBufferedReaderClient	= null;
	static PrintWriter mPrintWriterClient = null;
	
	private TextView recvText;
	private  String recvMessageClient = "";
	private  String recvMessageServer = "";
	//颜色选择器
	private ColorPickerDialog dialog;
    private TextView tvTextR;
    private TextView tvTextG;
    private TextView tvTextB;
    public  int int_color;
    public  int int_color_A;
    public  int int_color_R;
    public  int int_color_G;
    public  int int_color_B;
    public String color_S_R;
    public String color_S_G;
    public String color_S_B;
    //所有LED  编码
    public String ceshiLEDON  = "L\r\n";
    public String ceshiLEDOFF = "l\r\n";
    public String ceshiLightON = "M\r\n";
    public String ceshiLightOFF ="m\r\n";
    
    public String cheMoveOn     ="A\r\n";
    public String cheMoveBack   ="B\r\n";
    public String cheMoveLeft   ="C\r\n";
    public String cheMoveRight  ="D\r\n";
    public String cheStop       ="a\r\n";
    
    public String LED1ON = "PFL11\r\n";
    public String LED1OFF = "PFL10\r\n";
    public String LED2ON = "PFL21\r\n";
    public String LED2OFF = "PFL20\r\n";
    public String LED3ON = "PFL31\r\n";
    public String LED3OFF = "PFL30\r\n";
    public String LED4ON = "PFL41\r\n";
    public String LED4OFF = "PFL40\r\n";
    public String LED5ON = "PFL51\r\n";
    public String LED5OFF = "PFL50\r\n";
    public String LED6ON = "PFL61\r\n";
    public String LED6OFF = "PFL60\r\n";
    //电视机切换频道
    public String TV_Switch_on = "PSTr0";
    public String TV_Switch_up = "PSTr0";
    public String TV_Switch_down = "PSTl0";
    
    public String Switch_up = "PSC1";
    public String Switch_down = "PSC0";
    
    public String Zigbee_chuanglian_kai="ZL0";
    public String Zigbee_chuanglian_guan="ZL1";
    public String Zigbee_chuanglian_zidong="ZL2";
    
    public String WIFI_fengshan_kai="WF0";
    public String WIFI_fengshan_guan="WF1";
    
    public String ceshiGRB = "GRB";//WDHGRB
    public String G_00 = "00";
    public String G_0 = "0";
    public String R_00 = "00";
    public String R_0 = "0";
    public String B_00 = "00";
    public String B_0 = "0";
    public String G_bai;
    public String R_bai;
    public String B_bai;
	//按钮动画
	ImageView rocketImage_chuanglian=null;
	AnimationDrawable rocketAnimation; 
	AnimationDrawable rocketAnimation_chuanglian; 
	AnimationDrawable rocketAnimation_signal;
	Animation anim;
	Animation anim2;
	private int flag_anim_fengshan=1;
	private int flag_anim_chuanglian=1;
	private int flag_chazuo=1;
	// **************************BlueTooth***********************************
	private BluetoothAdapter mBluetoothAdapter = null;
	private BluetoothSocket btSocket = null;
	private OutputStream outStream = null;
	private static final UUID MY_UUID = UUID
			.fromString("00001101-0000-1000-8000-00805F9B34FB"); // 这条是蓝牙串口通用的UUID，不要更改
	// <==要连接的蓝牙设备MAC地址机器人
	private static String address = "20:13:01:30:05:56";
	// *************************************************************
	private ViewPager mViewPager;
	private PagerAdapter mAdapter;
	private List<View> mViews = new ArrayList<View>();


	private LinearLayout tab_blub1;
	private LinearLayout tab_blub2;
	private LinearLayout tab_blub3;
	private LinearLayout tab_blub4;


	private ImageButton bulb1_img;
	private ImageButton bulb2_img;
	private ImageButton bulb3_img;
	private ImageButton bulb4_img;
	private ImageButton chazuo_img;
	private TextView bulb1_txt;
	private TextView bulb2_txt;
	private int flag_bulb1 = 1;
	private int flag_bulb2 = 1;
	private int flag_bulb3 = 1;
	private int flag_bulb4 = 1;

	private LinearLayout root ;

	public void toastText(String message) {
		Toast.makeText(this, message, Toast.LENGTH_LONG).show();
	}
	public void handleException(Exception e, String prefix) {
		e.printStackTrace();
		toastText(prefix + e.toString());
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		context = this;
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);// 将标题栏去掉


		setContentView(R.layout.activity_main);
		//这段代码看不懂
		 StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder()        
	        .detectDiskReads()        
	        .detectDiskWrites()        
	        .detectNetwork()   // or .detectAll() for all detectable problems       
	        .penaltyLog()        
	        .build());        
	        StrictMode.setVmPolicy(new StrictMode.VmPolicy.Builder()        
	        .detectLeakedSqlLiteObjects()     
	        .penaltyLog()        
	        .penaltyDeath()        
	        .build()); 
	        
	      
	      

		
        //获取窗口管理器
        //WindowManager windowManager = getWindowManager();
        //Display display = windowManager.getDefaultDisplay();
        //DisplayMetrics metrics = new DisplayMetrics();
        
        //获取屏幕的宽和高
       // display.getMetrics(metrics);
        
       // int height = metrics.heightPixels +40 ;
       // int width = metrics.widthPixels ;
	    root = (LinearLayout) findViewById(R.id.Linear_mylayout);
	    root.setBackgroundColor(0xffffff);
       final ColorPickerView  myView = new ColorPickerView(this, 800,600);
       root.addView(myView);
        
        //setContentView(myView);
		InitView();// 界面初始化
		InitEvents();
	}
	
	
    private class ColorPickerView extends View {
    	private Paint mPaint;//渐变色环画笔
    	private Paint mCenterPaint;//中间圆画笔
    	private Paint mLinePaint;//分隔线画笔
    	private Paint mRectPaint;//渐变方块画笔
    	
    	private Shader rectShader;//渐变方块渐变图像
    	private float rectLeft;//渐变方块左x坐标
    	private float rectTop;//渐变方块上y坐标
    	private float rectRight;//渐变方块右x坐标
    	private float rectBottom;//渐变方块下y坐标
        
    	private final int[] mCircleColors;//渐变色环颜色
    	private final int[] mRectColors;//渐变方块颜色
    	
    	private int mHeight;//View高
    	private int mWidth;//View宽
    	private float r;//色环半径(paint中部)
    	private float centerRadius;//中心圆半径
    	
    	private boolean downInCircle = true;//按在渐变环上
    	private boolean downInRect;//按在渐变方块上
    	private boolean highlightCenter;//高亮
    	private boolean highlightCenterLittle;//微亮
    	
		public ColorPickerView(Context context, int height, int width) {
			super(context);
			this.mHeight = height - 36;
			this.mWidth = width;
			setMinimumHeight(height - 36);
			setMinimumWidth(width);
			
			//渐变色环参数
	    	mCircleColors = new int[] {0xFFFF0000, 0xFFFF00FF, 0xFF0000FF, 
	    			0xFF00FFFF, 0xFF00FF00,0xFFFFFF00, 0xFFFF0000};
	    	Shader s = new SweepGradient(0, 0, mCircleColors, null);
            mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
            mPaint.setShader(s);
            mPaint.setStyle(Paint.Style.STROKE);
            mPaint.setStrokeWidth(50);
            r = width / 2 * 0.7f - mPaint.getStrokeWidth() * 0.5f;
            
          //中心圆参数
            mCenterPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
            mCenterPaint.setColor(mInitialColor);
            mCenterPaint.setStrokeWidth(5);
            centerRadius = (r - mPaint.getStrokeWidth() / 2 ) * 0.7f;
            
          //边框参数
            mLinePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
            mLinePaint.setColor(Color.parseColor("#72A1D1"));
            mLinePaint.setStrokeWidth(4);
            
          //黑白渐变参数
            mRectColors = new int[]{0xFF000000, mCenterPaint.getColor(), 0xFFFFFFFF};
            mRectPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
            mRectPaint.setStrokeWidth(5);
            rectLeft = -r - mPaint.getStrokeWidth() * 0.8f;
            rectTop = r + mPaint.getStrokeWidth() * 0.8f + 
            mLinePaint.getStrokeMiter() * 0.8f + 15;
            rectRight = r + mPaint.getStrokeWidth() * 0.8f;
            rectBottom = rectTop + 50;
		}

		@Override
		protected void onDraw(Canvas canvas) {
			//移动中心
            canvas.translate(mWidth / 2, mHeight / 2 - 50);
            //画中心圆
            canvas.drawCircle(0, 0, centerRadius,  mCenterPaint);
            //是否显示中心圆外的小圆环
            if (highlightCenter || highlightCenterLittle) {
                int c = mCenterPaint.getColor();
                mCenterPaint.setStyle(Paint.Style.STROKE);
                if(highlightCenter) {
                	mCenterPaint.setAlpha(0xFF);
                }else if(highlightCenterLittle) {
                	mCenterPaint.setAlpha(0x90);
                }
                canvas.drawCircle(0, 0, 
                		centerRadius + mCenterPaint.getStrokeWidth(),  mCenterPaint);
                
                mCenterPaint.setStyle(Paint.Style.FILL);
                mCenterPaint.setColor(c);
            }
            //画色环
            canvas.drawOval(new RectF(-r, -r, r, r), mPaint);
          //画黑白渐变块
            if(downInCircle) {
            	mRectColors[1] = mCenterPaint.getColor();
            }
            rectShader = new LinearGradient(rectLeft, 0, rectRight, 0, mRectColors, null, Shader.TileMode.MIRROR);
            mRectPaint.setShader(rectShader);
            canvas.drawRect(rectLeft, rectTop, rectRight, rectBottom, mRectPaint);
            float offset = mLinePaint.getStrokeWidth() / 2;
            canvas.drawLine(rectLeft - offset, rectTop - offset * 2, 
            		rectLeft - offset, rectBottom + offset * 2, mLinePaint);//左
            canvas.drawLine(rectLeft - offset * 2, rectTop - offset, 
            		rectRight + offset * 2, rectTop - offset, mLinePaint);//上
            canvas.drawLine(rectRight + offset, rectTop - offset * 2, 
            		rectRight + offset, rectBottom + offset * 2, mLinePaint);//右
            canvas.drawLine(rectLeft - offset * 2, rectBottom + offset, 
            		rectRight + offset * 2, rectBottom + offset, mLinePaint);//下
			super.onDraw(canvas);
		}
		
		@Override
		public boolean onTouchEvent(MotionEvent event) {
			float x = event.getX() - mWidth / 2;
            float y = event.getY() - mHeight / 2 + 50;
            boolean inCircle = inColorCircle(x, y, 
            		r + mPaint.getStrokeWidth() / 2, r - mPaint.getStrokeWidth() / 2);
            boolean inCenter = inCenter(x, y, centerRadius);
            boolean inRect = inRect(x, y);

            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                	downInCircle = inCircle;
                	downInRect = inRect;
                	highlightCenter = inCenter;
                	break;
                case MotionEvent.ACTION_MOVE:
                	if(downInCircle && inCircle) {//down按在渐变色环内, 且move也在渐变色环内
                		float angle = (float) Math.atan2(y, x);
                        float unit = (float) (angle / (2 * Math.PI));
                        if (unit < 0) {
                            unit += 1;
                        }
	               		mCenterPaint.setColor(interpCircleColor(mCircleColors, unit));
	               		
	               		//在这里发送颜色
	    				int_color=interpCircleColor(mCircleColors, unit);
	    				int_color_R=((int_color&0x00FF0000)>>16);
	    				int_color_G=((int_color&0x0000FF00)>>8);
	    				int_color_B=(int_color&0x000000FF);
	    				tvTextR.setTextColor(int_color);
	    				tvTextG.setTextColor(int_color);
	    				tvTextB.setTextColor(int_color);
	    				//System.out.println(int_color);
	    				color_S_R=Integer.toString(int_color_R);
	    				color_S_G=Integer.toString(int_color_G);
	    				color_S_B=Integer.toString(int_color_B);
	    				tvTextR.setText(color_S_R);
	    				tvTextG.setText(color_S_G);
	    				tvTextB.setText(color_S_B);
	    				
	    				
	    				if((int_color_G>=0)&&(int_color_G<10))
	    				{
	    					G_bai=G_00.concat(color_S_G);
	    				}
	    				else if((int_color_G>=10)&&(int_color_G<100))
	    				{
	    					G_bai=G_0.concat(color_S_G);
	    				}
	    				else 
	    				{
	    					G_bai=color_S_G;
	    				} //Green
	    				
	    				
	    				if((int_color_R<10)&&(int_color_R>=0))
	    				{
	    					R_bai=R_00.concat(color_S_R);
	    				}
	    				else if((int_color_R>=10)&&(int_color_R<100))
	    				{
	    					R_bai=R_0.concat(color_S_R);
	    				}
	    				else 
	    				{
	    					R_bai=color_S_R;
	    				}//Red
	    				
	    				
	    				
	    				if((int_color_B<10)&&(int_color_B>=0))
	    				{
	    					B_bai=B_00.concat(color_S_B);
	    				}
	    				else if((int_color_B>=10)&&(int_color_B<100))
	    				{
	    					B_bai=B_0.concat(color_S_B);
	    				}
	    				else 
	    				{
	    					B_bai=color_S_B;
	    				}
	    				String send_G = ceshiGRB.concat(G_bai);
	    				String send_GR = send_G.concat(R_bai);
	    				String seng_GRB = send_GR.concat(B_bai);
	    				seng_GRB = seng_GRB.concat("\r\n");
	    				if(isConnecting)
	    				{
		    				try 
		    				{				    	
		    			    	mPrintWriterClient.print(seng_GRB);//发送给服务器
		    			    	mPrintWriterClient.flush();
		    				}
		    				catch (Exception e) 
		    				{
		    					// TODO: handle exception
		    					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
		    				}
	    				}
	    				//root.setBackgroundColor(interpCircleColor(mCircleColors, unit));
	    				
	               		//this.setBackgroundColor(interpCircleColor(mCircleColors, unit));
	               		//getWindow().setBackgroundDrawableResource(interpCircleColor(mCircleColors, unit));
                	}else if(downInRect && inRect) {//down在渐变方块内, 且move也在渐变方块内
                		mCenterPaint.setColor(interpRectColor(mRectColors, x));
	    				int_color=interpRectColor(mRectColors, x);
	    				
	    				int_color_R=((int_color&0x00FF0000)>>16);
	    				int_color_G=((int_color&0x0000FF00)>>8);
	    				int_color_B=(int_color&0x000000FF);
	    				tvTextR.setTextColor(int_color);
	    				tvTextG.setTextColor(int_color);
	    				tvTextB.setTextColor(int_color);
	    				//System.out.println(int_color);
	    				color_S_R=Integer.toString(int_color_R);
	    				color_S_G=Integer.toString(int_color_G);
	    				color_S_B=Integer.toString(int_color_B);
	    				tvTextR.setText(color_S_R);
	    				tvTextG.setText(color_S_G);
	    				tvTextB.setText(color_S_B);
	    				
	    				
	    				
	    				if((int_color_G>=0)&&(int_color_G<10))
	    				{
	    					G_bai=G_00.concat(color_S_G);
	    				}
	    				else if((int_color_G>=10)&&(int_color_G<100))
	    				{
	    					G_bai=G_0.concat(color_S_G);
	    				}
	    				else 
	    				{
	    					G_bai=color_S_G;
	    				} //Green
	    				
	    				
	    				if((int_color_R<10)&&(int_color_R>=0))
	    				{
	    					R_bai=R_00.concat(color_S_R);
	    				}
	    				else if((int_color_R>=10)&&(int_color_R<100))
	    				{
	    					R_bai=R_0.concat(color_S_R);
	    				}
	    				else 
	    				{
	    					R_bai=color_S_R;
	    				}//Red
	    				
	    				
	    				
	    				if((int_color_B<10)&&(int_color_B>=0))
	    				{
	    					B_bai=B_00.concat(color_S_B);
	    				}
	    				else if((int_color_B>=10)&&(int_color_B<100))
	    				{
	    					B_bai=B_0.concat(color_S_B);
	    				}
	    				else 
	    				{
	    					B_bai=color_S_B;
	    				}
	    				String send_G = ceshiGRB.concat(G_bai);
	    				String send_GR = send_G.concat(R_bai);
	    				String seng_GRB = send_GR.concat(B_bai);
	    				seng_GRB = seng_GRB.concat("\r\n");
	    				if(isConnecting)
	    				{
		    				try 
		    				{				    	
		    			    	mPrintWriterClient.print(seng_GRB);//发送给服务器
		    			    	mPrintWriterClient.flush();
		    				}
		    				catch (Exception e) 
		    				{
		    					// TODO: handle exception
		    					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
		    				}
	    				}
	    				//root.setBackgroundColor(interpRectColor(mRectColors, x));
	    				
                		//this.setBackgroundColor(interpRectColor(mRectColors, x));
                		//getWindow().setBackgroundDrawableResource(interpRectColor(mRectColors, x));
                	}
                	if((highlightCenter && inCenter) || (highlightCenterLittle && inCenter)) {//点击中心圆, 当前移动在中心圆
                		highlightCenter = true;
                		highlightCenterLittle = false;
                	} else if(highlightCenter || highlightCenterLittle) {//点击在中心圆, 当前移出中心圆
                		highlightCenter = false;
                		highlightCenterLittle = true;
                	} else {
                		highlightCenter = false;
                		highlightCenterLittle = false;
                	}
                   	invalidate();
                	break;
                case MotionEvent.ACTION_UP:
                	if(highlightCenter && inCenter) {//点击在中心圆, 且当前启动在中心圆
                		if(mListener != null) {
                			mListener.colorChanged(mCenterPaint.getColor());
                			
                			int_color = mCenterPaint.getColor();

    	    				int_color_R=((int_color&0x00FF0000)>>16);
    	    				int_color_G=((int_color&0x0000FF00)>>8);
    	    				int_color_B=(int_color&0x000000FF);
    	    				tvTextR.setTextColor(int_color);
    	    				tvTextG.setTextColor(int_color);
    	    				tvTextB.setTextColor(int_color);
    	    				//System.out.println(int_color);
    	    				color_S_R=Integer.toString(int_color_R);
    	    				color_S_G=Integer.toString(int_color_G);
    	    				color_S_B=Integer.toString(int_color_B);
    	    				tvTextR.setText(color_S_R);
    	    				tvTextG.setText(color_S_G);
    	    				tvTextB.setText(color_S_B);
            				if((int_color_G>=0)&&(int_color_G<10))
            				{
            					G_bai=G_00.concat(color_S_G);
            				}
            				else if((int_color_G>=10)&&(int_color_G<100))
            				{
            					G_bai=G_0.concat(color_S_G);
            				}
            				else 
            				{
            					G_bai=color_S_G;
            				} //Green
            				
            				
            				if((int_color_R<10)&&(int_color_R>=0))
            				{
            					R_bai=R_00.concat(color_S_R);
            				}
            				else if((int_color_R>=10)&&(int_color_R<100))
            				{
            					R_bai=R_0.concat(color_S_R);
            				}
            				else 
            				{
            					R_bai=color_S_R;
            				}//Red
            				
            				
            				
            				if((int_color_B<10)&&(int_color_B>=0))
            				{
            					B_bai=B_00.concat(color_S_B);
            				}
            				else if((int_color_B>=10)&&(int_color_B<100))
            				{
            					B_bai=B_0.concat(color_S_B);
            				}
            				else 
            				{
            					B_bai=color_S_B;
            				}
            				String send_G = ceshiGRB.concat(G_bai);
            				String send_GR = send_G.concat(R_bai);
            				String seng_GRB = send_GR.concat(B_bai);
            				seng_GRB = seng_GRB.concat("\r\n");
            				if(isConnecting)
            				{
        	    				try 
        	    				{				    	
        	    			    	mPrintWriterClient.print(seng_GRB);//发送给服务器
        	    			    	mPrintWriterClient.flush();
        	    				}
        	    				catch (Exception e) 
        	    				{
        	    					// TODO: handle exception
        	    					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
        	    				}
            				}
                		}
                	}
                	if(downInCircle) {
                		downInCircle = false;
                	}
                	if(downInRect) {
                		downInRect = false;
                	}
                	if(highlightCenter) {
                		highlightCenter = false;
                	}
                	if(highlightCenterLittle) {
                		highlightCenterLittle = false;
                	}
                	invalidate();
                    break;
            }
            return true;
		}

		@Override
		protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
			super.onMeasure(mWidth, mHeight);
		}

		/**
         * 坐标是否在色环上
         * @param x 坐标
         * @param y 坐标
         * @param outRadius 色环外半径
         * @param inRadius 色环内半径
         * @return
         */
		private boolean inColorCircle(float x, float y, float outRadius, float inRadius) {
			double outCircle = Math.PI * outRadius * outRadius;
			double inCircle = Math.PI * inRadius * inRadius;
			double fingerCircle = Math.PI * (x * x + y * y);
			if(fingerCircle < outCircle && fingerCircle > inCircle) {
				return true;
			}else {
				return false;
			}
		}
		
		 /**
         * 坐标是否在中心圆上
         * @param x 坐标
         * @param y 坐标
         * @param centerRadius 圆半径
         * @return
         */
		private boolean inCenter(float x, float y, float centerRadius) {
			double centerCircle = Math.PI * centerRadius * centerRadius;
			double fingerCircle = Math.PI * (x * x + y * y);
			if(fingerCircle < centerCircle) {
				return true;
			}else {
				return false;
			}
		}
		
		/**
         * 坐标是否在渐变色中
         * @param x
         * @param y
         * @return
         */
		private boolean inRect(float x, float y) {
			if( x <= rectRight && x >=rectLeft && y <= rectBottom && y >=rectTop) {
				return true;
			} else {
				return false;
			}
		}
		
		 /**
         * 获取圆环上颜色
         * @param colors
         * @param unit
         * @return
         */
		private int interpCircleColor(int colors[], float unit) {
            if (unit <= 0) {
                return colors[0];
            }
            if (unit >= 1) {
                return colors[colors.length - 1];
            }
            
            float p = unit * (colors.length - 1);
            int i = (int)p;
            p -= i;

            // now p is just the fractional part [0...1) and i is the index
            int c0 = colors[i];
            int c1 = colors[i+1];
            int a = ave(Color.alpha(c0), Color.alpha(c1), p);
            int r = ave(Color.red(c0), Color.red(c1), p);
            int g = ave(Color.green(c0), Color.green(c1), p);
            int b = ave(Color.blue(c0), Color.blue(c1), p);
            
            
            return Color.argb(a, r, g, b);//获取到 R G B 和  不透明度
            
        }
		
		/**
         * 获取渐变块上颜色
         * @param colors
         * @param x
         * @return
         */
		private int interpRectColor(int colors[], float x) {
			int a, r, g, b, c0, c1;
        	float p;
        	if (x < 0) {
        		c0 = colors[0]; 
        		c1 = colors[1];
        		p = (x + rectRight) / rectRight;
        	} else {
        		c0 = colors[1];
        		c1 = colors[2];
        		p = x / rectRight;
        	}
        	a = ave(Color.alpha(c0), Color.alpha(c1), p);
        	r = ave(Color.red(c0), Color.red(c1), p);
        	g = ave(Color.green(c0), Color.green(c1), p);
        	b = ave(Color.blue(c0), Color.blue(c1), p);
        	
        	
        	
        	return Color.argb(a, r, g, b);
		}
		
		private int ave(int s, int d, float p) {
            return s + Math.round(p * (d - s));
        }
    }
    
    /**
     * 回调接口
     * @author <a href="clarkamx@gmail.com">LynK</a>
     * 
     * Create on 2012-1-6 上午8:21:05
     *
     */
    public interface OnColorChangedListener {
    	/**
         * 回调函数
         * @param color 选中的颜色
         */
        void colorChanged(int color);
    }
	private void InitEvents() {

		// mTabmusic.setOnClickListener(this);
		// mTabplace.setOnClickListener(this);
		// mTabfrd.setOnClickListener(this);
		// mTabmsg.setOnClickListener(this);

		tab_blub1.setOnClickListener(this);
		tab_blub2.setOnClickListener(this);
		tab_blub3.setOnClickListener(this);
		tab_blub4.setOnClickListener(this);

		
		//客户端连接
		startButton.setOnClickListener(this);
		sendButtonClient.setOnClickListener(this);
		
		findViewById(R.id.id_chuanglian).setOnClickListener(this);
		findViewById(R.id.id_fengshan).setOnClickListener(this);
		findViewById(R.id.id_chazuo).setOnClickListener(this);
		findViewById(R.id.id_RGBLED).setOnClickListener(this);
	}

	private void InitView() {

		mViewPager = (ViewPager) findViewById(R.id.id_viewpager);

		// mTabmusic = (LinearLayout) findViewById(R.id.id_tab_light);
		// mTabplace = (LinearLayout) findViewById(R.id.id_tab_place);
		// mTabfrd = (LinearLayout) findViewById(R.id.id_tab_frd);
		// mTabmsg = (LinearLayout) findViewById(R.id.id_tab_msg);
		tab_blub1 = (LinearLayout) findViewById(R.id.id_blub1);
		tab_blub2 = (LinearLayout) findViewById(R.id.id_blub2);
		tab_blub3 = (LinearLayout) findViewById(R.id.id_blub3);
		tab_blub4 = (LinearLayout) findViewById(R.id.id_blub4);


		// mMusic_img = (ImageButton) findViewById(R.id.id_tab_light_img);//底端按钮
		// mPlace_img = (ImageButton) findViewById(R.id.id_tab_place_img);
		// mFrd_img = (ImageButton) findViewById(R.id.id_tab_frd_img);
		// mMsg_img = (ImageButton) findViewById(R.id.id_tab_msg_img);

		bulb1_img = (ImageButton) findViewById(R.id.id_bulb1_img);
		bulb2_img = (ImageButton) findViewById(R.id.id_bulb2_img);
		bulb3_img = (ImageButton) findViewById(R.id.id_bulb3_img);
		bulb4_img = (ImageButton) findViewById(R.id.id_bulb4_img);


		chazuo_img =(ImageButton) findViewById(R.id.id_chazuo_img);


		bulb1_txt = (TextView) findViewById(R.id.id_bulb1_txt);
		bulb2_txt = (TextView) findViewById(R.id.id_bulb2_txt);
		tvTextR = (TextView) findViewById(R.id.tv_text_R);
		tvTextG = (TextView) findViewById(R.id.tv_text_G);
		tvTextB = (TextView) findViewById(R.id.tv_text_B);
		rocketImage_chuanglian=(ImageView)findViewById(R.id.id_chuanglian_img);
		LayoutInflater mInflater = LayoutInflater.from(this);
		anim = AnimationUtils.loadAnimation(this, R.anim.round_loading);
		anim2 = AnimationUtils.loadAnimation(this, R.anim.round_loading2_1);
		rocketImage_chuanglian.setBackgroundResource(R.anim.myanimchuanglian);
		rocketAnimation_chuanglian = (AnimationDrawable) rocketImage_chuanglian.getBackground();
		LinearInterpolator lin = new LinearInterpolator();//线性变化
		anim.setInterpolator(lin);
		anim2.setInterpolator(lin);
		findViewById(R.id.id_fengshan_img).startAnimation(anim);
		findViewById(R.id.id_RGBLED_img).startAnimation(anim);
		IPText= (EditText)findViewById(R.id.IPText);
		
		
		preferences = getSharedPreferences("ipporttext", MODE_PRIVATE);
		editor=preferences.edit();
		
		IP_PORT = preferences.getString("ipporttext", "192.168.31.140:8080");
		IPText.setText(IP_PORT);
		startButton= (Button)findViewById(R.id.StartConnect);
		
		editMsgTextCilent= (EditText)findViewById(R.id.clientMessageText);	   
        editMsgTextCilent.setText("Send Messages");
        
        sendButtonClient= (Button)findViewById(R.id.SendButtonClient);
        recvText= (TextView)findViewById(R.id.RecvText);       
        recvText.setMovementMethod(ScrollingMovementMethod.getInstance()); 
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onClick(View v) {
		// Intent intent = new Intent();
		switch (v.getId()) {
		case R.id.StartConnect:
			if (isConnecting) 
			{				
				isConnecting = false;
				try {
					if(mSocketClient!=null)
					{
						mSocketClient.close();
						mSocketClient = null;
						
						mPrintWriterClient.close();
						mPrintWriterClient = null;
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				mThreadClient.interrupt();
				
				startButton.setText("开始连接");					
				IPText.setEnabled(true);		
				recvText.setText("信息:\n");
			}
			else
			{				
				isConnecting = true;
				startButton.setText("停止连接");						
				IPText.setEnabled(false);
				editor.putString("ipporttext", IPText.getText().toString());
				editor.commit();
				mThreadClient = new Thread(mRunnable);
				mThreadClient.start();				
			}
			break;
			
			
		case R.id.SendButtonClient:
			if ( isConnecting && mSocketClient!=null) 
			{
				String msgText =editMsgTextCilent.getText().toString();//取得编辑框中我们输入的内容
				if(msgText.length()<=0)
				{
					Toast.makeText(context, "发送内容不能为空！", Toast.LENGTH_SHORT).show();
				}
				else
				{
					try 
					{
						msgText =msgText.concat("\r\n");
				    	mPrintWriterClient.print(msgText);//发送给服务器
				    	mPrintWriterClient.flush();
					}
					catch (Exception e) 
					{
						// TODO: handle exception
						Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
					}
				}
			}
			else
			{
				Toast.makeText(context, "没有连接", Toast.LENGTH_SHORT).show();
			}
			break;
		case R.id.id_blub1:
			switch (flag_bulb1) {
			case 1:
				flag_bulb1 = 2;
				try 
				{				    	
			    	mPrintWriterClient.print(ceshiLEDON);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				 bulb1_txt.setText("关闭灯环");
				bulb1_img.setImageResource(R.drawable.light_bulb_on);
				break;
			case 2:
				flag_bulb1 = 1;
				try 
				{				    	
			    	mPrintWriterClient.print(ceshiLEDOFF);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				 bulb1_txt.setText("打开灯环");
				bulb1_img.setImageResource(R.drawable.light_bulb_off);
				break;
			}
			break;
		case R.id.id_blub2:
			switch (flag_bulb2) {
			case 1:
				flag_bulb2 = 2;
				try 
				{				    	
			    	mPrintWriterClient.print(ceshiLightON);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				 bulb2_txt.setText("关闭光感");
				bulb2_img.setImageResource(R.drawable.light_bulb_on);
				break;
			case 2:
				flag_bulb2 = 1;
				try 
				{				    	
			    	mPrintWriterClient.print(ceshiLightOFF);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				bulb2_txt.setText("打开光感");
				bulb2_img.setImageResource(R.drawable.light_bulb_off);
				break;
			}
			break;
		case R.id.id_blub3:
			switch (flag_bulb3) {
			case 1:
				flag_bulb3 = 2;
				try 
				{				    	
			    	mPrintWriterClient.print(cheMoveLeft);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				bulb3_img.setImageResource(R.drawable.light_bulb_on);
				break;
			case 2:
				flag_bulb3 = 1;
				try 
				{				    	
			    	mPrintWriterClient.print(cheStop);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				bulb3_img.setImageResource(R.drawable.light_bulb_off);

				break;
			}
			break;
		case R.id.id_blub4:
			switch (flag_bulb4) {
			case 1:
				flag_bulb4 = 2;
				try 
				{				    	
			    	mPrintWriterClient.print(cheMoveRight);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				bulb4_img.setImageResource(R.drawable.light_bulb_on);
				break;
			case 2:
				flag_bulb4 = 1;
				try 
				{				    	
			    	mPrintWriterClient.print(cheStop);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
				bulb4_img.setImageResource(R.drawable.light_bulb_off);
				break;
			}
			break;

	case R.id.id_fengshan:
		switch(flag_anim_fengshan)
		{
		case 1:
			flag_anim_fengshan=2;
			findViewById(R.id.id_fengshan_img).clearAnimation();
			break;
		case 2:
			flag_anim_fengshan=1;
			findViewById(R.id.id_fengshan_img).startAnimation(anim);
			break;
		}
		break;
	case R.id.id_chuanglian:
		switch(flag_anim_chuanglian)
		{
		case 1:
			flag_anim_chuanglian=2;
			try 
			{				    	
		    	mPrintWriterClient.print(Zigbee_chuanglian_kai);//发送给服务器
		    	mPrintWriterClient.flush();
			}
			catch (Exception e) 
			{
				// TODO: handle exception
				Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
			}
			rocketImage_chuanglian.setBackgroundResource(R.anim.myanimchuanglian);
			rocketAnimation_chuanglian = (AnimationDrawable) rocketImage_chuanglian.getBackground();
			
			rocketAnimation_chuanglian.setOneShot(true);
			rocketAnimation_chuanglian.start();
			break;
		case 2:
			flag_anim_chuanglian=1;
			try 
			{				    	
		    	mPrintWriterClient.print(Zigbee_chuanglian_guan);//发送给服务器
		    	mPrintWriterClient.flush();
			}
			catch (Exception e) 
			{
				// TODO: handle exception
				Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
			}
			rocketImage_chuanglian.setBackgroundResource(R.anim.myanimchuanglianclose);
			rocketAnimation_chuanglian = (AnimationDrawable) rocketImage_chuanglian.getBackground();
			rocketAnimation_chuanglian.setOneShot(true);
			rocketAnimation_chuanglian.start();
			break;
		}
		break;
	case R.id.id_chazuo:		
		switch(flag_chazuo)
		{
		case 1:
			flag_chazuo=2;
			try 
			{				    	
		    	mPrintWriterClient.print(Switch_up);//发送给服务器
		    	mPrintWriterClient.flush();
			}
			catch (Exception e) 
			{
				// TODO: handle exception
				Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
			}
			chazuo_img.setImageResource(R.drawable.device_calc_dock_open_big);
			break;
		case 2:
			try 
			{				    	
		    	mPrintWriterClient.print(Switch_down);//发送给服务器
		    	mPrintWriterClient.flush();
			}
			catch (Exception e) 
			{
				// TODO: handle exception
				Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
			}
			chazuo_img.setImageResource(R.drawable.device_calc_dock_close_big);
			
			flag_chazuo=1;
			
			break;
		}
		break;
	case R.id.id_RGBLED:
		dialog = new ColorPickerDialog(context, tvTextR.getTextColors().getDefaultColor(), 
				getResources().getString(R.string.btn_color_picker), 
				new ColorPickerDialog.OnColorChangedListener() {
			@Override
			public void colorChanged(int color) {
				int_color=color;
				int_color_R=((int_color&0x00FF0000)>>16);
				int_color_G=((int_color&0x0000FF00)>>8);
				int_color_B=(int_color&0x000000FF);
				tvTextR.setTextColor(int_color);
				tvTextG.setTextColor(int_color);
				tvTextB.setTextColor(int_color);
				//System.out.println(int_color);
				color_S_R=Integer.toString(int_color_R);
				color_S_G=Integer.toString(int_color_G);
				color_S_B=Integer.toString(int_color_B);
				tvTextR.setText(color_S_R);
				tvTextG.setText(color_S_G);
				tvTextB.setText(color_S_B);
				
				
				if((int_color_G>=0)&&(int_color_G<10))
				{
					G_bai=G_00.concat(color_S_G);
				}
				else if((int_color_G>=10)&&(int_color_G<100))
				{
					G_bai=G_0.concat(color_S_G);
				}
				else 
				{
					G_bai=color_S_G;
				} //Green
				
				
				if((int_color_R<10)&&(int_color_R>=0))
				{
					R_bai=R_00.concat(color_S_R);
				}
				else if((int_color_R>=10)&&(int_color_R<100))
				{
					R_bai=R_0.concat(color_S_R);
				}
				else 
				{
					R_bai=color_S_R;
				}//Red
				
				
				
				if((int_color_B<10)&&(int_color_B>=0))
				{
					B_bai=B_00.concat(color_S_B);
				}
				else if((int_color_B>=10)&&(int_color_B<100))
				{
					B_bai=B_0.concat(color_S_B);
				}
				else 
				{
					B_bai=color_S_B;
				}
				String send_G = ceshiGRB.concat(G_bai);
				String send_GR = send_G.concat(R_bai);
				String seng_GRB = send_GR.concat(B_bai);
				seng_GRB =seng_GRB.concat("\r\n");
				//改变光线感应开关
				flag_bulb2 = 1;
				bulb2_txt.setText("打开光感");
				bulb2_img.setImageResource(R.drawable.light_bulb_off);
				try 
				{				    	
			    	mPrintWriterClient.print(seng_GRB);//发送给服务器
			    	mPrintWriterClient.flush();
				}
				catch (Exception e) 
				{
					// TODO: handle exception
					Toast.makeText(context, "发送异常：" + e.getMessage(), Toast.LENGTH_SHORT).show();
				}
			//	int_color=tvText.getTextColors();
				}
		});
		dialog.show();
		break;
		default:
			break;
		}
	}
	
	//线程:监听服务器发来的消息
	private Runnable	mRunnable	= new Runnable() 
	{
		public void run()
		{
			String msgText =IPText.getText().toString();
			if(msgText.length()<=0)
			{
				//Toast.makeText(mContext, "IP不能为空！", Toast.LENGTH_SHORT).show();
				recvMessageClient = "IP不能为空！\n";//消息换行
				Message msg = new Message();
                msg.what = 1;
				mHandler.sendMessage(msg);
				return;
			}
			int start = msgText.indexOf(":");
			if( (start == -1) ||(start+1 >= msgText.length()) )
			{
				recvMessageClient = "IP地址不合法\n";//消息换行
				Message msg = new Message();
                msg.what = 1;
				mHandler.sendMessage(msg);
				return;
			}
			String sIP = msgText.substring(0, start);
			String sPort = msgText.substring(start+1);
			int port = Integer.parseInt(sPort);				
			
			Log.d("gjz", "IP:"+ sIP + ":" + port);		

			try 
			{				
				//连接服务器
				mSocketClient = new Socket(sIP, port);	//portnum
				//取得输入、输出流
				mBufferedReaderClient = new BufferedReader(new InputStreamReader(mSocketClient.getInputStream()));
				
				mPrintWriterClient = new PrintWriter(mSocketClient.getOutputStream(), true);
				
				recvMessageClient = "已经连接server!\n";//消息换行
				Message msg = new Message();
                msg.what = 1;
				mHandler.sendMessage(msg);		
				//break;
			}
			catch (Exception e) 
			{
				recvMessageClient = "连接IP异常:" + e.toString() + e.getMessage() + "\n";//消息换行
				Message msg = new Message();
                msg.what = 1;
				mHandler.sendMessage(msg);
				return;
			}			

			char[] buffer = new char[256];
			int count = 0;
			while (isConnecting)
			{
				try
				{
					//if ( (recvMessageClient = mBufferedReaderClient.readLine()) != null )
					if((count = mBufferedReaderClient.read(buffer))>0)
					{						
						recvMessageClient = getInfoBuff(buffer, count) + "\n";//消息换行
						Message msg = new Message();
		                msg.what = 1;
						mHandler.sendMessage(msg);
					}
				}
				catch (Exception e)
				{
					recvMessageClient = "接收异常:" + e.getMessage() + "\n";//消息换行
					Message msg = new Message();
	                msg.what = 1;
					mHandler.sendMessage(msg);
				}
			}
		}
	};
	Handler mHandler = new Handler()
	{										
		  public void handleMessage(Message msg)										
		  {											
			  super.handleMessage(msg);			
			  if(msg.what == 0)
			  {
				  
				  recvText.append("Server: "+recvMessageServer);	// 刷新
			  }
			  else if(msg.what == 1)
			  {
				  recvText.append("Client: "+recvMessageClient);	// 刷新

			  }
		  }									
	 };
	 private String getInfoBuff(char[] buff, int count)
		{
			char[] temp = new char[count];
			for(int i=0; i<count; i++)
			{
				temp[i] = buff[i];
			}
			return new String(temp);
		}
	
	
	
	
	
//	public void closeSocket() {
//		try {
//			output.close();
//			socket.close();
//		} catch (IOException e) {
//			handleException(e, "close exception: ");
//		}
//	}
//	private void initClientSocket() {
//		try {
//			socket = new Socket(SERVER_HOST_IP, SERVER_HOST_PORT);
//			output = new PrintStream(socket.getOutputStream(), true, "utf-8");
//			//btnConnect.setEnabled(false);
//			//editSend.setEnabled(true);
//			//btnSend.setEnabled(true);
//		} catch (UnknownHostException e) {
//			handleException(e, "unknown host exception: " + e.toString());
//		} catch (IOException e) {
//			handleException(e, "io exception: " + e.toString());
//		}
//	}
//	private void sendMessage(String msg) {
//		output.print(msg);
//	}

}

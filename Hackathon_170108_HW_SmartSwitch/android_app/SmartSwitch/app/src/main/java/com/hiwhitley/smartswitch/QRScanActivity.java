package com.hiwhitley.smartswitch;

import android.app.Application;
import android.os.Vibrator;
import android.support.annotation.NonNull;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.afollestad.materialdialogs.DialogAction;
import com.afollestad.materialdialogs.MaterialDialog;
import com.hiwhitley.smartswitch.base.BaseActivity;
import com.hiwhitley.smartswitch.base.SmartSwitchApp;
import com.hiwhitley.smartswitch.entity.ResponseBody;
import com.hiwhitley.smartswitch.entity.User;
import com.hiwhitley.smartswitch.network.RetrofitFactory;

import cn.bingoogolapple.qrcode.core.QRCodeView;
import cn.bingoogolapple.qrcode.zxing.ZXingView;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

/**
 * Created by hiwhitley on 2016/7/2.
 */
public class QRScanActivity extends BaseActivity implements QRCodeView.Delegate, View.OnClickListener {

    private static final String TAG = "QRScanActivity";

    ZXingView mQRCodeView;
    TextView mTlTitle;
    Toolbar mCustomToolbar;
    TextView mScanBarcode;
    TextView mScanQrcode;
    TextView mOpenFlashlight;
    TextView mCloseFlashlight;

    public final static String CODE_TYPE = "type";
    private MaterialDialog dialog;
    private TextView mDeviceIdTextView;
    private User mUser;

    @Override
    protected int getContentViewId() {
        return R.layout.activity_qr_scan;
    }

    protected void findViews() {
        mQRCodeView = findView(R.id.zxingview);
        mTlTitle = findView(R.id.tl_title);
        mCustomToolbar = findView(R.id.custom_toolbar);
        mScanBarcode = findView(R.id.scan_barcode);
        mScanQrcode = findView(R.id.scan_qrcode);
        mOpenFlashlight = findView(R.id.open_flashlight);
        mCloseFlashlight = findView(R.id.close_flashlight);
    }

    @Override
    protected void init() {
        findViews();
        mTlTitle.setText("绑定设备");
        initTootBar(mCustomToolbar);
        mQRCodeView.setDelegate(this);

        SmartSwitchApp application = (SmartSwitchApp) getApplication();
        mUser = application.getUser();
    }

    @Override
    protected void setListeners() {
        mScanBarcode.setOnClickListener(this);
        mScanQrcode.setOnClickListener(this);
        mOpenFlashlight.setOnClickListener(this);
        mCloseFlashlight.setOnClickListener(this);

        mQRCodeView.startSpot();
        mQRCodeView.changeToScanQRCodeStyle();
    }

    @Override
    protected void onStart() {
        super.onStart();
        mQRCodeView.startCamera();
//        mQRCodeView.startCamera(Camera.CameraInfo.CAMERA_FACING_FRONT);
    }

    @Override
    protected void onStop() {
        mQRCodeView.stopCamera();
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        mQRCodeView.onDestroy();
        super.onDestroy();
    }

    private void vibrate() {
        Vibrator vibrator = (Vibrator) getSystemService(VIBRATOR_SERVICE);
        vibrator.vibrate(200);
    }

    @Override
    public void onScanQRCodeSuccess(final String result) {
        Log.i(TAG, "result:" + result);
        dialog = new MaterialDialog.Builder(QRScanActivity.this)
                .title("绑定设备编号")
                .customView(R.layout.dialog_customview, true)
                .positiveText("确定")
                .negativeText("取消")
                .onPositive(new MaterialDialog.SingleButtonCallback() {
                    @Override
                    public void onClick(@NonNull MaterialDialog dialog, @NonNull DialogAction which) {
                        bindDevice(mUser.getUserID(), result);
                        dialog.dismiss();
                        finish();
                    }
                }).build();
        View view = dialog.getCustomView();
        mDeviceIdTextView = (TextView) view.findViewById(R.id.tv_device_id);
        mDeviceIdTextView.setText(result);

        dialog.show();

        vibrate();
        mQRCodeView.startSpot();
    }

    @Override
    public void onScanQRCodeOpenCameraError() {
        Log.e(TAG, "打开相机出错");
        showToast("打开相机出错");
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.open_flashlight:
                mQRCodeView.openFlashlight();
                break;
            case R.id.close_flashlight:
                mQRCodeView.closeFlashlight();
                break;
            case R.id.scan_qrcode:
                mQRCodeView.startSpot();
                mQRCodeView.changeToScanQRCodeStyle();
                break;
            case R.id.scan_barcode:
                mQRCodeView.startSpot();
                mQRCodeView.changeToScanBarcodeStyle();
                break;
            default:
                break;
        }
    }

    private void bindDevice(String userID, String deviceID) {
        Call<ResponseBody> call = RetrofitFactory.getSmartService().bindUserAndDev(userID, deviceID);
        call.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                if (response.body().isSuccessful()) {
                    showToast("绑定设备成功！");
                } else {
                    showToast("绑定出现错误！");
                }
                Log.d(TAG, "onResponse" + response.body());
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                Log.d(TAG, "onFailure" + t.getMessage());
                showToast("绑定失败:" + t.getMessage());
            }
        });
    }

    public void initTootBar(Toolbar mCustomToolbar) {
        mCustomToolbar.setTitle("");
        mCustomToolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }

}

package com.hiwhitley.smartswitch;

import android.content.Context;
import android.content.Intent;
import android.nfc.Tag;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.google.gson.Gson;
import com.hiwhitley.smartswitch.base.BaseActivity;
import com.hiwhitley.smartswitch.entity.Device;
import com.hiwhitley.smartswitch.network.RetrofitFactory;
import com.hiwhitley.smartswitch.network.SmartService;
import com.hiwhitley.smartswitch.support.widget.AirQualityIndexView;

import java.util.List;

import in.srain.cube.views.ptr.PtrDefaultHandler;
import in.srain.cube.views.ptr.PtrFrameLayout;
import in.srain.cube.views.ptr.PtrHandler;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

/**
 * Created by hiwhitley on 17-1-5.
 */

public class DeviceDetailActivity extends BaseActivity {

    private static final String TAG = "DeviceDetailActivity";
    public static final String TAG_DEVICE = "device";

    private Toolbar mToolBar;
    private TextView mTlTitle;
    private TextView mTemperatureTextView;
    private TextView mHumidityTextView;
    private EditText mDeviceNameEditText;
    private AirQualityIndexView mAirQualityIndexView;
    private PtrFrameLayout ptrFrameLayout;
    private LinearLayout mEditDeviceNameLinearLayout;

    private Device device;
    private boolean editStatus = false;

    @Override
    protected int getContentViewId() {
        return R.layout.activity_device_detail;
    }

    @Override
    protected void init() {
        mToolBar = findView(R.id.custom_toolbar);
        mTlTitle = findView(R.id.tl_title);
        mTemperatureTextView = findView(R.id.tv_temperature);
        mHumidityTextView = findView(R.id.tv_humidity);
        mDeviceNameEditText = findView(R.id.et_device_name);
        mAirQualityIndexView = findView(R.id.aqi_view);
        ptrFrameLayout = findView(R.id.classic_ptr_frame);
        mEditDeviceNameLinearLayout = findView(R.id.ll_edit_device_name);

        device = (Device) getIntent().getSerializableExtra(TAG_DEVICE);

        ptrFrameLayout.postDelayed(new Runnable() {
            @Override
            public void run() {
                ptrFrameLayout.autoRefresh(true);
            }
        }, 150);

        initTootBar(mToolBar);
        updateDeviceDataUI();
    }

    @Override
    protected void setListeners() {

        ptrFrameLayout.setPtrHandler(new PtrHandler() {
            @Override
            public void onRefreshBegin(PtrFrameLayout frame) {

                frame.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        SmartService smartService = RetrofitFactory.getSmartService();
                        final Call<Device> call = smartService.getNewOneDevices(device.getDeviceID());
                        call.enqueue(new Callback<Device>() {
                            @Override
                            public void onResponse(Call<Device> call, Response<Device> response) {
                                Log.d(TAG, "onResponse" + response.body());
                                device = response.body();
                                updateDeviceDataUI();
                            }

                            @Override
                            public void onFailure(Call<Device> call, Throwable t) {
                                Log.d(TAG, "onFailure" + t.getMessage());
                                showToast("出现错误： " + t.getMessage());
                            }
                        });
                        ptrFrameLayout.refreshComplete();
                    }
                }, 1000);
            }

            @Override
            public boolean checkCanDoRefresh(PtrFrameLayout frame, View content, View header) {
                return PtrDefaultHandler.checkContentCanBePulledDown(frame, content, header);
            }
        });

        mToolBar.setOnMenuItemClickListener(new Toolbar.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                switch (item.getItemId()) {
                    case R.id.menu_edit:
                        editStatus = !editStatus;
                        if (editStatus) {
                            mToolBar.getMenu().findItem(R.id.menu_edit).setTitle("完成");
                            mEditDeviceNameLinearLayout.setVisibility(View.VISIBLE);

                        } else {
                            SmartService smartService = RetrofitFactory.getSmartService();
                            Call<Device> call = smartService.updateNickName(device.getDeviceID(), mDeviceNameEditText.getText().toString().trim());
                            call.enqueue(new Callback<Device>() {
                                @Override
                                public void onResponse(Call<Device> call, Response<Device> response) {
                                    //device = response.body();
                                    Log.d(TAG, "onResponse" + device.toString());
                                    ptrFrameLayout.postDelayed(new Runnable() {
                                        @Override
                                        public void run() {
                                            ptrFrameLayout.autoRefresh(true);
                                        }
                                    }, 150);
                                    InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                                    imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
                                    showToast("修改设备成功");
                                }

                                @Override
                                public void onFailure(Call<Device> call, Throwable t) {
                                    Log.d(TAG, "onFailure" + t.getMessage());
                                    showToast("出现错误： " + t.getMessage());
                                    updateDeviceDataUI();
                                }
                            });
                            mToolBar.getMenu().findItem(R.id.menu_edit).setTitle("编辑");
                            mEditDeviceNameLinearLayout.setVisibility(View.GONE);
                        }
                        break;
                    case R.id.menu_time_task:
                        Intent intent = new Intent(getBaseContext(), TimeTaskActivity.class);
                        startActivity(intent);
                        break;
                    default:
                        break;
                }
                return true;
            }
        });

    }

    private void updateDeviceDataUI() {
        mTlTitle.setText(device.getNickName());
        mTemperatureTextView.setText("温度： " + device.getTemperature() + " ℃");
        mHumidityTextView.setText("湿度： " + device.getHumidity() + " %");
        mDeviceNameEditText.setText(device.getNickName());
        mAirQualityIndexView.setAqi((int) device.getPm());
    }

    public void initTootBar(Toolbar mCustomToolbar) {
        mCustomToolbar.setTitle("");
        mCustomToolbar.inflateMenu(R.menu.menu_device_detail_activity);
        mCustomToolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }

}

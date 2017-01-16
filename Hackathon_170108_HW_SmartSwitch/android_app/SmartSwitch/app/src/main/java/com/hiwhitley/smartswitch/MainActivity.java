package com.hiwhitley.smartswitch;

import android.content.Intent;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.listener.OnItemClickListener;
import com.espressif.iot.esptouch.demo_activity.EsptouchDemoActivity;
import com.hiwhitley.smartswitch.adapter.DeviceListAdapter;
import com.hiwhitley.smartswitch.base.BaseActivity;
import com.hiwhitley.smartswitch.base.SmartSwitchApp;
import com.hiwhitley.smartswitch.entity.Device;
import com.hiwhitley.smartswitch.network.RetrofitFactory;
import com.hiwhitley.smartswitch.network.SmartService;

import java.util.ArrayList;
import java.util.List;

import in.srain.cube.views.ptr.PtrDefaultHandler;
import in.srain.cube.views.ptr.PtrFrameLayout;
import in.srain.cube.views.ptr.PtrHandler;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MainActivity extends BaseActivity {

    private static final String TAG = "MainActivity";

    private List<Device> deviceList;
    private RecyclerView mRecyclerView;
    private TextView mTitleTextView;
    private Toolbar mToolbar;
    private DeviceListAdapter mDeviceListAdapter;
    private PtrFrameLayout ptrFrameLayout;

    @Override
    protected int getContentViewId() {
        return R.layout.activity_main;
    }

    @Override
    protected void init() {

        deviceList = new ArrayList<>();

        mRecyclerView = findView(R.id.rv_main);
        mTitleTextView = findView(R.id.tl_title);
        ptrFrameLayout = findView(R.id.store_house_ptr_frame);
        mToolbar = findView(R.id.custom_toolbar);

        mTitleTextView.setText("设备列表");
        initTootBar(mToolbar);

    }

    @Override
    protected void setListeners() {

        //自动刷新
        ptrFrameLayout.postDelayed(new Runnable() {
            @Override
            public void run() {
                ptrFrameLayout.autoRefresh(true);
            }
        }, 150);

        mDeviceListAdapter = new DeviceListAdapter(this, deviceList);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.setAdapter(mDeviceListAdapter);
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, DividerItemDecoration.VERTICAL));

        mRecyclerView.addOnItemTouchListener(new OnItemClickListener() {
            @Override
            public void SimpleOnItemClick(BaseQuickAdapter baseQuickAdapter, View view, int i) {
                Intent intent = new Intent(getBaseContext(), DeviceDetailActivity.class);
                intent.putExtra(DeviceDetailActivity.TAG_DEVICE, deviceList.get(i));
                startActivity(intent);

            }

        });

        ptrFrameLayout.setPtrHandler(new PtrHandler() {
            @Override
            public void onRefreshBegin(PtrFrameLayout frame) {

                frame.postDelayed(new Runnable() {
                    @Override
                    public void run() {

                        SmartSwitchApp app = (SmartSwitchApp) getApplication();
                        SmartService smartService = RetrofitFactory.getSmartService();
                        final Call<List<Device>> call = smartService.getDeviceList(app.getUser().getUserID());
                        call.enqueue(new Callback<List<Device>>() {
                            @Override
                            public void onResponse(Call<List<Device>> call, Response<List<Device>> response) {
                                Log.d(TAG, "onResponse" + response.body());
                                deviceList = response.body();
                                mDeviceListAdapter.setNewData(deviceList);
                            }

                            @Override
                            public void onFailure(Call<List<Device>> call, Throwable t) {
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

        mToolbar.setOnMenuItemClickListener(new Toolbar.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {
                switch (item.getItemId()) {
                    case R.id.menu_bind_device:
                        Intent intent = new Intent(getBaseContext(), QRScanActivity.class);
                        startActivity(intent);
                        break;
                    case R.id.menu_one_key:
                        Intent intent2 = new Intent(getBaseContext(), EsptouchDemoActivity.class);
                        startActivity(intent2);
                        break;
                    default:
                        break;
                }
                return true;
            }
        });
    }

    public void initTootBar(Toolbar mCustomToolbar) {
        mCustomToolbar.setTitle("");
        mCustomToolbar.inflateMenu(R.menu.menu_main_activity);
        mCustomToolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }
}

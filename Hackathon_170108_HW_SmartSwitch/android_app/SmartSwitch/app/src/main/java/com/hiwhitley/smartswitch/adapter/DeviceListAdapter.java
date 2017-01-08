package com.hiwhitley.smartswitch.adapter;

import android.content.Context;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.Toast;

import com.chad.library.adapter.base.BaseQuickAdapter;
import com.chad.library.adapter.base.BaseViewHolder;
import com.hiwhitley.smartswitch.R;
import com.hiwhitley.smartswitch.entity.Device;
import com.hiwhitley.smartswitch.entity.ResponseBody;
import com.hiwhitley.smartswitch.network.RetrofitFactory;
import com.hiwhitley.smartswitch.network.SmartService;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

/**
 * Created by hiwhitley on 17-1-5.
 */

public class DeviceListAdapter extends BaseQuickAdapter<Device> {

    private static final String TAG = "DeviceListAdapter";
    private Context mContext;
    private int status;

    public DeviceListAdapter(Context context, List<Device> deviceList) {
        super(R.layout.item_main, deviceList);
        this.mContext = context;
    }

    @Override
    protected void convert(BaseViewHolder baseViewHolder, final Device device) {
        baseViewHolder.setText(R.id.tv_device_name, device.getNickName())
                .addOnClickListener(R.id.switch_compat);
        Switch switchCompat = baseViewHolder.getView(R.id.switch_compat);

        if (device.getStatus() == 0) {
            switchCompat.setChecked(false);
        } else {
            switchCompat.setChecked(true);
        }

        switchCompat.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, final boolean isChecked) {

                if (isChecked) {
                    status = 1;
                } else {
                    status = 0;
                }

                SmartService smartService = RetrofitFactory.getSmartService();
                Call<Device> call = smartService.todoSwitch(device.getDeviceID(), status);
                call.enqueue(new Callback<Device>() {
                    @Override
                    public void onResponse(Call<Device> call, Response<Device> response) {
                        if (isChecked) {
                            showToast("开启成功！");
                        } else {
                            showToast("关闭成功！");
                        }
                        Log.i(TAG, "onResponse" + response.body());
                    }

                    @Override
                    public void onFailure(Call<Device> call, Throwable t) {
                        Log.i(TAG, "onFailure" + t.getMessage());
                        showToast("出现错误： " + t.getMessage());
                        notifyDataSetChanged();
                    }
                });
            }
        });
    }

    public void showToast(String msg) {
        Toast.makeText(mContext, msg, Toast.LENGTH_LONG).show();
    }
}

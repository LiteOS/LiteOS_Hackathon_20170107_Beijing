package com.hiwhitley.smartswitch;

import android.app.Application;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import com.hiwhitley.smartswitch.base.BaseActivity;
import com.hiwhitley.smartswitch.base.SmartSwitchApp;
import com.hiwhitley.smartswitch.entity.User;
import com.hiwhitley.smartswitch.network.RetrofitFactory;
import com.rengwuxian.materialedittext.MaterialEditText;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

/**
 * Created by hiwhitley on 2016/5/7.
 */
public class RegisterActivity extends BaseActivity {
    private static final String TAG = "RegisterActivity";
    private MaterialEditText mNameMaterialEditText;
    private MaterialEditText mPswMaterialEditText;
    private MaterialEditText mNickMaterialEditText;
    private TextView mTitleTextView;
    private Button mRegisterBtn;
    private Toolbar mToolbar;

    @Override
    protected int getContentViewId() {
        return R.layout.activity_register;
    }

    protected void findViews() {
        mNameMaterialEditText = findView(R.id.met_name);
        mPswMaterialEditText = findView(R.id.met_psw);
        mNickMaterialEditText = findView(R.id.met_nick);
        mRegisterBtn = findView(R.id.btn_register);
        mTitleTextView = findView(R.id.tl_title);
        mToolbar = findView(R.id.custom_toolbar);

        mTitleTextView.setText("注册用户");
        initTootBar(mToolbar);
    }

    @Override
    protected void init() {
        findViews();
    }

    @Override
    protected void setListeners() {
        mRegisterBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if ("".equals(mNameMaterialEditText.getText().toString())) {
                    mNameMaterialEditText.setError("用户名不能为空！");
                    return;
                }

                if ("".equals(mPswMaterialEditText.getText().toString())) {
                    mPswMaterialEditText.setError("密码不能为空！");
                    return;
                }

                if ("".equals(mNickMaterialEditText.getText().toString())) {
                    mNickMaterialEditText.setError("昵称不能为空！");
                    return;
                }

                toRegister();

            }
        });

    }

    private void toRegister() {
        final User user = new User();
        user.setTel(mNameMaterialEditText.getText().toString());
        user.setPassword(mPswMaterialEditText.getText().toString());
        user.setNickName(mNickMaterialEditText.getText().toString());

        Call<User> userCall = RetrofitFactory.getSmartService().saveUserInfo(user.getTel(), user.getPassword());
        userCall.enqueue(new Callback<User>() {
            @Override
            public void onResponse(Call<User> call, Response<User> response) {
                Log.d(TAG, "onResponse" + response.body());
                User user = response.body();
                if (user.isSuccessful()) {
                    showToast("注册用户成功： " + user.getUserID());
                } else {
                    showToast("注册失败");
                }
            }

            @Override
            public void onFailure(Call<User> call, Throwable t) {
                showToast("注册失败:" + t.getMessage());
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

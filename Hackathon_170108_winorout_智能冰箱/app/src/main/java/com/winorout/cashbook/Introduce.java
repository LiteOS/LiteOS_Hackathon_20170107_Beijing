package com.winorout.cashbook;

/**
 * Created by tom on 2017/1/7.
 */

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.provider.Settings;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Window;
import android.webkit.WebChromeClient;
import android.webkit.WebResourceRequest;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

public class Introduce extends Activity {
    private String url = "http://www.baidu.com/";
    private WebView webView;
    private ProgressDialog dialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.webview);


//        Uri uri = Uri.parse(url);
//        Intent intent = new Intent(Intent.ACTION_VIEW,uri);
//        startActivity(intent);
        init();
    }

    private void init() {

        webView = (WebView) findViewById(R.id.webView);
        webView.loadUrl("http://baike.baidu.com/link?url=rHTXckIyLxSQBgsCVJS6cTusRUXxxoOY9u6YzOwC5zoAqIHfM5t7W1JtzxIBOv4KEYcho9eQoDzz8FFEaZqHNUZoByx5A3hPcZypBczQGJD4M-r4thDUYBcFTo0NyAiA");
        webView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;

            }
        });

        // 启动支持JavaScript
        WebSettings settings = webView.getSettings();
        settings.setJavaScriptEnabled(true);

//        webView.setWebChromeClient(new WebChromeClient() {
//            @Override
//            public void onProgressChanged(WebView view, int newProgress) {
//                if (newProgress == 100) {
//                    closeDialog();
//                } else {
//                    openDialog(newProgress);
//                }
//            }
//
//            private void closeDialog() {
//                if (dialog != null && dialog.isShowing()) {
//                    dialog.dismiss();
//                    dialog = null;
//
//                }
//
//            }
//
//            private void openDialog(int newProgress) {
//                if (dialog == null) {
//                    dialog = new ProgressDialog(Introduce.this);
//                    dialog.setTitle("正在加载");
//                    dialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
//                    dialog.setProgress(newProgress);
//                    dialog.show();
//                } else {
//                    dialog.setProgress(newProgress);
//                }
//            }
//
//        });


    }

    // 改写返回按钮逻辑

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
//            Toast.makeText(this, webView.getUrl(), Toast.LENGTH_SHORT).show();
            if (webView.canGoBack()) {
                webView.goBack();
                return true;
            } else {
                System.exit(0);
            }
        }

        return super.onKeyDown(keyCode, event);

    }


}

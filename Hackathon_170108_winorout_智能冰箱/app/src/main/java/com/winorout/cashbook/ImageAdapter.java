package com.winorout.cashbook;

/**
 * Created by tom on 2017/1/8.
 */

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Gallery;
import android.widget.ImageView;

/**
 * Created by tom on 2016/12/26.
 */

public class ImageAdapter extends BaseAdapter{

    private int[] res;
    private Context context;
    private int z = 0;

    public ImageAdapter(int []res,Context context){
        this.res = res;
        this.context = context;

    }
    @Override
    public int getCount() {
        return Integer.MAX_VALUE;
    }

    @Override
    public long getItemId(int i) {

        return i;
    }

    @Override
    public Object getItem(int i) {


        return res[i];
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        Log.i("Main","position = " +i+"res角标"+i%res.length);

        ImageView image = new ImageView(context);

        image.setBackgroundResource(res[i%res.length]);   // Resource  res[]是一个id
        image.setLayoutParams(new Gallery.LayoutParams(700,600));
        image.setScaleType(ImageView.ScaleType.FIT_XY);





        return image;
    }


}

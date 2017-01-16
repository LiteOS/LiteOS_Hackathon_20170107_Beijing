package com.winorout.cashbook.xdwan;

/**
 * Created by Mr-x on 2016/12/09.
 */

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.winorout.cashbook.R;

import java.util.ArrayList;

public class ListAdapters extends BaseAdapter {

    private ArrayList<SaveItem> marrayList;

    private Context mcontext;

    private ViewHolder saveItem;

    public ListAdapters(Context context, ArrayList<SaveItem> arrayList) {
        mcontext = context;
        marrayList = arrayList;
        Initialization();
    }

    private void Initialization() {
    }

    @Override
    public int getCount() {
        return marrayList.size();
    }

    @Override
    public Object getItem(int i) {
        return marrayList.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        saveItem = null;
        if (view == null) {
            saveItem = new ViewHolder();
            view = LayoutInflater.from(mcontext).inflate(R.layout.item, null);
            saveItem.imageView = (ImageView) view.findViewById(R.id.pics);
            saveItem.textView = (TextView) view.findViewById(R.id.txt);
            saveItem.money = (TextView)view.findViewById(R.id.money);
            view.setTag(saveItem);
        } else {
            saveItem = (ViewHolder) view.getTag();
        }
        saveItem.imageView.setImageResource(marrayList.get(i).getImageView());
        saveItem.money.setText(marrayList.get(i).getMoney() + "");
        saveItem.textView.setText(marrayList.get(i).getTextView());
        return view;
    }

    private class ViewHolder
    {
        private ImageView imageView;
        private TextView textView;
        private TextView money;
    }
}





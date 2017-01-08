package com.hiwhitley.smartswitch.support.widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.view.View;

import com.hiwhitley.smartswitch.support.util.DensityUtil;

public class AirQualityIndexView extends View {

    private int GRADUATION_LENGTH = DensityUtil.dip2px(getContext(), 12f);

    private String[] mColors = {
            "#00FF00", "#FFFF00", "#FF8000", "#FF0000", "#800080", "#800080", "#800000"
    };

    private String[] mLevels = {
            "优", "良", "轻度污染", "中度污染", "重度污染", "重度污染", "严重污染"
    };

    private Context mContext;

    private int viewLeft;
    private int viewBottom;

    private int mAqi;
    private int mLevel;

    private Paint mTextPaint;

    private Paint mGraduationPaint;

    private int mCenterX;
    private int mCenterY;

    public AirQualityIndexView(Context context) {
        this(context, null);
    }

    public AirQualityIndexView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public AirQualityIndexView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mContext = context;
        init();
    }

    private void init() {

        mAqi = 450;
        mGraduationPaint = new Paint();
        mGraduationPaint.setStyle(Paint.Style.STROKE);
        mGraduationPaint.setStrokeWidth(DensityUtil.dip2px(mContext, 1));
        mGraduationPaint.setStrokeCap(Paint.Cap.ROUND);
        mGraduationPaint.setAntiAlias(true);
        mTextPaint = new TextPaint();
        mTextPaint.setColor(Color.WHITE);
        mTextPaint.setAntiAlias(true);
    }

    public void setAqi(int aqi) {
        mAqi = aqi;
        invalidate();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int width, height;
        int widthSize = MeasureSpec.getSize(widthMeasureSpec);
        int heightMode = MeasureSpec.getMode(heightMeasureSpec);
        int heightSize = MeasureSpec.getSize(heightMeasureSpec);
        if (heightMode == MeasureSpec.EXACTLY) {
            height = heightSize;
        } else {
            height = (widthSize - getPaddingLeft() - getPaddingRight()) / 4 +
                    getPaddingBottom() + getPaddingTop() +
                    DensityUtil.dip2px(getContext(), 16f);
        }
        width = widthSize;
        mCenterX = width / 2;
        mCenterY = (width - getPaddingLeft() - getPaddingRight()) / 2 + getPaddingTop();
        viewLeft = getPaddingLeft();
        viewBottom = height - getPaddingBottom();
        setMeasuredDimension(width, height);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        drawGraduatedCircle(canvas);
        drawText(canvas);
    }

    private void drawGraduatedCircle(Canvas canvas) {
        float minGraduation = 3f;
        canvas.save();
        int x0 = viewLeft, y0 = mCenterY,
                x1 = x0 + GRADUATION_LENGTH, y1 = mCenterY;
        int degree;
        canvas.rotate(30f, mCenterX, mCenterY);
        if (mAqi == -1) {
            mLevel = 0;
            degree = -1;
        } else {
            mLevel = (((mAqi - 1) / 50) > 6 ? 6 : ((mAqi - 1) / 50));
            degree = ((mAqi / 10 + 1) > 41 ? 41 : (mAqi / 10 + 1));
        }
        mGraduationPaint.setColor(Color.parseColor(mColors[mLevel]));
        for (int i = 0; i < degree; i++) {
            canvas.drawLine(x0, y0, x1, y1, mGraduationPaint);
            canvas.rotate(minGraduation, mCenterX, mCenterY);
        }
        mGraduationPaint.setColor(Color.WHITE);
        for (int i = 0; i < 41 - degree; i++) {
            canvas.drawLine(x0, y0, x1, y1, mGraduationPaint);
            canvas.rotate(minGraduation, mCenterX, mCenterY);
        }
        canvas.restore();
    }

    private void drawText(Canvas canvas) {
        mTextPaint.setTextSize(DensityUtil.sp2px(mContext, 30f));
        String aqi;
        Rect rect = new Rect();
        if (mAqi == -1) {
            aqi = "暂无数据";
        } else {
            aqi = mAqi + " " + mLevels[mLevel];
        }
        mTextPaint.getTextBounds(aqi, 0, aqi.length(), rect);
        int x = mCenterX - rect.width() / 2, y = viewBottom;
        canvas.drawText(aqi, x, y, mTextPaint);
        mTextPaint.setTextSize(DensityUtil.sp2px(mContext, 16f));
        y -= rect.height();
        String s = "空气质量指数";
        mTextPaint.getTextBounds(s, 0, s.length(), rect);
        x = mCenterX - rect.width() / 2;
        canvas.drawText(s, x, y, mTextPaint);
    }
}

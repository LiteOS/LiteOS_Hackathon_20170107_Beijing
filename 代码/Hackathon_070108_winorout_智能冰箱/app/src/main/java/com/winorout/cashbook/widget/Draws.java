package com.winorout.cashbook.widget;

/**
 * Created by Mr-x on 2016/11/25.
 */

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PointF;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;

import java.text.NumberFormat;

public class Draws extends View {

    private Paint mPaint, mPaint2;
    private Path mPath = new Path();
    protected int mViewWidth, mViewHeight;
    protected int mWidth, mHeight;
    private float r, rArc, x;
    private float percent = 0.5f;
    private RectF rectF;
    private PointF mPointF = new PointF(0, 0);
    private String temperature = "28" + "℃";

    public Draws(Context context, AttributeSet attrs) {
        super(context, attrs);

        mPaint = new Paint();
        mPaint.setColor(Color.BLACK);
        mPaint.setStrokeWidth(1);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setTextSize(30);

        mPaint2 = new Paint();
        mPaint2.setColor(Color.CYAN);
        mPaint2.setStrokeWidth(2);
        mPaint2.setStyle(Paint.Style.FILL);
    }

    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);

        mViewWidth = w;
        mViewHeight = h;

        mWidth = mViewWidth - getPaddingLeft() - getPaddingRight();
        mHeight = mViewHeight - getPaddingTop() - getPaddingBottom();

        r = Math.min(mWidth, mHeight) * 0.4f;

        rectF = new RectF(-r, -r, r, r);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        canvas.translate(mViewWidth / 2, mViewHeight / 2);

        mPaint.setStyle(Paint.Style.FILL);

        mPaint.setColor(Color.argb(225, 137, 97, 28));

        canvas.drawCircle(0, 0, r + 4, mPaint);

        mPaint.setColor(Color.argb(255, 169, 150, 115));

        canvas.drawCircle(0, 0, r, mPaint);

        /*
//        * 绘制一个最外围的半个圆弧，为了美观，可以不写*/
//
//        RectF oval = new RectF();
//        oval.top = -r - 6;
//        oval.left = -r - 6;
//        oval.right = 126;
//        oval.bottom = 126;
//        mPaint.setColor(Color.WHITE);
//        mPaint.setStyle(Paint.Style.STROKE);
//        mPaint.setStrokeWidth(6);
//        canvas.drawArc(oval, 180, 180, false, mPaint);

        rArc = r * (1 - 2 * percent);//percent 0-1

        double angle = Math.acos((double) rArc / r);

        x = r * (float) Math.sin(angle);

        /*
        * 给绘制中间的波形进度设置路径*/
        mPath.addArc(rectF, 90 - (float) Math.toDegrees(angle),
                (float) Math.toDegrees(angle) * 2);
        mPath.moveTo(-x, rArc);
        mPath.rQuadTo(x / 2, -r / 8, x, 0);
        mPath.rQuadTo(x / 2, r / 8, x, 0);

        /*
        * 为了给不同的进度时显示不同的圆环背景颜色*/
        if (percent < 0.6) {
            mPaint2.setColor(Color.CYAN);
        } else if (percent < 0.8) {
            mPaint2.setColor(Color.argb(225, 186, 76, 25));
        } else {
            mPaint2.setColor(Color.GRAY);
        }
//        canvas.drawPath(mPath, mPaint2); //开始绘制波形进度
        mPath.rewind(); //回收路径设置


        /**
         * 画文字
         */
        NumberFormat numberFormat = NumberFormat.getPercentInstance();
        numberFormat.setMinimumFractionDigits(1);
        textCenter(new String[]{"当前温度"}, mPaint,
                canvas, mPointF, Paint.Align.CENTER);
    }


    protected void textCenter(String[] strings, Paint paint, Canvas canvas,
                              PointF point, Paint.Align align) {
        paint.setTextAlign(align);
        Paint.FontMetrics fontMetrics = paint.getFontMetrics();
        float top = fontMetrics.top;
        float bottom = fontMetrics.bottom;
        int length = strings.length;
        float total = (length - 1) * (-top + bottom)
                + (-fontMetrics.ascent + fontMetrics.descent);
        float offset = total / 2 - bottom;
        /*
        * 绘制圆环中间的文字，为了精确的控制显示的位置*/
        for (int i = 0; i < length; i++) {
            paint.setStyle(Paint.Style.FILL);
            paint.setColor(Color.WHITE);
            float yAxis = -(length - i - 1) * (-top + bottom) + offset;
            paint.setTextSize(25);
            canvas.drawText(strings[i], point.x, point.y - 25 + yAxis, paint);
            paint.setTextSize(40);
            canvas.drawText(temperature, point.x, point.y + 25, paint);
        }
    }

    public void settemperature(String temperature)
    {
        this.temperature = temperature;
    }

    /*
    * 设置圆环的进度*/
    public void setProgress(float percent) {
        //percent是从0到1；
        if (percent != 0) {
            if (percent > 100) {
                this.percent = 1;
            } else {
                this.percent = percent / 100;
            }
        } else {
            this.percent = 0;
        }
        invalidate();
    }

}
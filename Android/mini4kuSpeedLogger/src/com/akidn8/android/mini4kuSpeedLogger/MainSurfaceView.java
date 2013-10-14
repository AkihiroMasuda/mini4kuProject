package com.akidn8.android.mini4kuSpeedLogger;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MainSurfaceView extends SurfaceView implements SurfaceHolder.Callback{

	Paint paint;
    private String str_txtview;
	
	public MainSurfaceView(Context context) {
		super(context);
        // コンストラクタ。SurfaceView描画に用いるコールバックを登録する。
        getHolder().addCallback(this);
        // 描画用の準備
        paint = new Paint();
        paint.setColor(Color.BLUE);
        paint.setAntiAlias(true);
        paint.setTextSize(24);
        str_txtview = new String();
     }
	
	 @Override
     public void surfaceCreated(SurfaceHolder holder) {
         // SurfaceView生成時に呼び出されるメソッド。
         // 今はとりあえず背景を白にするだけ。
         Canvas canvas = holder.lockCanvas();
         canvas.drawColor(Color.WHITE);
         holder.unlockCanvasAndPost(canvas);
     }

	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		// TODO Auto-generated method stub
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
		
	}	
	
	public void addText(String txt){
		str_txtview = txt + "\n" + str_txtview;
		final int str_txtview_maxlen =  1024;
		if (str_txtview.length() > str_txtview_maxlen){
			//長いので程々にカット
			str_txtview = str_txtview.substring(0, str_txtview_maxlen); 
		}
	}
	
    private int cnt = 0;
	public void updateSurface(){
		++cnt;
//		str_txtview = String.valueOf(cnt) + "\n" + str_txtview;
//		final int str_txtview_maxlen =  1024;
//		if (str_txtview.length() > str_txtview_maxlen){
//			//長いので程々にカット
//			str_txtview = str_txtview.substring(0, str_txtview_maxlen); 
//		}
		String[] str_s = str_txtview.split("\n");
		
//		tv.setText(str_txtview);
		
		Canvas canvas = getHolder().lockCanvas();
        if (canvas != null){
            canvas.drawColor(Color.WHITE);
//            canvas.drawText("count = " + count, 0, paint.getTextSize(), paint);
//            canvas.drawText(str_txtview, 0, paint.getTextSize(), paint);
            
            int lownum = 0;
            int padding = 4;
    		for(String s : str_s){
    			canvas.drawText(s, 0, paint.getTextSize() + (paint.getTextSize()+padding)*lownum, paint);
    			lownum++;
    		}
            
            getHolder().unlockCanvasAndPost(canvas);
        }
   }
}

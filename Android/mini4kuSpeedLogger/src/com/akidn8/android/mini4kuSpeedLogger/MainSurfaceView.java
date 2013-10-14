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
        // �R���X�g���N�^�BSurfaceView�`��ɗp����R�[���o�b�N��o�^����B
        getHolder().addCallback(this);
        // �`��p�̏���
        paint = new Paint();
        paint.setColor(Color.BLUE);
        paint.setAntiAlias(true);
        paint.setTextSize(24);
        str_txtview = new String();
     }
	
	 @Override
     public void surfaceCreated(SurfaceHolder holder) {
         // SurfaceView�������ɌĂяo����郁�\�b�h�B
         // ���͂Ƃ肠�����w�i�𔒂ɂ��邾���B
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
	
	// ��ʂɕ`�悷��P�b���̕������ǉ�
	public void addText(String txt){
		synchronized(this){
			str_txtview = txt + str_txtview;
			final int str_txtview_maxlen =  1024;
			if (str_txtview.length() > str_txtview_maxlen){
				//�����̂Œ��X�ɃJ�b�g
				str_txtview = str_txtview.substring(0, str_txtview_maxlen); 
			}
		}
	}
	
    private int cnt = 0;
	public void updateSurface(){
		++cnt;
		String[] str_a;
		synchronized (this) {
			str_a = str_txtview.split("\n");
		}
		
		Canvas canvas = getHolder().lockCanvas();
        if (canvas != null){
            canvas.drawColor(Color.WHITE);
            int lownum = 0;
            int padding = 4;
    		for(String s : str_a){
    			canvas.drawText(s, 0, 70 + (paint.getTextSize()+padding)*lownum, paint);
//    			canvas.drawLine(0.f, 0.f, 200.f, 200.f, paint);
    			lownum++;
    		}
            getHolder().unlockCanvasAndPost(canvas);
        }
   }
}

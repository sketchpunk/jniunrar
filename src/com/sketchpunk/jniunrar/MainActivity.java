package com.sketchpunk.jniunrar;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import android.os.Bundle;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.view.Menu;
import android.widget.ImageView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        //TEST: Just simple IO between Java and C++ with JNI
        //System.out.println(unrar.getVersion());
        
        /*TEST : Get list of filtered files from the rar based on the list of EXT.
        String[] ary = unrar.getEntries("/sdcard/test.rar",".JPG,.PNG,.jpeg,.gif");
        if(ary != null){
        	System.out.println(ary.length);
        	for(int i=0; i < ary.length; i++){
        		System.out.println(ary[i]);
        	}//for
        }//if
		*/

        /*TEST: Reading image out of rar, get its width+height then scale it down and display it in an image view.*/
        byte[] bAry = unrar.extractEntryToArray("/sdcard/test.rar","Highschool_of_the_Dead_c29-IRF\\48.jpg");
        if(bAry != null){
        	System.out.println(bAry.length);

        	ByteArrayInputStream iStream = new ByteArrayInputStream(bAry);
        	Bitmap bmp = null;
        	
        	BitmapFactory.Options bmpOption = new BitmapFactory.Options();
        	bmpOption.inJustDecodeBounds = true; //we only want header info, not pixel
        	BitmapFactory.decodeStream(iStream,null,bmpOption);
        	
        	System.out.println(bmpOption.outHeight);
        	System.out.println(bmpOption.outWidth);
        	
        	int maxSize = 900;
        	int scale = 0;
	        	
        	if(Math.max(bmpOption.outHeight,bmpOption.outWidth) > maxSize){
        		if(bmpOption.outHeight > bmpOption.outWidth) scale = Math.round((float)bmpOption.outHeight / maxSize);
        		else scale = Math.round((float)bmpOption.outWidth / maxSize);
        	}//if

        	bmpOption.inSampleSize = scale;
        	bmpOption.inJustDecodeBounds = false;
        	bmpOption.inScaled = false;
        	
        	iStream.reset();
        	bmp = BitmapFactory.decodeStream(iStream,null,bmpOption);

        	ImageView imageView1 = (ImageView) findViewById(R.id.imageView1);
        	imageView1.setImageBitmap(bmp);
        	
        	try{
        		iStream.close();
        	}catch(Exception e){}
        }else System.out.println("no data");
        
        
    }//func

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}

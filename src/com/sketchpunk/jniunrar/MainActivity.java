package com.sketchpunk.jniunrar;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        System.out.println(unrar.getVersion());
        String[] ary = unrar.getEntries("/sdcard/test.rar",".JPG,.PNG,.jpeg,.gif");
        if(ary != null){
        	System.out.println(ary.length);
        	for(int i=0; i < ary.length; i++){
        		System.out.println(ary[i]);
        	}//for
        }//if
    }//func

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}

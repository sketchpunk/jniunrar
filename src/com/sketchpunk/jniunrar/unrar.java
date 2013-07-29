package com.sketchpunk.jniunrar;

public class unrar{
    static{ 
    	try{
    		System.loadLibrary("jniunrar");
    	}catch(UnsatisfiedLinkError e){
    		e.printStackTrace();
    	}//try
    }//static
    
	public static native String getVersion();
	public static native String[] getListing(String filePath);
	public static native byte[] getBytes(String filePath,String itemName);
}//cls

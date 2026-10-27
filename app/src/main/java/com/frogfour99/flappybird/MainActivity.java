package com.frogfour99.flappybird;

import android.app.NativeActivity;
import android.os.Bundle;

public class MainActivity extends NativeActivity {
    
    static {
        System.loadLibrary("main");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

}
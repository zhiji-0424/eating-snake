package net.zhiji.snake;

import android.app.NativeActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import java.util.concurrent.LinkedBlockingQueue;

public class Main extends NativeActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        unicodeCharacterQueue = new LinkedBlockingQueue<Integer>();
        init_ui();
    }

    public void init_ui() {
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT >= android.os.Build.VERSION_CODES.KITKAT) {
            setImmersiveSticky();
            View decorView = getWindow().getDecorView();
            decorView.setOnSystemUiVisibilityChangeListener
            (new View.OnSystemUiVisibilityChangeListener() {
                    @Override
                    public void onSystemUiVisibilityChange(int visibility) {
                        setImmersiveSticky();
                    }
                });
        }
    }

    public void setImmersiveSticky() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
                                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }
    
    // 必须new一个
    private LinkedBlockingQueue<Integer> unicodeCharacterQueue;
    
    public int pollUnicodeChar() {
        Integer unicode_char = unicodeCharacterQueue.poll();
        return unicode_char!=null ? unicode_char : 0;
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (event.getAction() == KeyEvent.ACTION_DOWN) {
            int unicode_char = event.getUnicodeChar(event.getMetaState());
            unicodeCharacterQueue.offer(new Integer(unicode_char));
        }
        return super.dispatchKeyEvent(event);
    }
    
    public void showSoftInput() {
        InputMethodManager imm = (InputMethodManager)getSystemService(INPUT_METHOD_SERVICE);
        imm.showSoftInput(this.getWindow().getDecorView(), 0);
    }
    
    public void hideSoftInput() {
        InputMethodManager imm = (InputMethodManager)getSystemService(INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(this.getWindow().getDecorView().getWindowToken(), 0);
    }
}

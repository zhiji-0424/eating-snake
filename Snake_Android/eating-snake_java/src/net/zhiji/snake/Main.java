package net.zhiji.snake;

import android.app.NativeActivity;
import android.os.Bundle;
import android.view.View;

public class Main extends NativeActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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
    
    private LinkedBlockingQueue<Integer> unicodeCharacterQueue;
    
    public int pollUnicodeChar() {
        return unicodeCharacterQueue.poll();
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (event.getAction() == KeyEvent.ACTION_DOWN) {
            unicodeCharacterQueue.offer(event.getUnicodeChar(event.getMetaState()));
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

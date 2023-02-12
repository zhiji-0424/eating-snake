package net.zhiji.snake;

import android.app.NativeActivity;
import android.os.Bundle;
import android.view.View;

public class Main extends NativeActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        CrashHandler.init(getApplication(), getExternalFilesDir("") + "/crash_handler.log");

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

    // TODO: 使用更兼容的方法
    public void setImmersiveSticky() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
                                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }
}

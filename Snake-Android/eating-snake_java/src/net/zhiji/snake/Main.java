package net.zhiji.snake;

import android.app.AlertDialog;
import android.app.NativeActivity;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class Main extends NativeActivity {

    EditText editor = null;
    String edited_string = null;
    boolean is_inputing = false;

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

    public void showInputDialog() {
        edited_string = "";
        runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    AlertDialog.Builder bd = new AlertDialog.Builder(Main.this);
                    bd.setTitle(R.string.input_dialog_title);
                    editor = new EditText(Main.this);
                    bd.setView(editor);
                    bd.setPositiveButton(R.string.input_ok, new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                is_inputing = false;
                                edited_string = editor.getText().toString();
                            }
                        });
                    bd.setNegativeButton(R.string.input_cancel, new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                is_inputing = false;
                            }
                        });
                    bd.setCancelable(false);    //防止误触退出
                    bd.show();
                    is_inputing = true;
                }
            });
    }

    public boolean isInputing() {
        return is_inputing;
    }

    public String getEditedString() {
        String ret = edited_string;
        edited_string = "";
        return ret;
    }
}

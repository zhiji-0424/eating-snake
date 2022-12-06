package net.zhiji.snake;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.channels.FileChannel;

public class PreMain extends Activity {

    private String lib_name = "libmain.so";
    private String error = null;
    private boolean debug_mode = true;  // 改这里

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new View(this));

        if (debug_mode) {
            f1();
        } else {
            startActivity(new Intent(this, Main.class));
            finish();
        }
    }

    private void f1() {
        String from = getExternalFilesDir("") + "/" + lib_name;
        String to   = getFilesDir() + "/" + lib_name;
        if (copy_library(from, to) == 1) {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setCancelable(false);
            builder.setTitle(R.string.CopyError_title);
            builder.setMessage(getText(R.string.CopyError_message) + error);
            builder.setPositiveButton(getText(R.string.CopyError_exit),
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface p1, int p2) {
                        finish();
                    }
                });
            builder.show();
        } else {
            startActivity(new Intent(this, Main.class));
            finish();
        }
    }

    private int copy_library(String from, String to) {
        try {
            FileChannel channel_from = new FileInputStream(from).getChannel();
            FileChannel channel_to = new FileOutputStream(to).getChannel();
            channel_to.transferFrom(channel_from, 0, channel_from.size());
            channel_from.close();
            channel_to.close();
            return 0;
        } catch (Exception ex) {
            error = ex.toString();
            return 1;
        }
    }
}

package helixdown.testing.com.ndk_file_encrypt;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    static String fileName = "heihei.jpg";
    static String cryptFileName = "heihei_crypt.jpg";
    static String decryptFileName = "heihei_decrypt.jpg";

    static String fileNameNormal = "VID4.mp4";
    static String diffFileName_pattern = "VID4_%d";
    static String patchFileName = "VID4_patch.mp4";

    static int REQUEST_READ_EXTERNAL_STORAGE = 0;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

//        动态申请权限
        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            Log.i("permission", "request READ_EXTERNAL_STORAGE");
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    REQUEST_READ_EXTERNAL_STORAGE);
        }else {
            Log.i("permission", "READ_EXTERNAL_STORAGE already granted");
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String permissions[], @NonNull int[] grantResults) {
        if (requestCode == REQUEST_READ_EXTERNAL_STORAGE) {
            // If request is cancelled, the result arrays are empty.
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // permission was granted
                Log.i("permission", "READ_EXTERNAL_STORAGE granted");
//                read_external_storage_granted = true;
            } else {
                // permission denied
                Log.e("permission", "READ_EXTERNAL_STORAGE refused");
//                read_external_storage_granted = false;
            }
        }
    }

    public void Crypt(View v){
        String normaPath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar +  fileName;
        String cryptPath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar + cryptFileName;
        Cryptor.Encrypt(normaPath,cryptPath);
        Log.e("derek","加密完成");
    }

    public void Decrypt(View v){
        String normaPath =  Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar + cryptFileName;
        String cryptPath =  Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar + decryptFileName;
        Cryptor.Decrypt(normaPath,cryptPath);
        Log.e("derek","解密完成");
    }

    public void Diff(View v){
        String normaPath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar +  fileNameNormal;
        String path_pattern = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar + diffFileName_pattern;
        NDKFilePatch.Diff(normaPath,path_pattern,9);
        Log.e("derek","拆包完成");
    }

    public void Patch(View v){
        String path_pattern = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar + diffFileName_pattern;
        String path_patch = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separatorChar +  patchFileName;
        NDKFilePatch.Patch(path_pattern,9,path_patch);
        Log.e("derek","合并完成");
    }

}

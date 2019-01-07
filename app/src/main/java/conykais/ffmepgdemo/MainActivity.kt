package conykais.ffmepgdemo

import android.content.Context
import android.os.Bundle
import android.os.Environment
import android.support.v7.app.AppCompatActivity
import android.view.Surface
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        run(this).toString()

        button.setOnClickListener {
            val name = editText.text.toString()
            val url = Environment.getExternalStorageDirectory().absolutePath + "/youtube-dl/" + name
            object : Thread(){
                override fun run() {
                    super.run()
                    play(url,surfaceView.holder.surface)
                }
            }.start()
        }
        buttonSong.setOnClickListener {
            val url = Environment.getExternalStorageDirectory().absolutePath + "/youtube-dl/Red.m4a"
            object : Thread(){
                override fun run() {
                    super.run()
                    AudioPlayer().playAudio(url)
                }
            }.start()
        }
        buttonSong_openel.setOnClickListener {
            val url = Environment.getExternalStorageDirectory().absolutePath + "/youtube-dl/Red.m4a"
            object : Thread(){
                override fun run() {
                    super.run()
                    AudioPlayer().playAudioByOpenSL(url)
                }
            }.start()

        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    external fun stringFromJNI(): String

    external fun run(context: Context) : Int

    external fun play(url : String, surface : Surface) : Int

    fun callFromJNI(string: String){
        Toast.makeText(this,"call form JNI $string", Toast.LENGTH_SHORT).show()
    }

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}

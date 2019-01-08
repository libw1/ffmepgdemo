package conykais.ffmepgdemo

import android.media.AudioFormat
import android.media.AudioManager
import android.media.AudioTrack
import android.view.Surface

class MediaPlayer {

    external fun setup(filePath : String, surface : Surface) : Int
    external fun play() : Int
    external fun release()

    public fun createAudioTrack(sampleRate : Int, channels : Int) : AudioTrack {
        val audioFormat = AudioFormat.ENCODING_PCM_16BIT
        var channelConfig = if(channels == 1){
            AudioFormat.CHANNEL_OUT_MONO
        }else if(channels == 2){
            AudioFormat.CHANNEL_OUT_STEREO
        }else{
            AudioFormat.CHANNEL_OUT_STEREO
        }

        val bufferSizeInBytes = AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat)

        return AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, channelConfig, audioFormat,
                bufferSizeInBytes, AudioTrack.MODE_STREAM)
    }
}
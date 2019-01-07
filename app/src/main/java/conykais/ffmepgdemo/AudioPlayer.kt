package conykais.ffmepgdemo

import android.media.AudioFormat
import android.media.AudioManager
import android.media.AudioTrack

class AudioPlayer {

    external fun playAudio(audioFile : String)
    //调用OpenSL ES播放
    external fun playAudioByOpenSL(audioPath : String)
    //调用OpenSL ES播放
    external fun stopOpenSL(audioPath : String)

    public fun createAudioTrack(sampleRate : Int, channels : Int) : AudioTrack{
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
#include <jni.h>
#include <string>
#include <stdio.h>
#include <unistd.h>

extern "C" {
#include "ffmpeg/include/libavcodec/avcodec.h"
#include "ffmpeg/include/libavformat/avformat.h"
#include "ffmpeg/include/libswscale/swscale.h"
#include "ffmpeg/include/libavutil/imgutils.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
}
#ifdef ANDROID

#include <android/log.h>
#include "ffmpeg/include/libavutil/frame.h"

#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, "(>_<)", format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "(^_^)", format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  printf("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf("(^_^) " format "\n", ##__VA_ARGS__)
#endif
extern "C" JNIEXPORT jstring

JNICALL
Java_conykais_ffmepgdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C" JNIEXPORT jint

JNICALL
Java_conykais_ffmepgdemo_MainActivity_run(JNIEnv *env, jobject object,jobject context) {
//    std::string aaaa = "Hello ni hao!";
//    jstring p = env->NewStringUTF(aaaa.c_str());
//    char * className = "conykais/ffmepgdemo/MainActivity";
//    jclass clazz = env->FindClass(className);
//    jmethodID method = env->GetMethodID(clazz,"callFromJNI","(Ljava/lang/String;)V");
//    env->CallVoidMethod(object,method,p);

//    jclass tclss = env->FindClass("android/widget/Toast");
//    jmethodID mid = env->GetStaticMethodID(tclss,"makeText","(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
//    jobject job = env->CallStaticObjectMethod(tclss,mid,context,p);
//    jmethodID showId = env->GetMethodID(tclss,"show","()V");
//    env->CallVoidMethod(job,showId,context,p);
    return 12345;
}

extern "C" JNIEXPORT jint

JNICALL
Java_conykais_ffmepgdemo_MainActivity_play(JNIEnv *env, jobject instance,jstring url,jobject surface) {
    LOGI("play start");

    const char *file_name = env->GetStringUTFChars(url,NULL);
    LOGI("file name : %s\n",file_name);

    //register all
    av_register_all();
    avformat_network_init();

    AVFormatContext *pFormatCtx = avformat_alloc_context();

    //open video file
    if (avformat_open_input(&pFormatCtx, file_name, NULL, NULL) != 0){
        LOGE("Could not open file : %s\n", file_name);
        return -1;
    }
    
    //get stream information
    if (avformat_find_stream_info(pFormatCtx,NULL) < 0){
        LOGE("can not find stream information!");
        return -1;
    }
    
    //find first video stream
    int videoStream = -1, i;
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO
                && videoStream < 0){
            videoStream = i;
            break;
        }
    }
    
    if(videoStream == -1){
        LOGE("can not find a video stream or audio stream !");
        return -1;
    }
    
    LOGI("找到视频流");
    AVCodecParameters *pCodecPar = pFormatCtx->streams[videoStream]->codecpar;
    //查找解码器
    //获取一个适合的编码器pCodec
    AVCodec *pCodec = avcodec_find_decoder(pCodecPar->codec_id);
    if (pCodec == NULL){
        LOGE("can not find Codec.\n");
        return -1;
    }

    LOGI("获取解码器");
    //打开解码器
    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);

    //copy context
    if (avcodec_parameters_to_context(pCodecCtx,pCodecPar) != 0){
        fprintf(stderr,"can not copy codec context!");
        return -1;
    }

    LOGI("视频流帧率 :%d fps\n", pFormatCtx->streams[videoStream]->r_frame_rate.num
                            / pFormatCtx->streams[videoStream]->r_frame_rate.den);

    int iTotalSeconds = (int) pFormatCtx->duration / 1000000;
    int iHour = iTotalSeconds / 3600 / 60;
    int iMinute = iTotalSeconds % 3600 / 60;
    int iSecond = iTotalSeconds % 60;

    LOGI("持续时间:%02d:%d02d:%02d\n", iHour,iMinute, iSecond);

    LOGI("视频时长:%02d \n", pFormatCtx->streams[videoStream]->duration);
    LOGI("持续时间:%02d \n", (int)pFormatCtx->duration);

    LOGI("获取解码器SUCESS");

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
        LOGE("can not open codec.");
        return -1;
    }

    LOGI("获取native window");
    //获取native window
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    LOGI("获取视频高宽");
    //获取视频高宽
    int videoWidth = pCodecCtx->width;
    int videoHeight = pCodecCtx->height;
    LOGI("设置native window的buffer大小,可自动拉伸");
    // 设置native window的buffer大小,可自动拉伸
    ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer window_buffer;
    LOGI("Allocate video frame");
    // Allocate video frame
    AVFrame *pFrame = av_frame_alloc();
    LOGI("用于渲染");
    //用于渲染
    AVFrame *pFrameRGBA = av_frame_alloc();
    if (pFrameRGBA == NULL || pFrame == NULL){
        LOGE("can not allocate video frame");
        return -1;
    }

    LOGI("Determine required buffer size and allocate buffer");
    // Determine required buffer size and allocate buffer
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height,1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, buffer ,AV_PIX_FMT_RGBA, pCodecCtx->width,
                         pCodecCtx->height, 1);
    LOGI("由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换");
    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                                pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_RGBA,
                                                SWS_BICUBIC,NULL,NULL,NULL);
    if (sws_ctx == NULL){
        LOGE("can not init the conversion context!\n");
        return -1;
    }

    LOGI("格式转换成功");
    LOGE("开始播放");
    int ret;
    AVPacket packet;
    while (av_read_frame(pFormatCtx, &packet) >= 0){
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream){
            //该帧位置
            float timestamp = packet.pts * av_q2d(pFormatCtx->streams[videoStream]->time_base);
            LOGI("timestamp=%f", timestamp);
            //解码
            ret = avcodec_send_packet(pCodecCtx, &packet);
            if (ret < 0){
                break;
            }
            while (avcodec_receive_frame(pCodecCtx, pFrame) == 0){
                //lock native window buffer
                ANativeWindow_lock(nativeWindow, &window_buffer,0);
                //格式转换
                sws_scale(sws_ctx, (uint8_t const *const *)pFrame->data, pFrame->linesize, 0,pCodecCtx->height, pFrameRGBA->data, pFrameRGBA->linesize);
                //获取stride
                uint8_t *dst = (uint8_t *)window_buffer.bits;
                int dstStride = window_buffer.stride * 4;
                uint8_t *src = pFrameRGBA->data[0];
                int srcStride = pFrameRGBA->linesize[0];

                // 由于window的stride和帧的stride不同,因此需要逐行复制
                for (int h = 0; h < videoHeight; h++) {
                    memcpy(dst + h * srcStride, src + h * srcStride, srcStride);
                }
                ANativeWindow_unlockAndPost(nativeWindow);
            }

        }
        av_packet_unref(&packet);
    }
    LOGE("播放完成");
    av_free(buffer);
    av_free(pFrameRGBA);
    av_free(pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}


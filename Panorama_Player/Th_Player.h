#pragma once

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavutil/mathematics.h"
#include "libswresample/swresample.h"
#include "libavdevice/avdevice.h" 

#include "inttypes.h"
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_image.h"
}

#include "MyQueue.h"
#include "VideoWindow.h"

#define MAX_AUDIO_FRAME_SIZE 1920000
#define OUT_SAMPLE_RATE (48000)


typedef struct PacketQueue 
{
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	//SDL_mutex *mutex;
	int IsInit;
	CCriticalSection PktQueuelock;
	//SDL_cond *cond;
} PacketQueue;


typedef struct ShowFrame 
{
	int FrameNUM;
	int64_t FramePTS;
	int64_t FrameDuration;

	//AVFrame * pFrame;

	AVPicture *ShowPic;
	int haveData;
	uint8_t *data;
	int linesize;
	
	CCriticalSection ShowFramelock;
} ShowFrame;


typedef struct AudioSwrInfo 
{
	int out_sample_rate;
	int out_channels;
	int out_channels_layout;
	AVSampleFormat out_sample_fmt;

	int in_sample_rate;
	int in_channels;
	int in_channels_layout;
	AVSampleFormat in_sample_fmt;
	
} AudioSwrInfo;


#define ShowFrameBufferMax 1

// Th_Player
class Th_Player : public CWnd
{
	DECLARE_DYNAMIC(Th_Player)

public:
	Th_Player();
	virtual ~Th_Player();

//------------------------------------------
//自己加的
	int Type;

	int screen_w,screen_h;
	
	int FullW,FullH;
	
	AVFormatContext	*pFormatCtx;
	AVFrame	*pVidFrame;
	AVFrame	*pVidFrameYUV;
	struct SwsContext *img_convert_ctx;
	//uint8_t *out_buffer;
	//AVPacket  *Packet;

	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Surface * sdlSurface;
	SDL_Surface * sdlBlitSurface;
	SDL_Rect TextureRect;	
	CCriticalSection Texturelock;

	SDL_Thread * demux_tid;
	SDL_Thread * Video_decoder_tid;
	SDL_Thread * Audio1_decoder_tid;
	SDL_Thread * Audio2_decoder_tid;

	int FrameRate;

	int videoindex;
	int audioindex1;
	int audioindex2;

	AVCodecContext *pVidCodecCtx;
	AVCodecContext *pAudCodecCtx1;
	AVCodecContext *pAudCodecCtx2;

	AVCodec *pVidCodec;
	AVCodec *pAudCodec1;
	AVCodec *pAudCodec2;

	PacketQueue     videoq;
	PacketQueue     audioq1;	
	PacketQueue     audioq2;
	
	AudioSwrInfo audio1_Swr;
	AudioSwrInfo audio2_Swr;
	HANDLE		hAudioReadPipe;    
	HANDLE		hAudioWritePipe;

	void packet_queue_flush(PacketQueue *q);
	void packet_queue_init(PacketQueue *q);
	void packet_queue_deinit(PacketQueue *q);
	int packet_queue_put(PacketQueue *q, AVPacket *pkt);
	int packet_queue_get(PacketQueue *q, AVPacket *pkt);


	ShowFrame ShowFrameBuffer[ShowFrameBufferMax];
	ShowFrame CurShowFrameBuffer;
	
	int ShowFrameBufferPutCount;
	int ShowFrameBufferGetCount;
	AVPixelFormat PixelFormat;
	void UpdataUpdateTexture();
	int OldFrameNUM;

	char ffer[AV_ERROR_MAX_STRING_SIZE];
	char * av_fferr(int Ret){
		memset(ffer,0,AV_ERROR_MAX_STRING_SIZE);
		return av_make_error_string(ffer, AV_ERROR_MAX_STRING_SIZE, Ret);
	}




//------------------------------------------	


protected:
	DECLARE_MESSAGE_MAP()
public:
	int Play_Run;
	int play_end;	

	int PlayerIsInit;
	int Init_Player(int type,SDL_Renderer * renderer);
	int Deinit_Player(void);

	int Reset_Player(void);
	
	int PlayerIsOpen;
	int Open_Player(char * url);
	int Close_Player(void);

	int	PlayerIsPlay;
	int Play_Player(void);
	int Stop_Player(void);

	int GetVideoSize(int *width,int *high);
	

	//Clist<ShowFrame *,ShowFrame *>ShowFrameList;

	MyQueue ShowFrameQueue;
	
	//void display_pic();
	void display_pic(ShowFrame * pShowPic);

	//pthread_mutex_t RenderMutex;

	//CCriticalSection Renderlock;
	
	//SDL_mutex *RenderMutex;

	int Create_SDL_Texture();
	int Destroy_SDL_Texture();

	SDL_AudioSpec wanted_spec;

	
	int Open_SDL_Audio();
	int Close_SDL_Audio();

	int isShow;
	int GetWindowIsShow(){return isShow;};
	void ShowWindow(){isShow = 1;};
	void HideWindow(){isShow = 0;};
	void ChargeShowRect(const SDL_Rect rect);
	void SaveShowRectToINI();
};



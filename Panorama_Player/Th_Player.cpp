// Th_Player.cpp : 实现文件
//

#include "stdafx.h"
#include "Panorama_Player.h"
#include "Th_Player.h"

// Th_Player

IMPLEMENT_DYNAMIC(Th_Player, CWnd)

Th_Player::Th_Player()
{
	PlayerIsInit = 0;
	PlayerIsOpen = 0;
	videoindex = -1;
	audioindex1 = -1;
	audioindex2 = -1;

	pVidCodecCtx = NULL;
	pAudCodecCtx1 = NULL;
	pAudCodecCtx2 = NULL;

	pVidCodec = NULL;
	pAudCodec1 = NULL;
	pAudCodec2 = NULL;
	pVidFrame = NULL;
	pVidFrameYUV = NULL;
	img_convert_ctx = NULL;

	sdlRenderer = NULL;
	sdlTexture = NULL;
	sdlSurface = NULL;

	TextureRect.h = 0;
	TextureRect.w = 0;
	TextureRect.x = 0;
	TextureRect.y = 0;

	
	PlayerIsPlay = 0;
	play_end = 1;
	Play_Run = 0;
	//RenderMutex = NULL;
	FrameRate = 1;
	
	demux_tid = NULL;
	Video_decoder_tid = NULL;
	Audio1_decoder_tid = NULL;
	Audio2_decoder_tid = NULL;

	videoq.IsInit = 0;
	audioq1.IsInit = 0;	
	audioq2.IsInit = 0;
	//PixelFormat = AV_PIX_FMT_YUV420P;
	PixelFormat = AV_PIX_FMT_BGRA;
	OldFrameNUM = 0;
}

Th_Player::~Th_Player()
{

}

BEGIN_MESSAGE_MAP(Th_Player, CWnd)
END_MESSAGE_MAP()

void Th_Player::packet_queue_flush(PacketQueue *q)
{	
	AVPacketList *pkt, *pkt1;
	if(!q->IsInit)
		return ;

	q->PktQueuelock.Lock();
    for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1) {
            pkt1 = pkt->next;
			if(pkt->pkt.data)
           	 	av_free_packet(&pkt->pkt);
            av_freep(&pkt);
    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
	q->PktQueuelock.Unlock();
}

void Th_Player::packet_queue_init(PacketQueue *q)
{
	if(q->IsInit)
		return ;
	InitializeCriticalSection(q->PktQueuelock);
	q->PktQueuelock.Lock();
	q->first_pkt = NULL;
	q->last_pkt = NULL;
	q->nb_packets = 0;
	q->size = 0;
	
	q->IsInit = 1;
	q->PktQueuelock.Unlock();
}

void Th_Player::packet_queue_deinit(PacketQueue *q)
{
	if(!q->IsInit)
		return ;	
    packet_queue_flush(q);
	q->PktQueuelock.Lock();
	q->IsInit = 0;
	q->PktQueuelock.Unlock();
	DeleteCriticalSection(q->PktQueuelock);
}

int Th_Player::packet_queue_put(PacketQueue *q, AVPacket *pkt)
{
    AVPacketList *pkt1;

    if(!q->IsInit)
		return -1;  
	/*av_dup_packet, 通过调用 av_malloc、memcpy、memset等函数， 
	将shared buffer 的AVPacket duplicate(复制)到独立的buffer中。
	并且修改AVPacket的析构函数指针av_destruct_pkt。*/
	if(av_dup_packet(pkt) < 0)
        return -1;

    pkt1 = (AVPacketList *)av_malloc(sizeof(AVPacketList));
    if (!pkt1)
            return -1;
    pkt1->pkt = *pkt;
    pkt1->next = NULL;
	q->PktQueuelock.Lock();
    if (!q->last_pkt) {
            q->first_pkt = pkt1;
    } else {
            q->last_pkt->next = pkt1;
    }
    q->last_pkt = pkt1;
    q->nb_packets++;
    q->size += pkt1->pkt.size + sizeof(*pkt1);
    /* XXX: should duplicate packet data in DV case */
	q->PktQueuelock.Unlock();
    return 0;
}

int Th_Player::packet_queue_get(PacketQueue *q, AVPacket *pkt)
{
    AVPacketList *pkt1;
    int ret;
	if(!q->IsInit)
		return -1;  
	//	printf("Enter queue_get \n");
    //SDL_LockMutex(q->mutex);
	q->PktQueuelock.Lock();
    pkt1 = q->first_pkt;
    if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
                    q->last_pkt = NULL;
            q->nb_packets--;
            q->size -= pkt1->pkt.size + sizeof(*pkt1);
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
    }  else {
            ret = -1;
    }
	q->PktQueuelock.Unlock();
    return ret;
}
/*frame packet queue func define end*/


void initShowFrameBuffer(ShowFrame * pFrame,int dataSize)
{
	if(pFrame){
		pFrame->FrameNUM = 0;
		pFrame->FramePTS = 0;
		pFrame->FrameDuration = 0;
		pFrame->linesize = 0;
		pFrame->ShowPic = NULL;
		pFrame->haveData = 0;
		pFrame->data = (uint8_t *)av_malloc(dataSize);
		InitializeCriticalSection(pFrame->ShowFramelock);
	}
}
void resetShowFrameBuffer(ShowFrame * pFrame,int dataSize)
{
	if(pFrame){
		pFrame->FrameNUM = 0;
		pFrame->FramePTS = 0;
		pFrame->FrameDuration = 0;
		pFrame->linesize = 0;
		pFrame->ShowPic = NULL;
		pFrame->haveData = 0;
		if(pFrame->data)
			memset(pFrame->data,0,dataSize);
	}
}

void deinitShowFrameBuffer(ShowFrame * pFrame)
{
	if(pFrame){
		pFrame->FrameNUM = 0;
		pFrame->FramePTS = 0;
		pFrame->FrameDuration = 0;
		pFrame->linesize = 0;
		pFrame->ShowPic = NULL;
		pFrame->haveData = 0;
		av_free(pFrame->data);
		pFrame->data = NULL;
		DeleteCriticalSection(pFrame->ShowFramelock);
	}
}


//初始化ffmpeg SDL
int Th_Player::Init_Player(int type,SDL_Renderer * renderer)
{
	int i = 0;
	if(PlayerIsInit)
		return 0;
	sdlRenderer = renderer;
	Type = type;
	Create_SDL_Texture();
	pFormatCtx = NULL;
	for(i = 0;i<ShowFrameBufferMax;i++){
		initShowFrameBuffer(ShowFrameBuffer + i,avpicture_get_size(PixelFormat, screen_w,screen_h));
	}
	initShowFrameBuffer(&CurShowFrameBuffer,avpicture_get_size(PixelFormat, screen_w,screen_h));
		
	ShowFrameBufferPutCount = 0;
	ShowFrameBufferGetCount = 0;
	PlayerIsInit = 1;
	
	OldFrameNUM = 0;
	ShowWindow();
	return 0;
}

//去初始化ffmpeg SDL
int Th_Player::Deinit_Player()
{	
	if(!PlayerIsInit)
		return 0;

	/*if(pFormatCtx){
		avformat_close_input(&pFormatCtx);
		pFormatCtx = NULL;
	}
	*/
	
	Destroy_SDL_Texture();
//	avformat_network_deinit();
	int i = 0;
	for(i = 0;i<ShowFrameBufferMax;i++){
		deinitShowFrameBuffer(ShowFrameBuffer + i);
	}	
	deinitShowFrameBuffer(&CurShowFrameBuffer);
	
	ShowFrameBufferPutCount = 0;
	ShowFrameBufferGetCount = 0;
	PlayerIsInit = 0;
	OldFrameNUM = 0;
	Reset_Player();
	return 0;
}

//重置缓冲，播放状态，播放参数
int Th_Player::Reset_Player(void)
{	
	//PlayerIsInit = 0;
	PlayerIsOpen = 0;
	videoindex = -1;
	audioindex1 = -1;
	audioindex2 = -1;

	pVidCodecCtx = NULL;
	pAudCodecCtx1 = NULL;
	pAudCodecCtx2 = NULL;

	pVidCodec = NULL;
	pAudCodec1 = NULL;
	pAudCodec2 = NULL;
	pVidFrame = NULL;
	pVidFrameYUV = NULL;

	img_convert_ctx = NULL;
	
	PlayerIsPlay = 0;
	play_end = 1;
	
	Play_Run = 0;
	FrameRate = 1;
	
	demux_tid = NULL;
	Audio1_decoder_tid = NULL;
	Audio2_decoder_tid = NULL;
	
	ShowFrameBufferPutCount = 0;
	ShowFrameBufferGetCount = 0;
	OldFrameNUM = 0;
	int i = 0;
	for(i = 0;i<ShowFrameBufferMax;i++){
		resetShowFrameBuffer(ShowFrameBuffer + i,avpicture_get_size(PixelFormat, screen_w,screen_h));
	}
	//resetShowFrameBuffer(&CurShowFrameBuffer,avpicture_get_size(PixelFormat, screen_w,screen_h));
	return 0;
}


int Th_Player::GetVideoSize(int *width,int *height)
{
	if(pVidCodecCtx){
		*width = pVidCodecCtx->width;
		*height = pVidCodecCtx->height;
		
	}else{
		*width = 0;
		*height = 0;
	}
	return 0;
}

static int decode_interrupt_cb(void *ctx)
{
	int stream_id = (int)(ctx);
	printf("stream_id == %d\n",stream_id);
	return 0;
}

//ffmpeg 打开url并解析数据，寻找解码器，打开解码器，申请资源（SDL，ffmpeg）
int Th_Player::Open_Player(char * url)
{
	printf("--------- Open_Player start %d\n",Type);
	int Ret = -1,i = 0;
	if(!PlayerIsInit){
		Ret = 0;
		goto EXIT;
	}
	pFormatCtx = avformat_alloc_context();
	//pFormatCtx->interrupt_callback.callback = decode_interrupt_cb;
	//pFormatCtx->interrupt_callback.opaque = (void *)0x7878;
	
	if(Type == 1){//USB摄像头 HDMI_IN DShow 设备
		AVInputFormat *ifmt=av_find_input_format("dshow");
		if((avformat_open_input(&pFormatCtx,url,ifmt,NULL)!=0)||(pFormatCtx == NULL)){
		//if((avformat_open_input(&pFormatCtx,"video=USB2.0 PC CAMERA",ifmt,NULL)!=0)||(pFormatCtx == NULL)){
			printf("Couldn't open usb camera device %s.\n",url);
			goto EXIT;
		}
	}else{
		if(url == NULL){
			goto EXIT;
		}
		if((avformat_open_input(&pFormatCtx,url,NULL,NULL)!=0)|| (pFormatCtx == NULL)){
			printf("Couldn't open input stream %s.\n",url);
			goto EXIT;
		}
	}
	if(Type == 1){
		pFormatCtx->flags|= AVFMT_FLAG_NOBUFFER;
	}
	
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		goto EXIT;
	}

	av_dump_format(pFormatCtx,0,url,0);
	//寻找视频，音频流
	videoindex = -1;
	audioindex1 = -1;
	audioindex2 = -1;
	for(i = 0;i< pFormatCtx->nb_streams;i++){
		if(pFormatCtx->streams[i]->codec->codec_type ==  AVMEDIA_TYPE_VIDEO){
			videoindex = i;
		}else if(pFormatCtx->streams[i]->codec->codec_type ==  AVMEDIA_TYPE_AUDIO){
			if(audioindex1 == -1){
				audioindex1 = i;
			}else{
				audioindex2 = i;
				break;
			}
		}
	}
	//videoindex = -1;

	//if(Type != 1){
		//audioindex1 = -1;
		//audioindex2 = -1;
	//}
	if(Type != 0)
	{
		audioindex1 = -1;
	}
	audioindex2 = -1;
	//打开video 解码器
	
	if(videoindex >= 0){
		pVidCodecCtx=pFormatCtx->streams[videoindex]->codec;
		pVidCodec=avcodec_find_decoder(pVidCodecCtx->codec_id);
		if(pVidCodec==NULL){
			printf("Codec not found %d\n",Type);
			goto EXIT;
		}			
		//pVidCodecCtx->thread_count = 1; 默认值
		//多线程解码
		pVidCodecCtx->thread_count = 4;
//		pVidCodecCtx->thread_count = 16;
		//......

		if(avcodec_open2(pVidCodecCtx, pVidCodec,NULL)<0){
			printf("Could not open codec %d\n",Type);
			goto EXIT;
		}
		FrameRate = av_q2d(pFormatCtx->streams[videoindex]->r_frame_rate);
		packet_queue_init(&videoq);
	}
	//打开audio1 解码器
	#if 1
	if(audioindex1 >= 0){
		pAudCodecCtx1=pFormatCtx->streams[audioindex1]->codec;
		pAudCodec1=avcodec_find_decoder(pAudCodecCtx1->codec_id);
		if(pAudCodec1==NULL){
			printf("Codec not found.\n");
			goto EXIT;
		}			
		pAudCodecCtx1->thread_count = 0;
		
		if(avcodec_open2(pAudCodecCtx1, pAudCodec1,NULL)<0){
			printf("Could not open codec.\n");
			goto EXIT;
		}
		audio1_Swr.in_channels = pAudCodecCtx1->channels;
		audio1_Swr.in_channels_layout= av_get_default_channel_layout(pAudCodecCtx1->channels);
		
		audio1_Swr.in_sample_fmt = pAudCodecCtx1->sample_fmt;
		audio1_Swr.in_sample_rate = pAudCodecCtx1->sample_rate;

		packet_queue_init(&audioq1);
		if(Open_SDL_Audio() != 0){
			printf("Open_SDL_Audio call fail.\n");
			audioindex1 = -1;
			if(pAudCodecCtx1){
				avcodec_close(pAudCodecCtx1);
				pAudCodecCtx1 = NULL;
			}
			packet_queue_deinit(&audioq1);
		}
		
	}
	#endif
	#if 0
	//打开audio2 解码器
	if(audioindex2 >= 0){
		pAudCodecCtx2=pFormatCtx->streams[audioindex2]->codec;		
		pAudCodec2=avcodec_find_decoder(pAudCodecCtx2->codec_id);
		if(pAudCodec2==NULL){
			printf("Codec not found.\n");
			Ret = -1;
			goto EXIT;
		}
		pAudCodecCtx2->thread_count = 0;
		if(avcodec_open2(pAudCodecCtx2, pAudCodec2,NULL)<0){
			printf("Could not open codec.\n");
			Ret = -1;
			goto EXIT;
		}	
		packet_queue_init(&audioq2);
	}
	#endif
	Ret = 0;
EXIT:
	if(Ret < 0){
		if(pVidCodecCtx){
			avcodec_close(pVidCodecCtx);
			pVidCodecCtx = NULL;
		}
		if(pAudCodecCtx1){
			avcodec_close(pAudCodecCtx1);
			pAudCodecCtx1 = NULL;
		}
		if(pAudCodecCtx2){
			avcodec_close(pAudCodecCtx2);
			pAudCodecCtx2 = NULL;
		}
		if(pFormatCtx){
			avformat_close_input(&pFormatCtx);
			pFormatCtx = NULL;
		}
		packet_queue_deinit(&videoq);
		packet_queue_deinit(&audioq1);
		packet_queue_deinit(&audioq2); 
		PlayerIsOpen = 0;
	}else{
		PlayerIsOpen = 1;
	}
	printf("--------- Open_Player end\n");
	return Ret;
}
//关闭解码器，释放一些资源
int Th_Player::Close_Player(void)
{
	printf("--------- Close_Player start\n");
	if(!PlayerIsOpen){
		return 0;
	}
	if(pVidCodecCtx){
		avcodec_close(pVidCodecCtx);
		pVidCodecCtx = NULL;
	}
	if(pAudCodecCtx1){
		avcodec_close(pAudCodecCtx1);
		pAudCodecCtx1 = NULL;
	}
	if(pAudCodecCtx2){
		avcodec_close(pAudCodecCtx2);
		pAudCodecCtx2 = NULL;
	}
	if(pFormatCtx){
		avformat_close_input(&pFormatCtx);
		pFormatCtx = NULL;
	}	
	packet_queue_deinit(&videoq);
	packet_queue_deinit(&audioq1);
	packet_queue_deinit(&audioq2); 
	PlayerIsOpen = 0;
	Reset_Player();
	printf("--------- Close_Player end %d\n",Type);
	return 0;
}

int Th_Player::Create_SDL_Texture()
{
int winNum = 4;//可变

	int Ret = -1;
	SDL_GetRendererOutputSize(sdlRenderer,&FullW,&FullH);
	
	SDL_GetRendererOutputSize(sdlRenderer,&screen_w,&screen_h);
	/*if(Type!=0){
		screen_w = screen_w/((winNum*2));
		screen_h = screen_h/2;
		if(screen_w%4){
			screen_w+=screen_w%4;
		}
		if(screen_h%4){
			screen_h+=screen_h%4;
		}
		printf("screen_w = %d,screen_h = %d\n",screen_w,screen_h);
	}
	
	if(Type == 0){
		TextureRect.x = 0;
		TextureRect.y = 0;
		TextureRect.h = screen_h;
		TextureRect.w = screen_w;
	}else{
		TextureRect.x = screen_w*(Type*2 -1) - screen_w/2;
		TextureRect.y = screen_h/2;
		TextureRect.h = screen_h;
		TextureRect.w = screen_w;
	}
	*/
	if(!TextureRect.h||!TextureRect.w){
		TextureRect.x = 0;
		TextureRect.y = 0;
		TextureRect.h = screen_h;
		TextureRect.w = screen_w;
	}
	SaveShowRectToINI();
	Texturelock.Lock();
	 if(Type == 1){
		//sdlSurface = NULL;
		//sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,screen_w,screen_h); 
		sdlSurface = SDL_CreateRGBSurface(0,screen_w,screen_h,32,
															0x000000FF,
															0x0000FF00,
															0x00FF0000,
															0xFF000000
															);
		//sdlBlitSurface = IMG_Load("haha1.png");
		sdlBlitSurface = NULL;
		if(!sdlBlitSurface){
			sdlBlitSurface = SDL_CreateRGBSurface(0,screen_w,screen_h,32,
																0x000000FF,
																0x0000FF00,
																0x00FF0000,
																0xFF000000
																);
			SDL_FillRect(sdlBlitSurface,NULL,0xff000000);
		}
		SDL_SetSurfaceBlendMode(sdlBlitSurface,SDL_BLENDMODE_NONE);
		
		sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,screen_w,screen_h); 
	}else{
		sdlBlitSurface = NULL;
		sdlSurface = NULL;
		sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,screen_w,screen_h); 
	}
	Texturelock.Unlock();
	Ret = 0;
EXIT:
	if(Ret < 0){
		Destroy_SDL_Texture();
	}
	return Ret;
}

int Th_Player::Destroy_SDL_Texture()
{
	int Ret = 0;
	if(sdlTexture){
		Texturelock.Lock();
		if(sdlTexture){
			SDL_DestroyTexture(sdlTexture);
			sdlTexture = NULL;
		}
		if(sdlSurface){
			SDL_FreeSurface(sdlSurface);
			sdlSurface = NULL;
		}
		if(sdlBlitSurface){
			SDL_FreeSurface(sdlBlitSurface);
			sdlBlitSurface = NULL;
		}
		Texturelock.Unlock();
	}
	return 0;
}


void Th_Player::UpdataUpdateTexture()
{
	//if(Type == 1){
	//	printf("Old %d  cur %d  %s \n",OldFrameNUM,CurShowFrameBuffer.FrameNUM,(OldFrameNUM == CurShowFrameBuffer.FrameNUM)?"==========":"XXXXXXXXXX");
	//}
	if(!PlayerIsOpen||!PlayerIsPlay || !isShow){
		return;
	}
	/*
	if(Type != 0){
		TextureRect.w = FullW/8;
		TextureRect.h = FullH/2;
		if(TextureRect.w%4){
			TextureRect.w+=TextureRect.w%4;
		}
		if(TextureRect.w%4){
			TextureRect.h+=TextureRect.h%4;
		}
	}
	
	if(Type == 0){
		TextureRect.x = 0;
		TextureRect.y = 0;
		TextureRect.h = screen_h;
		TextureRect.w = screen_w;
	}else{
		TextureRect.x = TextureRect.w*(Type*2 -1) - TextureRect.w/2;
		TextureRect.y = TextureRect.h/2;
		TextureRect.h = TextureRect.h;
		TextureRect.w = TextureRect.w;
	}
	*/
	
	if(CurShowFrameBuffer.haveData){
		if(OldFrameNUM != 0 && OldFrameNUM == CurShowFrameBuffer.FrameNUM){
			SDL_RenderCopy(sdlRenderer,sdlTexture, 0,&TextureRect);
			return;
		}
		OldFrameNUM = CurShowFrameBuffer.FrameNUM;
		CurShowFrameBuffer.ShowFramelock.Lock();
		if(Type == 1&&0){
			//SDL_UpdateTexture(sdlTexture,0,CurShowFrameBuffer.data, CurShowFrameBuffer.linesize);
			SDL_FillRect(sdlSurface,NULL,0x0);
			SDL_BlitSurface(sdlBlitSurface,NULL,sdlSurface,NULL);
			if(1){ 
				SDL_Surface *	pSurface = SDL_CreateRGBSurfaceFrom(CurShowFrameBuffer.data,
											screen_w,screen_h, 32,
											CurShowFrameBuffer.linesize,
	                                        0x00FF0000,
	                                        0x0000FF00,
	                                        0x000000FF,
	                                        0xFF000000);
				if(pSurface){
						SDL_SetSurfaceBlendMode(pSurface,SDL_BLENDMODE_ADD);
						SDL_BlitSurface(pSurface,NULL,sdlSurface,NULL);
						SDL_FreeSurface(pSurface);
				}
			}
			if(sdlTexture){
				SDL_DestroyTexture(sdlTexture);
				sdlTexture = NULL;
			}
			sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer,sdlSurface);
			
		}else{
			SDL_UpdateTexture(sdlTexture,0,CurShowFrameBuffer.data, CurShowFrameBuffer.linesize);
		}
		CurShowFrameBuffer.ShowFramelock.Unlock();
		SDL_RenderCopy(sdlRenderer,sdlTexture, 0,&TextureRect);
	}
}

Uint32 display_pic_callback(Uint32 interval, void *param)  
{
	Th_Player* play = (Th_Player *)param;
	play->ShowFrameBuffer[play->ShowFrameBufferGetCount].ShowFramelock.Lock();
		if(play->ShowFrameBufferGetCount == ShowFrameBufferMax)
			play->ShowFrameBufferGetCount = 0;
		if(play->ShowFrameBuffer[play->ShowFrameBufferGetCount].haveData != 1){
			play->ShowFrameBuffer[play->ShowFrameBufferGetCount].ShowFramelock.Unlock();
			return interval;
		}
		play->CurShowFrameBuffer.ShowFramelock.Lock();
			play->CurShowFrameBuffer.linesize = play->ShowFrameBuffer[play->ShowFrameBufferGetCount].linesize;
			memcpy(play->CurShowFrameBuffer.data,play->ShowFrameBuffer[play->ShowFrameBufferGetCount].data,avpicture_get_size(play->PixelFormat, play->screen_w, play->screen_h));
			play->CurShowFrameBuffer.FrameDuration = play->ShowFrameBuffer[play->ShowFrameBufferGetCount].FrameDuration;
			play->CurShowFrameBuffer.FrameNUM = play->ShowFrameBuffer[play->ShowFrameBufferGetCount].FrameNUM;
			play->CurShowFrameBuffer.FramePTS = play->ShowFrameBuffer[play->ShowFrameBufferGetCount].FramePTS;
			play->CurShowFrameBuffer.haveData = play->ShowFrameBuffer[play->ShowFrameBufferGetCount].haveData;
			play->CurShowFrameBuffer.ShowPic = play->ShowFrameBuffer[play->ShowFrameBufferGetCount].ShowPic;
			//printf("updata CurShowFrameBuffer with FrameNUM %d\n",play->CurShowFrameBuffer.FrameNUM);
		play->CurShowFrameBuffer.ShowFramelock.Unlock();
	play->ShowFrameBuffer[play->ShowFrameBufferGetCount].haveData = 0;
	play->ShowFrameBuffer[play->ShowFrameBufferGetCount].ShowFramelock.Unlock();
	play->ShowFrameBufferGetCount++;
//	printf("interval == %d\n",interval);
	return interval;
}

int Video_decoder_thread(void *lpParam)
{	
	int Ret = 0,got_picture = 0;
	int c = 0;
	AVPacket pkt;
	Th_Player *play = (Th_Player *)lpParam;
	int TimerID = 0;
	printf("--------- Video_decoder_thread start（%d）\n",play->Type);
	//if(play->Type !=  1)

	printf("play[%d]->FrameRate == %d\n",play->Type,play->FrameRate);
	if(play->FrameRate <= 10||play->FrameRate > 60)
		play->FrameRate = 29;
	
	//if(play->Type ==  0)
	TimerID = SDL_AddTimer((1000/play->FrameRate),display_pic_callback,(void *)play);
	
 	int ShowBufferSize = avpicture_get_size(play->PixelFormat, play->screen_w, play->screen_h);
	
	uint8_t *out_buffer = (uint8_t *)av_malloc(ShowBufferSize); 	
	play->pVidFrame = av_frame_alloc(); 
	AVFrame * VidShowFrame = av_frame_alloc();
	
	avpicture_fill((AVPicture *)VidShowFrame, out_buffer, play->PixelFormat, play->screen_w, play->screen_h);

	play->img_convert_ctx = sws_getContext(play->pVidCodecCtx->width, play->pVidCodecCtx->height, play->pVidCodecCtx->pix_fmt, 
								play->screen_w, play->screen_h, play->PixelFormat, SWS_GAUSS, NULL, NULL, NULL); 


	int video_fram_num = 0;
	while(play->Play_Run){
		while(play->Play_Run){
			if(play->packet_queue_get(&play->videoq, &pkt) > 0)
			{
				break;
			}
		}
		//printf("Video_decoder_thread is running（%d）\n",play->Type);
		got_picture = 0;
		//AV_LOG_show = 1;
		Ret = avcodec_decode_video2(play->pVidCodecCtx, play->pVidFrame, &got_picture, &pkt);
		//AV_LOG_show = 0;
		if(Ret < 0){
			char * err = play->av_fferr(Ret);
			printf("Decode Error. %s\n",err);		
			av_free_packet(&pkt);
			continue;
		}
		if(got_picture){//play->pVidFrame->key_frame
			if(play->ShowFrameBufferPutCount == ShowFrameBufferMax)
				play->ShowFrameBufferPutCount = 0;
			while(play->Play_Run){
				if(play->ShowFrameBuffer[play->ShowFrameBufferPutCount].haveData == 0)
					break;
			}
			play->ShowFrameBuffer[play->ShowFrameBufferPutCount].ShowFramelock.Lock();
			play->ShowFrameBuffer[play->ShowFrameBufferPutCount].FrameNUM = video_fram_num++;
			play->ShowFrameBuffer[play->ShowFrameBufferPutCount].FramePTS = pkt.pts;
			play->ShowFrameBuffer[play->ShowFrameBufferPutCount].FrameDuration = pkt.duration;

			Ret = sws_scale(play->img_convert_ctx, (const uint8_t* const*)play->pVidFrame->data, play->pVidFrame->linesize, 0, play->pVidCodecCtx->height, VidShowFrame->data, VidShowFrame->linesize);
			//printf("sws_scale Ret == %d pts %d    data[0] 0x%x\n",Ret,pkt.pts,VidShowFrame->data[0][VidShowFrame->linesize[0]/2]);
			memset(play->ShowFrameBuffer[play->ShowFrameBufferPutCount].data,0,ShowBufferSize);
			memcpy(play->ShowFrameBuffer[play->ShowFrameBufferPutCount].data,VidShowFrame->data[0],ShowBufferSize);
			play->ShowFrameBuffer[play->ShowFrameBufferPutCount].linesize = VidShowFrame->linesize[0];
			play->ShowFrameBuffer[play->ShowFrameBufferPutCount].haveData = 1;
			
			play->ShowFrameBuffer[play->ShowFrameBufferPutCount].ShowFramelock.Unlock();
			play->ShowFrameBufferPutCount++;
		}
		av_free_packet(&pkt);
	}
	
	//if(play->Type != 1)
	while(1){
		if(SDL_RemoveTimer(TimerID))
			break;
	}
	ShowFrame * pShowFrame = NULL;
	while(1){
		play->ShowFrameQueue.GetQueue((void **)&pShowFrame); 
		play->ShowFrameQueue.CountQueue(&c);
		if(pShowFrame == NULL || c == 0)
			break;	
		if(pShowFrame->data){
			av_free(pShowFrame->data);
			pShowFrame->data = NULL;
		}
		if(pShowFrame){
			free(pShowFrame);
			pShowFrame = NULL;
		}
	}
	if(play->img_convert_ctx){
		sws_freeContext(play->img_convert_ctx);
		play->img_convert_ctx = NULL;
	}

	if(play->pVidFrame){
			av_frame_free(&play->pVidFrame);
			play->pVidFrame = NULL;
	}
	if(VidShowFrame){
		av_frame_free(&VidShowFrame);
		VidShowFrame = NULL;
	}
	
	if(out_buffer){
		av_free(out_buffer);
		out_buffer = NULL;
	}	
	printf("--------- Video_decoder_thread end \n---------（%d）\n",play->Type);

	return 0;
}

SDL_AudioFormat sample_rate_ff2sdl(AVSampleFormat ffsample_rate)
{
	SDL_AudioFormat sdlsample_rate = AUDIO_S16SYS;
	
	if(AV_SAMPLE_FMT_S16 == ffsample_rate)
		sdlsample_rate = AUDIO_S16SYS;
	
	return sdlsample_rate;
}

void  audio_callback(void *udata,Uint8 *stream,int len);
int Th_Player::Open_SDL_Audio()
{
	int Ret = -1;
	//创建管道  
	SECURITY_ATTRIBUTES      sat;                            //安全属性结构  
	sat.nLength              = sizeof(SECURITY_ATTRIBUTES);  //结构体大小  
	sat.bInheritHandle       = true;                         //指出安全描述符的对象能否被新进程继承  
	sat.lpSecurityDescriptor = NULL;                         //安全描述符,NULL: 使用默认的   

	if( !CreatePipe( &hAudioReadPipe, &hAudioWritePipe, &sat, NULL ) )    
     {    
         // MessageBox(_T("Create Pipe Error!"));    
          return Ret;     
     }  

	audio1_Swr.out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
	audio1_Swr.out_channels_layout = AV_CH_LAYOUT_STEREO;
	audio1_Swr.out_sample_rate = OUT_SAMPLE_RATE;
	audio1_Swr.out_sample_fmt = AV_SAMPLE_FMT_S16;

	wanted_spec.callback = audio_callback;
	
	wanted_spec.channels = audio1_Swr.out_channels;
	wanted_spec.format = sample_rate_ff2sdl(audio1_Swr.out_sample_fmt);
	wanted_spec.freq = audio1_Swr.out_sample_rate;
	
	wanted_spec.samples = pAudCodecCtx1->frame_size;
	wanted_spec.silence = 0;
	wanted_spec.userdata = this;
	printf("SDL_GetCurrentAudioDriver %s\n",SDL_GetCurrentAudioDriver());
	printf("SDL_GetNumAudioDevices %d\n",SDL_GetNumAudioDevices(0));

	SDL_OpenAudio(&wanted_spec,NULL);
	if(SDL_OpenAudio(&wanted_spec,NULL) < 0){
		printf("111error  %s\n",SDL_GetError());
		return Ret;
	}
//	SDL_OpenAudioDevice(NULL, 1, &wanted_spec, NULL,1);printf("111error  %s\n",SDL_GetError());

	SDL_PauseAudio(1);
	Ret = 0;
	return Ret;
}

int Th_Player::Close_SDL_Audio()
{
	int Ret = 0;
	CloseHandle( hAudioReadPipe );   
	CloseHandle( hAudioWritePipe );   
	SDL_CloseAudio();
	return Ret;
}

void  audio_callback(void *udata,Uint8 *stream,int len)
{ 
	Th_Player *play = (Th_Player *)udata;

	static uint8_t  buffer[MAX_AUDIO_FRAME_SIZE*2] = {0};
	
	SDL_memset(stream, 0, len);
	
	int readLen = len < MAX_AUDIO_FRAME_SIZE*2?len:(MAX_AUDIO_FRAME_SIZE*2);
	DWORD  byteRead = 0;				
	printf("ReadFile 111\n");
	ReadFile( play->hAudioReadPipe, buffer, readLen, &byteRead, NULL );
	//if(0)
	if(byteRead < readLen){
		DWORD byteRead_1 = 0;
		while(play->Play_Run && byteRead < readLen){
			ReadFile( play->hAudioReadPipe, buffer+byteRead, readLen - byteRead, &byteRead_1, NULL );
			byteRead = byteRead + byteRead_1;
		}
	}
	//printf("ReadFile %d %d %d\n",len,readLen,byteRead);
	SDL_MixAudio(stream,buffer,byteRead,SDL_MIX_MAXVOLUME/4);
}

int Audio1_decoder_thread(void *lpParam)
{
	int Ret = 0;
	AVPacket pkt;
	int got_frame_ptr = 0;
	Th_Player *play = (Th_Player *)lpParam;
	AVFrame * pFrame = NULL;
	int size = 0;

	printf("Audio1_decoder_thread star（%d）\n",play->Type);
	
	uint8_t *out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);
	uint8_t *out_buffer_1=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);
	
	struct SwrContext *aud_convert_ctx = NULL;

	aud_convert_ctx = swr_alloc();
	aud_convert_ctx=swr_alloc_set_opts(
		aud_convert_ctx,
/*out*/	play->audio1_Swr.out_channels_layout,play->audio1_Swr.out_sample_fmt,play->audio1_Swr.out_sample_rate,
/*in*/	play->audio1_Swr.in_channels_layout, play->audio1_Swr.in_sample_fmt, play->audio1_Swr.in_sample_rate,0, NULL);


	swr_init(aud_convert_ctx);
	
	pFrame = av_frame_alloc(); 
	SDL_PauseAudio(0);printf("error  %s\n",SDL_GetError());
	int i = 0; 
	int decoder_size = 0;
		
	while(play->Play_Run){
		if(play->packet_queue_get(&play->audioq1, &pkt) > 0)
		{
			printf("audio nb_packets == %d\n",play->audioq1.nb_packets);
			avcodec_decode_audio4(play->pAudCodecCtx1, pFrame, &got_frame_ptr, &pkt);
			av_free_packet(&pkt); 
			if(got_frame_ptr > 0){
				int nb_samples = swr_convert(aud_convert_ctx,&out_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)pFrame->data , pFrame->nb_samples);
				size = av_samples_get_buffer_size(NULL,av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO), nb_samples,AV_SAMPLE_FMT_S16, 1);

				memcpy(out_buffer_1 + decoder_size,out_buffer,size);

				decoder_size+=size;
				if((decoder_size + size) > size*10 || (decoder_size + size) > MAX_AUDIO_FRAME_SIZE*2){
					DWORD  BytesWritten = 0;
					//printf("11111WriteFile %d 111\n",decoder_size);	
					WriteFile(play->hAudioWritePipe,out_buffer_1,decoder_size,&BytesWritten,NULL);
					//printf("22222WriteFile %d %d\n",decoder_size,BytesWritten);
					memset(out_buffer_1,0,MAX_AUDIO_FRAME_SIZE*2);
					decoder_size = 0;
				}
			}
		}
	}
	SDL_PauseAudio(1);

	if(play->pAudCodecCtx1){
		avcodec_flush_buffers(play->pAudCodecCtx1);
	}
	if(out_buffer){
		av_free(out_buffer);
		out_buffer = NULL;
	}	
	if(out_buffer_1){
		av_free(out_buffer_1);
		out_buffer_1 = NULL;
	}
	if(pFrame){
		av_frame_free(&pFrame);
		pFrame = NULL;
	}
	//fclose(pFile);
	
	printf("Audio1_decoder_thread end（%d）\n",play->Type);
	return 0;
}

#if 0
int Audio2_decoder_thread(void *lpParam)
{
	int Ret = 0;
	AVPacket pkt;
	Th_Player *play = (Th_Player *)lpParam;
	while(play->Play_Run){
		while(play->Play_Run){
			if(play->packet_queue_get(&play->audioq2, &pkt) > 0)
			{	
				av_free_packet(&pkt);
				// means we quit getting packets
				//printf("packet_queue_get video pkt OK %ld \n",pkt.pts);
				break;
			}
		}

	}
	return Ret;
}
#endif
//解复用线程
int demux_thread(void *lpParam)
{
	Th_Player *play = (Th_Player *)lpParam;
	
    AVPacket pkt1, *Packet = &pkt1;
	int Ret = 0,got_picture = 0;
	int video_nb_packets_max = 2;
	int audio_nb_packets_max = 20;
	if(play->Type == 1){
		video_nb_packets_max = 2;
		audio_nb_packets_max = 20;
	}
	/*else if(play->Type == 2){
		video_nb_packets_max = 50;
		audio_nb_packets_max = 250;
	}*/
	play->play_end = 0;
	printf("--------- demux_thread start（%d）\n",play->Type);
	if(play->videoindex >= 0){
		play->Video_decoder_tid  = SDL_CreateThread(Video_decoder_thread,NULL,(void *)lpParam);
	}
	if(play->audioindex1 >= 0)
		play->Audio1_decoder_tid = SDL_CreateThread(Audio1_decoder_thread,NULL,(void *)lpParam);
	/*
	if(play->audioindex2 >= 0)
		play->Audio2_decoder_tid = SDL_CreateThread(Audio2_decoder_thread,NULL,(void *)lpParam);
	*/
	 AVFrame * VidFrame = av_frame_alloc();
	for(;;){
		if(!play->Play_Run){
			Ret = 0;
			break;
		}
		if(!play->pFormatCtx)
			break;
		Ret = av_read_frame(play->pFormatCtx, Packet);
		if(Ret >= 0){
			if((Packet->stream_index==play->videoindex) && play->pVidCodec){			
		#if 1
				if(play->packet_queue_put(&play->videoq, Packet) < 0){
					printf("cat not put packet  %ld \n",Packet->pts);
					if(Packet->data) {
	                  av_free_packet(Packet);
	                }
				}else{
					//printf("packet_queue_get video pkt OK %ld \n",Packet->pts);
				}
		#else
				Ret = avcodec_decode_video2(play->pVidCodecCtx, VidFrame, &got_picture, Packet);
				if(Ret < 0){
					char * err = play->av_fferr(Ret);
					printf("test Decode Error. %s\n",err);
					
				}
				
		#endif
				while(play->Play_Run && play->videoq.nb_packets > video_nb_packets_max){
					Sleep(100);
				}
			}else if((Packet->stream_index==play->audioindex1) && play->pAudCodec1){
				if(play->packet_queue_put(&play->audioq1, Packet) < 0){
					if(Packet->data) {
                  		av_free_packet(Packet);
               		}
				}
				//printf("audio nb_packets == %d\n",play->audioq1.nb_packets);
				while(play->Play_Run && play->audioq1.nb_packets > audio_nb_packets_max){
					Sleep(100);
				}
			}else if((Packet->stream_index==play->audioindex2)&& play->pAudCodec2){
				//while(play->Play_Run && play->audioq2.nb_packets > audio_nb_packets_max){
				//	Sleep(100);
				//}
				//if(play->packet_queue_put(&play->audioq2, Packet) < 0){
					if(Packet->data){
                  		av_free_packet(Packet);
               		}
				//}
			}else{
				//printf("Packet->stream_index == %d\n",Packet->stream_index);
 				av_free_packet(Packet);
            }
		}else{		
			char * err = play->av_fferr(Ret);
			printf("av_read_frame %s\n",err);
			if(Ret == AVERROR_EOF) {
				printf("demux over. %s\n",play->av_fferr(Ret));
			}else{
				printf("demux error %d [%s].", Ret, play->av_fferr(Ret));
				continue;
			}				
			play->Play_Run=0;
            break;
		}
	}

	if(VidFrame){
			av_frame_free(&VidFrame);
			VidFrame = NULL;
	}

	if(play->Video_decoder_tid){
		SDL_WaitThread(play->Video_decoder_tid,NULL);
		play->Video_decoder_tid = NULL;
	}
	
	if(play->Audio1_decoder_tid){
		SDL_WaitThread(play->Audio1_decoder_tid,NULL);
	}
/*	
	if(play->Audio2_decoder_tid){
		SDL_WaitThread(play->Audio2_decoder_tid,NULL);
	}
	*/
	play->packet_queue_flush(&play->videoq);
	play->packet_queue_flush(&play->audioq1);
	play->packet_queue_flush(&play->audioq2);
	play->play_end = 1;

	printf("--------- demux_thread end,（%d）\n",play->Type);
	return Ret;
}
//创建解复用线程
int Th_Player::Play_Player(void)
{
	printf("--------- Play_Player start（%d）\n",Type);
	if(!PlayerIsOpen)
		return -1;
	Play_Run = 1;
 	demux_tid = SDL_CreateThread(demux_thread,NULL,(void*)this);
	//Sleep(500);
	PlayerIsPlay = 1;
	printf("--------- Play_Player end（%d）\n",Type);
	return 0;
}
	

//关闭播放线程 解复用线程
int Th_Player::Stop_Player(void)
{
	printf("--------- Stop_Player start（%d）\n",Type);

	Play_Run = 0;
	while(1){
		if(play_end)
			break;
	}
	if(demux_tid){
		SDL_WaitThread(demux_tid,NULL);
		demux_tid = NULL;
	}
	PlayerIsPlay = 0;
	
	printf("--------- Stop_Player end（%d）\n",Type);
	return 0;
}

void Th_Player::ChargeShowRect(const SDL_Rect rect)
{
	if(rect.x > 0&&rect.x < FullW){
		TextureRect.x = rect.x;
	}
	
	if(rect.y > 0&&rect.y < FullH){
		TextureRect.y = rect.y;
	}

	if(rect.w>0&&rect.x < FullW){
		TextureRect.w = rect.w;
	}
	if(rect.h>0&&rect.h < FullH){
		TextureRect.h = rect.h;
	}
	SaveShowRectToINI();
}
void Th_Player::SaveShowRectToINI()
{
	CString strRect;
	CString strType;
	if(Type == 0)
		return;
	if(Type == 1){
		strType = (CString)"DshowPlayer";
	}
	if(Type == 2){
		strType = (CString)"Win1Player";
	}
	if(Type == 3){
		strType = (CString)"Win2Player";
	}
	if(Type == 4){
		strType = (CString)"Win3Player";
	}
	if(Type == 5){
		strType = (CString)"Win4Player";
	}
	strRect.Format(_T("%d"),TextureRect.x);
	::WritePrivateProfileString(strType,(CString)"Rect.x",strRect,(CString)".//Config.ini");
	strRect.Format(_T("%d"),TextureRect.y); 
	::WritePrivateProfileString(strType,(CString)"Rect.y",strRect,(CString)".//Config.ini");
	strRect.Format(_T("%d"),TextureRect.w); 
	::WritePrivateProfileString(strType,(CString)"Rect.w",strRect,(CString)".//Config.ini");
	strRect.Format(_T("%d"),TextureRect.h); 
	::WritePrivateProfileString(strType,(CString)"Rect.h",strRect,(CString)".//Config.ini");
}



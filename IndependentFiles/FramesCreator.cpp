#ifndef FRAMESCREATOR_H
    #define FRAMESCREATOR_H
    #include "FramesCreator.h"
#endif

//https://libav.org/documentation/doxygen/master/encode__video_8c_source.html


void FramesCreator::add_stream(OutputStream *pOutputStream, AVFormatContext *pFormatContext, const AVCodec *pCodec,
                       enum AVCodecID pCodec_id)
{
    AVCodecContext *codecContext;
    int i;
 
    /* find the encoder */
    pCodec = avcodec_find_encoder(pCodec_id);
    if (!(pCodec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
        avcodec_get_name(pCodec_id));
        exit(1);
    }
    if(pCodec->type==AVMEDIA_TYPE_VIDEO){
        pCodec = avcodec_find_encoder_by_name("libx264rgb");
        if(!(pCodec)){
            fprintf(stderr, "Couldn't find graphics encoder");
            exit(1);
        }
    }
 
    pOutputStream->tmp_pkt = av_packet_alloc();
    if (!pOutputStream->tmp_pkt) {
        fprintf(stderr, "Could not allocate AVPacket\n");
        exit(1);
    }
 
    pOutputStream->st = avformat_new_stream(pFormatContext, NULL);
    if (!pOutputStream->st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    pOutputStream->st->id = pFormatContext->nb_streams-1;
    codecContext = avcodec_alloc_context3(pCodec);
    if (!codecContext) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }
    pOutputStream->enc = codecContext;
 
    switch ((pCodec)->type) {
    case AVMEDIA_TYPE_AUDIO:
        codecContext->sample_fmt  = (pCodec)->sample_fmts ?
            (pCodec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
        codecContext->bit_rate    = 320000;
        codecContext->sample_rate = 48000;
        if ((pCodec)->supported_samplerates) {
            codecContext->sample_rate = (pCodec)->supported_samplerates[0];
            for (i = 0; (pCodec)->supported_samplerates[i]; i++) {
                if ((pCodec)->supported_samplerates[i] == 48000)
                    codecContext->sample_rate = 48000;
            }
        }
		codecContext->channel_layout = AV_CH_LAYOUT_STEREO;
        pOutputStream->st->time_base = (AVRational){ 1, codecContext->sample_rate };
        break;
 
    case AVMEDIA_TYPE_VIDEO:
        codecContext->codec_id = pCodec_id;
 
        codecContext->bit_rate = 85000000;
        /* Resolution must be a multiple of two. */
        codecContext->width    = gWidth;
        codecContext->height   = gHeight;
        /* timebase: This is the fundamental unit of time (in seconds) in terms
         * of which frame timestamps are represented. For fixed-fps content,
         * timebase should be 1/framerate and timestamp increments should be
         * identical to 1. */
        pOutputStream->st->time_base = (AVRational){ 1, gFramesPerSecond };
        codecContext->time_base       = pOutputStream->st->time_base;
 
        codecContext->gop_size      = 60; /* emit one intra frame every twelve frames at most */
        codecContext->pix_fmt       = AV_PIX_FMT_RGB24;//AV_PIX_FMT_YUV420P;
        if (codecContext->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B-frames */
            codecContext->max_b_frames = 4;
        }
        if (codecContext->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            /* Needed to avoid using macroblocks in which some coeffs overflow.
             * This does not happen with normal video, it just happens here as
             * the motion of the chroma plane does not match the luma plane. */
            codecContext->mb_decision = 2;
        }
        codecContext->compression_level = 0;
        break;
 
    default:
        break;
    }
 
    /* Some formats want stream headers to be separate. */
    if (pFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
        codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

AVFrame *FramesCreator::alloc_picture(enum AVPixelFormat pPix_fmt, int pWidth, int pHeight)
{
    AVFrame *picture;
    int ret;
 
    picture = av_frame_alloc();
    if (!picture)
        return NULL;
 
    picture->format = pPix_fmt;
    picture->width  = pWidth;
    picture->height = pHeight;
 
    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }
 
    return picture;
}

void FramesCreator::open_video(AVFormatContext *pFormatContext, const AVCodec *pCodec,
                       OutputStream *pOutputStream, AVDictionary *pOptions){
    int ret;
    AVCodecContext *c = pOutputStream->enc;
    AVDictionary *opt = NULL;
 
    av_dict_copy(&opt, pOptions, 0);
    av_dict_set(&opt, "crf", "0", 0);

    /* open the codec */
    ret = avcodec_open2(c, pCodec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec: \n");
        exit(1);
    }
 
    /* allocate and init a re-usable frame */
    pOutputStream->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!pOutputStream->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
 
    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    pOutputStream->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        pOutputStream->tmp_frame = alloc_picture(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!pOutputStream->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }
 
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(pOutputStream->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
}

AVFrame *FramesCreator::alloc_audio_frame(enum AVSampleFormat pSample_fmt, int pSample_rate, int pNb_samples){
    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Error allocating an audio frame\n");
        exit(1);
    }
 
    frame->format = pSample_fmt;
    frame->channel_layout = AV_CH_LAYOUT_STEREO;
    frame->sample_rate = pSample_rate;
    frame->nb_samples = pNb_samples;
 
    if (pNb_samples) {
        if (av_frame_get_buffer(frame, 0) < 0) {
            fprintf(stderr, "Error allocating an audio buffer\n");
            exit(1);
        }
    }
 
    return frame;
}


void FramesCreator::open_audio(AVFormatContext *pFormatContext, const AVCodec *pCodec,
                       OutputStream *pOutputStream, AVDictionary *pOptions){
    AVCodecContext *c;
    int nb_samples;
    int ret;
    AVDictionary *opt = NULL;
 
    c = pOutputStream->enc;
 
    /* open it */
    av_dict_copy(&opt, pOptions, 0);
    ret = avcodec_open2(c, pCodec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        fprintf(stderr, "Could not open audio codec: \n");
        exit(1);
    }
 
    /* init signal generator */
    pOutputStream->t     = 0;
    pOutputStream->tincr = 2 * M_PI * 110.0 / c->sample_rate;
    /* increment frequency by 110 Hz per second */
    pOutputStream->tincr2 = 2 * M_PI * 110.0 / c->sample_rate / c->sample_rate;
 
    if (c->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
        nb_samples = 10000;
    else
        nb_samples = c->frame_size;
 
    pOutputStream->frame     = alloc_audio_frame(c->sample_fmt, 
                                       c->sample_rate, nb_samples);
    pOutputStream->tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_S16, 
                                       c->sample_rate, nb_samples);
 
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(pOutputStream->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
 
    /* create resampler context */
    pOutputStream->swr_ctx = swr_alloc();
    if (!pOutputStream->swr_ctx) {
        fprintf(stderr, "Could not allocate resampler context\n");
        exit(1);
    }
 
    /* set options */
    av_opt_set_channel_layout  (pOutputStream->swr_ctx, "in_channel_layout",       c->channel_layout,      0);
    av_opt_set_int       (pOutputStream->swr_ctx, "in_sample_rate",     c->sample_rate,    0);
    av_opt_set_sample_fmt(pOutputStream->swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
    av_opt_set_channel_layout  (pOutputStream->swr_ctx, "out_channel_layout",      c->channel_layout,      0);
    av_opt_set_int       (pOutputStream->swr_ctx, "out_sample_rate",    c->sample_rate,    0);
    av_opt_set_sample_fmt(pOutputStream->swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);
 
    /* initialize the resampling context */
    if ((ret = swr_init(pOutputStream->swr_ctx)) < 0) {
        fprintf(stderr, "Failed to initialize the resampling context\n");
        exit(1);
    }
}

FramesCreator::FramesCreator(std::string pFileName, short unsigned pWidth, short unsigned pHeight, long unsigned pStartFrame, long unsigned pEndFrame, int pFramesPerSecond, IFrameGenerator *pFrameGenerator){
    gStartFrame = pStartFrame;
    gEndFrame = pEndFrame;
    gCurrentFrame = pStartFrame;
    gFrameGenerator = pFrameGenerator;
    gWidth = pWidth;
    gHeight = pHeight;
    gFramesPerSecond = pFramesPerSecond;
    std::cout << "Width: " << pWidth << ", Height: " << pHeight << ", FPS: " << pFramesPerSecond << ", Star Frame: " << pStartFrame << ", END FRAME: " << pEndFrame << std::endl;

   std::stringstream lSS;

    avformat_alloc_output_context2(&gFormatContext, 0, 0, pFileName.c_str());
    
    if(!gFormatContext){
        std::cout << " couldn't allocate context2 " << std::endl;
        exit(1);
    }

    gFormatContext->oformat->audio_codec = AV_CODEC_ID_AAC;
    
    if(gFormatContext->oformat->video_codec != AV_CODEC_ID_NONE){
        add_stream(&gVideoOutputStream, gFormatContext, gVideoCodec, gFormatContext->oformat->video_codec);
        gEncodeVideo = true;
    }
    if(gFormatContext->oformat->audio_codec != AV_CODEC_ID_NONE){
        add_stream(&gAudioOutputStream, gFormatContext, gAudioCodec, gFormatContext->oformat->audio_codec);
        gEncodeAudio = true;
    }

    AVDictionary *lOptions = NULL;
    open_video(gFormatContext, gVideoCodec, &gVideoOutputStream, lOptions); 
    open_audio(gFormatContext, gAudioCodec, &gAudioOutputStream, lOptions);

    av_dump_format(gFormatContext, 0, pFileName.c_str(), 1);

    if (!(gFormatContext->flags & AVFMT_NOFILE)) {
        if (avio_open(&gFormatContext->pb, pFileName.c_str(), AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Could not open '%s': \n", pFileName.c_str());
            exit(1);
        }
    }
 
    /* Write the stream header, if any. */
    if (avformat_write_header(gFormatContext, &lOptions) < 0) {
        fprintf(stderr, "Error occurred when opening output file: \n");
        exit(1);
    }
    createFrameCreators();
    generateFrames();    
}

void FramesCreator::createFrameCreators(){
    gFrameCreator = new FrameCreator(gWidth, gHeight, gFrameGenerator);
}


AVFrame *FramesCreator::generateFrame(FrameCreator *pFrameCreator, short unsigned pFrame){
    AVCodecContext *c = gVideoOutputStream.enc;
    /* check if we want to generate more frames */
    if (av_compare_ts(gVideoOutputStream.next_pts, c->time_base,
                      gEndFrame, (AVRational){ 1, 1 }) > 0){
        return NULL;
    }
    /* when we pass a frame to the encoder, it may keep a reference to it
     * internally; make sure we do not overwrite it here */
    if (av_frame_make_writable(gVideoOutputStream.frame) < 0){

        exit(1);
    }
 
    pFrameCreator->gVideoFrame = gVideoOutputStream.frame;
    pFrameCreator->processFrame(gCurrentFrame);

    gVideoOutputStream.frame->pts = gVideoOutputStream.next_pts++;
    return gVideoOutputStream.frame;
}

void FramesCreator::showTimeLeft(time_t pTimer){
    time_t bNewTime;
    time(&bNewTime);
    long double bTimeTaken = difftime(bNewTime, pTimer);
    unsigned long newSeconds = (long double)((double)bTimeTaken / (double)(gCurrentFrame+1)) * (double)((double)gEndFrame - (double)gCurrentFrame);
    unsigned long hours = newSeconds / 3600;
    newSeconds = newSeconds - (hours * 3600);
    unsigned long minutes = newSeconds / 60;
    newSeconds = newSeconds - (minutes * 60);
    cout << gCurrentFrame << " out of " << gEndFrame << " %" <<  (double)((gCurrentFrame+1)*100)/gEndFrame << " time left: " << hours << ":" << minutes << ":" << newSeconds << " time since: " << bTimeTaken <<  endl;
}

int FramesCreator::write_frame(AVFormatContext *pFormatContext, AVCodecContext *pCodecContext, AVStream *pAVStream, AVFrame *pFrame, AVPacket *pPacket){
    int ret;
    // send the frame to the encoder
    ret = avcodec_send_frame(pCodecContext, pFrame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame to the encoder: \n");
                //av_err2str(ret));
        exit(1);
    }
 
    while (ret >= 0) {
        ret = avcodec_receive_packet(pCodecContext, pPacket);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            fprintf(stderr, "Error encoding a frame: \n");
            exit(1);
        }
        /* rescale output packet timestamp values from codec to stream timebase */
        av_packet_rescale_ts(pPacket, pCodecContext->time_base, pAVStream->time_base);
        pPacket->stream_index = pAVStream->index;
 
        /* Write the compressed frame to the media file. */
        AVRational *time_base = &pFormatContext->streams[pPacket->stream_index]->time_base;
        ret = av_interleaved_write_frame(pFormatContext, pPacket);
        /* pkt is now blank (av_interleaved_write_frame() takes ownership of
         * its contents and resets pkt), so that no unreferencing is necessary.
         * This would be different if one used av_write_frame(). */
        if (ret < 0) {
            fprintf(stderr, "Error while writing output packet: \n");
            exit(1);
        }
    }
 
    return ret == AVERROR_EOF ? 1 : 0;
}

int FramesCreator::generateVideoFrame(AVFormatContext *pFormatContext, AVCodecContext *pCodecContext, AVStream *pAVStream, AVPacket *pPacket){
    AVFrame *lAVFrame = generateFrame(gFrameCreator, gCurrentFrame);
    int lRet = write_frame(pFormatContext, pCodecContext, pAVStream, lAVFrame, pPacket);
    return lRet;
}

AVFrame *FramesCreator::get_audio_frame(OutputStream *pOutputStream)
{
    AVFrame *frame = pOutputStream->tmp_frame;
 
    /* check if we want to generate more frames */
    if (av_compare_ts(pOutputStream->next_pts, pOutputStream->enc->time_base,
                      gEndFrame, (AVRational){ 1, 1 }) > 0)
        return NULL;
	
    gFrameCreator->generateSound(frame, pOutputStream->enc->channels);

    frame->pts = pOutputStream->next_pts;
    pOutputStream->next_pts  += frame->nb_samples;
    return frame;
}

int FramesCreator::generateAudioFrame(AVFormatContext *pOutputContext, OutputStream *pOutputStream)
{
    AVCodecContext *c;
    AVFrame *frame;
    int ret;
    int dst_nb_samples;
 
    c = pOutputStream->enc;
 
    frame = get_audio_frame(pOutputStream);
 
    if (frame) {
        /* convert samples from native format to destination codec format, using the resampler */
        /* compute destination number of samples */
        dst_nb_samples = av_rescale_rnd(swr_get_delay(pOutputStream->swr_ctx, c->sample_rate) + frame->nb_samples,
                                        c->sample_rate, c->sample_rate, AV_ROUND_UP);
        
 
        /* when we pass a frame to the encoder, it may keep a reference to it
         * internally;
         * make sure we do not overwrite it here
         */
        ret = av_frame_make_writable(pOutputStream->frame);
        if (ret < 0)
            exit(1);
 
        /* convert to destination format */
        ret = swr_convert(pOutputStream->swr_ctx,
                          pOutputStream->frame->data, dst_nb_samples,
                          (const uint8_t **)frame->data, frame->nb_samples);
        if (ret < 0) {
            fprintf(stderr, "Error while converting\n");
            exit(1);
        }
        frame = pOutputStream->frame;
 
        frame->pts = av_rescale_q(pOutputStream->samples_count, (AVRational){1, c->sample_rate}, c->time_base);
        pOutputStream->samples_count += dst_nb_samples;
    }
 
    return write_frame(pOutputContext, c, pOutputStream->st, frame, pOutputStream->tmp_pkt);
}


void FramesCreator::generateFrames(){
    this->gFrameGenerator->generateInitialFrame(gStartFrame);
    
    time_t lTimer;
    time(&lTimer);

    while(gEncodeVideo||gEncodeAudio){
        if(gEncodeVideo && (!gEncodeAudio||av_compare_ts(gVideoOutputStream.next_pts, gVideoOutputStream.enc->time_base, gAudioOutputStream.next_pts, gAudioOutputStream.enc->time_base) <= 0)){
            if((gCurrentFrame%100)==0){
                showTimeLeft(lTimer);
            }  
            gCurrentFrame++;
            gEncodeVideo = !generateVideoFrame(gFormatContext, gVideoOutputStream.enc, gVideoOutputStream.st, gVideoOutputStream.tmp_pkt);
        }else{
            gEncodeAudio = !generateAudioFrame(gFormatContext, &gAudioOutputStream);
            if(gCurrentFrame==gEndFrame){
                break;
            }
        }
      
    }
    av_write_trailer(gFormatContext);
    
    time_t bNewTime;
    time(&bNewTime);
    long double bTimeTaken = difftime(bNewTime, lTimer);
    unsigned long newSeconds = bTimeTaken;
    unsigned long hours = newSeconds / 3600;
    newSeconds = newSeconds - (hours * 3600);
    unsigned long minutes = newSeconds / 60;
    newSeconds = newSeconds - (minutes * 60);
    cout << "Finished it took " << hours << ":" << minutes << ":" << newSeconds << endl;    

}

FramesCreator::~FramesCreator(){
    delete gFrameCreator;

}

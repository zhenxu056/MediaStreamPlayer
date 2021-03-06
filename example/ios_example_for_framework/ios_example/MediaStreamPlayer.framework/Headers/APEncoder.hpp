//
//  APEncoder.hpp
//  MediaStreamPlayer
//
//  Created by xiaoming on 2018/12/26.
//  Copyright © 2018 freecoder. All rights reserved.
//

#ifndef APEncoder_hpp
#define APEncoder_hpp

#include <mutex>
#include <string>
#include "MSEncoderProtocol.h"
#include "APCodecContext.hpp"
#include "FFCodecContext.hpp"

namespace MS {
    namespace APhard {
        
        typedef MSEncoderProtocol<APFrame>   APEncoderProtocol;
        typedef MSMedia<MSDecodeMedia,APFrame>   APEncoderInputMedia;

        class APEncoder : public APEncoderProtocol {
            
//            typedef function<void(const uint8_t * MSNonnull const decodeData)> ThrowEncodeData;
            
            // 已编码 AAC 数据输出缓冲区
            AudioBufferList * MSNullable const aacOutBuffer;
            // 线性的 PCM 数据输入缓冲(AAC 编码要求: 输入数据长度不够 2048 字节时, 需要后来的数据拼接, 直到长度够 2048 字节)
            AudioBuffer     * MSNullable const pcmInBuffer;
            
            mutex fileWriteMutex;
            
            string filePath;
            
            bool _isEncoding = false;
            bool isWriteHeader = false;
            
            const MSCodecID videoCodecID;
            const MSCodecID audioCodecID;
            
            long long videoPts;
            long long audioPts;
            int nalUnitHeaderLen;
            
            AVFormatContext       * MSNullable outputFormatContext = nullptr;
            VTCompressionSessionRef MSNullable videoEncoderSession = nullptr;
            AudioConverterRef       MSNullable audioEncoderConvert = nullptr;
            
            AVStream * MSNullable videoStream = nullptr;
            AVStream * MSNullable audioStream = nullptr;
            
            AVFormatContext       * MSNullable configureOutputFormatContext();
            VTCompressionSessionRef MSNullable configureVideoEncoderSession(const MSVideoParameters &videoParameters);
            AudioConverterRef       MSNullable configureAudioEncoderConvert(const MSAudioParameters &audioParameters);
            
            void releaseEncoderConfiguration();
            
//            const ThrowEncodeData throwEncodeVideo;
//            const ThrowEncodeData throwEncodeAudio;
            
            static void compressionOutputCallback(void * MSNullable outputCallbackRefCon,
                                                  void * MSNullable sourceFrameRefCon,
                                                  OSStatus status,
                                                  VTEncodeInfoFlags infoFlags,
                                                  CMSampleBufferRef MSNullable sampleBuffer);
            
            static OSStatus compressionConverterInputProc(AudioConverterRef MSNonnull inAudioConverter,
                                                          UInt32 * MSNonnull ioNumberDataPackets,
                                                          AudioBufferList * MSNonnull ioData,
                                                          AudioStreamPacketDescription * MSNullable * MSNullable outDataPacketDescription,
                                                          void * MSNullable inUserData);
            
        public:
            void beginEncode();
            void encodeVideo(const APEncoderInputMedia &pixelData);
            void encodeAudio(const APEncoderInputMedia &sampleData);
            void endEncode();
            bool isEncoding();
            
            // 暂不支持自定义编码方式(因为还没有将 MP4 muxer 拆解出来)
//            APEncoder(const MSCodecID videoCodecID,
//                      const MSCodecID audioCodecID);
            APEncoder();
            ~APEncoder();
            
            bool configureEncoder(const string &muxingfilePath,
                                  const MSVideoParameters * MSNullable const videoParameters,
                                  const MSAudioParameters * MSNullable const audioParameters);
        };
        
    }
}

#endif /* APEncoder_hpp */

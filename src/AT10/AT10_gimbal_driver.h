/*
 * @Description:    Q10f吊舱的驱动文件
 * @Author: L LC @amov
 * @Date: 2022-10-28 12:24:21
 * @LastEditors: L LC @amov
 * @LastEditTime: 2023-12-06 10:27:48
 * @FilePath: /SpireCV/gimbal_ctrl/driver/src/AT10/AT10_gimbal_driver.h
 */
#ifndef __AT10_DRIVER_H
#define __AT10_DRIVER_H
#include "../amov_gimbal_private.h"
#include "AT10_gimbal_struct.h"
#include <mutex>
#if defined(__APPLE__)
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#include <iostream>

class AT10GimbalDriver : protected amovGimbal::amovGimbalBase
{
private:
    AT10::GIMBAL_SERIAL_STATE_T parserState;
    AT10::GIMBAL_EXTEND_FRAME_T extendRx;

    AT10::GIMBAL_STD_FRAME_T stdRx;
    fifoRing *stdRxQueue;
    fifoRing *stdTxQueue;

    // void send(void);

    void stackStart(void);
    void sendHeart(void);
    void sendStd(void);

    void parserStart(pAmovGimbalStateInvoke callback, void *caller);
    void parserLoop(void);
    void getExtRxPack(void);
    void getStdRxPack(void);

    pthread_t sendHreatThreadHandle;
    pthread_t extStackThreadHandle;

    bool parser(IN uint8_t byte);
    void convert(void *buf);
    uint32_t pack(IN uint32_t cmd, uint8_t *pPayload, uint8_t payloadSize);
    uint32_t calPackLen(void *pack);

public:
    // funtions
    uint32_t setGimabalPos(const AMOV_GIMBAL_POS_T &pos);
    uint32_t setGimabalSpeed(const AMOV_GIMBAL_POS_T &speed);
    uint32_t setGimabalFollowSpeed(const AMOV_GIMBAL_POS_T &followSpeed);
    uint32_t setGimabalHome(void);

    uint32_t setGimbalZoom(AMOV_GIMBAL_ZOOM_T zoom, float targetRate = 0);
    uint32_t setGimbalFocus(AMOV_GIMBAL_ZOOM_T zoom, float targetRate = 0);

    uint32_t takePic(void);
    uint32_t setVideo(const AMOV_GIMBAL_VIDEO_T newState);

    uint32_t extensionFuntions(void *cmd);

    // builds
    static amovGimbal::amovGimbalBase *creat(amovGimbal::IOStreamBase *_IO)
    {
        return new AT10GimbalDriver(_IO);
    }

    AT10GimbalDriver(amovGimbal::IOStreamBase *_IO);
    ~AT10GimbalDriver()
    {
        if (stdRxQueue != nullptr)
        {
            delete stdRxQueue;
        }
        if (stdTxQueue != nullptr)
        {
            delete stdTxQueue;
        }
        // set thread kill anytime
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
        #if defined(_WIN32)
        if (parserThreadHanle.p != 0)
        {
            pthread_cancel(parserThreadHanle);
        }
        if (sendThreadHanle.p != 0)
        {
            pthread_cancel(sendThreadHanle);
        }
        if (stackThreadHanle.p != 0)
        {
            pthread_cancel(stackThreadHanle);
        }
        if (sendHreatThreadHandle.p != 0)
        {
            pthread_cancel(sendHreatThreadHandle);
        }
        if (extStackThreadHandle.p != 0)
        {
            pthread_cancel(extStackThreadHandle);
        }
        #elif defined(__APPLE__)
        pthread_cancel(parserThreadHanle);
        pthread_cancel(sendThreadHanle);
        pthread_cancel(stackThreadHanle);
        pthread_cancel(sendHreatThreadHandle);
        pthread_cancel(extStackThreadHandle);
        #else
        parserThreadHanle = parserThreadHanle == 0 ? 0 : pthread_cancel(parserThreadHanle);
        sendThreadHanle = sendThreadHanle == 0 ? 0 : pthread_cancel(sendThreadHanle);
        stackThreadHanle = stackThreadHanle == 0 ? 0 : pthread_cancel(stackThreadHanle);
        sendHreatThreadHandle = sendHreatThreadHandle == 0 ? 0 : pthread_cancel(sendHreatThreadHandle);
        extStackThreadHandle = extStackThreadHandle == 0 ? 0 : pthread_cancel(extStackThreadHandle);
        #endif
    }
};

#endif

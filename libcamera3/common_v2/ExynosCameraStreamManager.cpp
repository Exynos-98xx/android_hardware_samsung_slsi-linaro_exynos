/*
 * Copyright (C) 2017, Samsung Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* #define LOG_NDEBUG 0 */
#define LOG_TAG "ExynosCameraStreamManager"

#include "ExynosCameraStreamManager.h"

namespace android {

ExynosCameraStream::ExynosCameraStream(int id, camera3_stream_t *stream)
{
    m_init();
    m_id = id;
    m_stream = stream;
}

ExynosCameraStream::~ExynosCameraStream()
{
    m_deinit();
}

status_t ExynosCameraStream::m_init()
{
    m_stream = NULL;
    m_id = -1;
    m_actualFormat = -1;
    m_actualPixelSize = CAMERA_PIXEL_SIZE_8BIT;
    m_planeCount = -1;
    m_outputPortId = -1;
    m_registerStream = EXYNOS_STREAM::HAL_STREAM_STS_INIT;
    m_registerBuffer = EXYNOS_STREAM::HAL_STREAM_STS_INIT;
    m_requestbuffer = -1;

    return NO_ERROR;
}

status_t ExynosCameraStream::m_deinit()
{
    m_stream = NULL;
    m_id = -2;
    m_actualFormat = -2;
    m_actualPixelSize = CAMERA_PIXEL_SIZE_8BIT;
    m_planeCount = -2;
    m_outputPortId = -2;
    m_registerStream = EXYNOS_STREAM::HAL_STREAM_STS_INIT;
    m_registerBuffer = EXYNOS_STREAM::HAL_STREAM_STS_INIT;
    m_requestbuffer = -2;

    return NO_ERROR;
}

status_t ExynosCameraStream::setStream(camera3_stream_t *stream)
{
    status_t ret = NO_ERROR;
    if (stream == NULL) {
        ALOGE("ERR(%s[%d]):setStream is NULL ", __FUNCTION__, __LINE__);
        ret = INVALID_OPERATION;
    } else {
        m_stream = stream;
    }

    return ret;
}

status_t ExynosCameraStream::getStream(camera3_stream_t **stream)
{
    status_t ret = NO_ERROR;
    if (m_stream != NULL) {
        *stream = m_stream;
    } else {
        ALOGE("ERR(%s[%d]):getStream m_stream is NULL ", __FUNCTION__, __LINE__);
        *stream = NULL;
        ret = INVALID_OPERATION;
    }
    return ret;
}

status_t ExynosCameraStream::setID(int id)
{
    status_t ret = NO_ERROR;

    if (id < 0) {
        ALOGE("ERR(%s[%d]):setStreamID invalid value(%d)", __FUNCTION__, __LINE__, id);
        ret = INVALID_OPERATION;
    } else {
        m_id = id;
    }
    return ret;
}

status_t ExynosCameraStream::getID(int *id)
{
    status_t ret = NO_ERROR;

    if (m_id < 0) {
        ALOGE("ERR(%s[%d]):getStreamID invalid value(%d)", __FUNCTION__, __LINE__, m_id);
        ret = INVALID_OPERATION;
    } else {
        *id = m_id;
    }

    return ret;
}

status_t ExynosCameraStream::setFormat(int format, camera_pixel_size pixelSize)
{
    status_t ret = NO_ERROR;

    if (format < 0) {
        ALOGE("ERR(%s[%d]):setFormat invalid value (%d)(%d)", __FUNCTION__, __LINE__, format, pixelSize);
        ret = INVALID_OPERATION;
    } else {
        m_actualFormat = format;
        m_actualPixelSize = pixelSize;
    }
    return ret;
}

status_t ExynosCameraStream::getFormat(int *format, camera_pixel_size *pixelSize)
{
    status_t ret = NO_ERROR;

    if (m_actualFormat < 0) {
        ALOGE("ERR(%s[%d]):getFormat invalid value(%d)(%d)", __FUNCTION__, __LINE__, m_actualFormat, m_actualPixelSize);
        ret = INVALID_OPERATION;
    } else {
        *format = m_actualFormat;
        *pixelSize = m_actualPixelSize;
    }
    return ret;
}

status_t ExynosCameraStream::setPlaneCount(int planes)
{
    status_t ret = NO_ERROR;

    if (planes < 0) {
        ALOGE("ERR(%s[%d]):setPlaneCount invalid value(%d)", __FUNCTION__, __LINE__, planes);
        ret = INVALID_OPERATION;
    } else {
        m_planeCount = planes;
    }
    return ret;
}

status_t ExynosCameraStream::getPlaneCount(int *planes)
{
    status_t ret = NO_ERROR;

    if (m_planeCount < 0) {
        ALOGE("ERR(%s[%d]):getFormat invalid value(%d)", __FUNCTION__, __LINE__, m_planeCount);
        ret = INVALID_OPERATION;
    } else {
        *planes = m_planeCount;
    }
    return ret;
}

status_t ExynosCameraStream::setOutputPortId(int id)
{
    status_t ret = NO_ERROR;

    if (id < 0) {
        ALOGE("ERR(%s[%d]):Invalid outputPortId %d",
                __FUNCTION__, __LINE__, id);
        ret = BAD_VALUE;
    } else {
        m_outputPortId = id;
    }

    return ret;
}

status_t ExynosCameraStream::getOutputPortId(int *id)
{
    status_t ret = NO_ERROR;

    if (m_outputPortId < 0) {
        ALOGE("ERR(%s[%d]):Invalid outputPortId %d",
                __FUNCTION__, __LINE__, m_outputPortId);
        ret = BAD_VALUE;
    } else {
        *id = m_outputPortId;
    }

    return ret;
}

status_t ExynosCameraStream::setRegisterStream(EXYNOS_STREAM::STATE state)
{
    status_t ret = NO_ERROR;
    switch (state) {
    case EXYNOS_STREAM::HAL_STREAM_STS_INVALID:
    case EXYNOS_STREAM::HAL_STREAM_STS_VALID:
        m_registerStream = state;
        break;
    case EXYNOS_STREAM::HAL_STREAM_STS_INIT:
    default:
        ALOGE("ERR(%s[%d]):setConfigState invalid value(%d)", __FUNCTION__, __LINE__, state);
        break;
    }
    return ret;
}

status_t ExynosCameraStream::getRegisterStream(EXYNOS_STREAM::STATE *state)
{
    status_t ret = NO_ERROR;
    switch (m_registerStream) {
    case EXYNOS_STREAM::HAL_STREAM_STS_INIT:
    case EXYNOS_STREAM::HAL_STREAM_STS_INVALID:
    case EXYNOS_STREAM::HAL_STREAM_STS_VALID:
        *state = m_registerStream;
        break;
    default:
        ALOGE("ERR(%s[%d]):setConfigState invalid value(%d)", __FUNCTION__, __LINE__, *state);
        break;
    }
    return ret;
}

status_t ExynosCameraStream::setRegisterBuffer(EXYNOS_STREAM::STATE state)
{
    status_t ret = NO_ERROR;
    switch (state) {
    case EXYNOS_STREAM::HAL_STREAM_STS_REGISTERED:
    case EXYNOS_STREAM::HAL_STREAM_STS_UNREGISTERED:
        m_registerBuffer = state;
        break;
    case EXYNOS_STREAM::HAL_STREAM_STS_INIT:
    default:
        ALOGE("ERR(%s[%d]):setRegisterBuffer invalid value(%d)", __FUNCTION__, __LINE__, state);
        break;
    }
    return ret;
}

status_t ExynosCameraStream::getRegisterBuffer(EXYNOS_STREAM::STATE *state)
{
    status_t ret = NO_ERROR;
    switch (m_registerBuffer) {
    case EXYNOS_STREAM::HAL_STREAM_STS_INIT:
    case EXYNOS_STREAM::HAL_STREAM_STS_REGISTERED:
    case EXYNOS_STREAM::HAL_STREAM_STS_UNREGISTERED:
        *state = m_registerBuffer;
        break;
    default:
        ALOGE("ERR(%s[%d]):getRegisterBuffer invalid value(%d)", __FUNCTION__, __LINE__, *state);
        break;
    }
    return ret;
}

status_t ExynosCameraStream::setRequestBuffer(int bufferCnt)
{
    status_t ret = NO_ERROR;

    if (bufferCnt < 0) {
        ALOGE("ERR(%s[%d]):setRequestBuffer invalid value(%d)", __FUNCTION__, __LINE__, bufferCnt);
        ret = INVALID_OPERATION;
    } else {
        m_requestbuffer = bufferCnt;
        m_stream->max_buffers = m_requestbuffer;
    }
    return ret;
}

status_t ExynosCameraStream::getRequestBuffer(int *bufferCnt)
{
    status_t ret = NO_ERROR;

    if (m_planeCount < 0) {
        ALOGE("ERR(%s[%d]):getRequestBuffer invalid value(%d)", __FUNCTION__, __LINE__, m_requestbuffer);
        ret = INVALID_OPERATION;
    } else {
        *bufferCnt = m_requestbuffer;
    }
    return ret;
}

ExynosCameraStreamManager::ExynosCameraStreamManager(int cameraId)
{
    ALOGD("DEBUG(%s[%d]):ID(%d)", __FUNCTION__, __LINE__, cameraId);
    m_cameraId = cameraId;
    m_init();
}

ExynosCameraStreamManager::~ExynosCameraStreamManager()
{
    ALOGD("DEBUG(%s[%d]):", __FUNCTION__, __LINE__);
    m_deinit();
}

void ExynosCameraStreamManager::m_init()
{
    ALOGD("DEBUG(%s[%d]):ID(%d)", __FUNCTION__, __LINE__, m_cameraId);
    m_streamInfoMap.clear();
    m_yuvStreamCount = 0;
    m_yuvStallStreamCount = 0;
    m_yuvStreamMaxCount = 0;
    m_inputStreamCount = 0;

    for (int i = 0; i < ExynosCameraParameters::YUV_OUTPUT_PORT_ID_MAX; i++)
        m_yuvStreamIdMap[i] = -1;
}

void ExynosCameraStreamManager::m_deinit()
{
    ALOGD("DEBUG(%s[%d]):ID(%d)", __FUNCTION__, __LINE__, m_cameraId);
    StreamInfoMap::iterator iter;

    ExynosCameraStream *streaminfo = NULL;

    m_streamInfoLock.lock();
    for (iter = m_streamInfoMap.begin() ; iter != m_streamInfoMap.end() ;) {
        streaminfo = iter->second;
        m_streamInfoMap.erase(iter++);
        if (streaminfo != NULL) {
            delete streaminfo;
            streaminfo = NULL;
        }
    }
    m_streamInfoMap.clear();
    m_streamInfoLock.unlock();
}

status_t ExynosCameraStreamManager::dumpCurrentStreamList(void)
{
    ALOGD("DEBUG(%s[%d]):Stream List dump-----", __FUNCTION__, __LINE__);
    status_t ret = NO_ERROR;
    ExynosCameraStream *streaminfo = NULL;
    int id = 0;
    EXYNOS_STREAM::STATE registerStream = EXYNOS_STREAM::HAL_STREAM_STS_INIT;
    EXYNOS_STREAM::STATE registerBuf    = EXYNOS_STREAM::HAL_STREAM_STS_INIT;
    camera3_stream_t *currentStream = NULL;



    if (m_streamInfoMap.empty()) {
        ALOGE("ERR(%s[%d]):list is empty", __FUNCTION__, __LINE__);
        return NOT_ENOUGH_DATA;
    }

    for (StreamInfoIterator s = m_streamInfoMap.begin(); s != m_streamInfoMap.end();) {
        streaminfo = s->second;
        if (streaminfo == NULL) {
            ALOGE("ERR(%s[%d]):streaminfo is NULL id(%d)", __FUNCTION__, __LINE__, s->first);
            s++;
            continue;
        }

        ret = streaminfo->getStream(&currentStream);
        if (ret < 0) {
            ALOGE("ERR(%s[%d]):m_insert failed -> delete stream, id(%d)", __FUNCTION__, __LINE__, s->first);
            s++;
            continue;
        }

        streaminfo->getID(&id);
        streaminfo->getRegisterStream(&registerStream);
        streaminfo->getRegisterBuffer(&registerBuf);
        ALOGD("DEBUG(%s[%d]):Stream(%p), ID(%d), type(%d), usage(0x%x) format(0x%x) (%d,%d)", __FUNCTION__,
                                __LINE__, currentStream, id,
                                currentStream->stream_type, currentStream->usage, currentStream->format,
                                currentStream->width, currentStream->height);
        ALOGD("DEBUG(%s[%d]):status %d / %d", __FUNCTION__, __LINE__, registerStream, registerBuf);
        s++;
    }

    return OK;
}

status_t ExynosCameraStreamManager::createStream(int id, camera3_stream_t *stream,
                                                            ExynosCameraStream **newStream)
{
    status_t ret = NO_ERROR;

    (*newStream) = new ExynosCameraStream(id, stream);
    if ((*newStream) == NULL){
        ALOGE("ERR(%s[%d]):m_insert failed stream id(%d)", __FUNCTION__, __LINE__, id);
        return INVALID_OPERATION;
    }
    stream->priv = static_cast<void*>((*newStream));

    ret = m_insert(id, (*newStream), &m_streamInfoMap, &m_streamInfoLock);
    if (ret != NO_ERROR){
        ret = m_delete(id, &m_streamInfoMap, &m_streamInfoLock);
        if (ret != NO_ERROR) {
            ALOGW("WARN(%s[%d]):Delete stream by id(%d) fail, ret(%d)", __FUNCTION__, __LINE__, id, ret);
            m_delete((*newStream));
        }

        ALOGE("ERR(%s[%d]):m_insert failed -> delete stream, id(%d)", __FUNCTION__, __LINE__, id);
        return ret;
    }

    return ret;
}

status_t ExynosCameraStreamManager::deleteStream(int id)
{
    int ret = NO_ERROR;

    ret = m_delete(id, &m_streamInfoMap, &m_streamInfoLock);
    if (ret < 0) {
        ALOGE("ERR(%s[%d]):eraseStream failed stream id(%d)", __FUNCTION__, __LINE__, id);
    }

    ret = m_decreaseYuvStreamCount(id);
    if (ret != NO_ERROR) {
        ALOGE("ERR(%s[%d]):Failed to decreaseYuvStreamCount. streamId %d",
                __FUNCTION__, __LINE__, id);
    }

    ret = m_decreaseInputStreamCount(id);
    if (ret != NO_ERROR) {
        ALOGE("ERR(%s[%d]):Failed to decreaseInputStreamCount. streamId %d",
                __FUNCTION__, __LINE__, id);
    }

    return ret;
}

status_t ExynosCameraStreamManager::getStream(int id, ExynosCameraStream **stream)
{
    status_t ret = NO_ERROR;
    *stream = NULL;

    ret = m_get(id, stream, &m_streamInfoMap, &m_streamInfoLock);

    if (ret < 0) {
        *stream = NULL;
        return BAD_VALUE;
    }

    return ret;
}

bool ExynosCameraStreamManager::findStream(int id)
{
    status_t ret = NO_ERROR;
    bool found = true;

    ret = m_find(id, &m_streamInfoMap, &m_streamInfoLock);
    if (ret != NO_ERROR)
        found = false;

    return found;
}

status_t ExynosCameraStreamManager::getStreamKeys(List<int> *keylist)
{
    status_t ret = NO_ERROR;
    StreamInfoIterator iter;

    for (iter = m_streamInfoMap.begin(); iter != m_streamInfoMap.end() ; iter++) {
        ALOGV("DEBUG(%s[%d]):stream key is(%d)", __FUNCTION__, __LINE__, iter->first);

        keylist->push_back(iter->first);
    }

    return ret;
}

status_t ExynosCameraStreamManager::setYuvStreamMaxCount(int32_t count)
{
    m_yuvStreamMaxCount = count;

    return NO_ERROR;
}

int32_t ExynosCameraStreamManager::getYuvStreamCount(void)
{
    return m_yuvStreamCount;
}

int32_t ExynosCameraStreamManager::getYuvStallStreamCount(void)
{
    return m_yuvStallStreamCount;
}

int32_t ExynosCameraStreamManager::getTotalYuvStreamCount(void)
{
    return m_yuvStreamCount + m_yuvStallStreamCount;
}

int ExynosCameraStreamManager::getYuvStreamId(int outputPortId)
{
    if (outputPortId < 0 || outputPortId >= ExynosCameraParameters::YUV_OUTPUT_PORT_ID_MAX) {
        ALOGE("ERR(%s[%d]):Invalid outputPortId %d",
                __FUNCTION__, __LINE__, outputPortId);
        return -1;
    }

    return m_yuvStreamIdMap[outputPortId];
}

status_t ExynosCameraStreamManager::increaseInputStreamCount(int inputStreamMaxNum)
{
    if (++m_inputStreamCount > inputStreamMaxNum) {
        ALOGE("ERR(%s[%d]):The number of the input streams which is the setting possible is exceeded. maxNum(%d)",
                __FUNCTION__, __LINE__, inputStreamMaxNum);
        return BAD_VALUE;
    }

    return NO_ERROR;
}

status_t ExynosCameraStreamManager::m_decreaseInputStreamCount(int streamId)
{
    if (streamId < 0) {
        ALOGE("ERR(%s[%d]):Invalid streamId %d",
                __FUNCTION__, __LINE__, streamId);
        return BAD_VALUE;
    }

    switch (streamId % HAL_STREAM_ID_MAX) {
    case HAL_STREAM_ID_ZSL_INPUT:
        --m_inputStreamCount;
        break;
    default:
        ALOGV("ERR(%s[%d]):Unsupported stream id %d",
            __FUNCTION__, __LINE__, streamId);
        break;
    }

    return NO_ERROR;
}

status_t ExynosCameraStreamManager::m_setYuvStreamId(int outputPortId, int streamId)
{
    if (outputPortId < 0 || outputPortId >= ExynosCameraParameters::YUV_OUTPUT_PORT_ID_MAX) {
        ALOGE("ERR(%s[%d]):Invalid outputPortId %d",
                __FUNCTION__, __LINE__, outputPortId);
        return BAD_VALUE;
    }

    m_yuvStreamIdMap[outputPortId] = streamId;

    return NO_ERROR;
}

status_t ExynosCameraStreamManager::m_insert(int id, ExynosCameraStream *item, StreamInfoMap *list, Mutex *lock)
{
    status_t ret = NO_ERROR;
    lock->lock();
    pair<StreamInfoMap::iterator,bool> listRet;

    listRet = list->insert( pair<int, ExynosCameraStream*>(id, item));

    while(listRet.second == false) {
        id += HAL_STREAM_ID_MAX;
        listRet = list->insert( pair<int, ExynosCameraStream*>(id, item));
        ALOGW("WARN(%s[%d]):insert id is re-define id + HAL_STREAM_ID_MAX = (%d)", __FUNCTION__, __LINE__, id);
    }

    if (listRet.second == true) {
        ALOGI("INFO(%s[%d]):inserted stream and id is (%d)", __FUNCTION__, __LINE__, id);
        item->setID(id);
    }

    ret = m_increaseYuvStreamCount(id);
    if (ret != NO_ERROR) {
        ALOGE("ERR(%s[%d]):Failed to updateYuvStreamCount. yuvStreamCount %d/%d streamId %d",
                __FUNCTION__, __LINE__, m_yuvStreamCount, m_yuvStallStreamCount, id);
        lock->unlock();
        return ret;
    }

    lock->unlock();
    return ret;
}

status_t ExynosCameraStreamManager::m_find(int id, StreamInfoMap *list, Mutex *lock)
{
    status_t ret = NO_ERROR;
    StreamInfoMap::iterator iter;

    if (list == NULL) {
        ALOGE("ERR(%s[%d]):list is NULL", __FUNCTION__, __LINE__);
        return BAD_VALUE;
    }

    if (lock == NULL) {
        ALOGE("ERR(%s[%d]):lock is NULL", __FUNCTION__, __LINE__);
        return BAD_VALUE;
    }

    lock->lock();
    iter = list->find(id);
    if (iter == list->end()) {
        ret = BAD_VALUE;
    }
    lock->unlock();

    return ret;
}

status_t ExynosCameraStreamManager::m_get(int id, ExynosCameraStream **item, StreamInfoMap *list, Mutex *lock)
{
    status_t ret = NO_ERROR;
    StreamInfoMap::iterator iter;

    lock->lock();
    iter = list->find(id);
    if (iter != list->end()) {
        *item = iter->second;
    } else {
        ALOGE("ERR(%s[%d]):StreamInfoMap is not EXIST stream id(%d)", __FUNCTION__, __LINE__, id);
        ret = BAD_VALUE;
    }
    lock->unlock();

    return ret;
}

status_t ExynosCameraStreamManager::m_delete(int id, StreamInfoMap *list, Mutex *lock)
{
    status_t ret = NO_ERROR;
    StreamInfoMap::iterator iter;
    ExynosCameraStream *item = NULL;

    lock->lock();
    iter = list->find(id);
    if (iter != list->end()) {
        item = iter->second;
        list->erase(iter);
        m_delete(item);
    } else {
        ret = BAD_VALUE;
        ALOGE("ERR(%s[%d]):StreamInfoMap is not EXIST stream id(%d)", __FUNCTION__, __LINE__, id);
    }
    lock->unlock();

    return ret;
}

status_t ExynosCameraStreamManager::m_delete(ExynosCameraStream *stream)
{
    status_t ret = NO_ERROR;
    camera3_stream_t *obj = NULL;
    int streamId = -1;

    if (stream == NULL){
        ret = BAD_VALUE;
        ALOGE("ERR(%s[%d]):StreamInfoMap is not EXIST stream", __FUNCTION__, __LINE__);
        return ret;
    }

    ret = stream->getStream(&obj);
    obj->priv = NULL;

    if (obj->stream_type == CAMERA3_STREAM_INPUT
        || obj->stream_type == CAMERA3_STREAM_BIDIRECTIONAL) {
        m_inputStreamCount--;
    }

    stream->getID(&streamId);

    delete stream;
    stream = NULL;
    return ret;
}

}; /* namespace android */

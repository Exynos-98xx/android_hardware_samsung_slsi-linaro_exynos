/*
**
** Copyright 2015, Samsung Electronics Co. LTD
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef EXYNOS_CAMERA_FRAME_REPROCESSING_FACTORY_H
#define EXYNOS_CAMERA_FRAME_REPROCESSING_FACTORY_H

#include "ExynosCamera3FrameFactory.h"

namespace android {

class ExynosCamera3FrameReprocessingFactory : public ExynosCamera3FrameFactory {
public:
    ExynosCamera3FrameReprocessingFactory()
    {
        m_init();
    }

    ExynosCamera3FrameReprocessingFactory(int cameraId, ExynosCamera3Parameters *param) : ExynosCamera3FrameFactory(cameraId, param)
    {
        m_init();

        strncpy(m_name, "ReprocessingFactory",  EXYNOS_CAMERA_NAME_STR_SIZE - 1);
    }

    virtual ~ExynosCamera3FrameReprocessingFactory();

    virtual status_t        create(void);

    virtual status_t        initPipes(void);
    virtual status_t        preparePipes(void);

    virtual status_t        startPipes(void);
    virtual status_t        stopPipes(void);
    virtual status_t        startInitialThreads(void);
    virtual status_t        setStopFlag(void);

    virtual ExynosCameraFrameSP_sptr_t createNewFrame(uint32_t frameCount = 0);

protected:
    virtual status_t        m_setupConfig(void);
    virtual status_t        m_constructReprocessingPipes(void);
    virtual status_t        m_fillNodeGroupInfo(ExynosCameraFrameSP_sptr_t frame);

private:
    void                    m_init(void);

private:
    bool                    m_flagHWFCEnabled;
};

}; /* namespace android */

#endif

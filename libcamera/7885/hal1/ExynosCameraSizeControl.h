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

#ifndef EXYNOS_CAMERA_SIZE_CONTROL_H
#define EXYNOS_CAMERA_SIZE_CONTROL_H

#include <cutils/properties.h>
#include <utils/threads.h>
#include <utils/String8.h>

#include "ExynosCamera1Parameters.h"
#include "ExynosCameraConfig.h"

#include "ExynosRect.h"
#include "fimc-is-metadata.h"
#include "ExynosCameraUtils.h"

/* #define DEBUG_PERFRAME_SIZE */

namespace android {

typedef struct size_control_info {
    ExynosRect      sensorSize;
#ifdef SUPPORT_DEPTH_MAP
    ExynosRect      depthMapSize;
#endif // SUPPORT_DEPTH_MAP
    ExynosRect      bnsSize;
    ExynosRect      bayerCropSize;
    ExynosRect      bdsSize;
    ExynosRect      yuvCropSize;
    ExynosRect      hwPreviewSize;
    ExynosRect      previewSize;
    ExynosRect      hwVideoSize;
    ExynosRect      pictureSize;
    ExynosRect      thumbnailSize;
#ifdef SAMSUNG_DUAL_SOLUTION
    ExynosRect      fusionSrcSize;
    ExynosRect      fusionDstSize;
#endif
#ifdef USE_VRA_GROUP
    ExynosRect      vraInputSize;
#endif
} size_control_info_t;

void updateNodeGroupInfo(
        int pipeId,
        ExynosCamera1Parameters *params,
        const size_control_info_t sizeControlInfo,
        camera2_node_group *node_group_info);

/* Helper function */
void setLeaderSizeToNodeGroupInfo(
        camera2_node_group *node_group_info,
        int cropX, int cropY,
        int width, int height);

void setCaptureSizeToNodeGroupInfo(
        camera2_node_group *node_group_info,
        uint32_t perframePosition,
        int width, int height);

void setCaptureCropNScaleSizeToNodeGroupInfo(
        camera2_node_group *node_group_info,
        uint32_t perframePosition,
        int cropX, int cropY,
        int cropWidth, int cropHeight,
        int targetWidth, int targetHeight);

bool useSizeControlApi(void);

}; /* namespace android */

#endif


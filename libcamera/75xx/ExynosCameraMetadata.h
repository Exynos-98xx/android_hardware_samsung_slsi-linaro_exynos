/*
**
** Copyright 2014, Samsung Electronics Co. LTD
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

#ifndef EXYNOS_CAMERA_METADATA_H
#define EXYNOS_CAMERA_METADATA_H

#ifdef USE_CAMERA2_API_SUPPORT
#include "fimc-is-metadata2.h"
#else
#include "fimc-is-metadata.h"
#endif

namespace android {

class ExynosCameraMetadata {
public:
    ExynosCameraMetadata();
    virtual ~ExynosCameraMetadata();

    bool        create();

    status_t    setCropRegion();
    status_t    getCropRegion();

private:
    struct camera2_shot_ext     m_metadata;
};

}; /* namespace android */
#endif

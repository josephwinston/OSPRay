//
//                 INTEL CORPORATION PROPRIETARY INFORMATION
//
//    This software is supplied under the terms of a license agreement or
//    nondisclosure agreement with Intel Corporation and may not be copied
//    or disclosed except in accordance with the terms of that agreement.
//    Copyright (C) 2014 Intel Corporation. All Rights Reserved.
//

#include "apps/common/fileio/OSPObjectFile.h"
#include "SeismicVolumeFile.h"

namespace ospray {

    //! Loader for seismic volume files for supported self-describing formats.
    OSP_REGISTER_VOLUME_FILE(SeismicVolumeFile, dds);
    OSP_REGISTER_VOLUME_FILE(SeismicVolumeFile, sgy);
    OSP_REGISTER_VOLUME_FILE(SeismicVolumeFile, segy);

} // namespace ospray
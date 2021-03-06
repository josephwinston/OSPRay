// ======================================================================== //
// Copyright 2009-2014 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

//ospray
#include "ospray/volume/BlockBrickedVolume.h"
#include "BlockBrickedVolume_ispc.h"
// std
#include <cassert>

namespace ospray {

  void BlockBrickedVolume::createEquivalentISPC() 
  {
    //! Get the voxel type.
    voxelType = getParamString("voxelType", "unspecified");  exitOnCondition(getVoxelType() == OSP_UNKNOWN, "unrecognized voxel type");

    //! Create an ISPC BlockBrickedVolume object and assign type-specific function pointers.
    ispcEquivalent = ispc::BlockBrickedVolume_createInstance((int) getVoxelType());

    //! Get the volume dimensions.
    volumeDimensions = getParam3i("dimensions", vec3i(0));  exitOnCondition(reduce_min(volumeDimensions) <= 0, "invalid volume dimensions");

    //! Get the transfer function.
    transferFunction = (TransferFunction *) getParamObject("transferFunction", NULL);  exitOnCondition(transferFunction == NULL, "no transfer function specified");

    //! Get the value range.
    //! Voxel range not used for now.
    // vec2f voxelRange = getParam2f("voxelRange", vec2f(0.0f));  exitOnCondition(voxelRange == vec2f(0.0f), "no voxel range specified");

    //! Get the gamma correction coefficient and exponent.
    vec2f gammaCorrection = getParam2f("gammaCorrection", vec2f(1.0f));

    //! Set the volume dimensions.
    ispc::BlockBrickedVolume_setVolumeDimensions(ispcEquivalent, (const ispc::vec3i &) volumeDimensions);

    //! Set the value range (must occur before setting the transfer function).
    //ispc::BlockBrickedVolume_setValueRange(ispcEquivalent, (const ispc::vec2f &) voxelRange);

    //! Set the transfer function.
    ispc::BlockBrickedVolume_setTransferFunction(ispcEquivalent, transferFunction->getEquivalentISPC());

    //! Set the recommended sampling rate for ray casting based renderers.
    ispc::BlockBrickedVolume_setSamplingRate(ispcEquivalent, getParam1f("samplingRate", 1.0f));

    //! Set the gamma correction coefficient and exponent.
    ispc::BlockBrickedVolume_setGammaCorrection(ispcEquivalent, (const ispc::vec2f &) gammaCorrection);

    //! Allocate memory for the voxel data in the ISPC object.
    ispc::BlockBrickedVolume_allocateMemory(ispcEquivalent);

  }

  void BlockBrickedVolume::finish() 
  {
    //! The ISPC volume container must exist at this point.
    assert(ispcEquivalent != NULL);

    //! Complete volume initialization.
    ispc::BlockBrickedVolume_finish(ispcEquivalent);

  }

  void BlockBrickedVolume::setRegion(const void *source, 
                                     const vec3i &index, 
                                     const vec3i &count)
  {
    //! Range check.
    //! Copy voxel data into the volume.
    ispc::BlockBrickedVolume_setRegion(ispcEquivalent, source, (const ispc::vec3i &) index, (const ispc::vec3i &) count);

  }

  void BlockBrickedVolume::updateEditableParameters() 
  {
    //! Get the transfer function.
    transferFunction = (TransferFunction *) getParamObject("transferFunction", NULL);  exitOnCondition(transferFunction == NULL, "no transfer function specified");

    //! Get the gamma correction coefficient and exponent.
    vec2f gammaCorrection = getParam2f("gammaCorrection", vec2f(1.0f));

    //! Set the gamma correction coefficient and exponent.
    ispc::BlockBrickedVolume_setGammaCorrection(ispcEquivalent, (const ispc::vec2f &) gammaCorrection);

    //! Set the recommended sampling rate for ray casting based renderers.
    ispc::BlockBrickedVolume_setSamplingRate(ispcEquivalent, getParam1f("samplingRate", 1.0f));

    //! Set the transfer function.
    ispc::BlockBrickedVolume_setTransferFunction(ispcEquivalent, transferFunction->getEquivalentISPC());

  }

} // ::ospray


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

// obj
#include "OBJRenderer.h"
#include "OBJMaterial.h"
#include "OBJPointLight.h"
#include "OBJSpotLight.h"
// ospray
#include "ospray/common/Model.h"
#include "ospray/common/Data.h"
#include "ospray/camera/Camera.h"
//embree
#include "embree2/rtcore.h"
//sys
#include <vector>
// ispc exports
#include "OBJRenderer_ispc.h"

namespace ospray {
  namespace obj {

    void OBJRenderer::commit()
    {
      Renderer::commit();

      world = (Model *)getParamObject("world",NULL);
      world = (Model *)getParamObject("model",world);
      camera = (Camera *)getParamObject("camera",NULL);

      pointLightData = (Data*)getParamData("pointLights",NULL);

      if (pointLightData && pointLightArray.empty()) {
        for (int i = 0; i < pointLightData->size(); i++) {
          pointLightArray.push_back(((Light**)pointLightData->data)[i]->getIE());
        }
      }

      dirLightData = (Data*)getParamData("directionalLights", NULL);

      if (dirLightData && dirLightArray.empty()) {
        for (int i = 0; i < dirLightData->size(); i++) {
          Light *light_i = ((Light**)dirLightData->data)[i];
          dirLightArray.push_back(light_i->getIE());
        }
      }

      spotLightData = (Data*)getParamData("spotLights", NULL);
      if( spotLightData && spotLightArray.empty()) {
        for (int i =0; i < spotLightData->size(); i++) {
          spotLightArray.push_back(((Light**)spotLightData->data)[i]->getIE());
        }
      }
      
      void **pointLightPtr = pointLightArray.empty() ? NULL : &pointLightArray[0];
      void **dirLightPtr = dirLightArray.empty() ? NULL : &dirLightArray[0];
      void **spotLightPtr = spotLightArray.empty() ? NULL : &spotLightArray[0];

      vec3f bgColor;
      bgColor = getParam3f("bgColor", vec3f(1.f));

      bool shadowsEnabled = bool(getParam1i("shadowsEnabled", 1));
      ispc::OBJRenderer_set(getIE(),
                            world?world->getIE():NULL,
                            camera?camera->getIE():NULL,
                            (ispc::vec3f&)bgColor,
                            shadowsEnabled,
                            pointLightPtr, pointLightArray.size(),
                            dirLightPtr,   dirLightArray.size(),
                            spotLightPtr,  spotLightArray.size());
    }
    
    OBJRenderer::OBJRenderer()
    {
      ispcEquivalent = ispc::OBJRenderer_create(this);
    }

    /*! \brief create a material of given type */
    Material *OBJRenderer::createMaterial(const char *type)
    {
      Material *mat = new OBJMaterial;
      return mat;
    }

    /*! \brief create a light of given type */
    Light *OBJRenderer::createLight(const char *type)
    {
      Light *light = NULL;

      if (strcmp("PointLight", type) == 0) {
        light = new OBJPointLight;
      } else if (strcmp("SpotLight", type) == 0) {
        light = new OBJSpotLight;
      }

      return light;
    }

    OSP_REGISTER_RENDERER(OBJRenderer,OBJ);
    OSP_REGISTER_RENDERER(OBJRenderer,obj);

  } // ::ospray::obj
} // ::ospray

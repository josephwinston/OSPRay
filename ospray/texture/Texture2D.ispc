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

#include "Texture2D.ih"

inline vec4f getTexel4uc(const uniform Texture2D *uniform THIS,
                         const uint32 ix, const uint32 iy)
{
  assert(THIS);
  const uint32 c = ((uniform uint32 *uniform)THIS->data)[ix+THIS->size.x*iy];
  const float one_over_255 = (1.f/255.f);
  const uint32 r = c         & 255;
  const uint32 g = (c >>  8) & 255;
  const uint32 b = (c >> 16) & 255;
  const uint32 a = (c >> 24) & 255;
  return make_vec4f(r*one_over_255, g*one_over_255, b*one_over_255, a*one_over_255);
}

inline vec4f getTexel4f(const uniform Texture2D *uniform THIS,
                        const uint32 ix, const uint32 iy)
{
  assert(THIS);
  const vec4f c = ((uniform vec4f *uniform)THIS->data)[ix+THIS->size.x*iy];
  return c;
}

inline vec4f getTexel3f(const uniform Texture2D *uniform THIS,
                         const uint32 ix, const uint32 iy)
{
  assert(THIS);
  vec3f v = ((uniform vec3f*uniform )THIS->data)[ix+THIS->size.x*iy];
  return make_vec4f(v.x,v.y,v.z,0.f);
}

inline vec4f getTexel3uc(const uniform Texture2D *uniform THIS,
                         const uint32 ix, const uint32 iy)
{
  assert(THIS);
  const uniform uint8 *uniform pixel = (const uniform uint8 *uniform)THIS->data;
  const float one_over_255 = (1.f/255.f);
  const uint32 pixelOfs = 3*(ix+THIS->size.x*iy);
  const uint32 r = pixel[pixelOfs+0];
  const uint32 g = pixel[pixelOfs+1];
  const uint32 b = pixel[pixelOfs+2];
  return make_vec4f(r*one_over_255, g*one_over_255, b*one_over_255, 0.f);
}

static void Texture2D_4uc_bilinear(const uniform Texture2D *uniform _this,
                                   varying vec4f &ret,
                                   const varying vec2f &p)
{
  assert(_this);
  const uniform Texture2D *uniform this = (const uniform Texture2D *uniform)_this;

  // repeat: get remainder within [0.1] parameter space
  float fx = p.x-floor(p.x);
  float fy = p.y-floor(p.y);

  // scale by texture size
  fx *= this->f_size_1.x;
  fy *= this->f_size_1.y;

  const int x0 = (int)(fx);
  const int y0 = (int)(fy);
  const int x1 = (int)(min(fx+1.f,this->f_size_1.x));
  const int y1 = (int)(min(fy+1.f,this->f_size_1.y));
  fx = fx - floor(fx);
  fy = fy - floor(fy);
  
  const vec4f c00 = getTexel4uc(this,x0,y0);
  const vec4f c01 = getTexel4uc(this,x1,y0);
  const vec4f c10 = getTexel4uc(this,x0,y1);
  const vec4f c11 = getTexel4uc(this,x1,y1);
  ret = lerp(fy,
             lerp(fx,c00,c01),
             lerp(fx,c10,c11));
}

static void Texture2D_3uc_bilinear(const uniform Texture2D *uniform _this,
                                   varying vec4f &ret,
                                   const varying vec2f &p)
{
  assert(_this);
  const uniform Texture2D *uniform this = (const uniform Texture2D *uniform)_this;

  // repeat: get remainder within [0.1] parameter space
  float fx = p.x-floor(p.x);
  float fy = p.y-floor(p.y);

  // scale by texture size
  fx *= this->f_size_1.x;
  fy *= this->f_size_1.y;

  const int x0 = (int)(fx);
  const int y0 = (int)(fy);
  const int x1 = (int)(min(fx+1.f,this->f_size_1.x));
  const int y1 = (int)(min(fy+1.f,this->f_size_1.y));
  fx = fx - floor(fx);
  fy = fy - floor(fy);
  
  const vec4f c00 = getTexel3uc(this,x0,y0);
  const vec4f c01 = getTexel3uc(this,x1,y0);
  const vec4f c10 = getTexel3uc(this,x0,y1);
  const vec4f c11 = getTexel3uc(this,x1,y1);
  ret = lerp(fy,
             lerp(fx,c00,c01),
             lerp(fx,c10,c11));
}

static void Texture2D_4f_bilinear(const uniform Texture2D *uniform texture,
                                  varying vec4f &ret,
                                  const varying vec2f &p)
{
  assert(texture);

  const uniform Texture2D *uniform this = (const uniform Texture2D *uniform)texture;

  // repeat: get remainder within [0.1] parameter space
  float fx = p.x-floor(p.x);
  float fy = p.y-floor(p.y);

  // scale by texture size
  fx *= this->f_size_1.x;
  fy *= this->f_size_1.y;

  const int x0 = (int)(fx);
  const int y0 = (int)(fy);
  const int x1 = (int)(min(fx+1.f,this->f_size_1.x));
  const int y1 = (int)(min(fy+1.f,this->f_size_1.y));
  fx = fx - floor(fx);
  fy = fy - floor(fy);
  
  const vec4f c00 = getTexel4f(this,x0,y0);
  const vec4f c01 = getTexel4f(this,x1,y0);
  const vec4f c10 = getTexel4f(this,x0,y1);
  const vec4f c11 = getTexel4f(this,x1,y1);
  ret = lerp(fy,
             lerp(fx,c00,c01),
             lerp(fx,c10,c11));
}

static void Texture2D_3f_bilinear(const uniform Texture2D *uniform texture,
                              varying vec4f &ret,
                              const varying vec2f &p)
{
  assert(texture);

  const uniform Texture2D *uniform this = (const uniform Texture2D *uniform)texture;

  // repeat: get remainder within [0.1] parameter space
  float fx = p.x-floor(p.x);
  float fy = p.y-floor(p.y);

  // scale by texture size
  fx *= this->f_size_1.x;
  fy *= this->f_size_1.y;

  const int x0 = (int)(fx);
  const int y0 = (int)(fy);
  const int x1 = (int)(min(fx+1.f,this->f_size_1.x));
  const int y1 = (int)(min(fy+1.f,this->f_size_1.y));
  fx = fx - floor(fx);
  fy = fy - floor(fy);
  
  const vec4f c00 = getTexel3f(this,x0,y0);
  const vec4f c01 = getTexel3f(this,x1,y0);
  const vec4f c10 = getTexel3f(this,x0,y1);
  const vec4f c11 = getTexel3f(this,x1,y1);
  ret = lerp(fy,
             lerp(fx,c00,c01),
             lerp(fx,c10,c11));
}

uniform Texture2D *uniform Texture2D_create(void *uniform cppE,
                                            uniform uint32 sizeX,
                                            uniform uint32 sizeY,
                                            void *uniform data,
                                            uniform Texture2D_get get)
{
  uniform Texture2D *uniform tex = uniform new uniform Texture2D;
  tex->size     = make_vec2ui(sizeX,    sizeY);
  tex->f_size   = make_vec2f (sizeX,    sizeY);
  tex->f_size_1 = make_vec2f (sizeX-1.f,sizeY-1.f);
  tex->data   = data;
  tex->get    = get;
  return tex;
}

export void *uniform Texture2D_4uc_create(void *uniform cppE, 
                                          uniform uint32 sizeX,
                                          uniform uint32 sizeY,
                                          void *uniform data)
{ return Texture2D_create(cppE,sizeX,sizeY,data,&Texture2D_4uc_bilinear); };

export void *uniform Texture2D_4f_create(void *uniform cppE,
                                         uniform uint32 sizeX,
                                         uniform uint32 sizeY,
                                         void *uniform data)
{ return Texture2D_create(cppE,sizeX,sizeY,data,&Texture2D_4f_bilinear); };

export void *uniform Texture2D_3f_create(void *uniform cppE,
                                         uniform uint32 sizeX,
                                         uniform uint32 sizeY,
                                         void *uniform data)
{ return Texture2D_create(cppE,sizeX,sizeY,data,&Texture2D_3f_bilinear); };

export void *uniform Texture2D_3uc_create(void *uniform cppE, 
                                          uniform uint32 sizeX,
                                          uniform uint32 sizeY,
                                          void *uniform data)
{ return Texture2D_create(cppE,sizeX,sizeY,data,&Texture2D_3uc_bilinear); };


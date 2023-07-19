//
//  MetalRender.hpp
//  metal-support
//
//  Created by chenqiudu on 2023/7/18.
//

#ifndef MetalRender_hpp
#define MetalRender_hpp

#include "QuartzCore/QuartzCore.hpp"

namespace xxd
{
class Camera;
class Renderer
{
public:
    Renderer(Camera* cam, CA::MetalLayer* layer);
    void Draw();
private:
    CA::MetalLayer* surface;
    Camera* renderCam;
};

};

#endif /* MetalRender_hpp */

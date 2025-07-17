#pragma

#include "core/Base.h"

namespace sal {

    class Texture;

    enum Primitive {
        Lines,
        Triangles,
    };

    enum BlendFactor {
        Zero,
        One,
        SrcAlpha,
        OneMinusScrAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
    };

    class RenderAPI {
    public:
        static void EnableBlending(BlendFactor sfactor, BlendFactor dfactor);
        static void Clear(const glm::vec4& color);
        static void DrawBuffer(Primitive type, size_t count);
        static void DrawBufferIndexed(Primitive type, size_t count);
        static void BindTextureUnit(Ref<Texture> texture, int unit);
    };

}
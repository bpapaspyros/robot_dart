#include "shadow_map.hpp"
#include "create_compatibility_shader.hpp"

namespace robot_dart {
    namespace gui {
        namespace magnum {
            namespace gs {
                ShadowMap::ShadowMap()
                {
                    Corrade::Utility::Resource rs_shaders("RobotDARTShaders");

                    const Magnum::GL::Version version = Magnum::GL::Context::current().supportedVersion(
                        {Magnum::GL::Version::GL400});

                    Magnum::GL::Shader vert = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Vertex);
                    Magnum::GL::Shader frag = Magnum::Shaders::Implementation::createCompatibilityShader(
                        rs_shaders, version, Magnum::GL::Shader::Type::Fragment);

                    vert.addSource(rs_shaders.get("generic.glsl"))
                        .addSource(rs_shaders.get("ShadowMap.vert"));
                    frag.addSource(rs_shaders.get("ShadowMap.frag"));

                    CORRADE_INTERNAL_ASSERT_OUTPUT(Magnum::GL::Shader::compile({vert, frag}));

                    attachShaders({vert, frag});

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_attrib_location>(version)) {
                        bindAttributeLocation(Position::Location, "position");
                    }

                    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

                    if (!Magnum::GL::Context::current().isExtensionSupported<Magnum::GL::Extensions::ARB::explicit_uniform_location>(version)) {
                        _transformationMatrixUniform = uniformLocation("transformationMatrix");
                        _projectionMatrixUniform = uniformLocation("projectionMatrix");
                    }
                }

                ShadowMap::ShadowMap(Magnum::NoCreateT) noexcept : Magnum::GL::AbstractShaderProgram{Magnum::NoCreate} {}

                ShadowMap& ShadowMap::setTransformationMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_transformationMatrixUniform, matrix);
                    return *this;
                }

                ShadowMap& ShadowMap::setProjectionMatrix(const Magnum::Matrix4& matrix)
                {
                    setUniform(_projectionMatrixUniform, matrix);
                    return *this;
                }
            } // namespace gs
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart
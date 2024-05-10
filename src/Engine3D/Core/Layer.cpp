#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Core/Layer.h>

namespace Engine3D{
    Layer::Layer(const std::string& name) : _debugName(name){}

    Layer::~Layer(){}
};
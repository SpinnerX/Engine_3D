#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/Core/Layer.h>

namespace RendererEngine{
    Layer::Layer(const std::string& name) : _debugName(name){}

    Layer::~Layer(){}
};
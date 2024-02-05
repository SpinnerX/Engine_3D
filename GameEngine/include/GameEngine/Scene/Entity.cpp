#include <GameEngine/Scene/Entity.h>


namespace RendererEngine{

	Entity::Entity(entt::entity handle, Scene* scene) : _entityHandler(handle), _scene(scene) {}
};

#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Event/Event.h>
#include <Engine3D/Core/Timestep.h>
#include <stdexcept>

namespace Engine3D{

    // Layer interface
    // - Shouldn't have names (but only if its used for debugging)
    // TODO: allow enabling/disabling layers meaning not being updated or rendering (essentially hiding from layer stack)

    /**
     * @param OnAttach
     * @note Initiating layers
     * 
     * @param OnDetach
     * @note Deinitiating our laters (doing special cleanup)
     *
     * @param OnUpdate
     * @note Update our frames that occur within our layer
     */
    class ENGINE_API Layer{
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        // When pushing onto the Layer Stack we attach it
        // Pop of thhe layer stack is for detaching (like shhutdown)
        virtual void OnAttach() { }

        virtual void OnDetach() { }

        // Updating the layer is updated when the application calls onUpdate
        virtual void OnUpdate(Timestep ts) { }

        virtual void OnUIRender() { }

        // Whichh is wherever events get sent to the layer, it'll know how to handle that event.
        virtual void OnEvent(Event& event){ }

        inline const std::string& getName() const { return _debugName; }

        friend std::ostream& operator<<(std::ostream& outs, const Layer* layer){
            return outs << layer->_debugName;
        }
    protected:
        std::string _debugName;
    };
};
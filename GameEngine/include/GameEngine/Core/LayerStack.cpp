#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/Core/LayerStack.h>

namespace RendererEngine{
    LayerStack::LayerStack(){
    }

    LayerStack::~LayerStack(){
        for(Layer* layer : _layers){
            delete layer;
        }
    }

    // pushing into the layer stack, and making sure your pushing into the right place
    // Layers get pushed to thhe first half of the list
    void LayerStack::pushLayer(Layer* layer){
        _layers.emplace(_layers.begin() + _layersInsertIndex, layer);
        _layersInsertIndex++;
    }

    // Overlays gets pushhed into thhe second half of the list.
    void LayerStack::pushOverlay(Layer* overlay){
        _layers.emplace_back(overlay);
    }

    // How ownership works with layers, are kind of interesting.
    // When you pop layers, you dont lose them, only when layers gets destroyed
    // - Another thing is that the Layer Stack is owned by the application, which means that 
    //  basically the way this works is that you can allocate your layer and as long you give it
    //  to the layer stack and don't take it out, thhey will be deallocated when the application shuts down
    // - Layers are meant to live throughout the lifetime of your application at this given point in time.
    // - In the future, if you create levels or anythhing like that then that is when it is worthhwhhile
    //   to recreate thhe entire layer stack
    void LayerStack::popLayer(Layer* layer){

        // Search for specific layer, then delete that layer
        // Then decrement the insertion iterator count.
        // Removing from actual vector but not actually deleted.
        auto iter = std::find(_layers.begin(), _layers.end(), layer);

        if(iter != _layers.end()){
            _layers.erase(iter);
            _layersInsertIndex--;
        }
    }

    // Reason why its done that way is because overlays, you always want to render last
    // - If you push an overlay just at any point, then you want to ensure that it suddenly gets put
    //   at the very end
    // - Whereas layers gets pushed before the overlays
    //  - meaning it can get pushed in the last layer, but not in front of overlays. (overlays will always be behind layers)
    void LayerStack::popOverlay(Layer* overlay){
        auto iter = std::find(_layers.begin(), _layers.end(), overlay);

        if(iter != _layers.end()){
            _layers.erase(iter);
        }
    }
};
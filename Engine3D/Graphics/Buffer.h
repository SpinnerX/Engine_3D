#pragma once
#include <ostream>
#include <vector>
#include <Engine3D/Core/core.h>

namespace Engine3D{
    // Buffer.h
    // - will represent Vertex buffer and Index buffer
    // - These are our pure virtual interfaces meaning no impl
    // - No member variables, and should not take no storage, only 1 byte when we initialize because it has to
    // - Literally just a vtable of functions of thosen virtual functions that'll get dispatched to whatever kind of
    //   subclass that realizes this class.
    // - Also containing BufferLayout and Buffer Element classes
    //      - These layouts are how we may want to layout our buffer data that we can use to send into our vertex 
    //        buffers, and shaders
    // BufferElement
    //      - offset
    //          - for the offset to be calculated, first we are going to need to be aware of all elements in the BufferLayout.

    enum class ShaderDataType : uint8_t {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type){
        switch (type){
            case ShaderDataType::None:
                return 0;
            case ShaderDataType::Float: // A float is 4 bytes.
                return 4;
            case ShaderDataType::Float2: // A float2 is 4*2 bytes.
                return 4 * 2;
            case ShaderDataType::Float3: // A float3 is 4*3 bytes.
                return 4 * 3;
            case ShaderDataType::Float4: // A float4 is 4*4 bytes.
                return 4 * 4;
            case ShaderDataType::Mat3: // A Mat3 is 4*3*3 bytes.
                return 4 * 3 * 3;
            case ShaderDataType::Mat4: // A float4 is 4*4*4 bytes.
                return 4 * 4 * 4;
            case ShaderDataType::Int: // A Int is 4 bytes.
                return 4;
            case ShaderDataType::Int2: // A Int2 is 4*2 bytes.
                return 4 * 2;
            case ShaderDataType::Int3: // A Int3 is 4*3 bytes.
                return 4 * 3;
            case ShaderDataType::Int4: // A Int4 is 4 bytes.
                return 4;
            case ShaderDataType::Bool: // A Bool is 1 byte.
                return 1;
        }
        // render_core_assert(false, "Unknown ShaderDataType!");
        assert(false);
        return 0;
    }

    // Simply just for debugging purpose.
    static std::string ShaderDataTypeToString(ShaderDataType type){
        switch (type){
            case ShaderDataType::None: return "ShaderDataType::None";
            case ShaderDataType::Float:  return "ShaderDataType::Float";
            case ShaderDataType::Float2: return "ShaderDataType::Float2";
            case ShaderDataType::Float3: return "ShaderDataType::Float3";
            case ShaderDataType::Float4: return "ShaderDataType::Float4";
            case ShaderDataType::Mat3:   return "ShaderDataType::Mat3";
            case ShaderDataType::Mat4:   return "ShaderDataType::Mat4";
            case ShaderDataType::Int:    return "ShaderDataType::Int";
            case ShaderDataType::Int2:   return "ShaderDataType::Int2";
            case ShaderDataType::Int3:   return "ShaderDataType::Int3";
            case ShaderDataType::Int4:   return "ShaderDataType::Int4";
            case ShaderDataType::Bool:   return "ShaderDataType::Bool";
        }

        return "Default: meaning no type was given!";
    }

    struct BufferElement{
        BufferElement() = default;
        BufferElement(ShaderDataType t, const std::string& n, bool normalized = false) : name(n), type(t), size(ShaderDataTypeSize(type)), offset(0), isNormalized(normalized) {}

        uint32_t GetComponentCount() const {
            switch (type){
                case ShaderDataType::None: return 0;
                case ShaderDataType::Float:  return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3:   return 3 * 3;
                case ShaderDataType::Mat4:   return 4 * 4;
                case ShaderDataType::Int:    return 1;
                case ShaderDataType::Int2:   return 2;
                case ShaderDataType::Int3:   return 3;
                case ShaderDataType::Int4:   return 4;
                case ShaderDataType::Bool:   return 1;
            }

            // render_core_assert(false, "Unknown ShaderDataType!");
            assert(false);
            return 0;
        }
        friend inline std::ostream& operator<<(std::ostream& outs, BufferElement& element);

        std::string name;
        ShaderDataType type;
        uint32_t offset; // Offset is going to be the offset of this elemenmt inside our BufferLayout class below
        uint32_t size; // size of buffer
        bool isNormalized;
    };

    // This is just for debugging purposes
    std::ostream& operator<<(std::ostream& outs, BufferElement& element){
        outs << "Name: " << element.name << '\n'
                << "Type: " << ShaderDataTypeToString(element.type) << '\n'
                << "Offset: " << element.offset << '\n'
                << "Size: " << element.size << '\n'
                << "Is Normalized: " << (bool)element.isNormalized;
        
        return outs;
    }

    class BufferLayout{
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& list) : _elements(list) {
            // Once we have our data, now we can calculate the offset
            calculateOffsetAndStride();
        }

        inline uint32_t getStride() const {
            return _stride;
        }

        std::vector<BufferElement>::iterator begin() {
            return _elements.begin();
        }

        std::vector<BufferElement>::iterator end() {
            return _elements.end();
        }

        // NOTE: To iterate using for range loops, we need const_iterator
        std::vector<BufferElement>::const_iterator begin() const {
            return _elements.begin();
        }

        std::vector<BufferElement>::const_iterator end() const {
            return _elements.end();
        }

        inline const std::vector<BufferElement>& getElements() const {
            return _elements;
        }

    private:
        void calculateOffsetAndStride(){
            uint32_t offset = 0;
            _stride = 0;

            // For every element we compute the offset, and while fetching the offset we get the stride.
            for(auto& element : _elements){
                element.offset = offset;
                offset += element.size;
                _stride += element.size;
            }
        }

    private:
        std::vector<BufferElement> _elements;
        uint32_t _stride = 0;
    };


	enum class VertexBufferUsage{
		NONE=0,
		STATIC=1,
		DYNAMIC=2
	};

    // Vertex Buffer (what does a vertex buffer need?)
    // - Needs a bind()
    // - 
    class VertexBuffer{
    public:
        virtual ~VertexBuffer(){}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
		
		static Ref<VertexBuffer> Create(uint32_t size);

        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
    };
	
	// Currently the engine only supports 32-bit index buffers
    class IndexBuffer{
    public:
        virtual ~IndexBuffer(){}
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t getCount() const = 0;

        static Ref<IndexBuffer> Create(uint32_t* vertices, uint32_t size);
    };
};

#include <Engine3D/Graphics/UniformBuffer.h>

namespace Engine3D{
	class OpenGLUniformBuffer : public UniformBuffer{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);

		virtual ~OpenGLUniformBuffer();

		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t _rendererID = 0;
	};
};

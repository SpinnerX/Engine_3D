#pragma once
#include <Engine3D/Graphics/Framebuffer.h>
#include <map>

namespace Engine3D{
	
	class OpenGLFrameBuffer : public FrameBuffer{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecifications& specs);
		virtual ~OpenGLFrameBuffer();

		void Bind() override;
		void Unbind() override;
		
		virtual void resize(uint32_t w, uint32_t h) override;
		virtual int readPixel(uint32_t attachmentIndex, int x, int y) override;
		
		virtual void clearColorAttachment(uint32_t attachmentIndex, int data) override;
		
		virtual const FrameBufferSpecifications& getSpecifications() const override { return _specifications; }
		virtual uint32_t getColorAttachmentRendererID(uint32_t index=0) const override { assert(index < _specifications.attachments.attachments.size()); return colorAttachments[index].attachmentID; }
		
		// @function invalidate
		// @note means that something has been changed/modified inside the frame buffer.
		// @note meaning it is no longer valid.
		void invalidate();

	private:
		void colorAttachment(GLenum target, uint32_t& attachmentID, GLenum internalFormat, GLenum format);
		void depthAttachment(GLenum target, uint32_t& attachmentID, GLenum internalFormat, GLenum format);

	private:
		uint32_t _rendererID;
		FrameBufferSpecifications _specifications;
		std::vector<FrameBufferTextureFormat> attachmentFormats;
		uint32_t attachmentID = 0; // attachment id and the index of where in the map attachment is

		std::vector<FrameBufferTextureSpecifications> colorAttachments;
		FrameBufferTextureSpecifications depthAttachmentAttachmentSpec = FrameBufferTextureFormat::None; // Keeping track of state.
	};
};

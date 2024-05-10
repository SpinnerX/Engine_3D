#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/OpenGL/OpenGLFramebuffer.h>

namespace Engine3D{
	static const uint32_t maxFrameBufferSize = 8192;

	static GLenum textureTarget(bool multisample){
		return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static bool isDepth(FrameBufferTextureFormat format){
		if(format == FrameBufferTextureFormat::DEPTH24STENCIL8) return true;

		return false;
	}

	static GLenum engineFBTexturFormatToGL(FrameBufferTextureFormat format){
		switch (format) {
			case Engine3D::FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
			case Engine3D::FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			default: return 0;
		}
		return 0;
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecifications& specs) : _specifications(specs){
		if(specs.attachments.attachments.size() <= 0){
			assert(specs.attachments.attachments.size() > 0);
		}

		for(auto spec : specs.attachments.attachments){
			attachmentFormats.push_back(spec.textureFormat);
			if(!isDepth(spec.textureFormat)){
				colorAttachments.emplace_back(spec);
			}
			else{
				depthAttachmentAttachmentSpec = std::move(spec);
			}
		}

		invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer(){
		glDeleteFramebuffers(1, &_rendererID);
		for(auto spec : _specifications.attachments.attachments){
			glDeleteTextures(1, &spec.attachmentID);
		}

		glDeleteTextures(1, &depthAttachmentAttachmentSpec.attachmentID);
		colorAttachments.clear();
	}

	void OpenGLFrameBuffer::invalidate(){
		
		// Validating if the renderer ID has already been set.
		if(!_rendererID){
			glDeleteFramebuffers(1, &_rendererID);
		}

		// Quick NOTE: glCreateFramebuffers and glCreateTextures work on OpenGL 4.5 and versions afterwards
		/* glCreateFramebuffers(1, &_rendererID); */
		glGenFramebuffers(1, &_rendererID);

		glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);

		if(depthAttachmentAttachmentSpec.textureFormat == FrameBufferTextureFormat::None){
			return;
		}

		bool multisample = _specifications.samples > 1;

		// Color Attachments
		if(colorAttachments.size() > 0){
			for(size_t i = 0; i < colorAttachments.size(); i++){
				glGenTextures(1, &colorAttachments[i].attachmentID);
				glBindTexture(GL_TEXTURE_2D, colorAttachments[i].attachmentID);
				// @note internalFormat is data to be stored
				// @note format is data that will be accessed

				switch(colorAttachments[i].textureFormat){
					case Engine3D::FrameBufferTextureFormat::RGBA8:
					{
						/* colorAttachment(textureTarget(multisample), colorAttachments[i].attachmentID, GL_RGBA8, GL_RGBA); */

						if(multisample){
							glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _specifications.samples, GL_RGBA8, _specifications.width, _specifications.height, GL_FALSE);
						}
						else{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _specifications.width, _specifications.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, textureTarget(multisample), colorAttachments[i].attachmentID, 0);
						}
					}
						break;
					case Engine3D::FrameBufferTextureFormat::RED_INTEGER:
						{
							if(multisample){
								glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _specifications.samples, GL_RGBA8, _specifications.width, _specifications.height, GL_FALSE);
							}
							else{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, _specifications.width, _specifications.height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
					
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
								glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, textureTarget(multisample), colorAttachments[i].attachmentID, 0);
							}
						}
						break;
					default:
						break;
				}
				/* glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachments[i].attachmentID, 0); */
				/* glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, textureTarget(multisample), colorAttachments[i].attachmentID, 0); */
			}
		}

		// Depth attachment
		if(depthAttachmentAttachmentSpec.textureFormat != FrameBufferTextureFormat::None){
			glGenTextures(1, &depthAttachmentAttachmentSpec.attachmentID);
			glBindTexture(GL_TEXTURE_2D, depthAttachmentAttachmentSpec.attachmentID);
			
			if(multisample){
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _specifications.samples, GL_DEPTH24_STENCIL8, _specifications.width, _specifications.height, GL_FALSE);
			}
			else{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _specifications.width, _specifications.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, textureTarget(multisample), depthAttachmentAttachmentSpec.attachmentID, 0);
		}
		
		if(colorAttachments.size() > 1){
			GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
			glDrawBuffers(colorAttachments.size(), buffers);
		}
		else if(colorAttachments.empty()){
			glDrawBuffer(GL_NONE);
		}

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){	
			coreLogError("Framebuffer is incomplete!");
			// render_core_assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");
			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::colorAttachment(GLenum target, uint32_t& attachmentID, GLenum internalFormat, GLenum format){
		bool multisample = _specifications.samples > 1;
		if(multisample){
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _specifications.samples, GL_RGBA8, _specifications.width, _specifications.height, GL_FALSE);
		}
		else{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _specifications.width, _specifications.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		/* if(multisample){ */
		/* 	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _specifications.samples, internalFormat, _specifications.width, _specifications.height, GL_FALSE); */
		/* } */
		/* else{ */
		/* 	/1* glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _specifications.width, _specifications.height, 0, format, GL_UNSIGNED_BYTE, nullptr); *1/ */
		/* 	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _specifications.width, _specifications.height, 0, format, GL_UNSIGNED_INT_24_8, NULL); */
		/* 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); */
		/* 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */
		/* 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); */
		/* 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); */
		/* 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); */
		/* } */
		
	}
		
	void OpenGLFrameBuffer::depthAttachment(GLenum target, uint32_t& attachmentID, GLenum internalFormat, GLenum format){
		/* bool multisample = _specifications.samples > 1; */
		glTexImage2D(target, 0, format, _specifications.width, _specifications.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void OpenGLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);	
		glViewport(0, 0, _specifications.width, _specifications.height);
	}

	void OpenGLFrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::resize(uint32_t w, uint32_t h){
		if(w == 0 || h == 0 || w > maxFrameBufferSize || h > maxFrameBufferSize){
			coreLogError("Attempted to resize frame buffer to (w={}, h={})", w, h);
			return;
		}

		_specifications.width = w;
		_specifications.height = h;
		this->invalidate();
	}
	
	int OpenGLFrameBuffer::readPixel(uint32_t attachmentIndex, int x, int y){
		// @note TODO: In the future we may want to have multiple selections
		// @note Where we might want to read an entire region and scan to see what colors(entities) are there.
		assert(attachmentIndex < colorAttachments.size());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData; // Reading data into this memory buffer.
		glReadPixels(x, y, 1, 1 , GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}
	
	void OpenGLFrameBuffer::clearColorAttachment(uint32_t attachmentIndex, int data){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glClearBufferiv(colorAttachments[attachmentIndex].attachmentID, 0, &data);
	}
	
};


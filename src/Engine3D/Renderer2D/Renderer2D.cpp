#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Renderer2D/Renderer2D.h>

namespace Engine3D{
	/*
	 * @class quadVertex
	 * @note Initially contains our vertices data
	 * @note These are data that will be used throughout the renderer
	 * @note QuadVertex is used to storing vertices.
	 *
	 *
	*/
	struct quadVertex{
		glm::vec3 pos; // @note contains position of the texture
		glm::vec4 color; // @note color the texture contains
		glm::vec2 texCoord;
		float texIndex; // @note containing whatever texture we have (if zero then it'll sample from white texture and draw nothing)
		float tilingFactor;

		// @note Editor-only stuff
		int entityID = 0;
	};
	
	/*
	 * @class Renderer2DData
	 * @note Storage class for storing our vertices into our buffers.
	 * @note Manages the data stored and handling data before the rendering stage.
	 * @note Keeps track of memory allocations, indices, textures slots effectively during the rendering process.
	 * 
	 * @param maxQuads
	 * @note Setting what the max quads that can be rendered
	 * 
	 * @param maxVertices
	 * @note Setting the max vertices that can be rendered.
	 * 
	 * @param maxIndices
	 * @note Setting max indices vertices that can be rendered
	 * 
	 * @param maxTextureSlots
	 * @note Setting max texture slots that can be attached.
	 * @note Since this differentiates based on the platform.
	 * 
	 * @param quadVertexArray
	 * @note Vertex Array for our draw quads every
	 * 
	 * @param quadVertexBuffer
	 * @param textureShader
	 * @param whiteTexture
	 * @param quadIndexCount
	 * 
	 * @param quadVertexBufferBase: ptr
	 * 
	 * @param quadVertexBufferPtr
	 * @note Everytime we make a draw call to DrawQuad we add the data into this buffer
	 * @note Storing all that information in this quadVertexBufferPtr.
	 * 
	 * @param textureSlots
	 * @note Contains an array of texture slots that have been submitted to the renderer
	 * 
	 * @param quadVertexPositions
	 * @note Used for keeping track of the quad vertex positions
	 * 
	 * @param Renderer2D::Statistics stats
	 * @note
	 * 
	*/
	struct Renderer2DData{
		static const uint32_t maxQuads = 10000; // max for how many quads we can Render
		static const uint32_t maxVertices = maxQuads * 4; // max of vertices we can have every single draw quad
		static const uint32_t maxIndices = maxQuads * 6; // Since we have around 6 indices per quad
		static const uint32_t maxTextureSlots = 16; // Mac = 16, Windows maybe = 32.
		Ref<VertexArray> quadVertexArray; // @note Vertex array will contain all of our Vertex buffer, initially.
		Ref<VertexBuffer> quadVertexBuffer; // @note Our actual vertex buffer.
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
		uint32_t quadIndexCount = 0; // @note Keeping track of the base vertex buffer count
		quadVertex* quadVertexBufferBase = nullptr; 
		quadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // @note 0 - white texture
		std::array<glm::vec4, 4> quadVertexPositions;

		Renderer2D::Statistics stats;
	};

	static Renderer2DData _data; // @note Unique to this translation unit

	void Renderer2D::Init(){
		RENDER_PROFILE_FUNCTION();
		_data.quadVertexArray = VertexArray::Create();

		_data.quadVertexBuffer = VertexBuffer::Create(_data.maxVertices * sizeof(quadVertex));
		
		_data.quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
		});

		_data.quadVertexArray->AddVertexBuffer(_data.quadVertexBuffer);
		
		_data.quadVertexBufferBase = new quadVertex[_data.maxVertices * sizeof(quadVertex)];


		uint32_t* quadIndices = new uint32_t[_data.maxIndices];
		
		uint32_t offset = 0;
		for(uint32_t i = 0; i < _data.maxIndices; i += 6){
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4; 
		}


		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, _data.maxIndices);
		_data.quadVertexArray->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		_data.whiteTexture = Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff; // four f's for every channel
		_data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[_data.maxTextureSlots];
		for(uint32_t i = 0; i < _data.maxTextureSlots; i++)
			samplers[i] = i;

		// _data.textureShader = Shader::CreateShader("assets/shaders/texture.glsl");
		_data.textureShader = Shader::Create("/usr/local/public/GameEngine/assets/shaders/texture.glsl");
		_data.textureShader->Bind();
		_data.textureShader->Set("u_Textures", samplers, _data.maxTextureSlots);

		// Texture slot at 0 to white texture
		_data.textureSlots[0] = _data.whiteTexture;
		_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f};
		_data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f};
		_data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f};
		_data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f};
	}

	void Renderer2D::Shutdown(){
		RENDER_PROFILE_FUNCTION();
	}
	
	void Renderer2D::Begin(const Camera& camera, const glm::mat4& transform){
		RENDER_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);
		
		
		_data.textureShader->Bind();
		_data.textureShader->Set("u_ViewProjection", viewProj);
		
		_data.quadIndexCount = 0;
		_data.quadVertexBufferPtr = _data.quadVertexBufferBase; // Keeping track o our base memory allocations
		
		_data.textureSlotIndex = 1;
		
	}
	
	void Renderer2D::setClearColor(const glm::vec4& color){
		Renderer2D::resetStats();
		RendererCommand::setClearColor(color);
		RendererCommand::clear();
	}

	void Renderer2D::Begin(const OrthographicCamera& camera){
		RENDER_PROFILE_FUNCTION();



		// @note Setting the clear color beforehand

		// Simply uploads the camera data
		// upload is more API specific (actual OpenGL to set that uniform)
		// Where set is just set is a much higher level concept.
		_data.textureShader->Bind();
		_data.textureShader->Set("u_ViewProjection", camera.getViewProjectionMatrix());
		
		_data.quadIndexCount = 0;
		_data.quadVertexBufferPtr = _data.quadVertexBufferBase; // Keeping track o our base memory allocations
		
		_data.textureSlotIndex = 1;
	}
	
	void Renderer2D::Begin(const EditorCamera& camera){
		RENDER_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.getViewProjection();
		
		_data.textureShader->Bind();
		_data.textureShader->Set("u_ViewProjection", viewProj);
		
		_data.quadIndexCount = 0;
		_data.quadVertexBufferPtr = _data.quadVertexBufferBase; // Keeping track o our base memory allocations
		
		_data.textureSlotIndex = 1;

	}

	void Renderer2D::End(){
		RENDER_PROFILE_FUNCTION();
		
		uint32_t dataSize = (uint8_t *)_data.quadVertexBufferPtr - (uint8_t *)_data.quadVertexBufferBase;
		_data.quadVertexBuffer->SetData(_data.quadVertexBufferBase, dataSize);

		flush();
	}

	void Renderer2D::flush(){
		RENDER_PROFILE_FUNCTION();

		// @note Binding all of our texture slots submitted with a renderer id (that is i).
		// @note Essentially uploading data to the GPU
		// @note Executing the actual rendering.
		for(uint32_t i = 0; i < _data.textureSlotIndex; i++)
			_data.textureSlots[i]->Bind(i);
		
		RendererCommand::drawIndexed(_data.quadVertexArray, _data.quadIndexCount);
		_data.stats.drawCalls++;
	}

	void Renderer2D::flushAndReset(){
		End();
		_data.quadIndexCount = 0;
		_data.quadVertexBufferPtr = _data.quadVertexBufferBase; // Keeping track o our base memory allocations
		
		_data.textureSlotIndex = 1;
		
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color){
		DrawQuad({pos.x, pos.y, 0.0f}, size, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color){
		RENDER_PROFILE_FUNCTION();
		
		// To prevent overflow.
		if(_data.quadIndexCount >= Renderer2DData::maxIndices){
			flushAndReset();
		}
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
							  * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor){
		DrawQuad({pos.x, pos.y, 0.0f}, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor){
		RENDER_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
							  * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		
		DrawQuad(transform, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor){
		DrawQuad({pos.x, pos.y, 0.0f}, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor){
		RENDER_PROFILE_FUNCTION();
		// To prevent overflow.
		if(_data.quadIndexCount >= Renderer2DData::maxIndices){
			flushAndReset();
		}
		
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->getTexCoords();
		const Ref<Texture2D> texture = subTexture->getTexture();

		constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

		float textureIndex = 0.0f;
		// Searching for texture index in our array.
		for(uint32_t i = 1; i < _data.textureSlotIndex; i++){
			if(*_data.textureSlots[i].get() == *texture.get()){
				textureIndex = (float)i;
				break;
			}
		}

		if(textureIndex == 0.0f){
			textureIndex = (float)_data.textureSlotIndex; // assign to next available texture slot. Should be 1
			_data.textureSlots[_data.textureSlotIndex] = texture;
			_data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
							  * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
	
		for(size_t i = 0; i < quadVertexCount; i++){
			_data.quadVertexBufferPtr->pos = transform * _data.quadVertexPositions[i];
			_data.quadVertexBufferPtr->color = color;
			_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			_data.quadVertexBufferPtr->texIndex = textureIndex;
			_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			_data.quadVertexBufferPtr++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;
	}
	

	// NEW DRAW QUAD
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID){
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
		
		const float textureIndex = 0.0f; // Being our white texture
		const float tilingFactor = 1.0f;

		for(size_t i = 0; i < quadVertexCount; i++){	
			_data.quadVertexBufferPtr->pos = transform * _data.quadVertexPositions[i];
			_data.quadVertexBufferPtr->color = color;
			_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			_data.quadVertexBufferPtr->texIndex = textureIndex;
			_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			_data.quadVertexBufferPtr->entityID = entityID;
			_data.quadVertexBufferPtr++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;

	}

	// NEW DRAW QUAD
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID){

		// To prevent overflow.
		if(_data.quadIndexCount >= Renderer2DData::maxIndices){
			flushAndReset();
		}
		
		constexpr size_t quadVertexCount = 4;
		/* const glm::vec2* textureCoords = subTexture->getTexCoords(); */
		/* const Ref<Texture2D> texture = subTexture->getTexture(); */
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

		float textureIndex = 0.0f;
		// Searching for texture index in our array.
		for(uint32_t i = 1; i < _data.textureSlotIndex; i++){
			if(*_data.textureSlots[i].get() == *texture.get()){
				textureIndex = (float)i;
				break;
			}
		}

		if(textureIndex == 0.0f){
			textureIndex = (float)_data.textureSlotIndex; // assign to next available texture slot. Should be 1
			_data.textureSlots[_data.textureSlotIndex] = texture;
			_data.textureSlotIndex++;
		}

		for(size_t i = 0; i < quadVertexCount; i++){
			_data.quadVertexBufferPtr->pos = transform * _data.quadVertexPositions[i];
			_data.quadVertexBufferPtr->color = color;
			_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			_data.quadVertexBufferPtr->texIndex = textureIndex;
			_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			_data.quadVertexBufferPtr->entityID = entityID;
			_data.quadVertexBufferPtr++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;
	}
	
	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color){
		DrawRotatedQuad({pos.x, pos.y, 0.0f}, size, rotation, color);
	}
	
	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color){
		RENDER_PROFILE_FUNCTION();
		// To prevent overflow.
		if(_data.quadIndexCount >= Renderer2DData::maxIndices){
			flushAndReset();
		}

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		const float textureIndex = 0.0f; // Being our white texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
							  * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							  * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		
		for(size_t i = 0; i < quadVertexCount; i++){
			_data.quadVertexBufferPtr->pos = transform * _data.quadVertexPositions[i];
			_data.quadVertexBufferPtr->color = color;
			_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			_data.quadVertexBufferPtr->texIndex = textureIndex;
			_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			_data.quadVertexBufferPtr++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor){
		DrawRotatedQuad({pos.x, pos.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor){
		RENDER_PROFILE_FUNCTION();
		// To prevent overflow.
		if(_data.quadIndexCount >= Renderer2DData::maxIndices){
			flushAndReset();
		}
		
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

		float textureIndex = 0.0f;
		// Searching for texture index in our array.
		for(uint32_t i = 1; i < _data.textureSlotIndex; i++){
			if(*_data.textureSlots[i].get() == *texture.get()){
				textureIndex = (float)i;
				break;
			}
		}

		if(textureIndex == 0.0f){
			textureIndex = (float)_data.textureSlotIndex; // assign to next available texture slot. Should be 1
			_data.textureSlots[_data.textureSlotIndex] = texture;
			_data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
							  * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							  * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
	
		for(size_t i = 0; i < quadVertexCount; i++){
			_data.quadVertexBufferPtr->pos = transform * _data.quadVertexPositions[i];
			_data.quadVertexBufferPtr->color = color;
			_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			_data.quadVertexBufferPtr->texIndex = textureIndex;
			_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			_data.quadVertexBufferPtr++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;
	}
	
	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor){
		DrawRotatedQuad({pos.x, pos.y, 0.0f}, size, rotation, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor){
		RENDER_PROFILE_FUNCTION();
		// To prevent overflow.
		if(_data.quadIndexCount >= Renderer2DData::maxIndices){
			flushAndReset();
		}
		
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->getTexCoords();
		const Ref<Texture2D> texture = subTexture->getTexture();

		constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

		float textureIndex = 0.0f;
		// Searching for texture index in our array.
		for(uint32_t i = 1; i < _data.textureSlotIndex; i++){
			if(*_data.textureSlots[i].get() == *texture.get()){
				textureIndex = (float)i;
				break;
			}
		}

		if(textureIndex == 0.0f){
			textureIndex = (float)_data.textureSlotIndex; // assign to next available texture slot. Should be 1
			_data.textureSlots[_data.textureSlotIndex] = texture;
			_data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
							  * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							  * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
	
		for(size_t i = 0; i < quadVertexCount; i++){
			_data.quadVertexBufferPtr->pos = transform * _data.quadVertexPositions[i];
			_data.quadVertexBufferPtr->color = color;
			_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			_data.quadVertexBufferPtr->texIndex = textureIndex;
			_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			_data.quadVertexBufferPtr++;
		}

		_data.quadIndexCount += 6;

		_data.stats.quadCount++;
	}

	void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID){
		/* auto t = src.color; */
		// @note If texture submitted for SpriteRendererComponent then we render with that texture
		if(src.texture)
			DrawQuad(transform, src.texture, src.tilingFactor, src.color, entityID);
		else
			DrawQuad(transform, src.color, entityID);
	}

	Renderer2D::Statistics Renderer2D::getStats(){ return _data.stats; }

	void Renderer2D::resetStats(){
		memset(&_data.stats, 0, sizeof(Statistics));
	}
};

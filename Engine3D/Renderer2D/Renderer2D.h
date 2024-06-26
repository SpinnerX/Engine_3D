#pragma once
#include <Engine3D/Graphics/Camera.h> // TODO: May need to remove this..
#include <Engine3D/Renderer2D/EditorCamera.h>
#include <Engine3D/Renderer2D/OrthographicCamera.h>
#include <Engine3D/Graphics/Texture.h>
#include <Engine3D/Graphics/SubTexture2D.h>

namespace Engine3D{

	class SpriteRendererComponent;
	class Renderer2D{
	public:

		static void Init();
		static void Shutdown();

		static void setClearColor(const glm::vec4& color = glm::vec4( 0.1f, 0.1f, 0.1f, 1 ));

		//! @note For each frame we indicate Begin() and End() for our scene
		//! @note When we reach the end of our scene, we flush our command buffer
		//! @note Flushing means re-initiating our batches after our render pass operations
		static void Begin(const Camera& camera, const glm::mat4& transform); // TODO: Remove
		static void Begin(const EditorCamera& camera);
		static void Begin(const OrthographicCamera& camera);

		static void End();

		static void Flush();

		/*!
		 * DrawQuad
		 * @param params
		 * @param pos = position of the objects
		 * @param size = scaling the size of objects
		 * @param color = colorizing specific rgba channels to a specific color
		 * @param rotation = specifying in glm::radians(angle) how much to rotate
		 * @param tilingFactor = is the offset for how many tiles can be rendered via GPU
		 * @param tintColor = sets the rgba channels value for specifying the color to tint that object
		 * @note TODO ---- Instead of manually typing in this data, we should instead have our scene context.
		 * @note Our render passes should be what handles how each frame things should be getting rendered, implicitly
		 * @note TODO ---- Submission/Task flow for how we currently submit tasks to our renderer
		 * @note Users should be able to specify how things get drawn.
		 * 
		 * @param Idea
		 * @note Reason why we want to specify our API to work like this is because we want to submit a task for our current frame
		 * @note By passing a lambda we have more control of the tasks we are submitting to our renderer
		 @example
		 Renderer2D::Submit([lines = m_Lines, renderCmdBuffer=m_RenderCmdBuffer](){
			uint32_t frame = Renderer::CurrentFrameIndex();
			// SetLineWidth(buffer, lineWidth)
			uint32_t currentFrameIndex = Renderer::GetCurrentFrameIndex();
			auto cmdBuffer = renderCmdBuffer.As<CmdBufferType>()->GetCommandBuffer(currentFrameIndex);
			SetLineWidth(cmdBuffer, lineWidth);
		 });
		*/
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor=glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor=glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		//! @note If texture submitted for SpriteRendererComponent then we render with that texture
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);


		//! @note Drawing Circle
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);

		// Allowing to control with our constant values
		// Such as allowing to modify in real time with these constant values.
		// How to know measuring, is through draw calls
		// They are resettable by client, when client resets the stats
		// This is up to the client for resetting
		struct Statistics{
			uint32_t drawCalls = 0; // number of draw calls
			uint32_t quadCount = 0;

			uint32_t getTotalVertexCount() { return quadCount * 4; }
			uint32_t getTotalIndexCount() { return quadCount * 6; }
		};

		static Statistics getStats();
		static void resetStats();

	private:
		static void InitiateBatch();
	};

};

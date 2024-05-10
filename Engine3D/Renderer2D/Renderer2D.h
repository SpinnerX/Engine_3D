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

		static void Begin(const Camera& camera, const glm::mat4& transform); // TODO: Remove
		static void Begin(const EditorCamera& camera);
		static void Begin(const OrthographicCamera& camera);

		static void End();

		static void flush();

		/*
		 * Small NOTE: These are DrawQuads for colors and textures.
		 * DrawQuad
		 * @params
		 * @pos = position of the objects
		 * @size = scaling the size of objects
		 * @color = colorizing specific rgba channels to a specific color
		 * @rotation = specifying in glm::radians(angle) how much to rotate
		 * @tilingFactor = is the offset for how many tiles can be rendered via GPU
		 * @tintColor = sets the rgba channels value for specifying the color to tint that object
		 *
		 *
		 * QUICK NOTE: Rotations is in radians.
		 *
		*/
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor=glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor=glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);


		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

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
		static void flushAndReset();
	};

};

#pragma once
#include <Engine3D/Graphics/Texture.h>
#include <glm/glm.hpp>

namespace Engine3D{

	// Not like a renderer primitive
	// Hence not having a create function
	// For turning things like a spritesheet into individual sprites.
	/*SubTexture2D
	 * @note is to allow to create individual sprites via spritesheets.
	 * @note where you can specify the dimensions, and location of the sprite, and it'll caluclate the textures w and h giving you that individual sprite
	 *
	 *
	 *
	 * */
	class SubTexture2D{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		
		const Ref<Texture2D> getTexture() const { return _texture; }
		const glm::vec2* getTexCoords() const { return _texCoords; }
		
		/*
		 * Function: CreateFromCoords
		 * @param texture
		 * @note the actual texture (could be a spritesheet) we are loading
		 * 
		 * @param coords
		 * @note location of where the individual sprite is in the spritesheet
		 *
		 * @param spriteSize
		 * @note actual size of the sprite itself individually
		 *
		 * @param spriteSizeOffset 
		 * @note this is to be taken in a +1, and increment that sprite size to give us the actual offset for the sprite size
		 * @example ((coords.x + 1) * spriteSize.x) / texture->getWidth(). Where 1 is now the value of the "x" and "y" of the spriteSizeOffset parameter.
		*/
		static Ref<SubTexture2D> createFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& spriteSizeOffset = {1, 1});
	private:
		Ref<Texture2D> _texture;
		glm::vec2 _texCoords[4];
	};
};

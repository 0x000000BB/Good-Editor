#pragma once
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Shader.h"

struct Character {
	unsigned int TextureID; 
	glm::ivec2   Size;      
	glm::ivec2   Bearing;   
	unsigned int Advance;   
};

class Renderer {
public:
	Renderer() {};

	void renderCursor(int x, int y, glm::vec2 cursorPosition, float scale, glm::vec3 color);
	void renderLine(std::string line, float x, float y, float scale, glm::vec3 color);
	void renderQuad(glm::vec2 position, glm::vec2 scale, glm::vec3 color);
	int init(float widht, float height);

private:
	Shader textShader;
	Shader cursorShader;

	unsigned int cursorVAO, cursorVBO;
	unsigned int textVAO, textVBO;

	std::map<char, Character> Characters;
};


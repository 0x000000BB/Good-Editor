#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Editor {
public:
	Editor() {
		lines.push_back("");
		cursorPosition = glm::vec3(0);
	}

	void insertChar(char c);
	void insertChar(char c, int count);
	void insertCharClosing(char c1, char c2);
	void removeChar();
	void insertLine(std::string line);

	std::vector<std::string> lines = std::vector<std::string>();
	glm::vec3 cursorPosition;
};
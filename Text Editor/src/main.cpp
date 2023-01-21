#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "Window.h"
#include "Editor.h"
#include "Renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define scale 1
#define WINDOW_WIDHT 1600
#define WINDOW_HEIGHT 900
#define LINE_HEIGHT 48

#define DARK_MODE 1

#if DARK_MODE
glm::vec3 TEXT_COLOR = glm::vec3(0, 0, 0);
glm::vec3 NUMBER_COLOR = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 BACKGROUND_COLOR = glm::vec3(0.14f, 0.14f, 0.14f);
glm::vec3 CURSOR_COLOR = glm::vec3(1.0f, 0.5f, 0.2f);
#else
glm::vec3 TEXT_COLOR = glm::vec3(1, 1, 1);
glm::vec3 NUMBER_COLOR = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 BACKGROUND_COLOR = glm::vec3(0.86f, 0.86f, 0.86f);
glm::vec3 CURSOR_COLOR = glm::vec3(0.7f, 0.7f, 0.7f);
#endif

Editor editor = Editor();
Renderer renderer = Renderer();

int getWidth(float x) {
	return WINDOW_WIDHT * x;
}

int getHeight(float y) {
	return WINDOW_HEIGHT * y;
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if (editor.cursorPosition.y > editor.lines.size()) {
		return;
	}
	std::string line = editor.lines[editor.cursorPosition.y];

	char c = (char)codepoint;
	char nextC = line[editor.cursorPosition.x];

	if (line.empty()) {
		editor.insertChar(c);
	}
	else {
		if (c == '{') {
			editor.insertCharClosing('{', '}');
		}
		else if (c == '(') {
			editor.insertCharClosing('(', ')');
		}
		else if (c == '[') {
			editor.insertCharClosing('[', ']');
		}
		else if (c == '}' && nextC == '}') {
			editor.cursorPosition.x += 1;
		}
		else if (c == ']' && nextC == ']') {
			editor.cursorPosition.x += 1;
		}
		else if (c == ')' && nextC == ')') {
			editor.cursorPosition.x += 1;
		}
		else {
			editor.insertChar(c);
		}
	}
}

bool pressed = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
		pressed = true;
	}

	if (key == GLFW_KEY_BACKSPACE && action == GLFW_RELEASE) {
		pressed = false;
	}

	if (pressed && editor.cursorPosition.x != 0) {
		editor.removeChar();
	}
	else if (pressed) {
		if (editor.cursorPosition.x == 0 && editor.cursorPosition.y > 0) {
			std::string line = editor.lines[editor.cursorPosition.y];
			editor.lines.erase(editor.lines.begin() + editor.cursorPosition.y);
			editor.cursorPosition.y -= 1;
			editor.cursorPosition.x = editor.lines[editor.cursorPosition.y].length();
			editor.lines[editor.cursorPosition.y] += line;
		}
	}


	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		std::string line = editor.lines[editor.cursorPosition.y];
		std::string sub = "";
		if (!line.empty()) {
			sub = line.substr(editor.cursorPosition.x, line.length() - editor.cursorPosition.x);
		}
		if (editor.cursorPosition.x > 0) {
			if (line[editor.cursorPosition.x - 1] == '{' && line[editor.cursorPosition.x] == '}') {
				editor.lines[editor.cursorPosition.y] = line.substr(0, editor.cursorPosition.x);
				editor.insertLine("   ");
				editor.insertLine(sub);
				editor.cursorPosition.y -= 1;
				editor.cursorPosition.x = 3;
			}
			else {
				editor.lines[editor.cursorPosition.y] = line.substr(0, editor.cursorPosition.x);
				editor.insertLine(line.substr(editor.cursorPosition.x, line.length() - editor.cursorPosition.x));
			}
		}
		else {
			editor.lines[editor.cursorPosition.y] = line.substr(0, editor.cursorPosition.x);
			editor.insertLine(line.substr(editor.cursorPosition.x, line.length() - editor.cursorPosition.x));
		}
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		editor.insertChar(' ', 3);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		if (editor.cursorPosition.x != 0) {
			editor.cursorPosition.x -= 1;
		}
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		if (editor.cursorPosition.x < editor.lines[editor.cursorPosition.y].length()) {
			editor.cursorPosition.x += 1;
		}
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (editor.cursorPosition.y != 0) {
			editor.cursorPosition.y -= 1;
			if (editor.cursorPosition.x > editor.lines[editor.cursorPosition.y].length()) {
				editor.cursorPosition.x = editor.lines[editor.cursorPosition.y].length();
			}
		}
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (editor.cursorPosition.y < editor.lines.size() - 1) {
			editor.cursorPosition.y += 1;
			if (editor.cursorPosition.x > editor.lines[editor.cursorPosition.y].length()) {
				editor.cursorPosition.x = editor.lines[editor.cursorPosition.y].length();
			}
		}
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
		std::cout << "saving" << std::endl;
		std::ofstream outFile;
		outFile.open("example.txt");
		for (std::string line : editor.lines) {
			outFile << line << "\n";
		}
		outFile.close();
	}
}

int main() {
	
	Window window = Window(WINDOW_WIDHT, WINDOW_HEIGHT, "Good Editor");
	if (renderer.init(WINDOW_WIDHT, WINDOW_HEIGHT) != 0) {
		std::cerr << "Failed to init Renderer" << std::endl;
	}

	glfwSetCharCallback(window.handel, character_callback);
	glfwSetKeyCallback(window.handel, key_callback);
	
	while (!window.shouldClose()) {
		window.pollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 1.0f);

		for (int i = 0; i < editor.lines.size(); i++) {
			renderer.renderLine(editor.lines[i], getWidth(0.06), getHeight(0.9) - LINE_HEIGHT * i * scale, scale, TEXT_COLOR);
		}
		for (int i = 0; i < WINDOW_HEIGHT / LINE_HEIGHT * scale; i++) {
			std::string number = std::to_string(i);
			renderer.renderLine(std::to_string(i), getWidth(0.03) - 26 * (number.length() - 1), getHeight(0.9) - LINE_HEIGHT * i * scale, scale, NUMBER_COLOR);
		}
		renderer.renderCursor(getWidth(0.06), getHeight(0.9), editor.cursorPosition, scale, CURSOR_COLOR);
		renderer.renderQuad(glm::vec2(getWidth(0.05), getHeight(0)), glm::vec2(6, WINDOW_HEIGHT), NUMBER_COLOR);

		window.swapBuffers();
	}

	window.terminate();

	return 0;
}
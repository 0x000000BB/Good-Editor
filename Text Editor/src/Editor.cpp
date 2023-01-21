#include "Editor.h"

void Editor::insertChar(char c)
{
	std::string& line = lines[cursorPosition.y];
	if (line.empty()) {
		line += c;
		cursorPosition.x += 1;
	}
	else {
		line.insert(cursorPosition.x, 1, c);
		cursorPosition.x += 1;
	}
}

void Editor::insertChar(char c, int count)
{
	lines[cursorPosition.y].insert(cursorPosition.x, count, c);
	cursorPosition.x += count;
}

void Editor::insertCharClosing(char c1, char c2)
{
	std::string& line = lines[cursorPosition.y];
	line.insert(cursorPosition.x, 1, c1);
	cursorPosition.x += 1;
	line.insert(cursorPosition.x, 1, c2);
}

void Editor::removeChar()
{
	cursorPosition.x -= 1;
	lines[cursorPosition.y].erase(cursorPosition.x, 1);
}

void Editor::insertLine(std::string line)
{
	lines.insert(lines.begin() + cursorPosition.y + 1, line);

	cursorPosition.x = 0;
	cursorPosition.y += 1;
}

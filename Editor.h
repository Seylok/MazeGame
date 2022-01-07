#pragma once

class Editor {
public:
	Editor();
	~Editor();

	void EditorMain();

private:
	char* LoadLevel(int& width, int& height, int& keyAmount, std::string& levelName);
	void SaveLevel(char* pLevel, int width, int height);
	bool EditLevel(char* pLevel, int& cursorX, int& cursorY, int width, int height);
	void GetLevelDimensions(int& width, int& height);
	void DisplayLevel(char* pLevel, int width, int height, int cursorX, int cursorY, std::string levelName);
	int GetIndexFromXY(int x, int y, int width);
	void DisplayTopBorder(int width);
	void DisplayBottomBorder(int width);
	void DisplayLeftBorder();
	void DisplayRightBorder();
};
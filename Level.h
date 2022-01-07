#pragma once
#include <vector>

class Actor;

class Level {
	char* m_pLevelData; // 1-dimensional level layout character array
	int m_width; // width of the level (x)
	int m_height; // height of the level (y)
	int m_finalLevel; // amount of existing levels 
	int m_currentLevel; // level number the player currently should be on
	int m_activeLevel; // currently active loaded level (layout)
	std::string m_levelName; // name of the loaded level -- std:: seems to be required for a string declaration here
	
    // extra vars to save the player start location so it can be called properly in "Game"
	int m_startLocationX;
	int m_startLocationY;

	// this array will store all available level file names
	std::vector<std::string> m_levelCollection;
	std::vector<Actor*> m_pActors;

public:
	Level();
	~Level();

	void FindLevels();
	void LoadLevel();
	void DrawMap();

	// check to quit out to the main menu during gameplay
	bool m_quitToMenu;

	// playfield objects:
	bool IsSpace(int x, int y);
	bool IsEasteregg(int x, int y);
	bool IsWall(int x, int y);

	Actor* UpdateActors(int x, int y);

	// level data modification:
	void ClearTile(int x, int y);

	int GetHeight() { return m_height; };
	int GetWidth() { return m_width; };

	int GetStartX() { return m_startLocationX; };
	int GetStartY() { return m_startLocationY; };

	int GetFinalLevel() { return m_finalLevel; };
	int GetCurrentLevel() { return m_currentLevel; };
	int GetActiveLevel() { return m_activeLevel; };
	std::string GetLevelName() { return m_levelName; };

	void IncreaseLevel();
	void IncreaseActiveLevel();
	void ResetLevelStates();
	void ResetCurrentLevel();

private:
	void ConvertTiles();
	void WallConvert();
	void RandomizeKeyLocation(int keyAmount);
	int GetIndexFromCoordinates(int x, int y);

	bool wallsDrawn = false;
};
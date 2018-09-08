#pragma once
#include "Renderer.h"
#include "Timer.h"
#include "Tile.h"
#include <ctime>
#include "TextureManager.h"

class M3Board {

public:
	M3Board();
	~M3Board();
	void init(int m); // init board, with number of moves and total time.
	void update(float displacement); // Make tile pieces move into place, check for any matches, spawn new tiles.
	void draw(TextureManager& tex, Renderer& ren); // Draw the board.
	void matchSearch(); // Determine if any are matched; if so, compute the result.
	void getMouseLoc(float mx, float my); // Translate mouse float coords to board index, keep track of selections, perform switch, call switchSearch.
	void createBoardMesh(Renderer& ren);
	void createTileMesh(Renderer& ren);
	
	float addToScore;
	bool moved;

private:
	void matchTiles();

	int board[289];
	int select1, select2; // The indices for the two pieces to be switched.
	bool allStill;
	bool scoreAdjustable;
	int width;
	int greatestMatchedTile;
	int numMatchedTiles;
	// Board and tile vertex buffers. Shared index buffer (they're all rectangles)
	ID3D11Buffer * mBVB;
	ID3D11Buffer * mSVB;
	ID3D11Buffer * mTVB;
	ID3D11Buffer * mIB;

	std::vector<Tile> tiles;
};
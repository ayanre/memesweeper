#pragma once
#include "Graphics.h"
#include "Vei2.h"
#include "RectI.h"

class Memefield {
private:
	class Tile {
	private:
		enum status
		{
			Hidden,
			Revealed,
			Flagged
		};
	public:
		void toggleFlagStatus();
		void SpawnMeme();
		bool HasMeme();
		void Reveal();
		void Draw(const Vei2& screenPos, Graphics& gfx, bool isFucked);
		void setNeighbours( int n );
		bool isFlagged();
	private:
		int neighbourMemes = 0;
		bool flagged = false;
		bool hasMeme = false;
		status status = status::Hidden;
	};
public:
	Memefield(int nMemes);
private:
	static constexpr int left = 260;
	static constexpr int top = 230;
	static constexpr int width = 14;
	static constexpr int height = 7;
	bool isFucked = false;
	int nMemes;
	Tile leTiles[width * height];
public:
	void Draw(Graphics& gfx);
	Tile& tileAt(Vei2& pos);
	void tileReveal(Vei2& pos);
	void toggleFlag(Vei2 screenPos);
	int calcNeighbouringMemes(Vei2& pos);
	Vei2 screenPosToPos(Vei2 screenPos);
	RectI getRect();
};
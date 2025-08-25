#pragma once
#include "Graphics.h"
#include "Vei2.h"

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
		void SpawnMeme();
		bool HasMeme();
		bool isRevealed();
		void Reveal();
		void Draw(const Vei2& screenPos, Graphics& gfx);
	private:
		bool revealed = false;
		bool hasMeme = false;
		status status = status::Hidden;
	};
public:
	Memefield(int nMemes);
	void settoreveal();
private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	int nMemes;
	Tile leTiles[width * height];
public:
	void Draw(Graphics& gfx);
	Tile& tileAt(Vei2& pos);
};
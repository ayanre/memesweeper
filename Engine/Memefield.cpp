#include "Memefield.h"
#include <assert.h>
#include <random>
#include "SpriteCodex.h"

Memefield::Memefield(int nMemes)
	:
	nMemes(nMemes)
{
	assert(nMemes > 0 && nMemes < (width * height));
	std::random_device rd;
	std::mt19937 rnd(rd());
	std::uniform_int_distribution<int> xDist(0, Memefield::width - 1);
	std::uniform_int_distribution<int> yDist(0, Memefield::height - 1);

	for (int x = 0; x < nMemes; x++) {
		Vei2 pos = { 0,0 };
		do {
			pos = { xDist(rnd), yDist(rnd) };
		} while (tileAt(pos).HasMeme());

		tileAt(pos).SpawnMeme();
	}
}

void Memefield::settoreveal()
{
	std::random_device rd;
	std::mt19937 rnd(rd());
	std::uniform_int_distribution<int> xDist(0, Memefield::width - 1);
	std::uniform_int_distribution<int> yDist(0, Memefield::height - 1);

	for (int x = 0; x < 50; x++) {
		Vei2 pos = { 0,0 };
		do {
			pos = { xDist(rnd), yDist(rnd) };
		} while (tileAt(pos).isRevealed());

		tileAt(pos).Reveal();
	}
}

void Memefield::Draw(Graphics& gfx) {
	gfx.DrawRect(0, 0, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize, SpriteCodex::baseColor);

	for (Vei2 pos = { 0,0 }; pos.y < height; pos.y++) {
		for (pos.x = 0; pos.x < width; pos.x++) {
			tileAt(pos).Draw({ pos.x * SpriteCodex::tileSize, pos.y * SpriteCodex::tileSize }, gfx);
		}
	}
}

Memefield::Tile& Memefield::tileAt(Vei2& pos)
{
	return leTiles[pos.y * width + pos.x];
}

void Memefield::Tile::SpawnMeme()
{
	hasMeme = true;
}

bool Memefield::Tile::HasMeme()
{
	return hasMeme;
}

bool Memefield::Tile::isRevealed()
{
	return revealed;
}

void Memefield::Tile::Reveal()
{
	status = status::Revealed;
}

void Memefield::Tile::Draw(const Vei2& screenPos, Graphics& gfx)
{
	switch (status) {
	case status::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case status::Flagged:
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case status::Revealed:
		if (hasMeme) {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else {
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		break;
	}
}

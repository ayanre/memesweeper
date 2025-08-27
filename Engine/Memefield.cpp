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

	for (Vei2 pos = { 0,0 }; pos.y < height; pos.y++) {
		for (pos.x = 0; pos.x < width; pos.x++) {
			tileAt(pos).setNeighbours(calcNeighbouringMemes(pos));
		}
	}
}

void Memefield::Draw(Graphics& gfx) {
	gfx.DrawRect(left, top, left + width * SpriteCodex::tileSize, top + height * SpriteCodex::tileSize, SpriteCodex::baseColor);

	for (Vei2 pos = { 0,0 }; pos.y < height; pos.y++) {
		for (pos.x = 0; pos.x < width; pos.x++) {
			tileAt(pos).Draw({ left + pos.x * SpriteCodex::tileSize, top + pos.y * SpriteCodex::tileSize }, gfx, isFucked);
		}
	}
}

Memefield::Tile& Memefield::tileAt(Vei2& pos)
{
	return leTiles[pos.y * width + pos.x];
}

void Memefield::tileReveal(Vei2& screenPos)
{
	if (!isFucked) {
		Vei2 pos = screenPosToPos(screenPos);
		tileAt(pos).Reveal();
		if (tileAt(pos).HasMeme() && !tileAt(pos).isFlagged()) {
			isFucked = true;
		}
	}
}

void Memefield::toggleFlag(Vei2 screenPos)
{
	if (!isFucked) {
		Vei2 pos = screenPosToPos(screenPos);
		tileAt(pos).toggleFlagStatus();
	}
}

int Memefield::calcNeighbouringMemes(Vei2& pos)
{
	int startX = std::max(0, pos.x - 1);
	int startY = std::max(0, pos.y - 1);
	int endX = std::min(width - 1, pos.x + 1);
	int endY = std::min(height - 1, pos.y + 1);
	int count = 0;

	for (Vei2 inpos = { startX, startY }; inpos.y <= endY; inpos.y++) {
		for (inpos.x = startX; inpos.x <= endX; inpos.x++) {
			if (inpos != pos && tileAt(inpos).HasMeme()) {
				count++;
			}
		}
	}
	return count;
}

Vei2 Memefield::screenPosToPos(Vei2 screenPos)
{
	if (getRect().isInsideOf(screenPos)) {
		Vei2 newscreenPos = { screenPos.x - left, screenPos.y - top };
		return (newscreenPos / (SpriteCodex::tileSize));
	}
}

RectI Memefield::getRect()
{
	return RectI(left, left + width * SpriteCodex::tileSize, top, top + height * SpriteCodex::tileSize);
}

void Memefield::Tile::toggleFlagStatus()
{

	if (flagged) {
		status = status::Hidden;
	}
	else {
		status = status::Flagged;
	}
	flagged = !flagged;

}

void Memefield::Tile::SpawnMeme()
{
	hasMeme = true;
}

bool Memefield::Tile::HasMeme()
{
	return hasMeme;
}

void Memefield::Tile::Reveal()
{
	if (status == status::Hidden && !flagged) {
		status = status::Revealed;
	}
}

void Memefield::Tile::Draw(const Vei2& screenPos, Graphics& gfx, bool isFucked)
{
	switch (status) {
	case status::Hidden:
		if (isFucked) {
			if (HasMeme()) {
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			else {
				SpriteCodex::DrawTileButton(screenPos, gfx);
			}
		}
		else {
			SpriteCodex::DrawTileButton(screenPos, gfx);
		}
		break;
	case status::Flagged:
		if (isFucked) {
			if (HasMeme()) {
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			else {
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
		}
		else {
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
		}
		break;
	case status::Revealed:
		if (hasMeme) {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else {
			switch (neighbourMemes) {
			case 0:
				SpriteCodex::DrawTile0(screenPos, gfx);
				break;
			case 1:
				SpriteCodex::DrawTile1(screenPos, gfx);
				break;
			case 2:
				SpriteCodex::DrawTile2(screenPos, gfx);
				break;
			case 3:
				SpriteCodex::DrawTile3(screenPos, gfx);
				break;
			case 4:
				SpriteCodex::DrawTile4(screenPos, gfx);
				break;
			case 5:
				SpriteCodex::DrawTile5(screenPos, gfx);
				break;
			case 6:
				SpriteCodex::DrawTile6(screenPos, gfx);
				break;
			case 7:
				SpriteCodex::DrawTile7(screenPos, gfx);
				break;
			case 8:
				SpriteCodex::DrawTile8(screenPos, gfx);
				break;
			}
		}
		break;
	}
}

void Memefield::Tile::setNeighbours( int n )
{
	neighbourMemes = n;
}

bool Memefield::Tile::isFlagged()
{
	return flagged;
}

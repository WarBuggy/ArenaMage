#include "ScreenPlayParent.h"

ScreenPlayParent::ScreenPlayParent(sf::RenderWindow & window) : Screen(window)
{
	scale = (float)Object::SCALE;
	windowWidth = (float)Window.getSize().x;
	windowHeight = (float)Window.getSize().y;
	arenaWidth = localClient.arena.PlayableWidth*scale;
	arenaHeight = localClient.arena.PlayableHeight*scale;
	unplayableHeight = windowHeight - arenaHeight;
	unplayableWidth = windowWidth - arenaWidth;

	arenaVertices.clear();
	arenaVertices = sf::VertexArray(sf::Quads, (localClient.arena.ArenaObjects.size() + 4) * 4);

	SetupBoundsVertices();
	SetupArenaObjectVertices();
}

void ScreenPlayParent::DrawScreen(sf::RenderWindow & window)
{
	localClient.Draw(window, SpriteSheet, unplayableWidth / 2, unplayableHeight / 2);
	Window.draw(arenaVertices, &SpriteSheet);
}

void ScreenPlayParent::SetupBoundsVertices()
{
	// ceiling
	sf::Vertex* ceiling = &arenaVertices[0];
	ceiling[0].position = sf::Vector2f(0, 0);
	ceiling[1].position = sf::Vector2f(windowWidth, 0);
	ceiling[2].position = sf::Vector2f(windowWidth, unplayableHeight / 2);
	ceiling[3].position = sf::Vector2f(0, unplayableHeight / 2);

	ceiling[0].texCoords = sf::Vector2f(0, 11);
	ceiling[1].texCoords = sf::Vector2f(4, 11);
	ceiling[2].texCoords = sf::Vector2f(4, 15);
	ceiling[3].texCoords = sf::Vector2f(0, 15);
	// ground
	sf::Vertex* ground = &arenaVertices[4];
	ground[0].position = sf::Vector2f(0, arenaHeight + unplayableHeight / 2);
	ground[1].position = sf::Vector2f(windowWidth, arenaHeight + unplayableHeight / 2);
	ground[2].position = sf::Vector2f(windowWidth, windowHeight);
	ground[3].position = sf::Vector2f(0, windowHeight);

	ground[0].texCoords = sf::Vector2f(5, 11);
	ground[1].texCoords = sf::Vector2f(9, 11);
	ground[2].texCoords = sf::Vector2f(9, 15);
	ground[3].texCoords = sf::Vector2f(5, 15);
	// left wall
	sf::Vertex* left = &arenaVertices[8];
	left[0].position = sf::Vector2f(0, unplayableHeight / 2);
	left[1].position = sf::Vector2f(unplayableWidth / 2, unplayableHeight / 2);
	left[2].position = sf::Vector2f(unplayableWidth / 2, arenaHeight + unplayableHeight / 2);
	left[3].position = sf::Vector2f(0, arenaHeight + unplayableHeight / 2);

	left[0].texCoords = sf::Vector2f(10, 11);
	left[1].texCoords = sf::Vector2f(14, 11);
	left[2].texCoords = sf::Vector2f(14, 15);
	left[3].texCoords = sf::Vector2f(10, 15);
	// right wall
	sf::Vertex* right = &arenaVertices[12];
	right[0].position = sf::Vector2f(arenaWidth + unplayableWidth / 2, unplayableHeight / 2);
	right[1].position = sf::Vector2f(windowWidth, unplayableHeight / 2);
	right[2].position = sf::Vector2f(windowWidth, arenaHeight + unplayableHeight / 2);
	right[3].position = sf::Vector2f(arenaWidth + unplayableWidth / 2, arenaHeight + unplayableHeight / 2);

	right[0].texCoords = sf::Vector2f(15, 11);
	right[1].texCoords = sf::Vector2f(19, 11);
	right[2].texCoords = sf::Vector2f(19, 15);
	right[3].texCoords = sf::Vector2f(15, 15);
}

void ScreenPlayParent::SetupArenaObjectVertices()
{
	float scale = (float)Object::SCALE;
	sf::Vector2f offset(unplayableWidth / 2, unplayableHeight / 2);

	size_t index = 4;
	for (std::map<Point2D, ObjectType>::iterator it = localClient.arena.ArenaObjects.begin(); it != localClient.arena.ArenaObjects.end(); ++it)
	{

		sf::Vertex* quad = &arenaVertices[index * 4];
		sf::Vector2f finalPos(it->first.X*scale + offset.x, it->first.Y*scale + offset.y);
		float width, height;
		if (it->second == ObjectType::ARENA_PLATFORM)
		{
			ArenaPlatform temp(it->first);
			width = (float)temp.width*scale;
			height = (float)temp.height;
			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(1, 0);
			quad[1].texCoords = sf::Vector2f(5, 0);
			quad[2].texCoords = sf::Vector2f(5, 2);
			quad[3].texCoords = sf::Vector2f(1, 2);
		}
		else if (it->second == ObjectType::ARENA_BLOCK)
		{
			ArenaBlock temp(it->first);
			width = (float)temp.width*scale;
			height = (float)temp.height;

			quad[0].texCoords = sf::Vector2f(6, 0);
			quad[1].texCoords = sf::Vector2f(10, 0);
			quad[2].texCoords = sf::Vector2f(10, 2);
			quad[3].texCoords = sf::Vector2f(6, 2);
		}
		// define its 4 corners
		quad[0].position = sf::Vector2f(finalPos.x, finalPos.y);
		quad[1].position = sf::Vector2f(finalPos.x + width, finalPos.y);
		quad[2].position = sf::Vector2f(finalPos.x + width, finalPos.y + height);
		quad[3].position = sf::Vector2f(finalPos.x, finalPos.y + height);
		index++;
	}
}

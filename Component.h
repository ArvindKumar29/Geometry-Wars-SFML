#pragma once
#include "Vec2.h"
#include <sfml/Graphics.hpp>

class Component
{
public:
	struct CTransform {
		Vec2 pos;
		Vec2 vel;

		CTransform(Vec2 pos, Vec2 vel) {
			this->pos = pos;
			this->vel = vel;
		}
	};

	struct CShape {
		sf::CircleShape shape;
		int vertices;
		float radius;
		sf::Color SColor;
		sf::Color OColor;
		Vec2 pos;
		float outlineThickness;

		CShape(int vertices, float radius, sf::Color SColor, sf::Color OColor, Vec2 pos, float outlineThickness) {
			shape.setPointCount(vertices);
			shape.setRadius(radius);
			shape.setFillColor(SColor);
			shape.setOutlineColor(OColor);
			shape.setPosition(pos.x, pos.y);
			shape.setOutlineThickness(outlineThickness);
			shape.setOrigin(radius, radius);
		};
	};

	struct CCollision {
		float radius;
	};

	struct CLifespan {
		int total;
		int remaining;

		CLifespan(int total) {
			this->total = total;
			this->remaining = total;
		}

	};

	struct CInput {
		bool up = false;
		bool down = false;
		bool right = false;
		bool left = false;

		CInput() {
			this->up = false;
			this->down = false;
			this->right = false;
			this->left = false;
		}
	};

	struct CHealth {
		int health;
	};
};


#pragma once


class Grid {
public:


	int8_t Scale = 10; // 10 pixels per movement
};



class Snake {
public:
	Snake() {
		x = 0;
		y = 0;
		xSpeed = 1;
		ySpeed = 0;
		speed = 0.25;
	}

	struct SnakeBody {
		float x, y;
	} body;

	void update() {
		x = x + xSpeed * speed;
		y = y + ySpeed * speed;
	}

	void dir(int8_t _x, int8_t _y) {
		xSpeed = _x;
		ySpeed = _y;
	}

	virtual void draw() {}



	float x, y;
	int8_t xSpeed, ySpeed;
	float speed;
};


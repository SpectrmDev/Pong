#include <stdio.h>
#include <memory.h>
#include <stdint.h>
#include <conio.h>
#include <math.h>
#include <Windows.h>

#define CANVAS_WIDTH	101
#define CANVAS_HEIGHT	29

enum BallDirection : uint8_t
{
	UP_LEFT = 1,
	DOWN_LEFT = 2,
	UP_RIGHT = 3,
	DOWN_RIGHT = 4,
};

char* canvas, *pcanvas;
uint8_t playerPos = 7, opponentPos = 7;
int8_t ballX = 0, ballY = 0;
BallDirection currentDir = BallDirection::UP_RIGHT;

void MovePlayer(char key)
{
	switch (key)
	{
	case 'w':
		if (playerPos - 4 <= 0) return;
		--playerPos;
		break;

	case 's':
		if (playerPos + 4 >= CANVAS_HEIGHT - 1) return;
		++playerPos;
		break;
	}
}

void DrawBall()
{
	switch (currentDir)
	{
	case BallDirection::UP_LEFT:
		ballY++, ballX--;
		break;

	case BallDirection::DOWN_LEFT:
		ballY--, ballX--;
		break;

	case BallDirection::UP_RIGHT:
		ballY++, ballX++;
		break;

	case BallDirection::DOWN_RIGHT:
		ballY--, ballX++;
		break;
	}

	uint8_t dx = round(CANVAS_WIDTH / 2) + ballX;
	uint8_t dy = round(CANVAS_HEIGHT / 2) - ballY;

	// Ball collision stuff
	if (dy <= 1)
	{
		if (currentDir == UP_LEFT) currentDir = DOWN_LEFT;
		else if (currentDir == UP_RIGHT) currentDir = DOWN_RIGHT;
	} else if (dy >= CANVAS_HEIGHT - 2)
	{
		if (currentDir == DOWN_LEFT) currentDir = UP_LEFT;
		else if (currentDir == DOWN_RIGHT) currentDir = UP_RIGHT;
	}

	canvas[dx + dy * CANVAS_WIDTH] = 'O';
}

void DrawPlatforms()
{
	// Player Platform
	for (uint8_t i = playerPos - 2; i <= playerPos + 2; i++)
		canvas[4 + i * CANVAS_WIDTH] = 'H';

	// Opponent Platform
	for (uint8_t i = opponentPos - 2; i <= opponentPos + 2; i++)
		canvas[(CANVAS_WIDTH - 5) + i * CANVAS_WIDTH] = 'H';
}

void ConstructCanvas()
{
	canvas = new char[CANVAS_WIDTH * CANVAS_HEIGHT];
	memset(canvas, ' ', CANVAS_WIDTH * CANVAS_HEIGHT);
	memset(canvas, '#', CANVAS_WIDTH);

	uint16_t i = CANVAS_WIDTH - 1;

	while (i < CANVAS_WIDTH * (CANVAS_HEIGHT - 2)) 
	{
		i++;
		canvas[i] = '#';
		i += (CANVAS_WIDTH - 1);
		canvas[i] = '#';
	}

	for (int j = i; j < CANVAS_WIDTH * CANVAS_HEIGHT; j++)
		canvas[j] = '#';

	pcanvas = new char[CANVAS_WIDTH * CANVAS_HEIGHT];
	memcpy(pcanvas, canvas, CANVAS_WIDTH * CANVAS_HEIGHT);
}

void DrawCanvas()
{
	for (uint8_t y = 0; y < CANVAS_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < CANVAS_WIDTH; x++)
			putchar(canvas[x + y * CANVAS_WIDTH]);

		putchar('\n');
	}
}

void UpdateCanvas()
{
	memcpy(canvas, pcanvas, CANVAS_WIDTH * CANVAS_HEIGHT);
}

int main(int argc, char** argv)
{
	ConstructCanvas();

	// Disable text cursor in the console
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = false;

	uint16_t timer = 0;
	
	while (true)
	{
		DrawPlatforms();
		DrawBall();
		DrawCanvas();

		if (_kbhit()) MovePlayer(_getch());

		SetConsoleCursorPosition(hOut, {0, 0});
		SetConsoleCursorInfo(hOut, &cci);

		UpdateCanvas();	
	}

	return 0;
}
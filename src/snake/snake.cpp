#include <cstdio>
#include <conio.h>
#include <vector>

#include "../console.h"


struct position
{
	int x, y;
};


constexpr int WIDTH = 50;
constexpr int HEIGHT = 50;


int width, height;
bool running;
float timer;

int direction;
int x, y;
bool dirChanged;

int fruitX, fruitY;
int points;

std::vector<position> tail;


void generate_fruit() {
	if (fruitX == -1 && fruitY == -1)
	{
		fruitX = 42;
		fruitY = 21;
	}
	else
	{
		fruitX = rand() % (WIDTH - 2) + 1;
		fruitY = rand() % (HEIGHT - 2) + 1;
	}

	put_char(fruitX, fruitY, '@', 0xFF451C, 0);
}

void init() {
	running = true;
	timer = 0.0f;

	direction = 0;
	x = 10;
	y = 10;
	fruitX = -1;
	fruitY = -1;
	points = 0;

	print_s(0, 0, std::string(WIDTH, (char)176).c_str(), 0xBBBBBB, 0);
	print_s(0, HEIGHT - 1, std::string(WIDTH, (char)176).c_str(), 0xBBBBBB, 0);
	for (int i = 1; i < HEIGHT - 1; i++)
	{
		put_char(0, i, (char)176, 0xBBBBBB, 0);
		put_char(WIDTH - 1, i, (char)176, 0xBBBBBB, 0);
	}

	print_s(10, 11, "Some tasty letters you can eat", 0xFFA042, 0);
	print_s(7, 21, "Your goal is to eat the red apple ( )", 0x4772FF, 0);
	print_s(15, 31, "Press ESC to ragequit", 0xEAC220, 0);

	srand((unsigned int)time(0));
	generate_fruit();
}

void game_over()
{
	print_s(16, 24, "YOUR SNEK JUST DIED.", 0xFF0000, 0);
	print_s(2, 26, "WHY DON'T YOU PLAY SOMETHING THAT DOESN'T TAKE", 0xFF0000, 0);
	print_s(17, 27, "AN OUNCE OF SKILL?", 0xFF0000, 0);
	print_s(17, 29, "Your score was: ", 0xDDDDDD, 0);
	print_s(std::to_string(points).c_str());
	print_s(11, 31, "Press any key to open roblox.", 0xDDDDDD, 0);
	Sleep(4000);
	_getch();

	running = false;
}

void key_press(int key) {
	switch (key)
	{
	case VK_ESCAPE:
		running = false;
		break;
	case 224:
		key = _getch();
		if (key == 77) // Right
		{
			if (direction != 2 && !dirChanged)
			{
				direction = 0;
				dirChanged = true;
				timer = 1.0f;
			}
		}
		else if (key == 75) // Left
		{
			if (direction != 0 && !dirChanged)
			{
				direction = 2;
				dirChanged = true;
				timer = 1.0f;
			}
		}
		else if (key == 72) // Up
		{
			if (direction != 1 && !dirChanged)
			{
				direction = 3;
				dirChanged = true;
				timer = 1.0f;
			}
		}
		else if (key == 80) // Down
		{
			if (direction != 3 && !dirChanged)
			{
				direction = 1;
				dirChanged = true;
				timer = 1.0f;
			}
		}
		break;
	}
}

void advance_tail()
{
	for (int i = tail.size() - 1; i >= 0; i--)
	{
		if (i == 0)
		{
			tail[i].x = x;
			tail[i].y = y;
		}
		else
		{
			tail[i].x = tail[i - 1].x;
			tail[i].y = tail[i - 1].y;
		}
	}
}

void tick() {
	if (tail.empty())
	{
		put_char(x, y, ' ');
	}
	else
	{
		put_char(tail[tail.size() - 1].x, tail[tail.size() - 1].y, ' ');
	}

	advance_tail();
	dirChanged = false;

	switch (direction)
	{
	case 0:
		x++;
		break;
	case 1:
		y++;
		break;
	case 2:
		x--;
		break;
	case 3:
		y--;
		break;
	}

	for (unsigned int i = 0; i < tail.size(); i++)
	{
		if (x == tail[i].x && y == tail[i].y)
		{
			game_over();
		}
	}
	if (x < 1 || y < 1 || x >= WIDTH - 1 || y >= HEIGHT - 1)
	{
		game_over();
	}
	else if (x == fruitX && y == fruitY)
	{
		points++;
		generate_fruit();
		tail.push_back({ x, y });
	}

	print_s(0, 0, "POINTS: ", 0, 0xBBBBBB);
	print_s(std::to_string(points).c_str());

	put_char(x, y, (char)219, 0x7DC446, 0);
	if (!tail.empty())
	{
		put_char(tail[0].x, tail[0].y, (char)219, 0x7DC446, 0);
	}
}

void update(float dt) {
	timer += dt;
	int speed = 10 + points;
	if (timer > 1.0f / speed)
	{
		tick();
		timer = 0;
	}
}

int main() {
	init_scr();
	set_size(WIDTH, HEIGHT);
	set_font(16, 16, L"Terminal");
	printf(USE_ALTERNATE_BUFFER  CLEAR_SCREEN  HIDE_CURSOR);

	init();

	long long last = get_millis();
	long long lastSecond = get_millis();
	int frames = 0;

	while (running)
	{
		long long now = get_millis();
		float delta = (now - last) / 1000.0f;
		last = now;
		if (now - lastSecond > 1000)
		{
			set_title(std::to_string(frames).c_str());
			lastSecond = now;
			frames = 0;
		}

		if (_kbhit())
		{
			key_press(_getch());
		}

		update(delta);

		frames++;

		Sleep(1);
	}

	printf(USE_MAIN_BUFFER  SOFT_RESET);
	end_scr();
}
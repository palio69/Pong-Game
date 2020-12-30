#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <window.hpp>
#include <image.hpp>

#define W 960
#define H 480
#define FPS 20
#define ADD_BORDERS add_obj( { 0, 0, W, 0 } );	\
	add_obj( { 0, 0, 0, H } );					\
	add_obj( { W, 0, 0, H } );					\
	add_obj( { 0, H, W, 0 } );

std::vector<std::pair<
	std::pair<int, int>,
	std::pair<int, int>>> pos;



void add_obj(SDL_Rect rec);
void update_obj(const int index, SDL_Rect new_);
bool check_x_collision(SDL_Rect rec);
bool check_y_collision(SDL_Rect rec);


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	bool game_over = false;
	int bx = W / 2 - 16, by = H / 2 - 16, bw = 32, bh = 32,
		x1 = 32, y1 = 0, w1 = 32, h1 = 120,
		x2 = W - 64, y2 = 0, w2 = 32, h2 = 120;
	const int delay = 1000 / FPS;

	enum { UP, DOWN } bhdir = UP, dir1 = UP, dir2 = UP;
	enum { LEFT, RIGHT } bwdir = LEFT;


	window win = window("Pong Game", W, H);
	image background = image(win, "res/background.jpg", {0, 0, 1024, 600}, { 0, 0, W, H } ),
		ball = image(win, "res/ball.jpg", { 510, 130, 190, 150 }, { bx, by, bw, bh } ),
		p1 = image(win, "res/player1.png", { 0, 0, 637, 392 }, { x1, y1, w1, h1 } ),
		p2 = image(win, "res/player2.jpg", { 0, 0, 447, 339 }, { x2, y2, w2, h2 } );

	ADD_BORDERS
	add_obj( { x1, y1, w1, h1 } );
	add_obj( { x2, y2, w2, h2 } );

	auto update_dirs = [&] () -> void {
		const Uint8* key = SDL_GetKeyboardState(nullptr);

		if (key[SDL_SCANCODE_W])
			dir1 = UP;

		else if (key[SDL_SCANCODE_S])
			dir1 = DOWN;

		if (key[SDL_SCANCODE_DOWN])
			dir2 = DOWN;

		else if (key[SDL_SCANCODE_UP])
			dir2 = UP;

	};



	auto check_x_collision_with_player = [&] () -> bool {
		if ( ( (bx == x1 + w1 || bx + bw == x1) && by < y1 + h1 && by + bh > y1) ||
			( (bx == x2 + w2 || bx + bw == x2) && by < y2 + h2 && by + bh > y2) )
			return true;

		return false;
	};

	auto check_y_collision_with_player = [&] () -> bool {
		if ( (bx == x1 + w1 || bx + bw == x1) ||
			(bx == x2 + w2 || bx + bw == x2) )
			return true;

		return false;
	};

	auto move_bx = [&] () -> void {

		for (int i = 0; i < 10; ++i) {

			if (check_x_collision( { bx, by, bw, bh } ) || check_x_collision_with_player())
				if (bwdir == LEFT)
						bwdir = RIGHT;

					else
						bwdir = LEFT;

			if (bwdir == LEFT)
				--bx;

			else
				++bx;

		}

	};

	auto move_by = [&] () -> void {

		for (int i = 0; i < 10; ++i) {

			if (check_y_collision( { bx, by, bw, bh } ))
				if (bhdir == UP)
						bhdir = DOWN;

					else
						bhdir = UP;

			if (bhdir == UP)
				--by;

			else
				++by;

		}

	};

	auto move_y1 = [&] () -> void {

		for (int i = 0; i < 10; ++i) {

			if (check_y_collision( { x1, y1, w1, h1 } ))
				if (dir1 == UP)
						dir1 = DOWN;

					else
						dir1 = UP;

			if (dir1 == UP)
				--y1;

			else
				++y1;

		}

	};

	auto move_y2 = [&] () -> void {

		for (int i = 0; i < 10; ++i) {

			if (check_y_collision( { x2, y2, w2, h2 } ))
				if (dir2 == UP)
						dir2 = DOWN;

					else
						dir2 = UP;

			if (dir2 == UP)
				--y2;

			else
				++y2;

		}

	};

	auto update_pos = [&] () -> void {
		move_y1();
		move_y2();
		move_bx();
		move_by();
		p1.change_pos(x1, y1);
		p2.change_pos(x2, y2);
		ball.change_pos(bx, by);
	};



	auto draw = [&] () -> void {
		background.draw();
		p1.draw();
		p2.draw();
		ball.draw();
	};

	SDL_Event evn;

	while (!game_over) {

		while(SDL_PollEvent(&evn))
			if (evn.type == SDL_QUIT)
				game_over = true;

		update_dirs();
		update_pos();

		win.clear(0, 0, 0);
		draw();
		win.update();

		SDL_Delay(delay);

	}

	SDL_Quit();
	IMG_Quit();

	return 0;
}



void add_obj(SDL_Rect rec) {
	pos.push_back( {
		{ rec.x, rec.y },
		{ rec.x + rec.w, rec.y + rec.h }
	} );
}

void update_obj(const int index, SDL_Rect new_) {
	pos[index] = {
		{ new_.x, new_.y },
		{ new_.x + new_.w, new_.y + new_.h }
	};
}

bool check_x_collision(SDL_Rect rec) {
	for (auto i : pos)
		if ( (rec.x == i.second.first || rec.x + rec.w == i.first.first) && rec.y < i.second.second && rec.y + rec.h > i.first.second)
			return true;

	return false;
}

bool check_y_collision(SDL_Rect rec) {
	for (auto i : pos)
		if ( (rec.y == i.first.second || rec.y + rec.h == i.second.second) && rec.x < i.second.first && rec.x + rec.w > i.first.first)
			return true;

	return false;
}
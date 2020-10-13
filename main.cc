/*
 * main.cc
 *
 *  Created on: 12 окт. 2020 г.
 *      Author: Ахаимов Сергей Владиславович МИ-1701
 */


/* Задание
 * 1. используя функции SDL (SDL_RenderDraw... и SDL_RenderFill...), нарисовать:
 * 		- поверхность земли
 * 		- траву
 * 		- дом с крышей, окном, дверью и трубой
 * 		- на заднем плане забор (опционально)
 * 	2. Реализовать рисование окружности с исползованием алгоритма Брезенхэма
 * 	3. Нарисовать у домика круглое окно на чердаке
 *
 */

#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

constexpr int WIDTH = 1280, HEIGHT = 720;

// 2 задание (алгоритм Брезенхэма)
void DrawCircle(SDL_Renderer * ren, int centerx, int centery, int R)
{
	int x = 0;
	int y = R;
	int delta = 1-2*R;
	int error = 0;
	while (y >= 0)
	{
		SDL_RenderDrawPoint(ren, centerx + x, centery + y);
		SDL_RenderDrawPoint(ren, centerx + x, centery - y);
		SDL_RenderDrawPoint(ren, centerx - x, centery + y);
		SDL_RenderDrawPoint(ren, centerx - x, centery - y);
		error = 2*(delta + y) - 1;
		if ((delta < 0) && (error <= 0))
		{
			delta += 2 * ++x + 1;
			continue;
		}
		if ((delta > 0) && (error > 0))
		{
			delta -= 2 * --y + 1;
			continue;
		}
		delta += 2 * (++x - --y);
	}
}

// Трава
void DrawGrassL(SDL_Renderer * ren, int centerx, int centery, int R) // стебель влево
{
	int x = 0;
	int y = R;
	int delta = 1-2*R;
	int error = 0;
	while (y >= 0)
	{
		SDL_RenderDrawPoint(ren, centerx + x, centery - y);
		error = 2*(delta + y) - 1;
		if ((delta < 0) && (error <= 0))
		{
			delta += 2 * ++x + 1;
			continue;
		}
		if ((delta > 0) && (error > 0))
		{
			delta -= 2 * --y + 1;
			continue;
		}
		delta += 2 * (++x - --y);
	}
}

void DrawGrassR (SDL_Renderer * ren, int centerx, int centery, int R) // стебель вправо
{
	int x = 0;
	int y = R;
	int delta = 1-2*R;
	int error = 0;
	while (y >= 0)
	{
		SDL_RenderDrawPoint(ren, centerx - x, centery - y);
		error = 2*(delta + y) - 1;
		if ((delta < 0) && (error <= 0))
		{
			delta += 2 * ++x + 1;
			continue;
		}
		if ((delta > 0) && (error > 0))
		{
			delta -= 2 * --y + 1;
			continue;
		}
		delta += 2 * (++x - --y);
	}
}

void DrawGrass (SDL_Renderer * ren, int centerx, int centery, int R) // объединение стеблей
{
	DrawGrassL(ren, centerx, centery, R);
	DrawGrassR(ren, centerx+2*R, centery, R);

}

void DrawGrassLine (SDL_Renderer * ren, int posx, int posy, int width, int height) // кустики
{
	SDL_RenderDrawLine(ren, posx, posy, posx + width/2, posy - height);
	SDL_RenderDrawLine(ren, posx, posy, posx - width/2, posy - height);
	SDL_RenderDrawLine(ren, posx, posy, posx, posy - height*2);
}

void DrawBlowball (SDL_Renderer * ren, int centerx, int centery, int R, int height) // одуванчик
{
	for (int i = 0; i < R; i++)
	{
		SDL_SetRenderDrawColor(ren, 255,255,0, 255);
		DrawCircle(ren, centerx, centery, i);
	}
	SDL_SetRenderDrawColor(ren, 131,241,0, 255);
	SDL_RenderDrawLine(ren, centerx, centery + R, centerx, centery + height);
}

void DrawFlower (SDL_Renderer * ren, int centerx, int centery, int R) // голубой цветочек
{
	SDL_SetRenderDrawColor(ren, 131,241,0, 255); //цветочек
	DrawGrass(ren, centerx, centery,R);
	SDL_RenderDrawLine(ren, centerx+R, centery, centerx+R, centery+R);
	for (int i = 0; i < 7; i++)
	{
		SDL_SetRenderDrawColor(ren, 67,154,255, 255);
		DrawCircle(ren, centerx, centery-10, i);
		DrawCircle(ren, centerx+R*2, centery-10, i);
	}
}

void DrawSunFlower (SDL_Renderer * ren, int centerx, int centery, int R) // подсолнух
{
	SDL_SetRenderDrawColor(ren, 131,241,0, 255); //подсолнух
//	DrawGrass(ren, 1100, 470, 50);
	DrawGrass(ren, centerx, centery, R);
	SDL_RenderDrawLine(ren, centerx+R, centery, centerx+R, centery+R);
	for (int i = 0; i < R-30; i++)
	{
		while (i <= R-30-5)
		{
			SDL_SetRenderDrawColor(ren, 0,0,0, 255);
			DrawCircle(ren, centerx+R, centery-R, i);
			i++;
		}
		SDL_SetRenderDrawColor(ren, 255,250,51, 255);
		DrawCircle(ren, centerx+R, centery-R, i);
	}

}

//Забор (опционально)
void Palisade(SDL_Renderer * ren, int posx, int posy, int width, int height)
{
	for (int i = 0; i < width; i ++)
	{
		SDL_RenderDrawLine(ren, posx - i, posy + i,
				posx + i, posy + i);
	}
	SDL_Rect rect {posx - width, posy + width, 2*width, height};
	SDL_RenderFillRect(ren, &rect);
}


int main(int, char **)
{
	std::cout << "Начало работы" << std::endl;

	SDL_Init(SDL_INIT_EVERYTHING);

	auto win = std::shared_ptr<SDL_Window>(
			SDL_CreateWindow("Okno prilogenja",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					WIDTH, HEIGHT,
				SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);

	if (win == nullptr) {
		std::cerr << "Oshibka pri sozdanii okna: " <<
				SDL_GetError() << std::endl;
		return 1;
	}

	auto ren = std::shared_ptr<SDL_Renderer>(
			SDL_CreateRenderer(win.get(), -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			SDL_DestroyRenderer);

	if (ren == nullptr)
	{
		std::cerr << "Oshibka pri sozdanii renderera: " <<
				SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Event event;
	bool finish = false;
	//Основной цикл программы
	while (not finish) {
		//Цикл обработки событий
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) // Если окно закрыли
				finish = true;
		}

		// небо
		SDL_SetRenderDrawColor(ren.get(), 205, 195, 255, 255);
		SDL_Rect rect_font_1 {0, 0, WIDTH, (HEIGHT-1)/4};
		SDL_RenderFillRect(ren.get(), &rect_font_1);

		// поверхность земли
		SDL_SetRenderDrawColor(ren.get(), 93, 141, 0, 100);
		SDL_Rect rect_font_2 {0, (HEIGHT-1)/4, WIDTH, HEIGHT-(HEIGHT-1)/4};
		SDL_RenderFillRect(ren.get(), &rect_font_2);

		//Солнышко
		SDL_SetRenderDrawColor(ren.get(), 255,129,0, 255);
		for (int i = 0; i < 50; i++)
		{
			DrawCircle(ren.get(), 1010, 60, i);
		}

		//Забор
		int posx = 0, posy = 100, width = 20, height = 150;
		for (int i = 0; i < 28; i++)
		{
			SDL_SetRenderDrawColor(ren.get(), 255,218,174, 255);
			Palisade(ren.get(), posx, posy, width, height);
			SDL_SetRenderDrawColor(ren.get(), 0,62,94, 255);
			SDL_RenderDrawLine(ren.get(), posx + width, posy + height/4, posx + width + 5, posy + height/4);
			SDL_RenderDrawLine(ren.get(), posx + width, posy + height*3/4, posx +width + 5, posy+ height*3/4);
			posx += 2*width + 5;
		}
		SDL_SetRenderDrawColor(ren.get(), 255,218,174, 255);
		Palisade(ren.get(), 28*(2*width + 5), posy, width, height);

		//Фундамент
		SDL_Rect r1 {(WIDTH-300)/2, (HEIGHT-150)/2, 300, 150};
		SDL_SetRenderDrawColor(ren.get(), 127, 97, 0, 255);
		SDL_RenderFillRect(ren.get(), &r1);



		//Бревна
		SDL_SetRenderDrawColor(ren.get(), 170,102,0, 255);
		DrawCircle(ren.get(), (WIDTH-300)/2 + 15, (HEIGHT-150)/2 + 15, 15);
		DrawCircle(ren.get(), (WIDTH-300)/2 + 300-15, (HEIGHT-150)/2 + 15, 15);

		for (int i = 1; i < 5; i++)
		{
			SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2, (HEIGHT-150)/2 + 150 - 30*i,
					(WIDTH-300)/2 + 300, (HEIGHT-150)/2 + 150 - 30*i);
			DrawCircle(ren.get(), (WIDTH-300)/2 + 15, (HEIGHT-150)/2 + 15 + 30*i, 15);
			DrawCircle(ren.get(), (WIDTH-300)/2 + 300 - 15, (HEIGHT-150)/2 + 15 + 30*i, 15);
		}



		//Рамки фундамента для контраста
		SDL_RenderDrawRect(ren.get(), &r1);

		//Труба
		for (int i = 0; i <= 100; i += 50)
		{
			SDL_Rect r4 {(WIDTH-300)/2 + 200, (HEIGHT-150)/2 - 50 - i, 50, 50};
			SDL_SetRenderDrawColor(ren.get(), 128, 128, 128, 255);
			SDL_RenderFillRect(ren.get(), &r4);
			SDL_SetRenderDrawColor(ren.get(), 255, 255, 255, 255);
			SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 200, (HEIGHT-150)/2 - 50/2 - i,
					(WIDTH-300)/2 + 250-1, (HEIGHT-150)/2 - 50/2 - i);
			SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 200, (HEIGHT-150)/2 - 50 - i,
					(WIDTH-300)/2 + 250-1, (HEIGHT-150)/2 - 50 - i);
		}
		SDL_SetRenderDrawColor(ren.get(), 255, 255, 255, 255);
		SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 225, (HEIGHT-150)/2 - 50,
					(WIDTH-300)/2 + 225, (HEIGHT-150)/2 - 150);
		SDL_Rect r4 {(WIDTH-300)/2 + 200, (HEIGHT-150)/2 - 150, 50, 150};
		SDL_SetRenderDrawColor(ren.get(), 0, 0, 0, 0);
		SDL_RenderDrawRect(ren.get(), &r4);


		//Дверь
		SDL_Rect r2 {(WIDTH-300)/2 + 40, (HEIGHT-150)/2 + 60, 50, 90};
		SDL_SetRenderDrawColor(ren.get(), 0,112,118, 255);
		SDL_RenderFillRect(ren.get(), &r2);

		SDL_SetRenderDrawColor(ren.get(), 42, 28, 0, 255);
		SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 40, (HEIGHT-150)/2 + 150,
				(WIDTH-300)/2 + 90, (HEIGHT-150)/2 + 150);

		//Ручка двери
		for (int i = 0; i < 3; i++)
		{
			DrawCircle(ren.get(), (WIDTH-300)/2 + 80, (HEIGHT-150)/2 + 105, i);
		}

		//Окно дома
		SDL_Rect r3 {(WIDTH-300)/2 + 200-50, (HEIGHT-150)/2 + 30, 50, 60};
		SDL_SetRenderDrawColor(ren.get(), 170, 255, 247, 255);
		SDL_RenderFillRect(ren.get(), &r3);
		SDL_SetRenderDrawColor(ren.get(), 127, 97, 0, 255);

		//Оконные рамы
		SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 225-50, (HEIGHT-150)/2 + 30,
				(WIDTH-300)/2 + 225-50, (HEIGHT-150)/2 + 120);
		SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 225-50, (HEIGHT-150)/2 + 30 + 60/4,
						(WIDTH-300)/2 + 250-50, (HEIGHT-150)/2 + 30 + 60/4);

		SDL_SetRenderDrawColor(ren.get(), 42, 28, 0, 255);
		SDL_RenderDrawRect(ren.get(), &r3);

		//Крыша
		SDL_SetRenderDrawColor(ren.get(), 127, 97, 0, 255);
		for (int i = 0; i < 150; i ++)
		{
			SDL_RenderDrawLine(ren.get(), 570 - i, 135 + i,
					570 + i*2, 135 + i);
		}

		//Рамки крыши для контраста
		SDL_SetRenderDrawColor(ren.get(), 42, 28, 0, 255);
		SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 - 70, (HEIGHT-150)/2,
				(WIDTH-300)/2 + 380, (HEIGHT-150)/2);
		SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 380, (HEIGHT-150)/2,
				(WIDTH-300)/2 + 80, (HEIGHT-150)/2 - 150);
		SDL_RenderDrawLine(ren.get(), (WIDTH-300)/2 + 80, (HEIGHT-150)/2 - 150,
				(WIDTH-300)/2 - 70, (HEIGHT-150)/2);

		// 3. Окно на чердаке
		SDL_SetRenderDrawColor(ren.get(), 67,154,207, 255);
		for (int i = 0; i < 20; i++)
		{
			DrawCircle(ren.get(), 585, 200, i);
		}


		//Растительность

		DrawFlower(ren.get(), 50, 500, 15); //цветочки слева
		DrawFlower(ren.get(), 100, 450, 15);
		DrawFlower(ren.get(), 150, 500, 15);

		DrawFlower(ren.get(), 830, 320, 15); //цветочки справа
		DrawFlower(ren.get(), 1200, 300, 15);
		DrawFlower(ren.get(), 1050, 350, 15);
		DrawFlower(ren.get(), 900, 350, 15);


		DrawSunFlower(ren.get(), 640, 600, 50); //подсолнухи
		DrawSunFlower(ren.get(), 540, 570, 50);
		DrawSunFlower(ren.get(), 440, 600, 50);

		SDL_SetRenderDrawColor(ren.get(), 131,241,0, 255);
		DrawGrass(ren.get(), 1100, 470, 50);
		SDL_SetRenderDrawColor(ren.get(), 131,241,0, 255); //стебельки
		DrawGrass(ren.get(), 350, 500, 35);
//		DrawGrass(ren.get(), 135, 250, 15);
//		SDL_RenderDrawLine(ren.get(), 135+15, 250, 135+15, 250+15);
		DrawGrass(ren.get(), 1050, 680, 60);

//		DrawGrass(ren.get(), 900, 350, 20);
//		SDL_RenderDrawLine(ren.get(), 900+20, 350, 900+20, 350+20);

		DrawGrassLine(ren.get(), 170, 360, 50, 10);//кустики с одуванчиками слева
		DrawBlowball(ren.get(), 170, 320, 10, 40);

		DrawGrassLine(ren.get(), 210, 360, 50, 10);
		DrawBlowball(ren.get(), 210, 310, 10, 40);

		DrawGrassLine(ren.get(), 250, 360, 50, 10);
		DrawBlowball(ren.get(), 250, 320, 10, 40);

		DrawGrassLine(ren.get(), 880, 560, 60, 10); //кустики с одуванчиками справа
		DrawBlowball(ren.get(), 880, 450, 16, 90);

		DrawGrassLine(ren.get(), 950, 530, 60, 10);
		DrawBlowball(ren.get(), 950, 430, 16, 90);

		DrawGrassLine(ren.get(), 1020, 560, 60, 10);
		DrawBlowball(ren.get(), 1020, 450, 16, 90);


		DrawBlowball(ren.get(), 200, 600, 30, 120); //одуванчики внизу
		DrawBlowball(ren.get(), 250, 650, 30, 120);
		DrawBlowball(ren.get(), 150, 650, 30, 120);
/*
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
		DrawGrass(ren.get(), 50, 500, 15);
*/

		/*
		 * Двойная буферизация:
		 * 1 буфер (передний) - содержимое отображается в окне
		 * 2 буфер (задний) - осуществляется построение изображения
		 */
		SDL_RenderPresent(ren.get());
	}





	std::cout << "Конец работы" << std::endl;
	return 0;
}

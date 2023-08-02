#include <SFML/Graphics.hpp>
#include <time.h>
#include <random>
#include <iostream>


using namespace sf;


//when an empty tile is click this sets all the ones in the grid to be shown
void showEmptyTiles(int i , int j,int g[12][12],int gs[12][12], bool checkedCords[12][12]){
	
	//check Cases
	if(i < 1 || i > 11 || j < 1 || j > 11) return; // out of bounds 
	if(checkedCords[i][j]) return; // already checked
	checkedCords[i][j] = true;
	if(g[i][j] == 9) return; // is a mine 

	//recurse only if 0 otherwise just show the tile and return
	//recursing through each neighbour and showing them
	if(g[i][j] == 0){
		for(int k = -1; k <= 1 ; k ++){
			for(int w = -1; w <= 1 ; w++){
				if(k == 0 && w == 0) continue;
				showEmptyTiles(i+k,j+w,g,gs,checkedCords);
			};
		};
	};
	gs[i][j] = g[i][j];
	return;
};


int main()
{
	srand(time(NULL));

	std::default_random_engine rd(time(NULL));
	std::uniform_int_distribution<int> dis;

	RenderWindow app(VideoMode(400, 400), "Minesweeper");

	int w = 32;
	int grid[12][12];
	int sgrid[12][12];

	Texture t;
	t.loadFromFile("textures/tiles.jpg");
	// t.loadFromFile("textures/tiles_2.jpg");
	Sprite s(t);

	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			sgrid[i][j] = 10;

			/*
			if (rand() %5 == 0)
			{
				grid[i][j] = 9;
			}
			*/

			if (dis(rd) % 5 == 0)
			{
				grid[i][j] = 9;
			}
			else
			{
				grid[i][j] = 0;
			}
		}
	}

	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			int n = 0;
			if (grid[i][j] == 9) continue;
			if (grid[i + 1][j] == 9) n++;
			if (grid[i][j + 1] == 9) n++;
			if (grid[i - 1][j] == 9) n++;
			if (grid[i][j - 1] == 9) n++;

			if (grid[i + 1][j + 1] == 9) n++;
			if (grid[i - 1][j - 1] == 9) n++;
			if (grid[i - 1][j + 1] == 9) n++;
			if (grid[i + 1][j - 1] == 9) n++;

			grid[i][j] = n;
		}
	}

	while (app.isOpen())
	{
		Vector2i pos = Mouse::getPosition(app);
		int x = pos.x / w;
		int y = pos.y / w;

		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				app.close();
			}

			if (e.type == Event::MouseButtonPressed)
			{
				if (e.key.code == Mouse::Left)
				{
					if(grid[x][y] == 0){
						bool checkedCords[12][12] = {{false}};
						showEmptyTiles(x,y,grid,sgrid,checkedCords);
						break;
					};
					sgrid[x][y] = grid[x][y];
				}
				else if (e.key.code == Mouse::Right)
				{
					sgrid[x][y] == 11;
				}
			}
		}

		app.clear(Color::White);
		for (int i = 1; i <= 10; i++)
		{
			for (int j = 1; j <= 10; j++)
			{
				if (sgrid[x][y] == 9)
				{
					sgrid[i][j] = grid[i][j];
				}

				s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
				s.setPosition(i * w, j * w);
				app.draw(s);
			}
		}

		app.display();
	}
}

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <vector>
#include <dos.h>
#include <iostream>
#include <cstdlib>

struct Vector2 {
	Vector2(int X, int Y) {
		x = X;
		y = Y;
	}
	int x;
	int y;
};
class util {
public:
	std::vector<Vector2> CalculateVertices(Vector2 start, Vector2 end) {

	}
};

class square {
public:
	square(Vector2 X, int w, int h, Vector2 DIR) : x(X), dir(DIR) {
		W = w;
		H = h;
		radius = (x.x + w) / 2;
		int rnd = std::rand() % 9;

		switch (rnd) {
		case (1):
			Char = '+';
			break;
		case (2):
			Char = '-';
			break;
		case (3):
			Char = '$';
			break;
		case (4):
			Char = '&';
			break;
		case (5):
			Char = '@';
			break;
		case (6):
			Char = '%';
			break;
		case (7):
			Char = '|';
			break;
		case (8):
			Char = '^';
			break;
		default:
			Char = ']';
			break;
		}

	}
	int rotation = 0;
	char Char;
	Vector2 x;
	Vector2 dir;
	int W, H;
	int radius;
	bool IsBetween(int min, int max, int value) {
		bool t = false;
		if (min > max) {
			return value <= min && value >= max;
		}
		else {
			return value <= max && value >= min;
		}
	}

	bool intersects(Vector2 point) {

		return IsBetween(x.x, x.x + W, point.x) && IsBetween(x.y, x.y + H, point.y);
	}

	int DistanceToCenter(Vector2 point) {
		if (rotation >= W + 2)
			rotation = 0;
		Vector2 center((x.x + x.x + W) / 2, (x.y + x.y + H) / 2);
		int d = std::abs(center.x - point.x);
		
		int t = std::abs(center.y - point.y);
		rotation++;
		return std::sqrt(d * d + t * t)-d+rotation;
	}

	bool inRange(Vector2 point) {
		return DistanceToCenter(point) <= radius;
	}

	void OutOfBoundsCheck(int w, int h) {
		if (!IsBetween(0, h, x.y + H + 1) || !IsBetween(0, h, x.y)) {
			dir.y = (-1) * dir.y;
		}
		if (!IsBetween(0, w, x.x + W + 1) || !IsBetween(0, w, x.x)) {
			dir.x = (-1) * dir.x;
		}
	}

};

int clamp(int value, int max) {
	return value > max ? max : value;
}

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

namespace drw {
	class Pencil {
	public:
		std::vector<square> objects;
		std::vector<std::vector<char>> frame;



		void CalculateNewPositions() {
			for (int i = 0; i < objects.size(); i++) {
				square& obj = objects[i];
				obj.OutOfBoundsCheck(screenWidth, screenHeight);
				obj.x.x += obj.dir.x;
				obj.x.y += obj.dir.y;
			}
		}

		Pencil(int w, int h) {
			screenWidth = w;
			screenHeight = h;
		}



		int screenWidth;
		int screenHeight;
		void initFrame() {
			frame = {};
			for (int x = 0; x < screenHeight; x++) {
				std::vector<char> ln = {};
				for (int i = 0; i < screenWidth; i++) {
					ln.push_back(' ');
				}
				frame.push_back(ln);
			}
		}
		void drawTestLine() {
			for (int x = 0; x < screenHeight; x++) {
				for (int i = 0; i < screenWidth; i++)
					if (x % 2 == 0) {
						std::cout << "-";
					}
					else {
						std::cout << "+";
					}

			}
			std::cout << std::endl;
		}

		void ProcessFrame() {
			for (int y = 0; y < screenHeight; y++) {

				std::vector<char>& line = frame[y];
				for (int x = 0; x < screenWidth; x++) {
					for (int i = 0; i < objects.size(); i++) {
						if (objects[i].intersects(Vector2(x, y)))
							line[x] = objects[i].Char;

					}
				}
			}
			CalculateNewPositions();
		}

		void DrawFrame() {
			std::string newFrame = "";
			for (int y = 0; y < screenHeight; y++) {
				std::string ln = "";
				for (int x = 0; x < screenWidth; x++) {
					setCursorPosition(x, y);
					ln.push_back(frame[y][x]);
				}
				setCursorPosition(screenWidth, y);
				ln.append("\n");
				newFrame.append(ln);
			}
			setCursorPosition(0, 0);
			std::cout << newFrame;
		}

		void GenerateRandomSquares(int amount) {
			int w = 1;
			int h = 1;
			for (int i = 0; i < amount; i++) {
				int speed = (std::rand() % 3) + 1;
				int dirx = std::rand() % 2 == 1 ? -1 * speed : 1 * speed;
				int diry = std::rand() % 2 == 1 ? -1 * speed : 1 * speed;
				int x = std::rand() % clamp(w, screenHeight);
				int y = std::rand() % clamp(h, screenHeight);
				square sqr = square(Vector2(x, y), std::rand() % clamp(20, screenWidth - w), std::rand() % clamp(20, screenHeight - h), Vector2(dirx, diry));
				objects.push_back(sqr);
				h += y;
				x += w;


			}
		}
	};

}



int main() {
	//pencil.objects = std::vector<square>{ square(Vector2(14, 4), 2, 2, Vector2(-1,1)), square(Vector2(4, 4), 13, 12, Vector2(1,-1)), square(Vector2(10, 10), 10, 3, Vector2(-1,1)), square(Vector2(10, 10), 10, 10, Vector2(1,-1)), square(Vector2(2, 2), 5, 10, Vector2(-1,1)), square(Vector2(3, 5), 6, 8, Vector2(0,1)) };
	int input1;
	std::cout << "How many wide must the room be: ";
	std::cin >> input1;
	system("cls");
	int input2;
	std::cout << "How height must the room be: ";
	std::cin >> input2;
	drw::Pencil pencil(input1, input2);
	system("cls");
	pencil.drawTestLine();
	std::string input3;
	std::cout << "Get the + and - all on their own lines together no mixed ones, then enter something here";
	std::cin >> input3;
	system("cls");
	int input4;
	std::cout << "How many random Objects: ";
	std::cin >> input4;
	pencil.GenerateRandomSquares(input4);
	system("cls");
	while (true) {
		pencil.initFrame();
		pencil.ProcessFrame();
		pencil.DrawFrame();
	}

	return 1;
}

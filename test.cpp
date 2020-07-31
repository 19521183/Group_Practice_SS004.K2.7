#include<Windows.h>﻿
#include<iostream>
#include<conio.h>
#include<cstdlib>
#include<time.h>
#include<fstream>
#include<stdlib.h>
#include<string>
#pragma execution_character_set( "utf-8" )
#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
#define width 60
#define height 20
#define Rong 25
#define Cao 7
#define CYAN 12
#define YELLOW 14

using namespace std;

struct HighScore
{
	string Name;
	int Score;
};
struct Point {
	int x;
	int y;
};
void XoaManHinh() {
	//source: blogkhanhtoan.wordpress.com/2016/03/07/mot-so-ham-mo-rong-trong-cc/
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount = 0;
	COORD homeCoords = { 0,0 };
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;
	//Lấy số lượng ô trong bộ nhớ đệm hiện tại
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))cellCount = csbi.dwSize.X * csbi.dwSize.Y;
	//Lấp đầy bộ nhớ đệm với khoảng trắng
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count)) return;
	//Lấp đầy bộ nhớ đệm với màu và thuộc tính hiện tại
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;
	//Di chuyển con trỏ về vị trí 0,0
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
void gotoXY(int column, int line) {
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setTextColor(int color)// đổi màu output
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void DuaConTroVeDau()// đưa con trỏ console về vị trí 0,0
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(hConsole, pos);
}
int getNumberOfDigit(int n)
{
	if (n == 0) return 1;
	int i = 0;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return i;
}

class MENU
{
public:
	void Write(string s, int x, int y, int color)//in thông tin các lựa chọn
	{
		setTextColor(color);
		gotoXY(x, y); cout << s;
		setTextColor(15);
	}
	void Khung(int x1, int y1, int x2, int y2)// in khung cho menu
	{
		x1 -= 2;
		x2 += 2;
		int x, y;
		setTextColor(3);
		for (x = x1; x <= x2; x += 2)
		{
			gotoXY(x, y1); cout << "'.";
			gotoXY(x, y2); cout << "'.";
		}
		for (y = y1; y <= y2; y++)
		{
			gotoXY(x1, y); cout << "'.";
			gotoXY(x2, y); cout << "'.";
		}
		setTextColor(14);
	}
	void Ve_menu(int x0, int y0, int chon, int n, string s[]) // vẽ menu
	{
		y0 += 4;
		x0 += 10;
		int a = 3, b = 14;
		system("cls");
		Write(" -----------------------------------------------\n", 26, 1, a);
		Write("|                                               |\n", 26, 2, a);
		Write("|                                               |\n", 26, 3, a);
		Write("|                                               |\n", 26, 4, a);
		Write("|                                               |\n", 26, 5, a);
		Write("|                                               |\n", 26, 6, a);
		Write("|                                               |\n", 26, 7, a);
		Write("|                                               |\n", 26, 8, a);
		Write(" -----------------------------------------------\n", 26, 9, a);
		Write("    ******   *     *      *     *   * *****    \n", 27, 3, b);
		Write("   *         * *   *     * *    * *   *        \n", 27, 4, b);
		Write("    ******   *  *  *    *   *   **    ****     \n", 27, 5, b);
		Write("          *  *   * *   *******  * *   *        \n", 27, 6, b);
		Write("    ******   *     *  *       * *   * *****    \n", 27, 7, b);
		cout << "" << endl << endl;
		Khung(x0 - 2, y0 - 1, x0 + 30, y0 + n);
		for (int i = 0; i < n; i++)
			if (i == chon) Write(s[i], x0, y0 + i, CYAN);
			else Write(s[i], x0, y0 + i, YELLOW);
	}
};

class BACKGROUND {
private:
	int sl_chuongNgaiVat;
	Point chuongNgaiVat[20];
public:
	BACKGROUND() {
		ofstream khung; // vẽ khung cho game vào tệp khung .txt
		khung.open("khung.txt", ios::out | ios::trunc);
		khung << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;
		for (int i = 0; i <= height - 2; i++)
			khung << "=                                                           =" << endl;
		khung << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;

		khung.close();
	}
	void veKhung() { // lấy khung từ tệp khung.txt và vẽ vào game
		fstream khung;
		khung.open("khung.txt", ios::in);
		char str[62];
		string s;
		setTextColor(15);
		for (int i = 0; i <= height; i++) {
			khung.getline(str, 62);
			s = str;
			cout << s << endl;
		}
		khung.close();
		setTextColor(14);
	}
	void drawFood(Point& F) { // vẽ thức ăn cho rắn
		gotoXY(F.x, F.y);
		setTextColor(12);
		cout << "+";
		setTextColor(14);
		DuaConTroVeDau();
	}
	void setChuongNgaiVat(int doKho) { // thiết lập số chướng ngại vật tùy theo độ khó
		srand(time(0));
		switch (doKho) {
		case 0: {
			sl_chuongNgaiVat = 5;
			break;
		}
		case 1: {
			sl_chuongNgaiVat = 10;
			break;
		}
		case 2: {
			sl_chuongNgaiVat = 15;
			break;
		}
		default: {
			sl_chuongNgaiVat = 20;
			break;
		}
		}
		for (int i = 0; i < sl_chuongNgaiVat; i++) {
			do {
				chuongNgaiVat[i].x = 1 + rand() % 59;
				chuongNgaiVat[i].y = 1 + rand() % 19;
			} while (chuongNgaiVat[i].x == 10 && (chuongNgaiVat[i].y >= 10 && chuongNgaiVat[i].y <= 40 ));
		}
	}
	void drawChuongNgaiVat() { // vẽ chướng ngại vật
		setTextColor(3);
		for (int i = 0; i < sl_chuongNgaiVat; i++) {
			gotoXY(chuongNgaiVat[i].x, chuongNgaiVat[i].y);
			cout << "X";
		}
		setTextColor(14);
	}
	bool checkObstaclesCollision(Point headSnack) { // kiểm tra rắn có va vào chướng ngại vật hay không
		for (int i = 0; i < sl_chuongNgaiVat; i++) {
			if (headSnack.x == chuongNgaiVat[i].x && headSnack.y == chuongNgaiVat[i].y)
				return true;
		}
		return false;
	}
	
};

class SNACK {
protected:
	Point ran[100];
	int doDai;
	int direction;
public:
	SNACK() {
		direction = 4;
		doDai = 3;
		ran[0].x = 12;
		ran[0].y = 10;
		ran[1].x = 11;
		ran[1].y = 10;
		ran[2].x = 10;
		ran[2].y = 10;
	}
	// vẽ rắn
	void Ve1() {
		int i;
		setTextColor(10);
		i = 0;
		while (i < doDai) {
			gotoXY(ran[i].x, ran[i].y);
			cout << "O";
			i++;
		}
		setTextColor(14);
	}
	void Ve() {
		int i;
		setTextColor(10);
		if (checkFrameConllision()) i = 1;
		else i = 0;
		gotoXY(ran[i].x, ran[i].y);
		cout << "O";
		setTextColor(14);
	}
	// di chuyển rắn theo hướng hiện tại
	void Move() {
		int t = doDai - 1;
			gotoXY(ran[t].x, ran[t].y);
			cout << " ";
			DuaConTroVeDau();
		for (int i = doDai - 1; i > 0; i--) {
			ran[i] = ran[i - 1];
		}
		if (direction == 1) {
			ran[0].y--;
		}
		else if (direction == 2) {
			ran[0].y ++ ;
		}
		else if (direction == 3) {
			ran[0].x--;
		}
		else {
			ran[0].x ++;
		}
		
		
	}
	void setDirection(int direct) {
		if (ran[0].x == ran[1].x && ran[0].y > ran[1].y && direct == 1);
		else if (ran[0].x == ran[1].x && ran[0].y < ran[1].y && direct == 2);
		else if (ran[0].y == ran[1].y && ran[0].x > ran[1].x && direct == 3);
		else if (ran[0].y == ran[1].y && ran[0].x < ran[1].x && direct == 4);
		else
			direction = direct;
	}
	//kiểm tra xem con rắn có tự đụng thân không
	bool checkCollision() {
		for (int i = 1; i < doDai; i++) {
			if (ran[0].x == ran[i].x && ran[0].y == ran[i].y) {
				return true;
			}
		}
		return false;
	}
	//Kiểm tra xem con rắn có đụng trúng khung hay không
	//Sử dụng hàm này khi chọn chế độ chơi không xuyên qua khung
	bool checkFrameConllision() {
		if (ran[0].x == 0 || ran[0].y == 0 || ran[0].x == width || ran[0].y == height)
			return true;
		return false;
	}
	//kiểm tra thức ăn có xuất hiện ở chỗ thân con rắn hiện tại hay không
	bool checkFoodCollision(Point F) {
		for (int i = 1; i < doDai; i++) {
			if (ran[i].x == F.x && ran[i].y == F.y)
				return true;
		}
		return false;
	}
	//Kiểm tra xem con rắn có ăn thức ăn hay chưa?
	bool ateFood(Point F) {
		if (ran[0].x == F.x && ran[0].y == F.y)
			return true;
		return false;
	}
	//Tăng độ dài của con rắn sau khi ăn mồi
	void growLength(Point T) {
		ran[doDai] = T;
		doDai++;
	}
	//di chuyển đầu rắn sang phía bên không gian ngược lại
	void goThroughWall() {
		if (ran[0].y == 0)
			ran[0].y = height - 1;
		else if (ran[0].y == height)
			ran[0].y = 1;
		else if (ran[0].x == 0)
			ran[0].x = width - 1;
		else if (ran[0].x == width)
			ran[0].x = 1;
	}
	void checkNearFood(Point F, Point &T)
	{
		if (ran[0].x == F.x && (ran[0].y == F.y - 1 || ran[0].y == F.y + 1)) {
			T = ran[doDai - 1];
		}
		if (ran[0].x == F.y && (ran[0].y == F.x - 1 || ran[0].y == F.x + 1)) {
			T = ran[doDai - 1];
		}
	}
	int GetDoDai()
	{
		return this->doDai;
	}
	Point getHeadSnack() {
		return ran[0];
	}
};

class INTRODUCTION
{
public:
	void ChuThich(bool i) // 1-xuyên tường, 2-có tường
	{
		setTextColor(3);
		for (int j = 1; j < height; j += 2)
		{
			gotoXY(0, j); cout << "/";
			gotoXY(0, j + 1); cout << "\\";
			gotoXY(width, j); cout << "/";
			gotoXY(width, j - 1); cout << "\\";
		}
		for (int i = 0; i < width; i++)
		{
			gotoXY(i, height); cout << "-";
		}
		for (int i = 1; i <= width; i++)
		{
			gotoXY(i, 0); cout << "-";
		}
		setTextColor(14);
		gotoXY(2, 2);
		cout << "+ Hướng dẫn:";
		string s;
		gotoXY(2, 4); cout << "  Bạn phải sử dụng các phím mũi tên hoặc các phím W,A,S,D";
		gotoXY(2, 5); cout << "để di chuyển rắn và thu thập thức ăn. Bạn thu được càng ";
		gotoXY(2, 6); cout << "nhiều thức ăn thì rắn sẽ càng dài ra và điểm số càng tăng.";
		gotoXY(2, 7); cout << "Bạn có thể bấm phím SPACE để tạm dừng.";
		if (i == 1) // in chú thích của chế độ cổ điển (xuyên tường)
		{
			gotoXY(2, 8); cout << "  Ở chế độ Classic, bạn sẽ thua cuộc nếu như để cho rắn ";
			gotoXY(2, 9); cout << "tự cắn vào bất kì phần nào trên thân nó.";
			gotoXY(2, 11); cout << "+ Chú thích:";
			gotoXY(5, 13);
			setTextColor(10); cout << "OOOOO";
			setTextColor(14); cout << ": rắn";
			gotoXY(width / 2 + 5, 13);
			setTextColor(12); cout << "+";
			setTextColor(14); cout << ": thức ăn";
			gotoXY(5, 15);
			setTextColor(3); cout << "X";
			setTextColor(14); cout << ": chướng ngại vật";
			gotoXY(5, 15);
		}
		else if (i == 0)  // in chú thích của chế độ hiện đại (không xuyên tường)
		{
			gotoXY(2, 8); cout << "  Ở chế độ Modern, bạn sẽ thua cuộc nếu như để cho rắn va";
			gotoXY(2, 9); cout << "vào tường hoặc tự cắn vào bất kì phần nào trên thân nó.";
			gotoXY(2, 10); cout << "";
			gotoXY(2, 12); cout << "+ Chú thích:";
			gotoXY(5, 14);
			setTextColor(10); cout << "OOOOO";
			setTextColor(14); cout << ": rắn";
			gotoXY(width / 2 + 5, 14);
			setTextColor(12); cout << "+";
			setTextColor(14); cout << ": thức ăn";
			gotoXY(width / 2 + 5, 15);
			setTextColor(15); cout << "= = =";
			setTextColor(14); cout << ": tường";
			gotoXY(5, 15);
			setTextColor(3); cout << "X";
			setTextColor(14); cout << ": chướng ngại vật";
		}
		DuaConTroVeDau();
	}
	int XuatChuThich(bool i) // trả về 1 là vào hàm play game, 0 là quay về menu
	{
		int k = 2; // mặc định là ở nút Play
		system("cls");
		ChuThich(i);
		char ch = NULL;
		while (ch != 13)
		{
			DuaConTroVeDau();
			switch (k)
			{
			case 1: // đang ở nút Back, nút Back in màu xanh
				gotoXY(9, 18);
				setTextColor(12); cout << "Back";
				setTextColor(14);
				gotoXY(42, 18); cout << "Play";
				DuaConTroVeDau();
				ch = _getch();
				if (ch == KB_RIGHT || ch == KB_LEFT)
					k = 2;
				if (ch == 13) // nhấn nút Back là quay về menu
					return 0;
				break;
			case 2: // đang ở nút Play, nút Play in màu xanh
				gotoXY(9, 18);
				cout << "Back";
				gotoXY(42, 18);
				setTextColor(12);
				cout << "Play";
				setTextColor(14);
				DuaConTroVeDau();
				ch = _getch();
				if (ch == KB_LEFT || ch == KB_RIGHT)
					k = 1;
				if (ch == 13) // nhấn nút Play là vào chơi game
					return 1;
				break;
			}
		}
	}
};

class HIGHSCORE
{
private:
	HighScore highscore[5];
public:
	HIGHSCORE()
	{
		ifstream in("highscore.txt"); // mở tệp highscore.txt để lấy điểm ra
		if (is_empty(in))// nếu tệp trống thì khởi tạo lại bảng điểm và đóng tệp
		{
			Initialize();
			in.close();
			return;
		}
		int i = 0;
		string Player, Score;
		while (i < 5)// lấy thông tin từ tệp vào highscore[5]
		{
			getline(in, Player);
			highscore[i].Name = Player;
			getline(in, Score);
			highscore[i].Score = stoi(Score);
			i++;
		}
		in.close();
	}
	bool is_empty(std::ifstream& pFile)// xem tệp có trống hay không
	{
		return pFile.peek() == ifstream::traits_type::eof();
	}
	void setHighScore(string newName, int newScore)// lấy thông tin điểm mỗi lần game over và sắp hạng lại
	{
		if (newScore <= highscore[4].Score) return; // nếu điểm mới <= điểm cuối danh sách thì không thêm vào
		int i = 4;
		while (newScore > highscore[i].Score && i >= 0) i--; // tìm và đưa điểm mới vào đúng vị trí
		i = i + 1;
		for (int j = 4; j > i; j--)
			highscore[j] = highscore[j - 1];
		highscore[i].Name = newName;
		highscore[i].Score = newScore;

		ofstream out("highscore.txt"); // sau khi sắp hạng in lại vào tệp để lưu lại
		for (int i = 0; i < 5; i++)
			out << highscore[i].Name << endl << highscore[i].Score << endl;
		out.close();
	}

	void Initialize()// khỏi tạo tập tin highscore.txt
	{
		ofstream out("highscore.txt");
		for (int i = 0; i < 5; i++)
		{
			highscore[i].Name = "Player";
			highscore[i].Score = -1;
			out << highscore[i].Name << i+1 << endl << highscore[i].Score << endl;
		}
		out.close();
	}
	void ShowScore(MENU& M)// in thông tin trong highscore.txt vào phần highscore trong menu
	{
		M.Khung(Rong - 2, Cao - 2, Rong + 30, Cao + 10);
		gotoXY(Rong - 2, Cao);
		setTextColor(12);
		cout << "  Rank           Name      Score";
		setTextColor(14);
		gotoXY(Rong - 2, Cao +1);
		ifstream in("highscore.txt");
		int i = 0;
		string Player, Score;
		while (i < 5)
		{
			getline(in, Player);
			highscore[i].Name = Player;
			getline(in, Score);
			highscore[i].Score = stoi(Score);
			gotoXY(Rong +3, Cao + i + 1);
			cout  << i + 1;
			gotoXY(Rong + 19 - highscore[i].Name.size(), Cao + 1 + i);
			cout << highscore[i].Name;
			if (highscore[i].Score == -1)
			{
				gotoXY(Rong + 29, Cao + 1 + i);
				cout << highscore[i].Score + 1;
			}
			else
			{
				gotoXY(Rong + 30 - getNumberOfDigit(highscore[i].Score), Cao + 1 + i);
				cout << highscore[i].Score;
			}
			i++;
		}
		int Xoa = 2; // mặc định nút xóa dữ liệu highscore là no
		setTextColor(14);
		gotoXY(Rong, Cao + 7); cout << "Delete highscore data?";
		char ch = NULL;
		while (ch != 13)
		{
			DuaConTroVeDau();
			switch (Xoa)
			{
			case 1:
				gotoXY(Rong, Cao + 8);
				setTextColor(12); cout << "Yes      ";
				setTextColor(14); cout << "No";
				DuaConTroVeDau();
				ch = _getch();
				if (ch == KB_RIGHT)
					Xoa = 2;
				break;
			case 2:
				gotoXY(Rong, Cao + 8);
				setTextColor(14); cout << "Yes      ";
				setTextColor(12); cout << "No";
				DuaConTroVeDau();
				ch = _getch();
				if (ch == KB_LEFT)
					Xoa = 1;
				break;
			}
		}
		if (Xoa == 1) { Initialize(); }// nếu chọn nút xóa là yes thì khởi tạo lại tập tin highscore.txt 
		in.close();
	}
	friend class GAME;
};

class GAME
{
private:
	MENU M;
	INTRODUCTION I;
	HIGHSCORE H;
	BACKGROUND B;
	int Score;
	int DoKho;
public:
	void run()
	{
		char ch;
		string st[20];
		system("cls");
		st[0] = "New Game";
		st[1] = "High Score";
		st[2] = "Quit game";
		string PA[2];
		PA[0] = "YES";
		PA[1] = "NO";
		int  chon = 0/*lựa chọn hiện tại*/, luuchon/*lựa chọn trước đó*/, soluachon = 3, ok = FALSE/*nhấn ENTER hay chưa*/;
		M.Ve_menu(Rong, Cao, chon, soluachon, st);
		do
		{
			DuaConTroVeDau();
			ch = _getch(); //nhấn một phím
			switch (ch)
			{
			case 72: //phím lên
				luuchon = chon;
				chon--;
				if (chon < 0) chon = soluachon - 1;//đến cuối thì biến đếm quay lại lựa chọn đầu
				M.Write(st[luuchon], Rong + 10, Cao + luuchon + 4, YELLOW);//lựa chọn trước đó đổi lại thành màu vàng
				M.Write(st[chon], Rong + 10, Cao + chon + 4, CYAN);//lựa chọn đang chọn sẽ là màu xanh
				break;
			case 80://phím xuống
				luuchon = chon;
				chon++;
				if (chon == soluachon) chon = 0;
				M.Write(st[luuchon], Rong + 10, Cao + luuchon + 4, YELLOW);
				M.Write(st[chon], Rong + 10, Cao + chon + 4, CYAN);
				break;
			case 13: //phím ENTER
				ok = TRUE; break;
			}
			if (ok == TRUE) //nếu phím ENTER được nhấn
			{
				switch (chon)
				{
				case 0:
					int brk;
				x1:
					system("cls");
					brk = PlayGame();
					if (brk == 1)
					{
						M.Write("   Play again?", Rong + 2, Cao + 9, 14);
						ch = 0;
						int ChonPA = 0, LuuPA;
						M.Write(PA[0], Rong + 2, Cao + 11, CYAN);
						M.Write(PA[1], Rong + 16, Cao + 11, YELLOW);
						while (ch != 13)
						{
							DuaConTroVeDau();
							ch = _getch(); //nhấn một phím
							switch (ch)
							{
							case 75: //phím trái
								LuuPA = ChonPA;
								ChonPA--;
								if (ChonPA < 0) ChonPA = 2 - 1;//đến cuối thì biến đếm quay lại lựa chọn đầu
								M.Write(PA[LuuPA], Rong + (LuuPA == 1 ? 16 : 2), Cao + 11, YELLOW);//lựa chọn trước đó đổi lại thành màu vàng
								M.Write(PA[ChonPA], Rong + (ChonPA == 1 ? 16 : 2), Cao + 11, CYAN);//lựa chọn đang chọn sẽ là màu xanh
								break;
							case 77://phim phải
								LuuPA = ChonPA;
								ChonPA++;
								if (ChonPA == 2) ChonPA = 0;
								M.Write(PA[LuuPA], Rong + (LuuPA == 1 ? 16 : 2), Cao + 11, YELLOW);
								M.Write(PA[ChonPA], Rong + (ChonPA == 1 ? 16 : 2), Cao + 11, CYAN);
								break;
							}
						}
						if (ChonPA == 0) goto x1;
					}
					M.Ve_menu(Rong, Cao, chon, soluachon, st);
					break;
				case 1:
					system("cls");
					H.ShowScore(M);
					M.Ve_menu(Rong, Cao, chon, soluachon, st);
					break;
				case 2:
					system("cls");
					exit(0);
				}
				ok = FALSE; // trả lại trạng thái ENTER chưa đc nhấn
			}

		} while (ch != 27);//nhấn phím ESC để thoát khỏi chướng trình
	}

	int PlayGame()
	{
		srand(time(NULL));
		int KB_CODE = 0;
		bool xuyenTuong = 0;
		string CD[2];
		string DK[4];
		int Size[4];
		Size[0] = 1;
		Size[1] = 2;
		Size[2] = 10;
		Size[3] = 12;
		CD[0] = "Classic";
		CD[1] = "Modern";
		DK[0] = "Dễ";
		DK[1] = "Trung Bình";
		DK[2] = "Khó";
		DK[3] = "Siêu Khó";
		//Vừa vào trò chơi yêu cầu người chơi chọn độ khó: dễ - trung bình - khó - siêu khó;
		//Chọn chế độ chơi 
		int speed = 0;
		M.Khung(Rong - 2, Cao - 1, Rong + 50, Cao + 5);
		gotoXY(Rong + 20, Cao);
		cout << "Chọn chế độ";
		int ChonCheDo = 0, ch = 0, LuuCHeDo;
		M.Write(CD[0], Rong + 9, Cao + 1, CYAN);
		M.Write(CD[1], Rong + 35, Cao + 1, YELLOW);
		while (ch != 13)
		{
			DuaConTroVeDau();
			ch = _getch(); //Nhan mot phim
			switch (ch)
			{
			case 75: //phim trai
				LuuCHeDo = ChonCheDo;
				ChonCheDo--;
				if (ChonCheDo < 0) ChonCheDo = 2 - 1;//Den cuoi thi bien dem quay lai lua chon dau
				M.Write(CD[LuuCHeDo], Rong + (LuuCHeDo == 1 ? 35 : 9), Cao + 1, YELLOW);//lua chon truoc do doi lai thanh mau vang
				M.Write(CD[ChonCheDo], Rong + (ChonCheDo == 1 ?35 : 9), Cao + 1, CYAN);//lua chon dang chon se doi thanh mau xanh
				break;
			case 77://phim phai
				LuuCHeDo = ChonCheDo;
				ChonCheDo++;
				if (ChonCheDo == 2) ChonCheDo = 0;
				M.Write(CD[LuuCHeDo], Rong + (LuuCHeDo == 1 ? 35 : 9), Cao + 1, YELLOW);
				M.Write(CD[ChonCheDo], Rong + (ChonCheDo == 1 ? 35 : 9), Cao + 1, CYAN);
				break;
			case 27:
				return 0;
			}
		}
		if (ChonCheDo == 0) { xuyenTuong = 1; }
		if (ChonCheDo == 1) { xuyenTuong = 0; }
		//Chọn độ khó
		ch = 0;
		int ChonDK = 0, LuuDK;
		gotoXY(Rong + 20, Cao + 3);
		setTextColor(14);
		cout << "Chọn độ khó";
		for (int i = 0; i < 4; i++)
		{
			if (i == 0) M.Write(DK[i], Rong + 1, Cao + 4, CYAN);
			//else if (i == 3) M.Write(DK[i], Rong + 44, Cao + 5, YELLOW);
			else M.Write(DK[i], Rong + Size[i] + i * 10, Cao + 4, YELLOW);
		}
		while (ch != 13)
		{
			DuaConTroVeDau();
			ch = _getch(); //Nhan mot phim
			switch (ch)
			{
			case 75: //phim trai
				LuuDK = ChonDK;
				ChonDK--;
				if (ChonDK < 0) ChonDK = 3;
				M.Write(DK[LuuDK], Rong + Size[LuuDK] + LuuDK * 10, Cao + 4, YELLOW);
				M.Write(DK[ChonDK], Rong + Size[ChonDK] + ChonDK * 10, Cao + 4, CYAN);
				break;
			case 77://phim phai
				LuuDK = ChonDK;
				ChonDK++;
				if (ChonDK == 4) ChonDK = 0;
				M.Write(DK[LuuDK], Rong + Size[LuuDK] + LuuDK * 10, Cao + 4, YELLOW);
				M.Write(DK[ChonDK], Rong + Size[ChonDK] + ChonDK * 10, Cao + 4, CYAN);
				break;
			case 27:
				return 0;
			}
		}
		switch (ChonDK)
		{
		case 0: speed = 150; break;
		case 1: speed = 100; break;
		case 2: speed = 75; break;
		case 3: speed = 50; break;
		}
		;
		B.setChuongNgaiVat(ChonDK);
		int t;
		t = I.XuatChuThich(xuyenTuong);
		if (t == 0) { system("cls"); return 0; }
		//Mặc định hướng của con rắn ban đầu là đi qua phải
		int huong = 4;
		SNACK S;
		//Khai báo + tự động khởi tạo background và con rắn
		//Ve khung và con rắn và chướng ngại vật
		B.veKhung();
		B.drawChuongNgaiVat();
		M.Write("Score: ", 62, 10, YELLOW);
		M.Write(to_string(Score), 69, 10, 15);
		//S.Move1(); //Lần đầu tiên di chuyển tọa độ đầu và đuôi sẽ bị trùng. Nếu không chạy hàm move 1 lần trước thì hàm checkCollision sẽ true -> Game Over ngay từ đầu
		//Khai báo và khởi tạo các tham số để quản lý con mồi
		Point foodPoint;
		Point tailPoint;
		do {
			foodPoint.x = 1 + rand() % 59;
			foodPoint.y = 1 + rand() % 19;
		} while (S.checkFoodCollision(foodPoint) || B.checkObstaclesCollision(foodPoint));
		B.drawFood(foodPoint);
		//Bắt đầu game
		
		S.Ve1();
		while (KB_CODE != KB_ESCAPE) {
			DuaConTroVeDau();
			if (_kbhit()) {
				KB_CODE = _getch();
				if (KB_CODE == 32)
				{
					B.veKhung();
					S.Ve1();
					B.drawChuongNgaiVat();
					B.drawFood(foodPoint);
					M.Write("Score: ", 62, 10, YELLOW);
					M.Write(to_string(Score), 69, 10, 15);
					gotoXY(18, height + 2);
					system("pause");
					gotoXY(18, height + 2); cout << "                                ";
					DuaConTroVeDau();
				}
				else if (KB_CODE == KB_ESCAPE) { return 0; }
				else if ((KB_CODE >= 'a' && KB_CODE <= 'z') || (KB_CODE >= 'A' && KB_CODE <= 'Z'))
				{
					if (KB_CODE == 'W' || KB_CODE == 'w') {
						huong = 1;
					}
					else if (KB_CODE == 'S' || KB_CODE == 's') {
						huong = 2;
					}
					else if (KB_CODE == 'A' || KB_CODE == 'a') {
						huong = 3;
					}
					else if (KB_CODE == 'D' || KB_CODE == 'd') {
						huong = 4;
					}
				}
				else
				{
					KB_CODE = _getch();
					if (KB_CODE == KB_UP) huong = 1;
					else if (KB_CODE == KB_DOWN) huong = 2;
					else if (KB_CODE == KB_LEFT) huong = 3;
					else if (KB_CODE == KB_RIGHT) huong = 4;
					
				}
				S.setDirection(huong);
			}
			
			if (xuyenTuong == 1 && S.checkFrameConllision()) {
				S.goThroughWall();
				if (B.checkObstaclesCollision(S.getHeadSnack())) {
					DuaConTroVeDau();
					system("cls");
					endgame();
					break;
				}
			}
			else {
				S.Move();
				if (B.checkObstaclesCollision(S.getHeadSnack())) {
					DuaConTroVeDau();
					system("cls");
					endgame();
					break;
				}
			}
			S.Ve();
			DuaConTroVeDau();
			S.checkNearFood(foodPoint, tailPoint);
			if (S.ateFood(foodPoint)) {
				Score += DoKho * 10;
				M.Write(to_string(Score), 69, 10, 15);
				Point T = foodPoint;
				do {
					foodPoint.x = 1 + rand() % 59;
					foodPoint.y = 1 + rand() % 19;
				} while (S.checkFoodCollision(foodPoint) || B.checkObstaclesCollision(foodPoint));
				B.drawFood(foodPoint);
				DuaConTroVeDau();
				S.growLength(tailPoint);
			}

			Sleep(speed);
			//Xử lý Game Over trong chế độ không xuyên tường
			if (xuyenTuong == 1) {
				if (S.checkCollision()) {
					DuaConTroVeDau();
					system("cls");
					endgame();
					break;
				}
			}
			else {
				if (S.checkCollision() || S.checkFrameConllision()) {
					DuaConTroVeDau();
					system("cls");
					endgame();
					break;
				}
			}
		}
		return 1;
	}

	void endgame() // hiển thị phần game over và nhập tên người chơi
	{
		setTextColor(14);
		cout << "" << endl << endl;
		cout << " ------------------------------------------------------------------------- " << endl;
		cout << "|    *****      *     *       * ******       ****  *       ****** ****    |" << endl;
		cout << "|   *          * *    * *   * * *           *    *  *     * *     *   *   |" << endl;
		cout << "|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |" << endl;
		cout << "|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |" << endl;
		cout << "|    *****  *       * *       * ******       ****      *    ***** *   *   |" << endl;
		cout << " ------------------------------------------------------------------------- " << endl;
		cout << "" << endl;
		setTextColor(7);
		cout << "                        Y O U R   S C O R E : " << Score << endl << endl;
		cout << "                        H I G H   S C O R E : "; 
		if (H.highscore[0].Score==-1) 
			cout << H.highscore[0].Score+1 << endl << endl;
		else cout << H.highscore[0].Score << endl << endl;
		string Player;
		gotoXY(Rong - 1, Cao + 7);
		setTextColor(14); cout << "Nhập tên: ";
		setTextColor(7); getline(cin, Player);
		H.setHighScore(Player, Score);
	}
};

int main()
{
	cin.tie(0);
	cout.tie(0);
	ios_base::sync_with_stdio(false);
	GAME G;
	SetConsoleOutputCP(65001);
	G.run();
	return 0;
}

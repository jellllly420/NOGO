#include<iostream>
#include<graphics.h>
#include<conio.h>/////////????????
#include<fstream>
/*rand*/
#include<stdlib.h>
#include<time.h>

using namespace std;

#define GAME_RUNNING     1
#define GAME_OVER        0  
#define MOUSE_POS_ELSE   99
#define MOUSE_POS_AREA   1
#define NUM_BLACK        3
#define NUM_WHITE        4
#define PVC 100
#define PVP 200
#define READ 300
#define PLAY_AGAIN 999
#define SAVE 9999
#define EXIT 888

#define hori_pixel 1024//水平像素
#define verti_pixel 768//垂直像素
#define n 8//格数
#define len 50//格子边长

int p_xl_a, p_xr_a, p_yu_a, p_yd_a;//内边框尺寸
int p_xl_b, p_xr_b, p_yu_b, p_yd_b;//外边框尺寸
int p_xl_c, p_xr_c, p_yu_c, p_yd_c;//落子区域尺寸

void init_system();
void init_globales();

void close_system();

void draw_home();
int  count_xyk_by_message1(MOUSEMSG* m, int* x, int* y);

void draw_chessboard();
void draw_menu();
void condition();
void text();

void play_chess();
void pvp();
void pvc();
int count_xyk_by_message2(MOUSEMSG* m, int* x, int* y, int* a, int* b);



int main() {
	init_system();
	draw_home();
	play_chess();
	close_system();
	return 0;
}

//开始菜单！！！！！！////////////////////////////////

/*功能：初始化系统*/
void init_system() {
	initgraph(hori_pixel, verti_pixel);
	init_globales();
}



/*功能：初始化尺寸参数*/
void init_globales() {
	p_xl_a = hori_pixel / 2 - (n / 2) * len;
	p_xr_a = hori_pixel / 2 + (n / 2) * len;
	p_yu_a = verti_pixel / 2 - (n / 2) * len;
	p_yd_a = verti_pixel / 2 + (n / 2) * len;

	p_xl_b = p_xl_a - len;
	p_xr_b = p_xr_a + len;
	p_yu_b = p_yu_a - len * 2;
	p_yd_b = p_yd_a + len;//外边框

	p_xl_c = p_xl_a - len / 2;
	p_xr_c = p_xr_a + len / 2;
	p_yu_c = p_yu_a - len / 2;
	p_yd_c = p_yd_a + len / 2;//落子区域
}



/*功能；关闭系统*/
void close_system() {
	_getch();//暂停
	closegraph();
}



/*功能：生成棋盘*/
void draw_chessboard() {
	setbkcolor(BLACK);//设置背景色
	cleardevice();//填充背景色

	setlinestyle(PS_SOLID, 1);//外边框类型(单实线）
	setlinecolor(WHITE);//外边框颜色
	rectangle(p_xl_b, p_yu_b, p_xr_b, p_yd_b);//生成矩形（外边框）
	setfillcolor(RGB(172, 81, 24));
	setfillstyle(BS_SOLID);//固实填充
	floodfill((p_xl_a + p_xr_a) / 2, (p_yu_a + p_yd_a) / 2, WHITE);//搞清含义！！！/////////////////////

	setlinestyle(PS_SOLID, 2);//内边框类型（双实线）
	setlinecolor(BLACK);
	rectangle(p_xl_a, p_yu_a, p_xr_a, p_yd_a);

	/*生成交叉线*/
	for (int i = p_xl_a + len; i < p_xr_a; i += len) {
		line(i, p_yu_a, i, p_yd_a);
	}
	for (int i = p_yu_a + len; i < p_yd_a; i += len) {
		line(p_xl_a, i, p_xr_a, i);
	}

	/*生成基准点位*/
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(p_xl_a + len * 2, p_yu_a + len * 2, 6);
	fillcircle(p_xl_a + len * 6, p_yu_a + len * 2, 6);
	fillcircle(p_xl_a + len * 2, p_yu_a + len * 6, 6);
	fillcircle(p_xl_a + len * 6, p_yu_a + len * 6, 6);
	fillcircle(p_xl_a + len * 4, p_yu_a + len * 4, 6);

	draw_menu();
	condition();
	text();
}



/*功能：生成选项卡*/
void draw_menu() {
	int x = 0, y = 0;
	x = p_xr_a + len*2;
	y = p_yd_a - len / 2 * 3;
	setlinecolor(WHITE);
	rectangle(x, y, x + len * 3, y + len /2*3);
	setfillcolor(RGB(172, 81, 24));
	setfillstyle(BS_SOLID);
	floodfill(x + 1, y + 1, WHITE);
	TCHAR str[] = _T("退出");
	settextstyle(len, len / 2, _T("楷体"));
	setbkmode(OPAQUE);/////////////////////to be learned
	settextcolor(BLACK);
	outtextxy(x + len / 2, y + len / 3, str);

	y = y - len / 2 * 3;
	rectangle(x, y, x + len * 3, y + len / 2 * 3);
	floodfill(x + 1, y + 1, WHITE);
	TCHAR str1[] = _T("存档");
	outtextxy(x + len / 2, y + len / 3, str1);

	int x1 = p_xl_a - len * 6;
	int y1 = p_yu_a + len * 4;
	rectangle(x1, y1, x1 + len * 4, y1 + len * 2);
	floodfill(x1 + 1, y1 + 1, WHITE);
	TCHAR str2[] = _T("重新开始");
	outtextxy(x1, y1 + len / 2, str2);
	return;
}



/*功能：生成状态栏*/
void condition() {
	/*状态栏位置参数*/
	int x = 0, y = 0;
	x = p_xr_a + len * 2;
	y = p_yu_a + len * 2;

	setlinecolor(WHITE);
	rectangle(x, y, x + len * 2, y + len * 2);
	setfillcolor(RGB(172, 81, 24));
	setfillstyle(BS_SOLID);//固实填充
	floodfill(x + 1, y + 1, WHITE);

	return;
}



/*功能：生成标题*/
void text() {
	int x1 = 0, y1 = 0;
	x1 = p_xl_a + len/3*2-30;
	y1 = p_yu_a - 95;//待测！！！////////////////////////////

	TCHAR str[] = _T("不 围 棋");
	settextstyle(len/2*3, len/2/2*3, _T("黑体"));
	setbkcolor(RGB(172, 81, 24));
	settextcolor(BLACK);
	outtextxy(x1+len, y1, str);
}


/*提示下一位棋手*/
void who_the_next(int num) {
	int x = 0, y = 0;
	x = p_xr_a + len * 2+50;
	y = p_yu_a + len * 2+50;
	if (num % 2 == 0) {
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(x, y, 40);
	}
	else {
		setlinecolor(WHITE);
		setfillcolor(WHITE);
		fillcircle(x, y, 40);
	}
	return;
}






/*功能：生成开始菜单*/
void draw_home() {
	setbkcolor(BLACK);//设置背景色
	cleardevice();//填充背景色

	setlinestyle(PS_SOLID, 1);//外边框类型(单实线）
	setlinecolor(WHITE);//外边框颜色
	rectangle(p_xl_b, p_yu_b, p_xr_b, p_yd_b);//生成矩形（外边框）
	setfillcolor(RGB(172, 81, 24));
	setfillstyle(BS_SOLID);//固实填充
	floodfill((p_xl_a + p_xr_a) / 2, (p_yu_a + p_yd_a) / 2, WHITE);//搞清含义！！！/////////////////////

	text();
	
	int x2 = 0, y2 = 0;
	x2 = hori_pixel / 2 - len *3;
	y2 = verti_pixel / 2 - len * 4;

	setlinecolor(WHITE);
	rectangle(x2, y2, x2 + len *6, y2 + len * 8);
	setfillcolor(RGB(172, 81, 24));
	setfillstyle(BS_SOLID);
	floodfill(x2 + 1, y2 + 1, WHITE);
	for (int i = y2 + len * 2; i < y2 + len * 8; i += len * 2) {
		line(x2, i, x2 + len * 6, i);
	}

	TCHAR str[] = _T("人机对战");
	settextstyle(len, len/3*2, _T("楷体"));
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	outtextxy(x2+20, y2 + len / 2, str);

	TCHAR str1[] = _T("玩家对战");
	settextstyle(len, len/ 3 * 2, _T("楷体"));
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	outtextxy(x2+20, y2 + len / 2 * 5, str1);

	TCHAR str2[] = _T("读取存档");
	settextstyle(len, len/ 3 * 2, _T("楷体"));
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	outtextxy(x2+20, y2 + len / 2 * 9, str2);

	TCHAR str3[] = _T("退出游戏");
	settextstyle(len, len/ 3 * 2, _T("楷体"));
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	outtextxy(x2+20, y2 + len / 2 * 13, str3);

}



/*计算开始菜单时鼠标所在区域状态*/
int  count_xyk_by_message1(MOUSEMSG* m, int* x, int* y)
{
	int x1, y1;
    x1 = m->x;
	y1 = m->y;

	if ((x1 >= p_xl_a +len*2 && x1 <= p_xr_a - len * 2)
		&& (y1 >= p_yu_a && y1 <= p_yu_a + len * 2))
		return (PVC);
    if ((x1 >= p_xl_a + len * 2 && x1 <= p_xr_a - len * 2)
		&& (y1 > p_yu_a+len*2 && y1 <= p_yu_a + len * 4))
		return (PVP);
    if ((x1 >= p_xl_a + len * 2 && x1 <= p_xr_a - len * 2)
		&& (y1 > p_yu_a+len*4 && y1 <= p_yu_a + len * 6))
		return (READ);
	if ((x1 >= p_xl_a + len * 2 && x1 <= p_xr_a - len * 2)
		&& (y1 > p_yu_a + len * 6 && y1 <= p_yu_a + len * 8))
		return (GAME_OVER);


	return (MOUSE_POS_ELSE);
}


/*计算游戏中鼠标动作*/
int count_xyk_by_message2(MOUSEMSG* m, int* x, int* y, int* a, int* b) {
	int x1, y1;

	x1 = m->x;
	y1 = m->y;

	if ((x1 >= p_xl_c && x1 <= p_xr_c)
		&& (y1 >= p_yu_c && y1 <= p_yd_c)){
		*x = p_xl_a + (m->x - p_xl_c) / len * len;
		*y = p_yu_a + (m->y - p_yu_c) / len * len;

		*b = (*x - p_xl_c) / len;
		*a = (*y - p_yu_c) / len;

		return (MOUSE_POS_AREA);
	}
	else if ((x1 >= p_xr_a + len * 2 && x1 <= p_xr_a + len * 5)
		&& (y1 >= p_yd_a - len / 2 * 3 && y1 <= p_yd_a)) {
		return (EXIT);
	}
	else if ((x1 >= p_xr_a + len * 2 && x1 <= p_xr_a + len * 5)
		&& (y1 >= p_yd_a - len * 3 && y1 <= p_yd_a+len/2*3)) {
		return (SAVE);
	}
    else if ((x1 >= p_xl_a - len * 6 && x1 <= p_xl_a - len * 2)
		&& (y1 >= p_yu_a + len * 4 && y1 <= p_yu_a + len * 6)) {
		return (PLAY_AGAIN);
	}
	return (MOUSE_POS_ELSE);
}


/*计算游戏后鼠标动作*/
int count_xyk_by_message3(MOUSEMSG* m, int* x, int* y, int* a, int* b) {
	int x1, y1;

	x1 = m->x;
	y1 = m->y;

	if ((x1 >= p_xr_a + len * 2 && x1 <= p_xr_a + len * 5)
	&& (y1 >= p_yd_a + len / 2 * 3 && y1 <= p_yd_a)) {
		return (EXIT);
	}
	else if ((x1 >= p_xr_a + len * 2 && x1 <= p_xr_a + len * 5)
	&& (y1 >= p_yd_a + len * 3 && y1 <= p_yd_a + len / 2 * 3)) {
		return (SAVE);
	}
	else if ((x1 >= p_xl_a - len * 6 && x1 <= p_xl_a - len * 2)
		&& (y1 >= p_yu_a + len * 4 && y1 <= p_yu_a + len * 6)) {
		return (PLAY_AGAIN);
	}
	return (MOUSE_POS_ELSE);
}

/*落子*/
void draw_chessman(int x, int y, int num, int a, int b, int chess[][9]) {
	if (num % 2 == 0){
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(x, y, 22);
		chess[a][b] = NUM_BLACK;
	}
	else {
		setlinecolor(WHITE);
		setfillcolor(WHITE);
		fillcircle(x, y, 22);
		chess[a][b] = NUM_WHITE;
	}
	return;
}


/*存档棋子位置*/
void save1(int chess[][9]) {
	ofstream fout("history1.txt");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			fout << chess[i][j] << endl;
		}
	}
	fout.close();
	return;
}


/*存档步数*/
void save2(int num) {
	ofstream fout("history2.txt");
	fout << num;
	fout.close();
	return;
}

/*存档种类*/
void save3(int now_state) {
	ofstream fout("history3.txt");
	fout << now_state;
	fout.close();
	return;
}


/*读档棋子位置*/
void read1(int chess[][9]) {
	ifstream fin("history1.txt");
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			fin >> chess[i][j];
		}
	}
	fin.close();
	return;
}








/*判别开始菜单时鼠标动作*/
int read_or_not = 0;
void play_chess()
{
	int x, y;//鼠标坐标
	int pos;//鼠标状态
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		pos = count_xyk_by_message1(&m, &x, &y);

		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:/*判别鼠标左键点击区域*/
		{
			if (pos == MOUSE_POS_ELSE)  continue;//其他区域
			if (pos == PVP) pvp();//开始方框
			if (pos == PVC) pvc();
			if (pos == GAME_OVER) exit(1);//结束方框
			if (pos == READ) {
				int now_state = 0;
				read_or_not = 1;
				ifstream fin("history3.txt");
				fin >> now_state;
				fin.close();
				if (now_state == PVP) {
					pvp();
				}
				else if(now_state==PVC) {
					pvc();
				}
			}
		}
		}
	}
	return;
}



/*判断胜负*/
int n_visit[9][9] = { 0 };
int judge(int a, int b, int chess[][9],int flag) {
	n_visit[a][b] = 1;
	int da[4] = { 0,0,1,-1 };
	int db[4] = { 1,-1,0,0 };
	for (int i = 0; i < 4; i++) {
		if (a + da[i] < 0 || b + db[i] < 0 || a + da[i]>8 || b + db[i]>8) {
			flag--;
		}
		else {
			if (chess[a + da[i]][b + db[i]] != 0 && chess[a + da[i]][b + db[i]] != chess[a][b]) {
				flag--;
			}
			else if (chess[a + da[i]][b + db[i]] == chess[a][b]){
				flag--;
				if (n_visit[a + da[i]][b + db[i]] == 0) {
					flag += judge(a + da[i], b + db[i], chess, 4);
				}
			}
			else if (chess[a + da[i]][b + db[i]] == 0) {
				continue;
			}
		    n_visit[a + da[i]][b + db[i]] = 1;
		}
	}
	return flag;
}


/*pvp出现优胜者后*/
void pvp_text_and_over(int a) {
	int x = 0, y = 0;
	x = p_xl_a - len * 6;
	y = p_yu_a + len * 2;
	setlinecolor(WHITE);
	rectangle(x, y, x + len * 4, y + len * 2);
	setfillcolor(RGB(172, 81, 24));
	setfillstyle(BS_SOLID);
	floodfill(x + 1, y + 1, WHITE);
	if (a == NUM_BLACK) {
		TCHAR str[] = _T("黑方胜");
		settextstyle(len, len / 2, _T("楷体"));
		setbkmode(OPAQUE);
		settextcolor(BLACK);
		outtextxy(x + len/2, y+len/2, str);
	}
	else {
		TCHAR str[] = _T("白方胜");
		settextstyle(len, len / 2, _T("楷体"));
		setbkmode(OPAQUE);
		settextcolor(BLACK);
		outtextxy(x + len / 2, y + len / 2, str);
	}
	y = y + len * 2;
	return;
	
}

/*随机算法*//////////////stack overflow????????????
void computer(int chess[][9], int num) {
	srand((unsigned)time(NULL));
	int a = 0;
	int b = 0;
	for (int i = 0; i < 81; i++) {
		a = rand() % 9;
		b = rand() % 9;
		if (chess[a][b] != 0) {
			continue;
		}
		else {
			if (num % 2 == 0) {
				chess[a][b] = NUM_BLACK;
			}
			else {
				chess[a][b] = NUM_WHITE;
			}
			if (judge(a, b, chess, 4) != 0) {
				int x = b * len + p_xl_c + 25;
				int y = a * len + p_yu_c + 25;
				draw_chessman(x, y, num, a, b, chess);
				return;
			}
			else {
				chess[a][b] = 0;
				continue;
			}
		}
	}
}

/*人机对战*/
void pvc() {
	draw_chessboard();
	draw_menu();

	int chess[9][9] = { 0 };
	int x = 0, y = 0;//鼠标坐标
	int pos = 0;//鼠标状态
	int x2 = 0, y2 = 0;
	int a = 0, b = 0;
	int num = 0;//当前应落子颜色
	int da[5] = { 1,-1,0,0,0 };
	int db[5] = { 0,0,1,-1,0 };
	int now_state = PVC;
	MOUSEMSG m;

	if (read_or_not == 1) {
		read1(chess);
		ifstream fin("history2.txt");
		fin >> num;
		fin.close();
		read_or_not = 0;
	}


	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (chess[i][j] == NUM_BLACK) {
				int x = j * len + p_xl_c + 25;
				int y = i * len + p_yu_c + 25;
				draw_chessman(x, y, 0, i, j, chess);
			}
			else if (chess[i][j] == NUM_WHITE) {
				int x = j * len + p_xl_c + 25;
				int y = i * len + p_yu_c + 25;
				draw_chessman(x, y, 1, i, j, chess);
			}
		}
	}
	/*选择先后手
	if (num == 0) {

	}
	*/

	while (now_state == PVC) {
		if (num % 2 == 0) {
			who_the_next(num);
			m = GetMouseMsg();
			pos = count_xyk_by_message2(&m, &x, &y, &a, &b);

			switch (m.uMsg) {
			case WM_LBUTTONDOWN: {
				if (pos == MOUSE_POS_AREA) {
					if (chess[a][b] == NUM_BLACK || chess[a][b] == NUM_WHITE) {
						continue;
					}
					else {
						int flag = 0;
						draw_chessman(x, y, num, a, b, chess);
						num++;
						x2 = x;
						y2 = y;
						for (int i = 0; i < 5; i++) {
							if (a + da[i] < 0 || a + da[i]>8 || b + db[i] < 0 || b + db[i]>8) {
								continue;
							}
							if (chess[a + da[i]][b + db[i]] == 0) {
								continue;
							}
							else {
								for (int i = 0; i < 9; i++) {
									for (int j = 0; j < 9; j++) {
										n_visit[i][j] = 0;
									}
								}
								if (judge(a + da[i], b + db[i], chess, 4) != 0) {
									continue;
								}
								else {
									pvp_text_and_over(chess[a + da[i]][b + db[i]]);
									break;
								}
							}
						}
					}
				}
				if (pos == SAVE) {
					save1(chess);
					save2(num);
					save3(now_state);
				}
				if (pos == EXIT) {
					draw_home();
					return;
				}
				if (pos == PLAY_AGAIN) {
					pvc();
					return;
				}
			}
			break;
			}
		}
		if (num % 2 == 1) {
			who_the_next(num);
			m = GetMouseMsg();
			pos = count_xyk_by_message2(&m, &x, &y, &a, &b);
			computer(chess, num);
			num++;
			switch (m.uMsg) {
			case WM_LBUTTONDOWN: {
				
				if (pos == SAVE) {
					save1(chess);
					save2(num);
					save3(now_state);
				}
				if (pos == EXIT) {
					draw_home();
					return;
				}
				if (pos == PLAY_AGAIN) {
					pvp();
					return;
				}
			}
			break;
			}
			
        }

	}
	return;
}






/*玩家对战*/
void pvp() {
	draw_chessboard();
	draw_menu();

	int chess[9][9] = { 0 };
	int x = 0, y = 0;//鼠标坐标
	int pos = 0;//鼠标状态
	int x2 = 0, y2 = 0;
	int a = 0, b = 0;
	int num = 0;//当前应落子颜色
	int da[5] = { 1,-1,0,0,0 };
	int db[5] = { 0,0,1,-1,0 };
	int now_state = PVP;
	MOUSEMSG m;

	if (read_or_not == 1) {
		read1(chess);
		ifstream fin("history2.txt");
		fin >> num;
		fin.close();
	    read_or_not = 0;
	}


	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (chess[i][j] == NUM_BLACK) {
				int x = j * len + p_xl_c + 25;
				int y = i * len + p_yu_c + 25;
				draw_chessman(x, y, 0, i, j, chess);
			}
			else if (chess[i][j] == NUM_WHITE) {
				int x = j * len + p_xl_c + 25;
				int y = i * len + p_yu_c + 25;
				draw_chessman(x, y, 1, i, j, chess);
			}
		}
	}

	

	while (now_state==PVP) {
		who_the_next(num);
		m = GetMouseMsg();
		pos = count_xyk_by_message2(&m, &x, &y, &a, &b);

		switch (m.uMsg) {
		case WM_LBUTTONDOWN: {
			if (pos == MOUSE_POS_AREA) {
				if (chess[a][b] == NUM_BLACK || chess[a][b] == NUM_WHITE) {
					continue;
				}
				else {
					int flag = 0;
					draw_chessman(x, y, num, a, b, chess);
					num++;
					x2 = x;
					y2 = y;
					for (int i = 0; i < 5; i++) {
						if (a + da[i] < 0 || a + da[i]>8 || b + db[i] < 0 || b + db[i]>8) {
							continue;
						}
						if (chess[a + da[i]][b + db[i]] == 0) {
							continue;
						}
						else {
							for (int i = 0; i < 9; i++) {
								for (int j = 0; j < 9; j++) {
									n_visit[i][j] = 0;
								}
							}
							if (judge(a + da[i], b + db[i], chess, 4) != 0) {
								continue;
							}
							else {
								pvp_text_and_over(chess[a + da[i]][b + db[i]]);
								break;
							}
						}
					}
				}
			}
			if (pos == SAVE) {
				save1(chess);
				save2(num);
				save3(now_state);
			}
			if (pos == EXIT) {
				draw_home();
				return;
			}
			if (pos == PLAY_AGAIN) {
				pvp();
				return;
			}
		}
		break;
		}

		
	}
	return;
}




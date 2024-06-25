#include <graphics.h>
#include <iostream>
char board[3][3];
void InitBoard() {
	memset(board, 0, sizeof(board));
}
enum GameState {
	RUNNING = 0, WIN, LOSE, DRAW
};
GameState JudgeGameState() {
#define judge(x) if((x) == 3) return WIN;\
				 if((x) == -3) return LOSE;
	int sumr[3] = { 0 }, sumc[3] = { 0 }, num = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sumr[i] += board[i][j];
			sumc[i] += board[j][i];
			num += (board[i][j] != 0);
		}
	}
	for (int i = 0; i < 3; i++) {
		judge(sumr[i]);
		judge(sumc[i]);
	}
	judge(board[0][0] + board[1][1] + board[2][2]);	// diagonal line 1
	judge(board[0][2] + board[1][1] + board[2][0]);	// diagonal line 2
	if (num == 9)	return DRAW;
	return RUNNING;
#undef judge
}

void DrawBoard(int lx = 0, int uy = 0, int rx = 600, int by = 600) {
	line(lx, uy + (by - uy) / 3, rx, uy + (by - uy) / 3);
	line(lx, by - (by - uy) / 3, rx, by - (by - uy) / 3);
	line(lx + (rx - lx) / 3, uy, lx + (rx - lx) / 3, by);
	line(rx - (rx - lx) / 3, uy, rx - (rx - lx) / 3, by);
}
void DrawPiece() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 1) {
				circle(200 * j + 100, 200 * i + 100, 80);
			} else if (board[i][j] == -1) {
				line(200 * j + 20, 200 * i + 20, 200 * (j + 1) - 20, 200 * (i + 1) - 20);
				line(200 * (j + 1) - 20, 200 * i + 20, 200 * j + 20, 200 * (i + 1) - 20);
			}
		}
	}
}
TCHAR end_msg[4][11];
void InitEndMessage() {
	wcscpy_s(end_msg[WIN], _T("游戏胜利"));
	wcscpy_s(end_msg[LOSE], _T("游戏失败"));
	wcscpy_s(end_msg[DRAW], _T("平局"));
}
int main() {
	initgraph(600, 600);
	InitBoard();
	InitEndMessage();
	BeginBatchDraw();
	ExMessage msg;
	bool player_action = false;
	while (true) {
		// 读取操作
		while (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN) {
				int x = msg.x;
				int y = msg.y;
				int index_x = y / 200;
				int index_y = x / 200;
				if (board[index_x][index_y] == 0) {
					board[index_x][index_y] = 1;
					player_action = true;
					break;
				}
			}
		}
		// 绘制画面
		cleardevice();
		DrawBoard();
		DrawPiece();
		FlushBatchDraw();
		// 处理操作
		if (player_action) {
			player_action = false;
			GameState state = JudgeGameState();
			if (state != RUNNING) {
				MessageBox(GetHWnd(), end_msg[state], _T("游戏结束"), MB_OK);
				InitBoard();
				continue;
			}
			int x = rand() % 3;
			int y = rand() % 3;
			while (board[x][y] != 0) {
				x = rand() % 3;
				y = rand() % 3;
			}
			board[x][y] = -1;
			cleardevice();
			DrawBoard();
			DrawPiece();
			FlushBatchDraw();
			state = JudgeGameState();
			if (state != RUNNING) {
				MessageBox(GetHWnd(), end_msg[state], _T("游戏结束"), MB_OK);
				InitBoard();
				continue;
			}
		}
		Sleep(10);
	}
	EndBatchDraw();
	return 0;
}
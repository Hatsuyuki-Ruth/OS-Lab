#include <include/string.h>
#include <include/stdio.h>
#include "inc.h"

char get_key();
void update_cursor(int row, int col);
bool check_new_key();

static int seek = 0;
char ans[MAX_LEN];
static int len = 0;

char look_up[256] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
'\n', BACK_SPACE,
'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

void scroll() {
	// printk("%d\n", seek);
	seek -= COL * 2;
	for (int i = 1; i < ROW - 1; i++) { // First line reserved
		for (int j = 0; j < COL; j++) {
			*((char *)STRING_START + (i * COL + j) * 2) = *((char *)STRING_START + ((i + 1) * COL + j) * 2);
			*((char *)STRING_START + (i * COL + j) * 2 + 1) = *((char *)STRING_START + ((i + 1) * COL + j) * 2 + 1);
		}
	}
	for (int j = 0; j < COL; j++) {
		*((char *)STRING_START + ((ROW - 1) * COL + j) * 2) = 0;
		*((char *)STRING_START + ((ROW - 1) * COL + j) * 2 + 1) = 0;
	}
	// printk("%d\n", seek);
}

void print_char(int color, char c) {
	if (c != '\n') {
		*((char *)STRING_START + seek) = c;
		seek++;
		*((char *)STRING_START + seek) = color;
		seek++;
		if (seek >= COL * ROW * 2) scroll();
	}
	else {
		seek = seek / (2 * COL) * (2 * COL) + 2 * COL;
		if (seek >= COL * ROW * 2) scroll();
	}
}

void back_space() {
	seek--;
	*((char *)STRING_START + seek) = 0;
	seek--;
	*((char *)STRING_START + seek) = 0;
	update_cursor(seek / COL / 2, seek % (COL * 2) / 2);
}

void print_string(int color, char *st) {
	// printk("%s\n", st);
	char c = *st;
	while (c) {
		print_char(color, c);
		st++;
		c = *st;
	}
	update_cursor(seek / COL / 2, seek % (COL * 2) / 2);
}

void process_ans() {
	for (int i = 0; i < len; i++) {
		if ('A' <= ans[i] && ans[i] <= 'Z') ans[i] += 'a' - 'A';
	}
	if (strcmp(ans, "two") == 0) {
		ans[0] = '2';
		ans[1] = 0;
	}
	else if (strcmp(ans, "seven") == 0) {
		ans[0] = '7';
		ans[1] = 0;
	}
}

void wait_input() {
	char c;
	len = 0;
	print_string(BLACK_AND_WHITE, "> ");
	while (1) {
		while (!check_new_key())
			;
		c = get_key();
		char c_translated = look_up[(int)c];
		if (c_translated == BACK_SPACE) {
			if (len) {
				len--;
				back_space();
			}
		}
		else if (len < MAX_LEN - 1) {
			ans[len] = c_translated;
			len++;
			print_char(BLACK_AND_WHITE, c_translated);
		}
		update_cursor(seek / COL / 2, seek % (COL * 2) / 2);
		if (c_translated == '\n') break;
	}
	// printk("%d\n", len);
	len--;
	ans[len] = 0;
	process_ans();
	// printk("%s\n", ans);
}

void put_key() {
	char c = get_key();
	c++;
}

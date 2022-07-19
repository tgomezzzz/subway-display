/*****
 * 
 * Class that organizes and displays each train page.
 * 
*****/

#include "page.h"

bool symbolPixelIsSet(char train, int i, int j) {
	switch (train) {
	case 'A':
		return ICON_A[i][j];
	case 'C':
		return ICON_C[i][j];
	case 'E':
		return ICON_E[i][j];
	case '1':
		return ICON_1[i][j];
	case 'Q':
		return ICON_Q[i][j];
	case 'N':
		return ICON_N[i][j];
	case 'R':
		return ICON_R[i][j];
	}
	return false;
}

bool numPixelIsSet(int num, int i, int j) {
	return num > -1 && num < 10 ? NUMS[num][i][j] : false;
}

bool letterPixelIsSet(char letter, int i, int j) {
	switch (letter) {
	case 's':
		return S[i][j];
	case 't':
		return T[i][j];
	case 'r':
		return R[i][j];
	case 'e':
		return E[i][j];
	case 'a':
		return A[i][j];
	case 'v':
		return V[i][j];
	case 'm':
		return M[i][j];
	case 'i':
		return I[i][j];
	case 'n':
		return N[i][j];
	case ',':
		return COMMA[i][j];
	}
	return false;
}

int letterWidth(char letter) {
	switch (letter) {
	case 's':
		return S_WIDTH;
	case 't':
		return T_WIDTH;
	case 'r':
		return R_WIDTH;
	case 'e':
		return E_WIDTH;
	case 'a':
		return A_WIDTH;
	case 'v':
		return V_WIDTH;
	case 'm':
		return M_WIDTH;
	case 'i':
		return I_WIDTH;
	case 'n':
		return N_WIDTH;
	case ',':
		return COMMA_WIDTH;
	}
	return 2;
}

Page::Page(char _train, const char* _stop_id, const char* _station, int _group, MatrixPanel_I2S_DMA* _m, uint16_t _color, int _nresults) :
		train(_train), stop_id(_stop_id), station(_station), group(_group), m(_m), color(_color), nresults(_nresults), n_uptown(0), n_downtown(0) {
			uptown = new char*[nresults];
			downtown = new char*[nresults];
			for (int i = 0; i < nresults; i++) {
				uptown[i] = new char[3];
				downtown[i] = new char[3];
			}
			Serial.println("created new pages");
}

Page::~Page() {
	for (int i = 0; i < nresults; i++) {
		delete uptown[i];
		delete downtown[i];
	}
	delete[] uptown;
	delete[] downtown;
}

void Page::set_uptown(int i, const char* value) {
	if (i < nresults) {
		strcpy(uptown[i], value);
		uptown[i][strlen(value)] = 0;
		n_uptown = i + 1;
	}
}

void Page::set_downtown(int i, const char* value) {
	if (i < nresults) {
		strcpy(downtown[i], value);
		downtown[i][strlen(value)] = 0;
		n_downtown = i + 1;
	}
}

void Page::display(int prev_group) {
	m->fillRect(8, 11, m->width(), 21, 0x1082);
	draw_icon(12, 0, 3);
	if (group != prev_group) {
		m->fillRect(25, 1, m->width() - 13, 9, 0xFFFF);
		draw_text(station, 25, 1, 0x0000, 3);
	}
	delay(100);
	draw_arrow(UP, 0, 12, 0xFFFF, 0);
	draw_arrow(DOWN, 0, 22, 0xFFFF, 0);
	int cursor = 10;
	for (int i = 0; i < n_uptown; i++) {
		cursor += draw_text(uptown[i], cursor, 12, 0xFFFF, 1);
		if (i < n_uptown - 1) {
			cursor += draw_text(", ", cursor, 12, 0xFFFF, 1);
		}
	}
	cursor += draw_text(" min", cursor, 12, 0xFFFF, 1);

	cursor = 10;
	for (int i = 0; i < n_downtown; i++) {
		cursor += draw_text(downtown[i], cursor, 22, 0xFFFF, 1);
		if (i < n_uptown - 1) {
			cursor += draw_text(", ", cursor, 22, 0xFFFF, 1);
		}
	}
	cursor += draw_text(" min", cursor, 22, 0xFFFF, 1);

}

void Page::draw_icon(int x, int y, int d) {
	for (int i = 0; i < ICON_SIZE; i++) {
		for (int j = 0; j < ICON_SIZE; j++) {
			if (ICON_BACKGROUND[i][j]) {
				delay(d);
				m->drawPixel(x + j, y + i, color);
			}
		}
	}
	for (int i = 0; i < ICON_LETTER_HEIGHT; i++) {
		for (int j = 0; j < ICON_LETTER_WIDTH; j++) {
			if (symbolPixelIsSet(train, i, j)) {
				delay(d * 5);
				m->drawPixel(x + 3 + j, y + 2 + i, train == 'Q' ||
												   train == 'N' ||
												   train == 'R' ? 0x000 : 0xFFFF);
			}
		}
	}
}

int Page::draw_text(const char* text, int x, int y, uint16_t color, int d) {
	int cursor = x;
	for (int i = 0; i < strlen(text); i++) {
		char c = text[i];
		if (c >= '0' && c <= '9') {
			cursor += draw_num(c - '0', cursor, y, color, d) + 1;
		} else {
			cursor += draw_letter(c, cursor, y + 1, color, d) + 1;
		}
	}
	return cursor - x;
}

int Page::draw_num(int num, int x, int y, uint16_t color, int d) {
	for (int i = 0; i < NUM_HEIGHT; i++) {
		for (int j = 0; j < NUM_WIDTH; j++) {
			if (numPixelIsSet(num, i, j)) {
				delay(d);
				m->drawPixel(x + j, y + i, color);
			}
		}
	}
	return NUM_WIDTH;
}

int Page::draw_letter(char letter, int x, int y, uint16_t color, int d) {
	int width = letterWidth(letter);
	for (int i = 0; i < LETTER_HEIGHT; i++) {
		for (int j = 0; j < width; j++) {
			if (letterPixelIsSet(letter, i, j)) {
				delay(d);
				m->drawPixel(x + j, y + i, color);
			}
		}
	}
	return letter == ',' ? 1 : width;
}

void Page::draw_arrow(arrow_dir dir, int x, int y, uint16_t color, int d) {
	for (int i = 0; i < ARROW_WIDTH; i++) {
		for (int j = 0; j < ARROW_WIDTH; j++) {
			if (ARROW[i][j]) {
				delay(d);
				switch (dir) {
				case UP:
					m->drawPixel(x + j, y + i, color);
					break;
				case DOWN:
					m->drawPixel(x + j, y + ((ARROW_WIDTH - 1) - i), color);
					break;
				case LEFT:
					m->drawPixel(x + i, y + j, color);
					break;
				case RIGHT:
					m->drawPixel(x + ((ARROW_WIDTH - 1) - i), y + j, color);
					break;
				}
			}
		}
	}
}

char Page::get_train() {
	return train;
}

const char* Page::get_station() {
	return station;
}

const char* Page::get_stop_id() {
	return stop_id;
}

int Page::get_group() {
	return group;
}

int Page::get_nresults() {
	return nresults;
}
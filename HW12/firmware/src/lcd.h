#ifndef LCD_H__
#define LCD_H__

#define fc ILI9341_WHITE
#define bkc ILI9341_BLACK
#define bc ILI9341_RED

void LCD_print_letter(char letter, int x, int y);
void LCD_print_string(char *m, int x, int y);
void LCD_print_bar(int x, int y);
void LCD_bar_progress(int x, int y, int i);
void LCD_xbar_progress(short k);
void LCD_ybar_progress(short k);

#endif

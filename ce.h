#ifndef CE_H
#define CE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ncurses.h>
#include <errno.h>

#define CE_CONFIG "ce_config.so"

#define NEWLINE 10
#define COLOR_BACKGROUND -1
#define COLOR_FOREGROUND -1
#define COLOR_BRIGHT_BLACK 8
#define COLOR_BRIGHT_RED 9
#define COLOR_BRIGHT_GREEN 10
#define COLOR_BRIGHT_YELLOW 11
#define COLOR_BRIGHT_BLUE 12
#define COLOR_BRIGHT_MAGENTA 13
#define COLOR_BRIGHT_CYAN 14
#define COLOR_BRIGHT_WHITE 15

#define CE_CHECK_PTR_ARG(arg)                                                \
     if(!arg){                                                            \
          ce_message("%s() received NULL argument %s\n", __FUNCTION__, #arg); \
          return false;                                                   \
     }

typedef struct {
     int64_t x;
     int64_t y;
} Point;

typedef struct {
     char** lines; // NULL terminated, does not contain newlines
     int64_t line_count;
     char* filename;
} Buffer;

typedef bool ce_initializer(Buffer*, Point*);
typedef bool ce_key_handler(int, Buffer*, Point*);
typedef void ce_view_drawer(const Buffer*, const Point*);

extern Buffer* g_message_buffer;
extern Point* g_terminal_dimensions;

// TODO: strdup() isn't in my string.h c standard libary ?
char* ce_alloc_string(const char* string);
bool ce_alloc_lines(Buffer* buffer, int64_t line_count);
bool ce_load_file(Buffer* buffer, const char* filename);
void ce_free_buffer(Buffer* buffer);
bool ce_point_on_buffer(const Buffer* buffer, const Point* location);
bool ce_insert_char(Buffer* buffer, const Point* location, char c);
bool ce_insert_string(Buffer* buffer, const Point* location, const char* string);
bool ce_remove_char(Buffer* buffer, const Point* location);

// NOTE: passing NULL to string causes an empty line to be inserted
bool ce_insert_line(Buffer* buffer, int64_t line, const char* string);
bool ce_append_line(Buffer* buffer, const char* string);
bool ce_insert_newline(Buffer* buffer, int64_t line);
bool ce_remove_line(Buffer* buffer, int64_t line);

// NOTE: unused/untested
bool ce_set_line(Buffer* buffer, int64_t line, const char* string);
bool ce_save_buffer(const Buffer* buffer, const char* filename);
bool ce_message(const char* format, ...);

bool ce_draw_buffer(const Buffer* buffer, const Point* term_top_left, const Point* term_bottom_right,
                    const Point* buffer_top_left);

#endif
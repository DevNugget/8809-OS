// tiny_editor_with_gutter.c
#include "syscall.h"
#include "vga.h"
#include <stdint.h>
#include <stddef.h>

#define MAX_BUFFER (128 * 1024)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 28

// Control codes
#define CTRL_Q 0x11
#define CTRL_S 0x13
#define KEY_UP 0x81
#define KEY_DOWN 0x82
#define KEY_LEFT 0x83
#define KEY_RIGHT 0x84

#define GUTTER_WIDTH 4  // 2 digits + 2 spaces

#define COL_FG current_theme.light_gray
#define COL_BG current_theme.black
#define COL_CURSOR_FG current_theme.black
#define COL_CURSOR_BG current_theme.white
#define COL_STATUS_FG current_theme.black
#define COL_STATUS_BG current_theme.light_green
#define COL_GUTTER current_theme.light_gray

static char buffer[MAX_BUFFER];
static size_t buffer_len = 0;
static int modified = 0;

static int cursor_pos = 0;
static int scroll_line = 0;
static int last_cursor_pos = -1;
static int last_scroll_line = -1;

theme_t current_theme;

// Helpers
static void memmove_internal(char *dst, const char *src, size_t n) {
    if (dst < src) for (size_t i=0;i<n;i++) dst[i]=src[i];
    else if (dst>src) for (size_t i=n;i-->0;) dst[i]=src[i];
}

static int compute_cursor_line_col(int pos, int *out_col) {
    int line = 0, col = 0, i = 0;
    while(i < pos && i < (int)buffer_len) {
        if(buffer[i]=='\n'){ line++; col=0; }
        else col++;
        i++;
    }
    if(out_col) *out_col = col;
    return line;
}

static int find_line_start(int target_line) {
    int line=0, i=0;
    if(target_line<=0) return 0;
    while(i < (int)buffer_len){
        if(buffer[i]=='\n'){
            line++;
            if(line==target_line) return i+1;
        }
        i++;
    }
    return buffer_len;
}

static int find_line_end_from_start(int start_idx){
    int i=start_idx;
    while(i<(int)buffer_len && buffer[i]!='\n') i++;
    return i;
}

// insert/backspace/delete
static void insert_at_cursor(char c){
    if(buffer_len+1>=MAX_BUFFER) return;
    memmove_internal(buffer+cursor_pos+1, buffer+cursor_pos, buffer_len-cursor_pos);
    buffer[cursor_pos++] = c;
    buffer_len++;
    modified=1;
}

static void backspace_at_cursor(){
    if(cursor_pos==0) return;
    memmove_internal(buffer+cursor_pos-1, buffer+cursor_pos, buffer_len-cursor_pos);
    cursor_pos--;
    buffer_len--;
    modified=1;
}

static void delete_at_cursor(){
    if(cursor_pos>=(int)buffer_len) return;
    memmove_internal(buffer+cursor_pos, buffer+cursor_pos+1, buffer_len-cursor_pos-1);
    buffer_len--;
    modified=1;
}

// cursor movement
static void move_cursor_left(){ if(cursor_pos>0) cursor_pos--; }
static void move_cursor_right(){ if(cursor_pos<(int)buffer_len) cursor_pos++; }
static void move_cursor_up(){ 
    int col; 
    int line = compute_cursor_line_col(cursor_pos,&col);
    if(line==0){ cursor_pos=0; return; }
    int cur_start=find_line_start(line);
    int prev_start=find_line_start(line-1);
    int prev_end=find_line_end_from_start(prev_start);
    int len=prev_end-prev_start;
    int target_col=col<=len?col:len;
    cursor_pos=prev_start+target_col;
}
static void move_cursor_down(){ 
    int col;
    int line=compute_cursor_line_col(cursor_pos,&col);
    int next_start=find_line_start(line+1);
    if(next_start>= (int)buffer_len){ cursor_pos=buffer_len; return; }
    int next_end=find_line_end_from_start(next_start);
    int len=next_end-next_start;
    int target_col=col<=len?col:len;
    cursor_pos=next_start+target_col;
}

static void ensure_cursor_visible(){
    int col;
    int line = compute_cursor_line_col(cursor_pos,&col);
    int max_lines = SCREEN_HEIGHT-1;
    if(line < scroll_line) scroll_line=line;
    else if(line >= scroll_line+max_lines) scroll_line=line-(max_lines-1);
    if(scroll_line<0) scroll_line=0;
}

// --- Partial redraw functions ---

static void redraw_line(int line_no){
    int start = find_line_start(line_no);
    int end   = find_line_end_from_start(start);
    int cur_col;
    int cur_line = compute_cursor_line_col(cursor_pos, &cur_col);

    // Move cursor to the correct *screen* row (row 0 is the top visible line)
    syscall_movecursor(line_no - scroll_line, 0);

    // Clear the whole line first
    syscall_clear_line_from(0);

    // Draw gutter
    syscall_termcolor(COL_GUTTER, COL_BG);
    // Print line number (1-based)
    int display_line = line_no + 1;
    if (display_line < 10) syscall_write("  ");  // 2 spaces for single-digit
    else if (display_line < 100) syscall_write(" "); // 1 space for double-digit
    syscall_writeint(display_line);

    // Separator
    syscall_write(" |");

    syscall_termcolor(COL_FG, COL_BG);

    // Draw line content
    for (int i = start; i < end; i++) {
        if (i == cursor_pos) {
            syscall_termcolor(COL_CURSOR_FG, COL_CURSOR_BG);
            char tmp[2] = { buffer[i], 0 };
            syscall_write(tmp);
            syscall_termcolor(COL_FG, COL_BG);
        } else {
            char tmp[2] = { buffer[i], 0 };
            syscall_write(tmp);
        }
    }

    // If cursor is at end of line, show visible placeholder (inverted space)
    if (cursor_pos == end) {
        syscall_termcolor(COL_CURSOR_FG, COL_CURSOR_BG);
        syscall_write(" ");
        syscall_termcolor(COL_FG, COL_BG);
    }
}

static void redraw_status(const char* filename){
    syscall_movecursor(SCREEN_HEIGHT-1,0);
    syscall_termcolor(COL_STATUS_FG,COL_STATUS_BG);
    syscall_write(" ");
    syscall_write(filename);
    syscall_write(" ");
    if(modified) syscall_write("[+]");
    else syscall_write("[ ]");
    int col; int line=compute_cursor_line_col(cursor_pos,&col);
    syscall_write(" Ln:"); syscall_writeint(line+1);
    syscall_write(" Col:"); syscall_writeint(col+1);
    syscall_write("\n");
    syscall_termcolor(COL_FG,COL_BG);
}

static void redraw_screen_partial(const char* filename){
    if (last_cursor_pos < 0 || last_scroll_line < 0) {
        for (int i = 0; i < SCREEN_HEIGHT - 1; i++) {
            redraw_line(scroll_line + i);
        }
        redraw_status(filename);
        last_cursor_pos = cursor_pos;
        last_scroll_line = scroll_line;
        return;
    }

    int cur_line, cur_col;
    compute_cursor_line_col(cursor_pos, &cur_col);
    cur_line = compute_cursor_line_col(cursor_pos, NULL);

    if (scroll_line != last_scroll_line) {
        for (int i = 0; i < SCREEN_HEIGHT - 1; i++) redraw_line(scroll_line + i);
    } else if (cur_line != compute_cursor_line_col(last_cursor_pos, NULL)) {
        int old_line = compute_cursor_line_col(last_cursor_pos, NULL);
        redraw_line(old_line);
        redraw_line(cur_line);
    } else {
        redraw_line(cur_line);
    }

    redraw_status(filename);
    last_cursor_pos = cursor_pos;
    last_scroll_line = scroll_line;
}

// Load/save
static void save_buffer_to_file(const char* filename){
    int res = syscall_writefile(filename,buffer,buffer_len);
    if(res!=0) syscall_write("Failed to save file\n");
    else { modified=0; syscall_write(" File saved\n"); }
}

static void load_file_into_buffer(const char* filename){
    int read_size = read_file(filename,buffer,MAX_BUFFER);
    if(read_size>0){
        buffer_len = (size_t)read_size;
        if(buffer_len>=MAX_BUFFER) buffer_len=MAX_BUFFER-1;
        cursor_pos=0;
        modified=0;
    } else {
        buffer_len=0; cursor_pos=0; modified=0;
    }
}

// Editor main loop
void editor_loop(const char* filename){
    redraw_screen_partial(filename);
    while(1){
        char c = syscall_readchar();

        if((unsigned char)c==CTRL_Q) break;
        if((unsigned char)c==CTRL_S){
            save_buffer_to_file(filename);
            redraw_screen_partial(filename);
            continue;
        }

        switch((unsigned char)c){
            case KEY_LEFT: move_cursor_left(); break;
            case KEY_RIGHT: move_cursor_right(); break;
            case KEY_UP: move_cursor_up(); break;
            case KEY_DOWN: move_cursor_down(); break;
            case '\b': case 127: backspace_at_cursor(); break;
            case '\n': insert_at_cursor('\n'); break;
            default:
                if((unsigned char)c>=0x20) insert_at_cursor(c);
                else continue;
        }
        ensure_cursor_visible();
        redraw_screen_partial(filename);
    }
}

// Entry point
void _start(){
    char filename[256];
    syscall_write("Enter filename to edit: ");
    readline(filename,sizeof(filename));

    syscall_gettheme(&current_theme);
    syscall_termcolor(current_theme.white,current_theme.black);
    syscall_termclear();

    load_file_into_buffer(filename);

    // force first full draw
    last_cursor_pos = -1;
    last_scroll_line = -1;
    redraw_screen_partial(filename);

    editor_loop(filename);
    syscall_exit();
}

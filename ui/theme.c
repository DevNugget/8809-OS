#include "theme.h"

// Private helper function
uint32_t hex_to_uint32(const char* hex) {
    uint32_t color = 0;

    // Ensure the string starts with '#' and has 7 characters
    if (hex[0] == '#' && strlen(hex) == 7) {
        for (int i = 1; i < 7; ++i) {
            color *= 16; // Shift left by 4 bits for each new digit
            if (hex[i] >= '0' && hex[i] <= '9') {
                color += hex[i] - '0';
            } else if (hex[i] >= 'a' && hex[i] <= 'f') {
                color += hex[i] - 'a' + 10;
            } else if (hex[i] >= 'A' && hex[i] <= 'F') {
                color += hex[i] - 'A' + 10;
            }
        }
    }

    return color;
}

int load_theme(const char* path, theme_t* theme, gterminal_t* gterm, 
        framebuffer_t* fb, psf1_font_t* loaded_font) {
    FIL file;
    UINT read;
    char buffer[256];
    char line[256];
    int line_index = 0;
    console_write(path);
    console_write("\n");
    // Open the theme file
    if (f_open(&file, path, FA_READ) != FR_OK) {
        console_write("couldnt open file\n");
        // Default theme if the file cannot be opened
        theme->black = COLOR_BLACK;
        theme->red = COLOR_RED;
        theme->green = COLOR_GREEN;
        theme->yellow = COLOR_YELLOW;
        theme->blue = COLOR_BLUE;
        theme->magenta = COLOR_MAGENTA;
        theme->cyan = COLOR_CYAN;
        theme->white = COLOR_WHITE;
        theme->gray = COLOR_GRAY;

        theme->light_black = COLOR_LIGHT_BLACK;
        theme->light_red = COLOR_LIGHT_RED;
        theme->light_green = COLOR_LIGHT_GREEN;
        theme->light_yellow = COLOR_LIGHT_YELLOW;
        theme->light_blue = COLOR_LIGHT_BLUE;
        theme->light_magenta = COLOR_LIGHT_MAGENTA;
        theme->light_cyan = COLOR_LIGHT_CYAN;
        theme->light_white = COLOR_LIGHT_WHITE;
        theme->light_gray = COLOR_LIGHT_GRAY;
        return -1; // File not found
    }

    // Default values for theme colors
    theme->black = COLOR_BLACK;
    theme->red = COLOR_RED;
    theme->green = COLOR_GREEN;
    theme->yellow = COLOR_YELLOW;
    theme->blue = COLOR_BLUE;
    theme->magenta = COLOR_MAGENTA;
    theme->cyan = COLOR_CYAN;
    theme->white = COLOR_WHITE;
    theme->gray = COLOR_GRAY;

    theme->light_black = COLOR_LIGHT_BLACK;
    theme->light_red = COLOR_LIGHT_RED;
    theme->light_green = COLOR_LIGHT_GREEN;
    theme->light_yellow = COLOR_LIGHT_YELLOW;
    theme->light_blue = COLOR_LIGHT_BLUE;
    theme->light_magenta = COLOR_LIGHT_MAGENTA;
    theme->light_cyan = COLOR_LIGHT_CYAN;
    theme->light_white = COLOR_LIGHT_WHITE;
    theme->light_gray = COLOR_LIGHT_GRAY;

    // Read the file content into the buffer and process line by line
    while (f_read(&file, buffer, sizeof(buffer), &read) == FR_OK && read > 0) {
        for (int i = 0; i < read; i++) {
            // If we've encountered a newline or EOF, process the line
            if (buffer[i] == '\n' || buffer[i] == '\r') {
                if (line_index > 0) {
                    line[line_index] = '\0'; // Null-terminate the line
                    line_index = 0;

                    // Skip comments

                    // Parse each line and update the theme
                    if (strncmp(line, "black=", 6) == 0) {
                        theme->black = hex_to_uint32(line + 6);
                    } else if (strncmp(line, "red=", 4) == 0) {
                        theme->red = hex_to_uint32(line + 4);
                    } else if (strncmp(line, "green=", 6) == 0) {
                        theme->green = hex_to_uint32(line + 6);
                    } else if (strncmp(line, "yellow=", 7) == 0) {
                        theme->yellow = hex_to_uint32(line + 7);
                    } else if (strncmp(line, "blue=", 5) == 0) {
                        theme->blue = hex_to_uint32(line + 5);
                    } else if (strncmp(line, "magenta=", 8) == 0) {
                        theme->magenta = hex_to_uint32(line + 8);
                    } else if (strncmp(line, "cyan=", 5) == 0) {
                        theme->cyan = hex_to_uint32(line + 5);
                    } else if (strncmp(line, "white=", 6) == 0) {
                        theme->white = hex_to_uint32(line + 6);
                    }  else if (strncmp(line, "gray=", 5) == 0) {
                        theme->gray = hex_to_uint32(line + 5);
                    }

                    // Light variants
                    else if (strncmp(line, "light_black=", 12) == 0) {
                        theme->light_black = hex_to_uint32(line + 12);
                    } else if (strncmp(line, "light_red=", 10) == 0) {
                        theme->light_red = hex_to_uint32(line + 10);
                    } else if (strncmp(line, "light_green=", 12) == 0) {
                        theme->light_green = hex_to_uint32(line + 12);
                    } else if (strncmp(line, "light_yellow=", 13) == 0) {
                        theme->light_yellow = hex_to_uint32(line + 13);
                    } else if (strncmp(line, "light_blue=", 11) == 0) {
                        theme->light_blue = hex_to_uint32(line + 11);
                    } else if (strncmp(line, "light_magenta=", 14) == 0) {
                        theme->light_magenta = hex_to_uint32(line + 14);
                    } else if (strncmp(line, "light_cyan=", 11) == 0) {
                        theme->light_cyan = hex_to_uint32(line + 11);
                    } else if (strncmp(line, "light_white=", 12) == 0) {
                        theme->light_white = hex_to_uint32(line + 12);
                    } else if (strncmp(line, "light_gray=", 11) == 0) {
                        theme->light_gray = hex_to_uint32(line + 11);
                    }
                }
            } else {
                // Accumulate characters in the line buffer
                if (line_index < sizeof(line) - 1) {
                    line[line_index++] = buffer[i];
                }
            }
        }
    }

    // Close the file
    f_close(&file);

    return 0; // Success
}

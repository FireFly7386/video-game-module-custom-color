#include "cli.h"
#include "cli_commands.h"
#include "../frame.h"
#include <cstdint>
#include <cstring>

/*static bool str_to_int(const char* str, uint16_t* value) {
    char* end;
    long int val = strtol(str, &end, 10);
    if(val > UINT8_MAX || val < 0 || *end != '\0') {
        return false;
    }

    *value = (uint16_t)val;
    return true;
}*/

void cli_color(Cli* cli, std::string& args) {
    if(args == "red") {
        color_bg = 0xf000;
    }else if(args == "green") {
        color_bg = 0x0f00;
    }else if(args == "blue") {
        color_bg = 0x00f0;
    }else if(args == "magenta") {
        color_bg = 0xc0f0;
    }else if(args == "orange") {
        color_bg = 0xfc00;
    }else if(args == "yellow") {
        color_bg = 0xff00;
    }
}
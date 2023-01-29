// file: send_message.c
//
// LCM example program.
//
// compile with:
//  $ gcc -o send_message send_message.c -llcm
//
// On a system with pkg-config, you can also use:
//  $ gcc -o send_message send_message.c `pkg-config --cflags --libs lcm`

#include <lcm/lcm.h>
#include <stdio.h>

#include "command_t.h"
#include "response_t.h"
int main(int argc, char **argv)
{
    lcm_t *lcm = lcm_create(NULL);
    if (!lcm)
        return 1;

    api_command_t my_data = {
	.code=4,
    };

    api_command_t_publish(lcm, "API", &my_data);

    lcm_destroy(lcm);
    return 0;
}

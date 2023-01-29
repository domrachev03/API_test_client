#include <stdio.h>
#include <stdlib.h>
#include <lcm/lcm.h>
#include "request_t.h"
#include "response_t.h"
#include <unistd.h>

const char* URL = "udpm://224.0.0.0:7667?ttl=1";
const char* CHANNEL_REQ = "REQUEST";
const char* CHANNEL_RESP = "RESPONSE";

int is_server_working = 0;

static
void handle_response(const lcm_recv_buf_t *rbuf, const char *channel,
                     const response_t *msg, void *user) {
    if (msg->type == 0) {
        // Inverse boolean
        printf("SERVER OK STATUS %d\n", msg->status);
        is_server_working = (msg->status + 1) % 2;
    }
    else if (msg->type == 1) {
        printf("SERVER ERROR STATUS %d\n", msg->status);
    }
    else if (msg->type == 2) {
        printf("API ERROR STATUS %d\n", msg->status);

    }
}

void send_message(lcm_t* lcm) {
    request_t request;
    printf("Enter message type and code (for shutdown, send type 0 and code 2): ");
    scanf("%d", &request.type);
    scanf("%d", &request.code);

    request_t_publish(lcm, CHANNEL_REQ, &request);
}

void check_messages(lcm_t* lcm) {
    int lcm_fd = lcm_get_fileno(lcm);
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(lcm_fd, &fds);

    struct timeval timeout = {
            0,  // seconds
            1000   // microseconds
    };

    // wait a limited amount of time for an incoming message
    int status = select(lcm_fd + 1, &fds, 0, 0, &timeout);

    if (0 == status) {
        // no messages came
    } else if (FD_ISSET(lcm_fd, &fds)) {
        // LCM has events ready to be processed.
        lcm_handle(lcm);
    }
}

void ask_status(lcm_t* lcm) {
    request_t r_status = {
            .type = 0,
            .code = 3,
    };

    request_t_publish(lcm, CHANNEL_REQ, &r_status);
}

int main() {
    lcm_t* lcm = lcm_create(URL);

    response_t_subscribe(lcm, CHANNEL_RESP, &handle_response, NULL);

    // Turning the server on and start running
    ask_status(lcm);
    lcm_handle(lcm);

    while (is_server_working) {
        send_message(lcm);
        usleep(100);
        check_messages(lcm);
    }

    printf("Server died!\n");
    return 0;
}

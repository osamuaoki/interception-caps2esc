#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/input.h>

// clang-format off
const struct input_event
esc_up          = {.type = EV_KEY, .code = KEY_ESC,      .value = 0},
ctrl_up         = {.type = EV_KEY, .code = KEY_LEFTCTRL, .value = 0},
capslock_up     = {.type = EV_KEY, .code = KEY_CAPSLOCK, .value = 0},
esc_down        = {.type = EV_KEY, .code = KEY_ESC,      .value = 1},
ctrl_down       = {.type = EV_KEY, .code = KEY_LEFTCTRL, .value = 1},
capslock_down   = {.type = EV_KEY, .code = KEY_CAPSLOCK, .value = 1},
esc_repeat      = {.type = EV_KEY, .code = KEY_ESC,      .value = 2},
ctrl_repeat     = {.type = EV_KEY, .code = KEY_LEFTCTRL, .value = 2},
capslock_repeat = {.type = EV_KEY, .code = KEY_CAPSLOCK, .value = 2},
syn             = {.type = EV_SYN, .code = SYN_REPORT,   .value = 0};
// clang-format on

void print_usage(FILE *stream, const char *program) {
    // clang-format off
    fprintf(stream,
            "caps2esc - transforming the most useless key ever in the most useful one\n"
            "\n"
            "usage: %s [-h] [-m mode]\n"
            "\n"
            "options:\n"
            "    -h        show this message and exit\n"
            "    -m mode   0: default\n"
            "                - caps as esc/ctrl\n"
            "                - esc as caps\n"
            "              1: minimal\n"
            "                - caps as esc/ctrl\n"
            "              2: useful on 60%% layouts\n"
            "                - caps as esc/ctrl\n"
            "                - esc as grave accent\n"
            "                - grave accent as caps\n",
            program);
    // clang-format on
}

int equal(const struct input_event *first, const struct input_event *second) {
    return first->type == second->type && first->code == second->code &&
           first->value == second->value;
}

int read_event(struct input_event *event) {
    return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

void write_event(const struct input_event *event) {
    if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
        exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int opt, mode = 0;
    while ((opt = getopt(argc, argv, "hm:")) != -1) {
        switch (opt) {
            case 'h':
                return print_usage(stdout, argv[0]), EXIT_SUCCESS;
            case 'm':
                mode = optarg[0] - '0';
                continue;
        }

        return print_usage(stderr, argv[0]), EXIT_FAILURE;
    }

    int capslock_is_down = 0, esc_give_up = 0;
    struct input_event input;

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (read_event(&input)) {
        if (input.type == EV_MSC && input.code == MSC_SCAN)
            continue;

        if (input.type != EV_KEY) {
            write_event(&input);
            continue;
        }

        if (capslock_is_down) {
            if (equal(&input, &capslock_down) ||
                equal(&input, &capslock_repeat))
                continue;

            if (equal(&input, &capslock_up)) {
                capslock_is_down = 0;
                if (esc_give_up) {
                    esc_give_up = 0;
                    write_event(&ctrl_up);
                    continue;
                }
                write_event(&esc_down);
                write_event(&syn);
                usleep(20000);
                write_event(&esc_up);
                continue;
            }

            if (!esc_give_up && input.value) {
                esc_give_up = 1;
                write_event(&ctrl_down);
                write_event(&syn);
                usleep(20000);
            }
        } else if (equal(&input, &capslock_down)) {
            capslock_is_down = 1;
            continue;
        }

        switch (mode) {
            case 0:
                if (input.code == KEY_ESC)
                    input.code = KEY_CAPSLOCK;
                break;
            case 2:
                switch (input.code) {
                    case KEY_ESC:
                        input.code = KEY_GRAVE;
                        break;
                    case KEY_GRAVE:
                        input.code = KEY_CAPSLOCK;
                        break;
                }
                break;
        }

        write_event(&input);
    }
}

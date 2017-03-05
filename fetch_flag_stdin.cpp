#include "all.hpp"

#include <unistd.h>
#include <fcntl.h>

bool flags[256];

int get_input() {
    if (fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK) != -1) {
        return 0;
    } else {
        return -1;
    }
}

void fetch_flag() {
    static int input = -1;

    if (input == -1) {
        input = get_input();
    }

    if (input != -1) {
        char c;

        while (read(input, &c, 1) == 1) {
            switch (c) {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                    flags[Flag::pitch_1_2] = false;
                    flags[Flag::pitch_2_3] = false;
                    flags[Flag::pitch_3_4] = false;
                    flags[Flag::pitch_4_5] = false;
                    flags[Flag::pitch_5_6] = false;
                    flags[Flag::pitch_6_7] = false;

                    break;

                case 'q':
                case 'w':
                case 'e':
                case 'r':
                case 't':
                case 'y':
                case 'u':
                case 'i':
                case 'o':
                case 'p':
                    flags[Flag::inst_flute] = false;
                    flags[Flag::inst_clarinet] = false;
                    flags[Flag::inst_saxofony] = false;
                    flags[Flag::inst_bowed] = false;
                    flags[Flag::inst_plucked] = false;
                    flags[Flag::inst_mandolin] = false;
                    flags[Flag::inst_rhodey] = false;

                    break;

                case 'm':
                    flags[Flag::fx_over] = false;
                    flags[Flag::fx_dist] = false;
                    flags[Flag::fx_reverb] = false;
                    flags[Flag::fx_echo] = false;

                    break;
            }

            switch (c) {
                case '1': flags[Flag::pitch_1_2] = true; break;
                case '2': flags[Flag::pitch_2_3] = true; break;
                case '3': flags[Flag::pitch_3_4] = true; break;
                case '4': flags[Flag::pitch_4_5] = true; break;
                case '5': flags[Flag::pitch_5_6] = true; break;
                case '6': flags[Flag::pitch_6_7] = true; break;
                case '-':
                    for (size_t i = Flag::pitch_6_7; i > Flag::pitch_1_2; --i) {
                        if (flags[i]) {
                            flags[i - 1] = true;
                            flags[i] = false;

                            break;
                        }
                    }

                    break;
                case '=':
                    for (size_t i = Flag::pitch_1_2; i < Flag::pitch_6_7; ++i) {
                        if (flags[i]) {
                            flags[i + 1] = true;
                            flags[i] = false;

                            break;
                        }
                    }

                    break;

                case 'q': flags[Flag::inst_flute] = true; break;
                case 'w': flags[Flag::inst_clarinet] = true; break;
                case 'e': flags[Flag::inst_saxofony] = true; break;
                case 'r': flags[Flag::inst_bowed] = true; break;
                case 't': flags[Flag::inst_plucked] = true; break;
                case 'y': flags[Flag::inst_mandolin] = true; break;
                case 'u': flags[Flag::inst_rhodey] = true; break;

                case 'a': flags[Flag::pitch_round] = !flags[Flag::pitch_round]; break;
                case 's': flags[Flag::pitch_fixed] = !flags[Flag::pitch_fixed]; break;
                case 'd': flags[Flag::note_smooth] = !flags[Flag::note_smooth]; break;
                case 'f': flags[Flag::note_delay] = !flags[Flag::note_delay]; break;
                case 'g': flags[Flag::vibe_enable] = !flags[Flag::vibe_enable]; break;
                case 'h': flags[Flag::vibe_smooth] = !flags[Flag::vibe_smooth]; break;
                case 'j': flags[Flag::vibe_delay] = !flags[Flag::vibe_delay]; break;

                case 'z': flags[Flag::fx_over] = !flags[Flag::fx_over]; break;
                case 'x': flags[Flag::fx_dist] = !flags[Flag::fx_dist]; break;
                case 'c': flags[Flag::fx_reverb] = !flags[Flag::fx_reverb]; break;
                case 'v': flags[Flag::fx_echo] = !flags[Flag::fx_echo]; break;
            }

            switch (c) {
                case 'd': flags[Flag::note_delay] &= flags[Flag::note_smooth]; break;
                case 'f': flags[Flag::note_smooth] |= flags[Flag::note_delay]; break;
                case 'h': flags[Flag::vibe_delay] &= flags[Flag::vibe_smooth]; break;
                case 'j': flags[Flag::vibe_smooth] |= flags[Flag::vibe_delay]; break;
            }
        }
    }
}
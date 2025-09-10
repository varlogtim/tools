#include <cstring>
#include <wchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <bits/stdc++.h>
// #include <boost/coroutine2/all.hpp>

uint16_t BUF_SIZE = 1024;
uint8_t TICK_AFTER_NUM_BUFS = 10;

static std::map<char, uint8_t> METRIC_PREFIX = {
    {'k', 10},
    {'M', 20},
    {'G', 30},
    {'T', 40}};

void USAGE(std::string filename) {
    std::cerr << "Purpose: write number of bytes to file.\n"
        << "Usage: " << filename << " <size> <filename>\n";
}

static const char *DOT_PROGRESS[11] = {
    u8"\u2804", // ⠄
    u8"\u2802", // ⠆
    u8"\u2807", // ⠇
    u8"\u280F", // ⠏
    u8"\u281F", // ⠟
    u8"\u283F", // ⠿
    u8"\u283B", // ⠻
    u8"\u2839", // ⠹
    u8"\u2838", // ⠸
    u8"\u2830", // ⠰
    u8"\u2820", // ⠠
};


int tick_count = 0;
size_t tc_num_elems = sizeof(DOT_PROGRESS) / sizeof(DOT_PROGRESS[0]);

void tick_status() {
    printf("\b");
    if (tick_count == tc_num_elems)
        tick_count = 0;
    // printf("%lc..", DOT_PROGRESS[tick_count]);
    std::cout << DOT_PROGRESS[tick_count];
    //printf(".");
    tick_count++;
}


uint64_t parse_size(std::string size_str) {
    uint8_t power = 0;
    char ch = size_str[size_str.length() - 1];

    bool found_prefix = false;
    if (METRIC_PREFIX.find(ch) != METRIC_PREFIX.end()) {
        power = METRIC_PREFIX.at(ch);
        found_prefix = true;
    }

    long double value;
    if (found_prefix) {
        value = stold(size_str.substr(0, size_str.length() - 1));
    } else {
        value = stold(size_str);
    }

    return (uint64_t) (value * pow(2, power));
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        USAGE(argv[0]);
        exit(1);
    }

    std::ofstream file;

    std::string size_str(argv[1]);
    std::string file_path(argv[2]);

    uint64_t size = parse_size(size_str);
    std::cout << "Writing random bytes of size: " << size_str << " ("
        << size << " bytes) to file '" << file_path << "'" << std::endl;

    int fd_args = O_CREAT | O_WRONLY | O_EXCL;
    int pm_args = S_IRUSR | S_IWUSR;
    int fd = open(file_path.c_str(), fd_args, pm_args);
    if (fd > 2) {
        uint64_t rem_bytes = size;
        uint16_t num_bytes = 0;
        char buff[BUF_SIZE];

        printf(" ");
        int tick_count = 0;
        while (rem_bytes > 0) {
            num_bytes = BUF_SIZE;
            if (rem_bytes - BUF_SIZE <= 0) {
                num_bytes = rem_bytes;
            }
            for (int ii = 0; ii < num_bytes; ii++) {
                // buff[ii] = rand() % 255; // Binary data
                buff[ii] = (rand() % ('Z' - 'A') + 'A');
            }
            int bytes_written = write(fd, buff, num_bytes);
            if (bytes_written > 0) {
                rem_bytes = rem_bytes - bytes_written;
            } else {
                std::cerr << "ERROR: writing to file '" << file_path
                    << "': " << std::strerror(errno) << std::endl;
            }
            if (tick_count == TICK_AFTER_NUM_BUFS) {
                tick_status();
                tick_count = 0;
            }
            tick_count++;
        }
        printf("\b\b");
        std::cout << std::endl;
    } else {
        if (errno == EEXIST) {
            std::cerr << "ERROR: file exists '" << file_path
                << "' will not replace." << std::endl;
        } else {
            std::cerr << "ERROR: opening file '" << file_path
                << "': " << std::strerror(errno) << std::endl;
        }
        return 1;
    }
    
    return 0;
}

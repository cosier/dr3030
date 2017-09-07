#include "utils.h"

#ifdef __APPLE__
char* cf_string_ref_to_chars(CFStringRef string) {
    if (string == NULL) {
        return NULL;
    }

    CFIndex length = CFStringGetLength(string);
    CFIndex maxSize =
        CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;

    // Allocate memory
    char* buffer = (char*)malloc(maxSize);

    // Attempt conversion into the buffer
    if (CFStringGetCString(string, buffer, maxSize, kCFStringEncodingUTF8)) {
        return buffer;
    }

    // Could not convert CFStringRef, free then return NULL.
    free(buffer);
    return NULL;
}

CFStringRef char_to_cf_string_ref(char* c) {
    return CFStringCreateWithCStringNoCopy(NULL, c, kCFStringEncodingUTF8,
                                           NULL);
}

#endif

static FILE* LOG_FILE = NULL;

void printd(const char* format, ...) {
#ifdef _DEBUG_
    va_list* ap = malloc(sizeof(va_list));
    char* fmt = malloc(sizeof(char*) * 64);

    va_start(*ap, format);
    sprintf(fmt, "%s%s%s", RED, format, RESET);
    vfprintf(stderr, fmt, *ap);
    va_end(*ap);

    free(ap);
    free(fmt);
#endif
}

void dm_debug(const char* format, ...) {
#ifdef _DEBUG_
    if (dm_driver_debug_mode) {
        if (LOG_FILE == NULL) {
            struct passwd* pw = getpwuid(getuid());
            char* file = pw->pw_dir;
            int size = sizeof(char*) * 128;
            char* file_log = malloc(size);

            snprintf(file_log, size, "%s/.midi-mapper.log", file);
            LOG_FILE = fopen(file_log, "a");
            free(file_log);

            if (LOG_FILE == NULL) {
                error("Failed to write to log file");
                exit(EXIT_FAILURE);
            }
        }

        va_list* ap = malloc(sizeof(va_list));
        va_start(*ap, format);
        vfprintf(LOG_FILE, format, *ap);
        va_end(*ap);

        free(ap);
        fflush(LOG_FILE);
        /* fclose(LOG_FILE); */
        /* LOG_FILE = NULL; */
    }
#endif
}

int dm_count_lines(char* input) {
    int i = 0;
    int lines = 1;
    while (input[i] != '\0') {
        ++i;
        if (input[i] == '\n') {
            lines++;
        }
    }
    return lines;
}

void dm_clear(int lines) {
    for (int i = 0; i < lines; ++i) {
        printf("\33[2K\r");
        printf("\33[1A\r");
        printf("\33[2K\r");
    }
}

int64_t dm_micros() {
    struct timespec tms;
    timespec_get(&tms, TIME_UTC);
    int64_t micros = tms.tv_sec * 1000000;
    return micros += tms.tv_nsec / 1000;
}

void error(char* format, ...) {
    va_list ap;
    va_start(ap, format);
    char* fmt = malloc(sizeof(char*) * 64);
    sprintf(fmt, "%s%s%s", RED, format, RESET);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    putc('\n', stderr);
}

bool contains_bit(unsigned val, unsigned bitindex) {
    return (val & (1 << bitindex)) != 0;
}

int dm_tokenize(char* src, char* delim, char** result) {
    char* container = NULL;
    char* token = NULL;
    int i = 0;
    do {
        token = strtok_r(src, delim, &container);
        src = NULL;
        if (token != NULL) {
            result[i] = strdup(token);
            i++;
        }
    } while (token != NULL);
    free(token);

    return i;
}

/**
 * There be fast cats here, use at your own risk!
 * Appends a src string onto the tail of an existing buffer;
 *
 * Take caution to keep a handle onto the original starting buffer point,
 * ise you will lose it due to pointer arithmetic.
 *
 * When you need fast appending of cats, create a char* on the stack and
 * continually use the return value of this function to track the end.
 * Then once you are done with fast cat appendages, you may discard the stack
 * pointer.
 */
void dm_cat(char** orig, char* src) {
    // printf("dm_cat: appending(%s) to buf(%s)\n", src, buf);
    char* buf = *orig;
    assert(buf != NULL);

    while (*buf) {
        buf++;
    }

    while ((*buf++ = *src++)) {
    };

    // buf++;
    // *buf = 0;
    *orig = --buf;
    // return --buf;
}

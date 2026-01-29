#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Define log levels as enum
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

// Convert enum to string
const char* logLevelToString(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void l(int num, LogLevel log_level, int message_level, const char *format, ...) {
    va_list args;

    // Print 'num' with fixed width at start
    printf("[%-5d]", num);
    // Print log_level
    printf(" [%*s] ", 5, logLevelToString(log_level));

    // Indent based on message_level
    for (int i = 0; i < message_level; i++) {
        printf("    ");
    }
    // Print message
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

// Structure to pass the function pointer
typedef struct {
    void (*func)(void);
} thread_data_t;

// Wrapper function for pthread
void* thread_wrapper(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    data->func();
    free(data); // Free the allocated memory
    return NULL;
}

// Function to start a background thread with a custom code piece
void run_in_background(void (*func)(void)) {
    pthread_t thread_id;
    thread_data_t* data = malloc(sizeof(thread_data_t));
    if (data == NULL) {
        l(200, LOG_LEVEL_ERROR, 1, "malloc failed");
        exit(EXIT_FAILURE);
    }
    data->func = func;
    int result = pthread_create(&thread_id, NULL, thread_wrapper, data);
    if (result != 0) {
        l(201, LOG_LEVEL_ERROR, 1, "pthread_create failed: %s", strerror(result));
        free(data);
        exit(EXIT_FAILURE);
    }
    pthread_detach(thread_id);
}

// Example code piece to run in background
void my_code_piece() {
    l(300, LOG_LEVEL_INFO, 0, "Background code piece is running.");
    for (int i = 0; i < 5; i++) {
        l(301, LOG_LEVEL_DEBUG, 1, "Working... %d", i);
        sleep(1);
    }
    l(302, LOG_LEVEL_INFO, 0, "Background code piece finished.");
}

int main() {
    // Using l() with enum log levels
    l(100, LOG_LEVEL_INFO, 0, "Main thread starting background task.");
    run_in_background(my_code_piece);

    // Main thread continues
    l(101, LOG_LEVEL_INFO, 0, "This is a level 0 message");
    l(102, LOG_LEVEL_DEBUG, 1, "This is a level %d message", 1);
    l(103, LOG_LEVEL_WARN, 2, "This message has level %d and number %d", 2, 3);

    // Continue main thread
    l(104, LOG_LEVEL_INFO, 0, "Main thread continues...");
    sleep(10); // Wait for background thread to finish
    l(105, LOG_LEVEL_INFO, 0, "Main thread finished.");

    return 0;
}

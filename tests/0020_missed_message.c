#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "tox.h"

/*
 *
 *  gcc -g -O0 -fsanitize=address xx.c -o a && ./xx
 *
 */


int s_online[3] = { 0, 0, 0};
int f_online[3] = { 0, 0, 0};
uint8_t s_num1 = 1;
uint8_t s_num2 = 2;
int message_receipt = 0;

static struct timeval start_tv;
static int first_call = 1;

int exit_code = 0;

static struct Node1 {
    char *ip;
    char *key;
    uint16_t udp_port;
    uint16_t tcp_port;
} nodes1[] = {
{ "2604:a880:1:20::32f:1001", "BEF0CFB37AF874BD17B9A8F9FE64C75521DB95A37D33C5BDB00E9CF58659C04F", 33445, 33445 },
{ "2400:8902::f03c:93ff:fe69:bf77", "F76A11284547163889DDC89A7738CF271797BF5E5E220643E97AD3C7E7903D55", 33445, 443 },
{"139.162.110.188","F76A11284547163889DDC89A7738CF271797BF5E5E220643E97AD3C7E7903D55",33445,443},
{ "46.101.197.175", "CD133B521159541FB1D326DE9850F5E56A6C724B5B8E5EB5CD8D950408E95707", 33445, 3389 },
{ "144.217.167.73","7E5668E0EE09E19F320AD47902419331FFEE147BB3606769CFBE921A2A2FD34C",33445,33445},
{ "tox1.mf-net.eu", "B3E5FA80DC8EBD1149AD2AB35ED8B85BD546DEDE261CA593234C619249419506", 33445, 3389 },
{ "bg.tox.dcntrlzd.network", "20AD2A54D70E827302CDF5F11D7C43FA0EC987042C36628E64B2B721A1426E36", 33445, 33445 },
{"91.219.59.156","8E7D0B859922EF569298B4D261A8CCB5FEA14FB91ED412A7603A585A25698832",33445,33445},
{"85.143.221.42","DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43",33445,33445},
{"tox.initramfs.io","3F0A45A268367C1BEA652F258C85F4A66DA76BCAA667A49E770BCC4917AB6A25",33445,33445},
{"144.217.167.73","7E5668E0EE09E19F320AD47902419331FFEE147BB3606769CFBE921A2A2FD34C",33445,3389},
{"tox.abilinski.com","10C00EB250C3233E343E2AEBA07115A5C28920E9C8D29492F6D00B29049EDC7E",33445,33445},
{"tox.novg.net","D527E5847F8330D628DAB1814F0A422F6DC9D0A300E6C357634EE2DA88C35463",33445,33445},
{"198.199.98.108","BEF0CFB37AF874BD17B9A8F9FE64C75521DB95A37D33C5BDB00E9CF58659C04F",33445,33445},
{"tox.kurnevsky.net","82EF82BA33445A1F91A7DB27189ECFC0C013E06E3DA71F588ED692BED625EC23",33445,33445},
{"81.169.136.229","E0DB78116AC6500398DDBA2AEEF3220BB116384CAB714C5D1FCD61EA2B69D75E",33445,33445},
{"205.185.115.131","3091C6BEB2A993F1C6300C16549FABA67098FF3D62C6D253828B531470B53D68",53,53},
{"bg.tox.dcntrlzd.network","20AD2A54D70E827302CDF5F11D7C43FA0EC987042C36628E64B2B721A1426E36",33445,33445},
{"46.101.197.175","CD133B521159541FB1D326DE9850F5E56A6C724B5B8E5EB5CD8D950408E95707",33445,33445},
{"tox1.mf-net.eu","B3E5FA80DC8EBD1149AD2AB35ED8B85BD546DEDE261CA593234C619249419506",33445,33445},
{"tox2.mf-net.eu","70EA214FDE161E7432530605213F18F7427DC773E276B3E317A07531F548545F",33445,33445},
{"195.201.7.101","B84E865125B4EC4C368CD047C72BCE447644A2DC31EF75BD2CDA345BFD310107",33445,33445},
{"tox4.plastiras.org","836D1DA2BE12FE0E669334E437BE3FB02806F1528C2B2782113E0910C7711409",33445,443},
{"gt.sot-te.ch","F4F4856F1A311049E0262E9E0A160610284B434F46299988A9CB42BD3D494618",33445,33445},
{"188.225.9.167","1911341A83E02503AB1FD6561BD64AF3A9D6C3F12B5FBB656976B2E678644A67",33445,33445},
{"122.116.39.151","5716530A10D362867C8E87EE1CD5362A233BAFBBA4CF47FA73B7CAD368BD5E6E",33445,33445},
{"195.123.208.139","534A589BA7427C631773D13083570F529238211893640C99D1507300F055FE73",33445,33445},
{"104.225.141.59","933BA20B2E258B4C0D475B6DECE90C7E827FE83EFA9655414E7841251B19A72C",43334,43334},
{"198.98.49.206","28DB44A3CEEE69146469855DFFE5F54DA567F5D65E03EFB1D38BBAEFF2553255",33445,33445},
{"172.105.109.31","D46E97CF995DC1820B92B7D899E152A217D36ABE22730FEA4B6BF1BFC06C617C",33445,33445},
{"ru.tox.dcntrlzd.network","DBB2E896990ECC383DA2E68A01CA148105E34F9B3B9356F2FE2B5096FDB62762",33445,33445},
{"91.146.66.26","B5E7DAC610DBDE55F359C7F8690B294C8E4FCEC4385DE9525DBFA5523EAD9D53",33445,33445},
{"tox01.ky0uraku.xyz","FD04EB03ABC5FC5266A93D37B4D6D6171C9931176DC68736629552D8EF0DE174",33445,33445},
{"tox02.ky0uraku.xyz","D3D6D7C0C7009FC75406B0A49E475996C8C4F8BCE1E6FC5967DE427F8F600527",33445,33445},
{"kusoneko.moe","BE7ED53CD924813507BA711FD40386062E6DC6F790EFA122C78F7CDEEE4B6D1B",33445,33445},
{"tox2.plastiras.org","B6626D386BE7E3ACA107B46F48A5C4D522D29281750D44A0CBA6A2721E79C951",33445,33445},
{"172.104.215.182","DA2BD927E01CD05EBCC2574EBE5BEBB10FF59AE0B2105A7D1E2B40E49BB20239",33445,33445},
    { NULL, NULL, 0, 0 }
};

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
    printf("[%-2d]", num);

    if (first_call) {
        gettimeofday(&start_tv, NULL);
        first_call = 0;
    }

    struct timeval current_tv;
    gettimeofday(&current_tv, NULL);

    // Calculate elapsed time
    long elapsed_sec = current_tv.tv_sec - start_tv.tv_sec;
    long elapsed_usec = current_tv.tv_usec - start_tv.tv_usec;

    if (elapsed_usec < 0) {
        elapsed_usec += 1000000;
        elapsed_sec -= 1;
    }

    int milliseconds = (int)(elapsed_usec / 1000);
    int hours = (int)(elapsed_sec / 3600);
    int minutes = (int)((elapsed_sec % 3600) / 60);
    int seconds = (int)(elapsed_sec % 60);

    char time_str[20];
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d.%03d",
             hours, minutes, seconds, milliseconds);
    // Print timestamp column
    printf(" [%s] ", time_str);

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

static void yieldcpu(uint32_t ms)
{
    usleep(1000 * ms);
}

// Define the function pointer type with two parameters
typedef void (*func_with_params_t)(int, const char*);

// Structure to pass to thread
typedef struct {
    func_with_params_t func;
    int param1;
    const char* param2;
} thread_data_t;

// Wrapper function for pthread
void* thread_wrapper(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    data->func(data->param1, data->param2);
    free(data); // Free the allocated memory
    return NULL;
}

// Function to start a background thread with a function that takes two parameters
void run_in_background(func_with_params_t func, int param1, const char* param2) {
    pthread_t thread_id;
    thread_data_t* data = malloc(sizeof(thread_data_t));
    if (data == NULL) {
        l(0, LOG_LEVEL_ERROR, 1, "malloc failed");
        exit(EXIT_FAILURE);
    }
    data->func = func;
    data->param1 = param1;
    data->param2 = param2;

    int result = pthread_create(&thread_id, NULL, thread_wrapper, data);
    if (result != 0) {
        l(0, LOG_LEVEL_ERROR, 1, "pthread_create failed: %s", strerror(result));
        free(data);
        exit(EXIT_FAILURE);
    }
    pthread_detach(thread_id);
}

static void to_hex(char *out, uint8_t *in, int size) {
    while (size--) {
        if (*in >> 4 < 0xA) {
            *out++ = '0' + (*in >> 4);
        } else {
            *out++ = 'A' + (*in >> 4) - 0xA;
        }

        if ((*in & 0xf) < 0xA) {
            *out++ = '0' + (*in & 0xF);
        } else {
            *out++ = 'A' + (*in & 0xF) - 0xA;
        }
        in++;
    }
}

static void hex_string_to_bin2(const char *hex_string, uint8_t *output)
{
    size_t len = strlen(hex_string) / 2;
    size_t i = len;
    if (!output)
    {
        return;
    }
    const char *pos = hex_string;
    for (i = 0; i < len; ++i, pos += 2)
    {
        sscanf(pos, "%2hhx", &output[i]);
    }
}

static void bootstrap_tox(Tox *tox, int num)
{
    // ----- bootstrap -----
    for (int i = 0; nodes1[i].ip; i++) {
        uint8_t *key = (uint8_t *)calloc(1, 100);
        if (!key) {
            continue;
        }
        hex_string_to_bin2(nodes1[i].key, key);
        tox_bootstrap(tox, nodes1[i].ip, nodes1[i].udp_port, key, NULL);
        if (nodes1[i].tcp_port != 0) {
            tox_add_tcp_relay(tox, nodes1[i].ip, nodes1[i].tcp_port, key, NULL);
        }
        free(key);
    }
    // ----- bootstrap -----
}

static void self_connection_change_callback(Tox *tox, TOX_CONNECTION status, void *user_data)
{
    uint8_t* unum = (uint8_t *)user_data;
    uint8_t num = *unum;

    switch (status) {
        case TOX_CONNECTION_NONE:
            l(num, LOG_LEVEL_INFO, 0, "offline");
            s_online[num] = 0;
            break;
        case TOX_CONNECTION_TCP:
            l(num, LOG_LEVEL_INFO, 0, "online TCP");
            s_online[num] = 1;
            break;
        case TOX_CONNECTION_UDP:
            l(num, LOG_LEVEL_INFO, 0, "online UDP");
            s_online[num] = 2;
            break;
    }
}

static void friend_connection_status_callback(Tox *tox, uint32_t friend_number, TOX_CONNECTION status, void *user_data)
{
    uint8_t* unum = (uint8_t *)user_data;
    uint8_t num = *unum;

    switch (status) {
        case TOX_CONNECTION_NONE:
            l(num, LOG_LEVEL_INFO, 0, "friend offline");
            f_online[num] = 0;
            break;
        case TOX_CONNECTION_TCP:
            l(num, LOG_LEVEL_INFO, 0, "friend online TCP");
            f_online[num] = 1;
            break;
        case TOX_CONNECTION_UDP:
            l(num, LOG_LEVEL_INFO, 0, "friend online UDP");
            f_online[num] = 2;
            break;
    }
}

static void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data)
{
    uint8_t* unum = (uint8_t *)user_data;
    uint8_t num = *unum;
    l(num, LOG_LEVEL_INFO, 0, "incoming friend request");
    tox_friend_add_norequest(tox, public_key, NULL);
}

static void save_message_to_db(int number, const char* message)
{
    l(number, LOG_LEVEL_INFO, 1, "incoming message, saving to db in a background thread, msg=>%s<", message);
    yieldcpu(30);
    if (message_receipt > 0)
    {
        l(number, LOG_LEVEL_INFO, 1, "friend got read receipt, but we did not yet save the message into the db");
        l(number, LOG_LEVEL_ERROR, 1, "now the user stops the application, or it crashes here");
        exit_code = 1;
    }
    else
    {
        l(number, LOG_LEVEL_INFO, 1, "message is saved in the db, thread finished");
    }
    free((void*)message);
}

static void friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                       size_t length, void *user_data)
{
    uint8_t* unum = (uint8_t *)user_data;
    uint8_t num = *unum;

    if (type == TOX_MESSAGE_TYPE_NORMAL)
    {
        if ((message != NULL) && (length > 0))
        {
            char *message2 = calloc(1, length + 1);
            if (message2)
            {
                memcpy(message2, message, length);
                run_in_background(save_message_to_db, num, message2);
            }
        }
    }
    l(num, LOG_LEVEL_INFO, 0, "message callback finished");
}

static void friend_read_receipt_cb(Tox *tox, uint32_t friend_number, uint32_t message_id, void *user_data)
{
    uint8_t* unum = (uint8_t *)user_data;
    uint8_t num = *unum;
    message_receipt++;
    l(num, LOG_LEVEL_INFO, 0, "got read receipt for message: %d", message_id);
}

static void set_tox_callbacks(Tox *tox, int num)
{
    // ----- CALLBACKS -----
    tox_callback_self_connection_status(tox, self_connection_change_callback);
    tox_callback_friend_connection_status(tox, friend_connection_status_callback);
    tox_callback_friend_request(tox, friend_request_cb);
    tox_callback_friend_message(tox, friend_message_cb);
    tox_callback_friend_read_receipt(tox, friend_read_receipt_cb);
    // ----- CALLBACKS -----
}

int main() {

    struct Tox_Options options;
    tox_options_default(&options);
    Tox_Err_New error_tox;

    Tox *t1 = tox_new(&options, &error_tox);
    bootstrap_tox(t1, 1);
    set_tox_callbacks(t1, 1);
    Tox *t2 = tox_new(&options, &error_tox);
    bootstrap_tox(t2, 2);
    set_tox_callbacks(t2, 2);
    l(0, LOG_LEVEL_INFO, 0, "created toxes");

    f_online[1] = 0;
    f_online[2] = 0;
    s_online[1] = 0;
    s_online[2] = 0;

    // ----------- get online -----------
    uint32_t loops = 0;
    bool get_online = false;
    while (!get_online)
    {
        if (s_online[1] == 0) {
            loops++;
            if ((loops % 100) == 0) {
                bootstrap_tox(t1, 1);
            }
        }
        tox_iterate(t1, (void *)&s_num1);
        yieldcpu(tox_iteration_interval(t1));

        if (s_online[1] > 0) {
            get_online = true;
        }
    }
    l(1, LOG_LEVEL_INFO, 0, "tox1 online");
    // ----------- get online -----------

    // ----------- get online -----------
    loops = 0;
    get_online = false;
    while (!get_online)
    {
        if (s_online[2] == 0) {
            loops++;
            if ((loops % 100) == 0) {
                bootstrap_tox(t2, 2);
            }
        }
        tox_iterate(t2, (void *)&s_num2);
        yieldcpu(tox_iteration_interval(t2));

        if (s_online[2] > 0) {
            get_online = true;
        }
    }
    l(2, LOG_LEVEL_INFO, 0, "tox2 online");
    // ----------- get online -----------

    uint8_t t2_public_key_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(t2, t2_public_key_bin);

    Tox_Err_Friend_Add fr_error;
    uint32_t res = tox_friend_add(t1, t2_public_key_bin, "1", 1, &fr_error);
    l(1, LOG_LEVEL_INFO, 0, "sent friend request: res=%d", (int)fr_error);

    loops = 0;
    bool friend_online = false;
    while (!friend_online)
    {
        if (friend_online == 0) {
            loops++;
            if ((loops % 100) == 0) {
                bootstrap_tox(t2, 2);
                bootstrap_tox(t1, 1);
            }
        }
        tox_iterate(t1, (void *)&s_num1);
        tox_iterate(t2, (void *)&s_num2);
        yieldcpu(tox_iteration_interval(t2));

        if ((f_online[1] > 0) && (f_online[2] > 0)) {
            friend_online = true;
        }
    }
    l(0, LOG_LEVEL_INFO, 0, "friends online and connected");


    // =================================================
    // =================================================

    Tox_Err_Friend_Send_Message err_m1;
    uint32_t msd_id_1 = tox_friend_send_message(t1, 0, TOX_MESSAGE_TYPE_NORMAL, "m1", 2, &err_m1);
    l(1, LOG_LEVEL_INFO, 0, "sent message: res=%d id=%d", (int)err_m1, (int)msd_id_1);

    l(1, LOG_LEVEL_INFO, 0, "iterate start");
    tox_iterate(t1, (void *)&s_num1);
    l(1, LOG_LEVEL_INFO, 0, "iterate end");
    yieldcpu(10);

    l(2, LOG_LEVEL_INFO, 0, "iterate start");
    tox_iterate(t2, (void *)&s_num2);
    l(2, LOG_LEVEL_INFO, 0, "iterate end");
    yieldcpu(10);

    for(int i=0;i<100;i++) {
        tox_iterate(t1, (void *)&s_num1);
        tox_iterate(t2, (void *)&s_num2);
        yieldcpu(10);
    }

    // =================================================
    // =================================================

    l(0, LOG_LEVEL_INFO, 0, "shutting down toxes ...");
    tox_kill(t1);
    tox_kill(t2);
    l(0, LOG_LEVEL_INFO, 0, "finished");

    exit(exit_code);
}

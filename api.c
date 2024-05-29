#include <stdio.h>
#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8000
#define PAGE "{\"code\": 200, \"message\": \"\\u67e5\\u8be2\\u6210\\u529f\", \"data\": {\"apk_version\": \"9999\", \"apk_url\": \"%s\", \"isUpdate\":\"Yes\", \"apk_forceUpdate\":\"Yes\", \"notice\":{\"vin_notice\":[\"VIN\\u7801\\u53efu4ee5\\u5728\\u4eea\\u8868u677fu\\u5de6\\u4e0a\\u65b9\\uff08\\u524d\\u98ce\\u6321\\u73bbu7483\\u540e\\u9762\\uff09\\u548c\\u8f66\\u8f86\\u94ed\\u724c\\u4e0a\\u83b7\\u5f97\\u3002\",\"u672c\\u5e94\\u7528\\u9002\\u7528\\u4e8e2019\\u5e74\\u53ca\\u4e4b\\u540eu751f\\u4ea7\\u7684\\u8f66\\u578b\\u3002\"],\"add_notice\":[\"u5236\\u9020\\u5e74\\u6708\\u53ef\\u901a\\u8fc7\\u8f66\\u8f86\\u94ed\\u724c\\u83b7\\u5f97\\u3002\",\"u672c\\u5e94\\u7528\\u9002\\u7528\\u4e8e2019\\u5e74\\u53calu4e4\\u4e4b\\u540e\\u751f\\u4ea7\\u7684\\u8f66\\u578b\\u3002\"]},\"notice_en\":{\"vin_notice\":[], \"add_notice\":[\"The date can be obtained from the certification label.\"]}}}"

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {
    if (0 != strcmp(method, "POST"))
        return MHD_NO;  // Only accept POST method

    const char *apk_url = cls;  // cls will be used to pass the apk_url

    char *page = malloc(1024);
    sprintf(page, PAGE, apk_url);

    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page),
                                                                    (void *)page,
                                                                    MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, "Content-Type", "application/json");
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int main(int argc, char **argv) {
    struct MHD_Daemon *daemon;
    int port = PORT;

    if (argc == 3) {
        port = atoi(argv[2]); // Custom port number from command line
    }

    char *apk_url = argc > 1 ? argv[1] : "https://gitee.com/awahaha/hafu_apk/releases/download/3.0/3.1.apk";

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port,
                              NULL, NULL, &answer_to_connection, (void *)apk_url,
                              MHD_OPTION_END);
    if (NULL == daemon)
        return 1;

    getchar(); // Press Enter to stop the server

    MHD_stop_daemon(daemon);
    return 0;
}


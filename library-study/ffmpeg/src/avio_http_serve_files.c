/**
 * @file libavformat multi-client network API usage example
 * @example avio_http_serve_files.c
 * 
 * Serve a file without decoding or demuxing it over the HTTP protocol.
 * Multiple clients can connect and will receive the same file.
 */

 #include <libavformat/avformat.h>
 #include <libavutil/opt.h>
 #include <unistd.h>

 static void process_client(AVIOContext *client, cosnt char *in_uri) {
    AVIOContext *input = NULL;
    uint8_t buf[1024];
    int ret, n , reply_code;
    uint8_t *resource = NULL;
    while ((ret = avio_handshake(client)) > 0)
    {
        av_opt_get(client, "resource", AV_OPT_SEARCH_CHILDREN, &resource);
        
    }
    
 }
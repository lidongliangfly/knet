#include "example_config.h"

#if COMPILE_RPC

#include "knet.h"

krpc_t* rpc = 0;

int rpc_cb(krpc_object_t* o) {
    printf("call rpc_cb, param count %d\n", krpc_vector_get_size(o));
    return rpc_ok;
}

/* 客户端 - 连接器回调 */
void connector_cb(channel_ref_t* channel, channel_cb_event_e e) {
    int i = 0;
    krpc_object_t* o = 0;
    krpc_object_t* v = 0;
    char* hello = "hello world";
    stream_t* stream = channel_ref_get_stream(channel);
    if (e & channel_cb_event_connect) { /* 连接成功 */
        /* 写入 */
        v = krpc_object_create();
        for (i = 0; i < 10; i++) {
            o = krpc_object_create();
            krpc_string_set_s(o, hello, 12);
            krpc_vector_push_back(v, o);
        }
        o = krpc_object_create();
        krpc_number_set_i64(o, 9238948959);
        krpc_vector_push_back(v, o);
        krpc_call(rpc, stream, 1, v);
        /* 数组内对象也会被销毁 */
        krpc_object_destroy(v);
    }
}

/* 服务端 - 客户端回调 */
void client_cb(channel_ref_t* channel, channel_cb_event_e e) {
    char buffer[32] = {0};
    /* 获取对端地址 */
    address_t* peer_addr = channel_ref_get_peer_address(channel);
    stream_t* stream = channel_ref_get_stream(channel);
    if (e & channel_cb_event_recv) { /* 有数据可以读 */
        if (error_ok == krpc_proc(rpc, stream)) {
            /* 退出循环 */
            loop_exit(channel_ref_get_loop(channel));
        }
    }
}

/* 监听者回调 */
void acceptor_cb(channel_ref_t* channel, channel_cb_event_e e) {
    if (e & channel_cb_event_accept) { /* 新连接 */
        /* 设置回调 */
        channel_ref_set_cb(channel, client_cb);
    }
}

int main() {
    /* 创建循环 */
    loop_t* loop = loop_create();
    /* 创建客户端 */
    channel_ref_t* connector = loop_create_channel(loop, 8, 1024);
    /* 创建监听者 */
    channel_ref_t* acceptor = loop_create_channel(loop, 8, 1024);
    rpc = krpc_create();
    /* 设置RPC回调 */
    krpc_add_cb(rpc, 1, rpc_cb);
    /* 设置回调 */
    channel_ref_set_cb(connector, connector_cb);
    channel_ref_set_cb(acceptor, acceptor_cb);
    /* 监听 */
    channel_ref_accept(acceptor, 0, 80, 10);
    /* 连接 */
    channel_ref_connect(connector, "127.0.0.1", 80, 5);
    /* 启动 */
    loop_run(loop);
    /* 销毁, connector, acceptor不需要手动销毁 */
    loop_destroy(loop);
    krpc_destroy(rpc);
    return 0;
}

#endif /* COMPILE_RPC */

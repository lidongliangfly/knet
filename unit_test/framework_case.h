/*
 * Copyright (c) 2014-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL dennis wang BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "helper.h"
#include "knet.h"

framework_t* Test_Framework_Framework = 0;
bool Test_Framework_Echo = false;

CASE(Test_Framework) {
    struct holder {
        static void connector_cb(channel_ref_t* channel, channel_cb_event_e e) {
            if (e & channel_cb_event_connect) {
                stream_t* stream = channel_ref_get_stream(channel);
                stream_push(stream, "1234", 5);
            } else if (e & channel_cb_event_recv) {
                Test_Framework_Echo = true;
                channel_ref_close(channel);
            } else if (e & channel_cb_event_close) {
                // �ر�
                loop_exit(channel_ref_get_loop(channel));
                framework_stop(Test_Framework_Framework);
            }
        }

        static void channel_cb(channel_ref_t* channel, channel_cb_event_e e) {
            if (e & channel_cb_event_recv) {
                stream_t* stream = channel_ref_get_stream(channel);
                stream_push(stream, "1234", 5);
            }
        }
    };

    loop_t* loop = loop_create();

    Test_Framework_Framework = framework_create();
    framework_config_t* c = framework_get_config(Test_Framework_Framework);
    framework_config_set_address(c, 0, 80);
    // �������
    EXPECT_TRUE(error_ok == framework_start(Test_Framework_Framework, &holder::channel_cb));

    // ģ��һ���ⲿ������
    channel_ref_t* connector = loop_create_channel(loop, 1, 1024);
    channel_ref_connect(connector, "127.0.0.1", 80, 1);
    channel_ref_set_cb(connector, &holder::connector_cb);
    // ���������������¼�ѭ��
    loop_run(loop);

    EXPECT_TRUE(Test_Framework_Echo);
    framework_wait_for_stop(Test_Framework_Framework);
    framework_destroy(Test_Framework_Framework);
    loop_destroy(loop);
}

CASE(Test_Framework_Start_Fail) {
    framework_t* f = framework_create();
    framework_config_t* c = framework_get_config(f);
    framework_config_set_address(c, "128.0.0.1", 80);
    EXPECT_FALSE(error_ok == framework_start(f, 0));
    framework_destroy(f);

    f = framework_create();
    c = framework_get_config(f);
    framework_config_set_address(c, "128.0.0.1", 80);
    EXPECT_FALSE(error_ok == framework_start_wait(f, 0));
    framework_destroy(f);

    f = framework_create();
    c = framework_get_config(f);
    framework_config_set_address(c, "128.0.0.1", 80);
    EXPECT_FALSE(error_ok == framework_start_wait_destroy(f, 0));
}
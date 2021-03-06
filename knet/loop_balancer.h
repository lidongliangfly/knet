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
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LOOP_BALANCER_H
#define LOOP_BALANCER_H

#include "config.h"
#include "loop_balancer_api.h"

/**
 * 负载均衡 - 选取一个kloop_t实例
 * @param balancer kloop_balancer_t实例
 * @return kloop_t实例
 */
kloop_t* knet_loop_balancer_choose(kloop_balancer_t* balancer);

/**
 * 设置用户数据
 * @param balancer kloop_balancer_t实例
 * @param data 用户数据指针
 */
void knet_loop_balancer_set_data(kloop_balancer_t* balancer, void* data);

/**
 * 取得用户数据
 * @param balancer kloop_balancer_t实例
 * @return 用户数据指针
 */
void* knet_loop_balancer_get_data(kloop_balancer_t* balancer);

#endif /* LOOP_BALANCER_H */

/*
 * Copyright 2014 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MONGOC_SDAM_SCANNER_PRIVATE_H
#define MONGOC_SDAM_SCANNER_PRIVATE_H

/* TODO: rename to SDAM scanner */

#if !defined (MONGOC_I_AM_A_DRIVER) && !defined (MONGOC_COMPILATION)
#error "Only <mongoc.h> can be included directly."
#endif

#include <bson.h>
#include "mongoc-async-private.h"
#include "mongoc-async-cmd-private.h"
#include "mongoc-host-list.h"

BSON_BEGIN_DECLS

typedef bool (*mongoc_sdam_scanner_cb_t)(uint32_t      id,
                                         const bson_t *bson,
                                         int64_t       rtt,
                                         void         *data,
                                         bson_error_t *error);

struct mongoc_sdam_scanner;

typedef struct mongoc_sdam_scanner_node
{
   uint32_t                    id;
   mongoc_async_cmd_t         *cmd;
   mongoc_stream_t            *stream;
   mongoc_host_list_t          host;
   struct addrinfo            *dns_results;
   struct addrinfo            *current_dns_result;
   struct mongoc_sdam_scanner *ss;

   struct mongoc_sdam_scanner_node *next;
   struct mongoc_sdam_scanner_node *prev;
} mongoc_sdam_scanner_node_t;

typedef struct mongoc_sdam_scanner
{
   mongoc_async_t             *async;
   mongoc_sdam_scanner_node_t *nodes;
   uint32_t                    seq;
   bson_t                      ismaster_cmd;
   mongoc_sdam_scanner_cb_t    cb;
   void                       *cb_data;
   bool                        in_progress;

#ifdef MONGOC_ENABLE_SSL
   mongoc_ssl_opt_t *ssl_opts;
#endif
} mongoc_sdam_scanner_t;

mongoc_sdam_scanner_t *
mongoc_sdam_scanner_new (mongoc_sdam_scanner_cb_t cb,
                         void                    *data);

void
mongoc_sdam_scanner_destroy (mongoc_sdam_scanner_t *ss);

void
mongoc_sdam_scanner_add (mongoc_sdam_scanner_t    *ss,
                         const mongoc_host_list_t *host,
                         uint32_t                  id);

void
mongoc_sdam_scanner_rm (mongoc_sdam_scanner_t *ss,
                        uint32_t               id);

void
mongoc_sdam_scanner_start_scan (mongoc_sdam_scanner_t *ss,
                                int32_t                timeout_msec);

bool
mongoc_sdam_scanner_scan (mongoc_sdam_scanner_t *ss,
                          int32_t                timeout_msec);

BSON_END_DECLS

#endif /* MONGOC_SDAM_SCANNER_PRIVATE_H */
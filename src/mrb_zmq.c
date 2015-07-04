#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mrb_zmq.h"
#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/data.h>
#include <mruby/array.h>
#include <mruby/class.h>

struct zmq_context {
    void *context;
    unsigned refs;
};

struct zmq_socket {
    void *socket;
    struct zmq_context *context;
};

const struct mrb_data_type mrb_zmq_socket_type = {"Socket", mrb_free};

#define Check_Socket(__socket) \
    do {\
        if ((__socket->socket) == NULL)\
            mrb_raise(mrb, E_RUNTIME_ERROR, "closed socket");\
    } while(0)

static void context_free (mrb_state *mrb, void *ptr)
{
  struct zmq_context * ctx = (struct zmq_context *)ptr;

  assert(ctx->refs != 0);
  ctx->refs--;

  if (ctx->refs == 0) {
    if (ctx->context != NULL) {
      int rc = zmq_term(ctx->context);
      assert (rc == 0);
    }

    mrb_free(mrb, ctx);
  }
}

const struct mrb_data_type mrb_zmq_context_type = {"Context", context_free};

static mrb_value context_alloc (mrb_state *mrb, mrb_value self) {
    struct zmq_context * ctx;

    ctx = mrb_malloc(mrb, sizeof(*ctx));
    ctx->context = NULL;
    ctx->refs = 1;

    return mrb_data_object_alloc(mrb, 0, ctx, &mrb_zmq_context_type);
}

static mrb_value module_version (mrb_state *mrb, mrb_value self) {
  mrb_value version[3];
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);

  version[0] = mrb_fixnum_value(major);
  version[1] = mrb_fixnum_value(minor);
  version[2] = mrb_fixnum_value(patch);
  return mrb_ary_new_from_values(mrb, 3, version);
}

void mrb_mruby_zmq_gem_init(mrb_state *mrb) {
  struct RClass * class_zmq;
  class_zmq = mrb_define_class(mrb, "ZMQ", mrb->object_class);
  mrb_define_class_method(mrb, class_zmq, "version", module_version, MRB_ARGS_NONE());
}

void mrb_mruby_zmq_gem_final(mrb_state *mrb) {}

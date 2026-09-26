#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_screen_main_screen(js_env_t *env, js_callback_info_t *info) {
  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIScreen mainScreen]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_screen_bounds_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  uint32_t offset;
  if (!bare_ui_kit__read_uint32(env, argv[2], "offset", &offset)) return NULL;

  double *out;

  if (!bare_ui_kit__buffer(env, argv[1], offset, 4, &out)) {
    err = js_throw_type_error(env, NULL, "Expected an array buffer with room for 4 doubles");
    assert(err == 0);

    return NULL;
  }

  @autoreleasepool {
    UIScreen *screen = (__bridge UIScreen *) handle;

    CGRect value = screen.bounds;

    out[0] = value.origin.x;
    out[1] = value.origin.y;
    out[2] = value.size.width;
    out[3] = value.size.height;
  }

  return NULL;
}

static void
bare_ui_kit_screen_bounds_into_typed(js_value_t *receiver, int32_t bare_tag, js_value_t *bare_buffer, uint32_t bare_offset, js_typed_callback_info_t *info) {
  int err;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, NULL);
  assert(err == 0);

  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  double *out;

  if (!bare_ui_kit__buffer(env, bare_buffer, bare_offset, 4, &out)) return;

  @autoreleasepool {
    UIScreen *screen = (UIScreen *) bare_object;

    CGRect value = screen.bounds;

    out[0] = value.origin.x;
    out[1] = value.origin.y;
    out[2] = value.size.width;
    out[3] = value.size.height;
  }
}

static js_value_t *
bare_ui_kit_screen_native_bounds_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  uint32_t offset;
  if (!bare_ui_kit__read_uint32(env, argv[2], "offset", &offset)) return NULL;

  double *out;

  if (!bare_ui_kit__buffer(env, argv[1], offset, 4, &out)) {
    err = js_throw_type_error(env, NULL, "Expected an array buffer with room for 4 doubles");
    assert(err == 0);

    return NULL;
  }

  @autoreleasepool {
    UIScreen *screen = (__bridge UIScreen *) handle;

    CGRect value = screen.nativeBounds;

    out[0] = value.origin.x;
    out[1] = value.origin.y;
    out[2] = value.size.width;
    out[3] = value.size.height;
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_screen_scale(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIScreen *screen = (__bridge UIScreen *) handle;

    err = js_create_double(env, screen.scale, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_screen_native_scale(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIScreen *screen = (__bridge UIScreen *) handle;

    err = js_create_double(env, screen.nativeScale, &result);
    assert(err == 0);
  }

  return result;
}

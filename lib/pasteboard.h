#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_pasteboard_general_pasteboard(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation_bridge(env, state->registry, [UIPasteboard generalPasteboard]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_pasteboard_string(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIPasteboard *pasteboard = (__bridge UIPasteboard *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_string(env, pasteboard.string);
    } else {
      pasteboard.string = bare_ui_kit__to_string(env, argv[1]);
    }
  }

  return result;
}

#define V(name, property) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    bare_ui_kit_state_t *state; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    void *handle; \
    if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL; \
\
    js_value_t *result; \
\
    @autoreleasepool { \
      UIPasteboard *pasteboard = (__bridge UIPasteboard *) handle; \
\
      err = js_get_boolean(env, pasteboard.property, &result); \
      assert(err == 0); \
    } \
\
    return result; \
  }

V(bare_ui_kit_pasteboard_has_strings, hasStrings)
V(bare_ui_kit_pasteboard_has_urls, hasURLs)
V(bare_ui_kit_pasteboard_has_images, hasImages)
V(bare_ui_kit_pasteboard_has_colors, hasColors)
#undef V

#define V(name, property) \
  static js_value_t * \
  name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
\
    size_t argc = 1; \
    js_value_t *argv[1]; \
\
    bare_ui_kit_state_t *state; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state); \
    assert(err == 0); \
\
    assert(argc == 1); \
\
    void *handle; \
    if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL; \
\
    js_value_t *result; \
\
    @autoreleasepool { \
      UIPasteboard *pasteboard = (__bridge UIPasteboard *) handle; \
\
      err = js_create_int64(env, pasteboard.property, &result); \
      assert(err == 0); \
    } \
\
    return result; \
  }

V(bare_ui_kit_pasteboard_number_of_items, numberOfItems)
V(bare_ui_kit_pasteboard_change_count, changeCount)
#undef V

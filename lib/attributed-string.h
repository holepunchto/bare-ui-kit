#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_attributed_string_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  js_value_t *result;

  @autoreleasepool {
    NSString *string = bare_ui_kit__to_string(env, argv[0]);

    NSDictionary<NSAttributedStringKey, id> *attributes = bare_ui_kit__to_attributes(env, state, argv[1]);

    NSMutableAttributedString *handle = [[[NSMutableAttributedString alloc]
      initWithString:string ? string : @"" attributes:attributes] autorelease];

    result = bare_foundation_bridge(env, state->registry, handle);
  }

  return result;
}

static js_value_t *
bare_ui_kit_attributed_string_string(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result;

  @autoreleasepool {
    NSAttributedString *string = (__bridge NSAttributedString *) handle;

    result = bare_ui_kit__from_string(env, string.string);
  }

  return result;
}

static js_value_t *
bare_ui_kit_attributed_string_length(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result;

  @autoreleasepool {
    NSAttributedString *string = (__bridge NSAttributedString *) handle;

    err = js_create_uint32(env, (uint32_t) string.length, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_attributed_string_attributes_at(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  uint32_t location;
  if (!bare_ui_kit__read_uint32(env, argv[1], "location", &location)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    NSAttributedString *string = (__bridge NSAttributedString *) handle;

    result = bare_ui_kit__from_attributes(env, state, [string attributesAtIndex:location effectiveRange:NULL]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_attributed_string_append(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    NSMutableAttributedString *string = (__bridge NSMutableAttributedString *) handle;

    [string appendAttributedString:bare_foundation_to_object(env, state->registry, argv[1])];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_attributed_string_append_string(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    NSMutableAttributedString *string = (__bridge NSMutableAttributedString *) handle;

    NSString *appended = bare_ui_kit__to_string(env, argv[1]);

    NSDictionary<NSAttributedStringKey, id> *attributes = bare_ui_kit__to_attributes(env, state, argv[2]);

    [string appendAttributedString:[[[NSAttributedString alloc]
      initWithString:appended ? appended : @"" attributes:attributes] autorelease]];
  }

  return NULL;
}

#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_font_system_font(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  double size;
  if (!bare_ui_kit__read_double(env, argv[0], "size", &size)) return NULL;

  double weight;
  if (!bare_ui_kit__read_double(env, argv[1], "weight", &weight)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIFont systemFontOfSize:size weight:weight]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_bold_system_font(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  double size;
  if (!bare_ui_kit__read_double(env, argv[0], "size", &size)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIFont boldSystemFontOfSize:size]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_monospaced_system_font(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  double size;
  if (!bare_ui_kit__read_double(env, argv[0], "size", &size)) return NULL;

  double weight;
  if (!bare_ui_kit__read_double(env, argv[1], "weight", &weight)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIFont monospacedSystemFontOfSize:size weight:weight]);
  }

  return result;
}

// A family that is not installed has no font, which is a miss rather than an
// error: the caller falls back to the system font.
static js_value_t *
bare_ui_kit_font_with_name(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  double size;
  if (!bare_ui_kit__read_double(env, argv[1], "size", &size)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    NSString *name = bare_ui_kit__to_string(env, argv[0]);

    result = bare_foundation__bridge(env, [UIFont fontWithName:name size:size]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_font_name(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    result = bare_ui_kit__from_string(env, font.fontName);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_family_name(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    result = bare_ui_kit__from_string(env, font.familyName);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_point_size(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    err = js_create_double(env, font.pointSize, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_ascender(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    err = js_create_double(env, font.ascender, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_descender(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    err = js_create_double(env, font.descender, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_cap_height(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    err = js_create_double(env, font.capHeight, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_x_height(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    err = js_create_double(env, font.xHeight, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_leading(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    err = js_create_double(env, font.leading, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_line_height(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    err = js_create_double(env, font.lineHeight, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_font_descriptor(js_env_t *env, js_callback_info_t *info) {
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
    UIFont *font = (__bridge UIFont *) handle;

    result = bare_foundation__bridge(env, font.fontDescriptor);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_with_descriptor(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  double size;
  if (!bare_ui_kit__read_double(env, argv[1], "size", &size)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIFontDescriptor *descriptor = (__bridge UIFontDescriptor *) handle;

    result = bare_foundation__bridge(env, [UIFont fontWithDescriptor:descriptor size:size]);
  }

  return result;
}

#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// The single source of truth for the system colors: the ids below, the switch
// in `bare_ui_kit_color_system()`, and the constants exported from `binding.m`
// are all expanded from this list.
#define BARE_UI_KIT_COLORS(V) \
  V(BLACK, blackColor) \
  V(WHITE, whiteColor) \
  V(CLEAR, clearColor) \
  V(LABEL, labelColor) \
  V(SECONDARY_LABEL, secondaryLabelColor) \
  V(TERTIARY_LABEL, tertiaryLabelColor) \
  V(QUATERNARY_LABEL, quaternaryLabelColor) \
  V(PLACEHOLDER_TEXT, placeholderTextColor) \
  V(SEPARATOR, separatorColor) \
  V(OPAQUE_SEPARATOR, opaqueSeparatorColor) \
  V(LINK, linkColor) \
  V(SYSTEM_BACKGROUND, systemBackgroundColor) \
  V(SECONDARY_SYSTEM_BACKGROUND, secondarySystemBackgroundColor) \
  V(TERTIARY_SYSTEM_BACKGROUND, tertiarySystemBackgroundColor) \
  V(SYSTEM_GROUPED_BACKGROUND, systemGroupedBackgroundColor) \
  V(SYSTEM_FILL, systemFillColor) \
  V(TINT, tintColor) \
  V(SYSTEM_RED, systemRedColor) \
  V(SYSTEM_ORANGE, systemOrangeColor) \
  V(SYSTEM_YELLOW, systemYellowColor) \
  V(SYSTEM_GREEN, systemGreenColor) \
  V(SYSTEM_MINT, systemMintColor) \
  V(SYSTEM_TEAL, systemTealColor) \
  V(SYSTEM_CYAN, systemCyanColor) \
  V(SYSTEM_BLUE, systemBlueColor) \
  V(SYSTEM_INDIGO, systemIndigoColor) \
  V(SYSTEM_PURPLE, systemPurpleColor) \
  V(SYSTEM_PINK, systemPinkColor) \
  V(SYSTEM_BROWN, systemBrownColor) \
  V(SYSTEM_GRAY, systemGrayColor)

enum {
#define V(name, selector) bare_ui_kit_color_##name,
  BARE_UI_KIT_COLORS(V)
#undef V
};

static js_value_t *
bare_ui_kit_color_system(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  int32_t id;
  if (!bare_ui_kit__read_int32(env, argv[0], "id", &id)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIColor *color = nil;

    switch (id) {
#define V(name, selector) \
  case bare_ui_kit_color_##name: \
    color = [UIColor selector]; \
    break;
      BARE_UI_KIT_COLORS(V)
#undef V
    }

    result = bare_foundation__bridge(env, color);
  }

  return result;
}

static js_value_t *
bare_ui_kit_color_rgb(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 4);

  double red;
  if (!bare_ui_kit__read_double(env, argv[0], "red", &red)) return NULL;

  double green;
  if (!bare_ui_kit__read_double(env, argv[1], "green", &green)) return NULL;

  double blue;
  if (!bare_ui_kit__read_double(env, argv[2], "blue", &blue)) return NULL;

  double alpha;
  if (!bare_ui_kit__read_double(env, argv[3], "alpha", &alpha)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIColor colorWithRed:red green:green blue:blue alpha:alpha]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_color_hsb(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 4);

  double hue;
  if (!bare_ui_kit__read_double(env, argv[0], "hue", &hue)) return NULL;

  double saturation;
  if (!bare_ui_kit__read_double(env, argv[1], "saturation", &saturation)) return NULL;

  double brightness;
  if (!bare_ui_kit__read_double(env, argv[2], "brightness", &brightness)) return NULL;

  double alpha;
  if (!bare_ui_kit__read_double(env, argv[3], "alpha", &alpha)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIColor colorWithHue:hue saturation:saturation brightness:brightness alpha:alpha]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_color_white(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  double white;
  if (!bare_ui_kit__read_double(env, argv[0], "white", &white)) return NULL;

  double alpha;
  if (!bare_ui_kit__read_double(env, argv[1], "alpha", &alpha)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIColor colorWithWhite:white alpha:alpha]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_color_with_alpha_component(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  double alpha;
  if (!bare_ui_kit__read_double(env, argv[1], "alpha", &alpha)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIColor *color = (__bridge UIColor *) handle;

    result = bare_foundation__bridge(env, [color colorWithAlphaComponent:alpha]);
  }

  return result;
}

// A color that is not expressible in RGBA, such as a pattern, reports zeroes
// rather than leaving the caller's storage holding whatever was there before.
static js_value_t *
bare_ui_kit_color_components_into(js_env_t *env, js_callback_info_t *info) {
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
    UIColor *color = (__bridge UIColor *) handle;

    CGFloat red = 0, green = 0, blue = 0, alpha = 0;

    [color getRed:&red green:&green blue:&blue alpha:&alpha];

    out[0] = red;
    out[1] = green;
    out[2] = blue;
    out[3] = alpha;
  }

  return NULL;
}

static void
bare_ui_kit_color_components_into_typed(js_value_t *receiver, int32_t bare_tag, js_value_t *bare_buffer, uint32_t bare_offset, js_typed_callback_info_t *info) {
  int err;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, NULL);
  assert(err == 0);

  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  double *out;

  if (!bare_ui_kit__buffer(env, bare_buffer, bare_offset, 4, &out)) return;

  @autoreleasepool {
    UIColor *color = (UIColor *) bare_object;

    CGFloat red = 0, green = 0, blue = 0, alpha = 0;

    [color getRed:&red green:&green blue:&blue alpha:&alpha];

    out[0] = red;
    out[1] = green;
    out[2] = blue;
    out[3] = alpha;
  }
}

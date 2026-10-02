#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_font_metrics_default_metrics(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation_bridge(env, state->registry, [UIFontMetrics defaultMetrics]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_metrics_for_text_style(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  js_value_t *result;

  @autoreleasepool {
    NSString *style = bare_ui_kit__to_string(env, argv[0]);

    result = bare_foundation_bridge(env, state->registry, [UIFontMetrics metricsForTextStyle:style]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_metrics_scaled_value_for_value(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  double value;
  if (!bare_ui_kit__read_double(env, argv[1], "value", &value)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIFontMetrics *metrics = (__bridge UIFontMetrics *) handle;

    err = js_create_double(env, [metrics scaledValueForValue:value], &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_font_metrics_scaled_font_for_font(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  void *font;
  if (bare_foundation_read_tag(env, state->registry, argv[1], "font", &font) < 0) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIFontMetrics *metrics = (__bridge UIFontMetrics *) handle;

    result = bare_foundation_bridge(env, state->registry, [metrics scaledFontForFont:(__bridge UIFont *) font]);
  }

  return result;
}

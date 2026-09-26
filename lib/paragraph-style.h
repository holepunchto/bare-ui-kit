#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_paragraph_style_init(js_env_t *env, js_callback_info_t *info) {
  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [[[NSMutableParagraphStyle alloc] init] autorelease]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_paragraph_style_alignment(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    NSMutableParagraphStyle *style = (__bridge NSMutableParagraphStyle *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) style.alignment, &result);
      assert(err == 0);
    } else {
      int32_t alignment;
      if (!bare_ui_kit__read_int32(env, argv[1], "alignment", &alignment)) return NULL;

      style.alignment = (NSTextAlignment) alignment;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_paragraph_style_line_break_mode(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    NSMutableParagraphStyle *style = (__bridge NSMutableParagraphStyle *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) style.lineBreakMode, &result);
      assert(err == 0);
    } else {
      int32_t mode;
      if (!bare_ui_kit__read_int32(env, argv[1], "lineBreakMode", &mode)) return NULL;

      style.lineBreakMode = (NSLineBreakMode) mode;
    }
  }

  return result;
}

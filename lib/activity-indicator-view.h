#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_activity_indicator_view_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 4);

  double x;
  if (!bare_ui_kit__read_double(env, argv[0], "x", &x)) return NULL;

  double y;
  if (!bare_ui_kit__read_double(env, argv[1], "y", &y)) return NULL;

  double width;
  if (!bare_ui_kit__read_double(env, argv[2], "width", &width)) return NULL;

  double height;
  if (!bare_ui_kit__read_double(env, argv[3], "height", &height)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIActivityIndicatorView *handle = [[[UIActivityIndicatorView alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation__bridge(env, handle);
  }

  return result;
}

static js_value_t *
bare_ui_kit_activity_indicator_view_style(js_env_t *env, js_callback_info_t *info) {
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
    UIActivityIndicatorView *view = (__bridge UIActivityIndicatorView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) view.activityIndicatorViewStyle, &result);
      assert(err == 0);
    } else {
      int32_t style;
      if (!bare_ui_kit__read_int32(env, argv[1], "style", &style)) return NULL;

      view.activityIndicatorViewStyle = (UIActivityIndicatorViewStyle) style;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_activity_indicator_view_color(js_env_t *env, js_callback_info_t *info) {
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
    UIActivityIndicatorView *view = (__bridge UIActivityIndicatorView *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, view.color);
    } else {
      view.color = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_activity_indicator_view_hides_when_stopped(js_env_t *env, js_callback_info_t *info) {
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
    UIActivityIndicatorView *view = (__bridge UIActivityIndicatorView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, view.hidesWhenStopped, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "hidesWhenStopped", &value)) return NULL;

      view.hidesWhenStopped = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_activity_indicator_view_animating(js_env_t *env, js_callback_info_t *info) {
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
    UIActivityIndicatorView *view = (__bridge UIActivityIndicatorView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, view.animating, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "animating", &value)) return NULL;

      if (value) [view startAnimating];
      else [view stopAnimating];
    }
  }

  return result;
}

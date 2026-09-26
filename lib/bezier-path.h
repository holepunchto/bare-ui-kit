#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_bezier_path_init(js_env_t *env, js_callback_info_t *info) {
  js_value_t *result;

  @autoreleasepool {
    result = bare_foundation__bridge(env, [UIBezierPath bezierPath]);
  }

  return result;
}

#define V(name, expr) \
  static js_value_t * \
  bare_ui_kit_bezier_path_##name(js_env_t *env, js_callback_info_t *info) { \
    int err; \
    size_t argc = 3; \
    js_value_t *argv[3]; \
    err = js_get_callback_info(env, info, &argc, argv, NULL, NULL); \
    assert(err == 0); \
    assert(argc == 3); \
    void *handle; \
    if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL; \
    double x, y; \
    err = js_get_value_double(env, argv[1], &x); \
    assert(err == 0); \
    err = js_get_value_double(env, argv[2], &y); \
    assert(err == 0); \
    @autoreleasepool { \
      UIBezierPath *path = (__bridge UIBezierPath *) handle; \
      [path expr:CGPointMake(x, y)]; \
    } \
    return NULL; \
  }

V(move_to, moveToPoint)
V(line_to, addLineToPoint)
#undef V

static js_value_t *
bare_ui_kit_bezier_path_curve_to(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 7;
  js_value_t *argv[7];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 7);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  double values[6];

  for (int i = 0; i < 6; i++) {
    err = js_get_value_double(env, argv[i + 1], &values[i]);
    assert(err == 0);
  }

  @autoreleasepool {
    UIBezierPath *path = (__bridge UIBezierPath *) handle;

    [path addCurveToPoint:CGPointMake(values[0], values[1])
            controlPoint1:CGPointMake(values[2], values[3])
            controlPoint2:CGPointMake(values[4], values[5])];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_bezier_path_add_arc(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 6;
  js_value_t *argv[6];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 6);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  double values[5];

  for (int i = 0; i < 5; i++) {
    err = js_get_value_double(env, argv[i + 1], &values[i]);
    assert(err == 0);
  }

  @autoreleasepool {
    UIBezierPath *path = (__bridge UIBezierPath *) handle;

    [path addArcWithCenter:CGPointMake(values[0], values[1])
                    radius:values[2]
                startAngle:values[3]
                  endAngle:values[4]
                 clockwise:YES];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_bezier_path_close(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  @autoreleasepool {
    [(__bridge UIBezierPath *) handle closePath];
  }

  return NULL;
}

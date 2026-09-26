#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

enum {
  bare_ui_kit_switch_event_value_changed = 1 << 0,
};

@interface BareSwitch : UISwitch <BareEventTarget> {
@public
  js_env_t *env;
  js_ref_t *ctx;

  int32_t mask;
}

@end

@implementation BareSwitch

- (void)dealloc {
  int err;

  err = js_delete_reference(env, ctx);
  assert(err == 0);

  [super dealloc];
}

- (int32_t)eventMask {
  return mask;
}

- (void)setEventMask:(int32_t)value {
  mask = value;
}

// A `UIControl` reports through a target and an action rather than a delegate,
// and it sends this one only when someone works the control, never when its
// value is set.
- (void)bareValueChanged:(id)sender {
  if ((mask & bare_ui_kit_switch_event_value_changed) == 0) return;

  bare_ui_kit__emit(env, ctx, "_onvaluechanged");
}

@end

static js_value_t *
bare_ui_kit_switch_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 5);

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
    BareSwitch *handle = [[[BareSwitch alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation__bridge(env, handle);

    handle->env = env;

    err = js_create_reference(env, argv[4], 0, &handle->ctx);
    assert(err == 0);

    [handle addTarget:handle
               action:@selector(bareValueChanged:)
     forControlEvents:UIControlEventValueChanged];
  }

  return result;
}

static js_value_t *
bare_ui_kit_switch_on(js_env_t *env, js_callback_info_t *info) {
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
    UISwitch *control = (__bridge UISwitch *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, control.on, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "on", &value)) return NULL;

      control.on = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_switch_enabled(js_env_t *env, js_callback_info_t *info) {
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
    UISwitch *control = (__bridge UISwitch *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, control.enabled, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "enabled", &value)) return NULL;

      control.enabled = value;
    }
  }

  return result;
}

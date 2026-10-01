#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// The keyboard is reported as a notification, carrying where it will be when it
// stops moving. A window is what that frame is measured against, so a window
// observes it while the mask asks.
enum {
  bare_ui_kit_window_event_keyboard_will_change_frame = 1 << 0,
};

@interface BareWindow : UIWindow <BareEventTarget> {
@public
  js_env_t *env;
  js_ref_t *ctx;

  int32_t mask;
}

@end

@implementation BareWindow

- (void)dealloc {
  int err;

  [NSNotificationCenter.defaultCenter removeObserver:self];

  err = js_delete_reference(env, ctx);
  assert(err == 0);

  [super dealloc];
}

- (int32_t)eventMask {
  return mask;
}

- (void)setEventMask:(int32_t)value {
  int32_t changed = mask ^ value;

  mask = value;

  if ((changed & bare_ui_kit_window_event_keyboard_will_change_frame) == 0) return;

  if ((value & bare_ui_kit_window_event_keyboard_will_change_frame) != 0) {
    [NSNotificationCenter.defaultCenter
      addObserver:self
         selector:@selector(bareKeyboardWillChangeFrame:)
             name:UIKeyboardWillChangeFrameNotification
           object:nil];
  } else {
    [NSNotificationCenter.defaultCenter
      removeObserver:self
                name:UIKeyboardWillChangeFrameNotification
              object:nil];
  }
}

- (void)bareKeyboardWillChangeFrame:(NSNotification *)notification {
  NSValue *frame = notification.userInfo[UIKeyboardFrameEndUserInfoKey];

  if (frame == nil) return;

  // In screen coordinates, as the notification reports it.
  bare_ui_kit__emit_rect(env, ctx, "_onkeyboardwillchangeframe", [frame CGRectValue]);
}

@end

// A window without a scene is never shown.
static js_value_t *
bare_ui_kit_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 6;
  js_value_t *argv[6];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 6);

  js_value_type_t type;
  err = js_typeof(env, argv[1], &type);
  assert(err == 0);

  bool framed = type != js_null;

  double x = 0, y = 0, width = 0, height = 0;

  if (framed) {
    if (!bare_ui_kit__read_double(env, argv[1], "x", &x)) return NULL;
    if (!bare_ui_kit__read_double(env, argv[2], "y", &y)) return NULL;
    if (!bare_ui_kit__read_double(env, argv[3], "width", &width)) return NULL;
    if (!bare_ui_kit__read_double(env, argv[4], "height", &height)) return NULL;
  }

  js_value_t *result;

  @autoreleasepool {
    UIWindowScene *scene = bare_foundation_to_object(env, state->registry, argv[0]);

    BareWindow *handle = scene
                           ? [[[BareWindow alloc] initWithWindowScene:scene] autorelease]
                           : [[[BareWindow alloc] initWithFrame:CGRectZero] autorelease];

    if (framed) handle.frame = CGRectMake(x, y, width, height);

    result = bare_foundation_bridge(env, state->registry, handle);

    handle->env = env;

    err = js_create_reference(env, argv[5], 0, &handle->ctx);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_window_window_scene(js_env_t *env, js_callback_info_t *info) {
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
    UIWindow *window = (__bridge UIWindow *) handle;

    if (argc == 1) {
      result = bare_foundation_bridge(env, state->registry, window.windowScene);
    } else {
      window.windowScene = bare_foundation_to_object(env, state->registry, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_window_root_view_controller(js_env_t *env, js_callback_info_t *info) {
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
    UIWindow *window = (__bridge UIWindow *) handle;

    if (argc == 1) {
      result = bare_foundation_bridge(env, state->registry, window.rootViewController);
    } else {
      window.rootViewController = bare_foundation_to_object(env, state->registry, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_window_make_key_window(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    UIWindow *window = (__bridge UIWindow *) handle;

    [window makeKeyWindow];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_window_make_key_and_visible(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  @autoreleasepool {
    UIWindow *window = (__bridge UIWindow *) handle;

    [window makeKeyAndVisible];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_window_key_window(js_env_t *env, js_callback_info_t *info) {
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
    UIWindow *window = (__bridge UIWindow *) handle;

    err = js_get_boolean(env, window.keyWindow, &result);
    assert(err == 0);
  }

  return result;
}

#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// The keyboard is not a property of anything and not an inset on a view: it is
// a notification, carrying where it will be when it has finished moving. A
// window is what that frame has to be measured against, so a window is what
// observes it, for as long as the mask asks.
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

  // Reported in the screen's space, which is what the notification carries;
  // what it covers of a window is the window's business.
  bare_ui_kit__emit_rect(env, ctx, "_onkeyboardwillchangeframe", [frame CGRectValue]);
}

@end

// `initWithWindowScene:` is the designated initializer from iOS 13 on, and a
// window that belongs to no scene is never displayed. A window is still
// constructible without one, which is what a headless caller gets.
static js_value_t *
bare_ui_kit_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 6;
  js_value_t *argv[6];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 6);

  double x;
  if (!bare_ui_kit__read_double(env, argv[1], "x", &x)) return NULL;

  double y;
  if (!bare_ui_kit__read_double(env, argv[2], "y", &y)) return NULL;

  double width;
  if (!bare_ui_kit__read_double(env, argv[3], "width", &width)) return NULL;

  double height;
  if (!bare_ui_kit__read_double(env, argv[4], "height", &height)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIWindowScene *scene = bare_foundation__to_object(env, argv[0]);

    BareWindow *handle = scene
                           ? [[[BareWindow alloc] initWithWindowScene:scene] autorelease]
                           : [[[BareWindow alloc] initWithFrame:CGRectZero] autorelease];

    handle.frame = CGRectMake(x, y, width, height);

    result = bare_foundation__bridge(env, handle);

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

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIWindow *window = (__bridge UIWindow *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, window.windowScene);
    } else {
      window.windowScene = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_window_root_view_controller(js_env_t *env, js_callback_info_t *info) {
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
    UIWindow *window = (__bridge UIWindow *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, window.rootViewController);
    } else {
      window.rootViewController = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_window_make_key_window(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

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

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

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

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIWindow *window = (__bridge UIWindow *) handle;

    err = js_get_boolean(env, window.keyWindow, &result);
    assert(err == 0);
  }

  return result;
}

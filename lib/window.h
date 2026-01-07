#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

@interface BareWindow : UIWindow {
@public
  js_env_t *env;
  js_ref_t *ctx;
}

@end

@implementation BareWindow

- (void)dealloc {
  int err;

  err = js_delete_reference(env, ctx);
  assert(err == 0);

  [super dealloc];
}

@end

static js_value_t *
bare_ui_kit_window_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  js_value_t *result;

  @autoreleasepool {
    BareWindow *handle = [[BareWindow alloc] init];

    err = js_create_external(env, (void *) CFBridgingRetain(handle), bare_ui_kit__on_bridged_release, NULL, &result);
    assert(err == 0);

    handle->env = env;

    err = js_create_reference(env, argv[0], 1, &handle->ctx);
    assert(err == 0);
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
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  js_value_t *result = NULL;

  @autoreleasepool {
    UIWindow *window = (__bridge UIWindow *) handle;

    if (argc == 1) {
      UIViewController *view_controller = window.rootViewController;

      if (view_controller == NULL) return NULL;

      err = js_create_external(env, (void *) CFBridgingRetain(handle), bare_ui_kit__on_bridged_release, NULL, &result);
      assert(err == 0);
    } else {
      void *handle;
      err = js_get_value_external(env, argv[1], &handle);
      assert(err == 0);

      UIViewController *view_controller = (__bridge UIViewController *) handle;

      window.rootViewController = view_controller;
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
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWindow *window = (__bridge BareWindow *) handle;

    [window makeKeyWindow];
  }

  return NULL;
}

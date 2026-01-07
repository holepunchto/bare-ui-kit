#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

@interface BareViewController : UIViewController {
@public
  js_env_t *env;
  js_ref_t *ctx;
}

@end

@implementation BareViewController

- (void)dealloc {
  int err;

  err = js_delete_reference(env, ctx);
  assert(err == 0);

  [super dealloc];
}

@end

static js_value_t *
bare_ui_kit_view_controller_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  js_value_t *result;

  @autoreleasepool {
    BareViewController *handle = [[BareViewController alloc] init];

    err = js_create_external(env, (void *) CFBridgingRetain(handle), bare_ui_kit__on_bridged_release, NULL, &result);
    assert(err == 0);

    handle->env = env;

    err = js_create_reference(env, argv[0], 1, &handle->ctx);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_controller_view(js_env_t *env, js_callback_info_t *info) {
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
    UIViewController *view_controller = (__bridge UIViewController *) handle;

    if (argc == 1) {
      UIView *view = view_controller.view;

      err = js_create_external(env, (void *) CFBridgingRetain(view), bare_ui_kit__on_bridged_release, NULL, &result);
      assert(err == 0);
    } else {
      void *handle;
      err = js_get_value_external(env, argv[1], &handle);
      assert(err == 0);

      UIView *view = (__bridge UIView *) handle;

      view_controller.view = view;
    }
  }

  return result;
}

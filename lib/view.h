
#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

static js_value_t *
bare_ui_kit_view_hidden(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, view.hidden, &result);
      assert(err == 0);
    } else {
      bool value;
      err = js_get_value_bool(env, argv[1], &value);
      assert(err == 0);

      view.hidden = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_add_subview(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    void *handle;
    err = js_get_value_external(env, argv[1], &handle);
    assert(err == 0);

    UIView *subview = (__bridge UIView *) handle;

    [view addSubview:subview];
  }

  return NULL;
}

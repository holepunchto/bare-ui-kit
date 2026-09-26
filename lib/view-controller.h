#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// The controller lays its view out to fill the window, so this is where a size
// change is seen. Watching every view would report the same layout once per
// view.
enum {
  bare_ui_kit_view_controller_event_did_layout_subviews = 1 << 0,
  bare_ui_kit_view_controller_event_safe_area_insets_did_change = 1 << 1,
};

@interface BareViewController : UIViewController <BareEventTarget> {
@public
  js_env_t *env;
  js_ref_t *ctx;

  int32_t mask;
}

@end

@implementation BareViewController

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

- (void)viewDidLayoutSubviews {
  [super viewDidLayoutSubviews];

  if ((mask & bare_ui_kit_view_controller_event_did_layout_subviews) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidlayoutsubviews");
}

- (void)viewSafeAreaInsetsDidChange {
  [super viewSafeAreaInsetsDidChange];

  if ((mask & bare_ui_kit_view_controller_event_safe_area_insets_did_change) == 0) return;

  bare_ui_kit__emit(env, ctx, "_onsafeareainsetsdidchange");
}

@end

static js_value_t *
bare_ui_kit_view_controller_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  js_value_t *result;

  @autoreleasepool {
    BareViewController *handle = [[[BareViewController alloc] init] autorelease];

    result = bare_foundation_bridge(env, state->registry, handle);

    handle->env = env;

    // Weak, so the native object does not keep its own wrapper alive. Events
    // are dropped once the wrapper is collected.
    err = js_create_reference(env, argv[0], 0, &handle->ctx);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_controller_present(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  void *presented;
  if (bare_foundation_read_tag(env, state->registry, argv[1], "presented", &presented) < 0) return NULL;

  bool animated;
  if (!bare_ui_kit__read_bool(env, argv[2], "animated", &animated)) return NULL;

  @autoreleasepool {
    UIViewController *controller = (__bridge UIViewController *) handle;

    [controller presentViewController:(__bridge UIViewController *) presented
                             animated:animated
                           completion:nil];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_controller_dismiss(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  bool animated;
  if (!bare_ui_kit__read_bool(env, argv[1], "animated", &animated)) return NULL;

  @autoreleasepool {
    UIViewController *controller = (__bridge UIViewController *) handle;

    [controller dismissViewControllerAnimated:animated completion:nil];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_controller_view(js_env_t *env, js_callback_info_t *info) {
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
    UIViewController *view_controller = (__bridge UIViewController *) handle;

    if (argc == 1) {
      result = bare_foundation_bridge(env, state->registry, view_controller.view);
    } else {
      view_controller.view = bare_foundation_to_object(env, state->registry, argv[1]);
    }
  }

  return result;
}

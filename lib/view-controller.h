#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// The controller lays its view out to fill the window, so this is where a
// size change is observed. Watching every view instead would report the same
// pass once per node, and re-entrantly for anything that sets a frame in
// response.
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

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  js_value_t *result;

  @autoreleasepool {
    BareViewController *handle = [[[BareViewController alloc] init] autorelease];

    result = bare_foundation__bridge(env, handle);

    handle->env = env;

    // Weak, so that the native object does not keep its own JS wrapper
    // alive. Events are dropped once the wrapper has been collected.
    err = js_create_reference(env, argv[0], 0, &handle->ctx);
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
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIViewController *view_controller = (__bridge UIViewController *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, view_controller.view);
    } else {
      view_controller.view = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

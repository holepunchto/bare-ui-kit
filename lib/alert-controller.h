#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// An action gets its handler when it is made, so it always reports and has no
// mask.
@interface BareAlertActionHandler : NSObject {
@public
  js_env_t *env;
  js_ref_t *ctx;
}
@end

@implementation BareAlertActionHandler

- (void)dealloc {
  int err;

  err = js_delete_reference(env, ctx);
  assert(err == 0);

  [super dealloc];
}

- (void)selected {
  bare_ui_kit__emit(env, ctx, "_onselected");
}

@end

static js_value_t *
bare_ui_kit_alert_action_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  double style;
  if (!bare_ui_kit__read_double(env, argv[1], "style", &style)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    NSString *title = bare_ui_kit__to_string(env, argv[0]);

    BareAlertActionHandler *handler = [[BareAlertActionHandler alloc] init];

    handler->env = env;

    // Weak, so the native object does not keep its own wrapper alive.
    err = js_create_reference(env, argv[2], 0, &handler->ctx);
    assert(err == 0);

    UIAlertAction *action = [UIAlertAction actionWithTitle:title
                                                     style:(UIAlertActionStyle) style
                                                   handler:^(UIAlertAction *selected) {
                                                     [handler selected];
                                                   }];

    // The action owns the copied block, and the block retains what it captured,
    // so the handler outlives this scope.
    [handler release];

    result = bare_foundation_bridge(env, state->registry, action);
  }

  return result;
}

static js_value_t *
bare_ui_kit_alert_action_title(js_env_t *env, js_callback_info_t *info) {
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
    UIAlertAction *action = (__bridge UIAlertAction *) handle;

    result = bare_ui_kit__from_string(env, action.title);
  }

  return result;
}

static js_value_t *
bare_ui_kit_alert_action_style(js_env_t *env, js_callback_info_t *info) {
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
    UIAlertAction *action = (__bridge UIAlertAction *) handle;

    err = js_create_int32(env, (int32_t) action.style, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_alert_controller_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  double style;
  if (!bare_ui_kit__read_double(env, argv[2], "preferred_style", &style)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    NSString *title = bare_ui_kit__to_string(env, argv[0]);

    js_value_t *message = argv[1];

    js_value_type_t type;
    err = js_typeof(env, message, &type);
    assert(err == 0);

    UIAlertController *controller =
      [UIAlertController alertControllerWithTitle:title
                                          message:type == js_string ? bare_ui_kit__to_string(env, message) : nil
                                   preferredStyle:(UIAlertControllerStyle) style];

    result = bare_foundation_bridge(env, state->registry, controller);
  }

  return result;
}

static js_value_t *
bare_ui_kit_alert_controller_add_action(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  void *action;
  if (bare_foundation_read_tag(env, state->registry, argv[1], "action", &action) < 0) return NULL;

  @autoreleasepool {
    UIAlertController *controller = (__bridge UIAlertController *) handle;

    [controller addAction:(__bridge UIAlertAction *) action];
  }

  return NULL;
}

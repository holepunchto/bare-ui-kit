#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_scene_connected(js_env_t *env, js_callback_info_t *info) {
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, NULL, NULL, NULL, (void **) &state);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    NSMutableArray<UIWindowScene *> *scenes = [NSMutableArray array];

    for (UIScene *scene in UIApplication.sharedApplication.connectedScenes) {
      if ([scene isKindOfClass:UIWindowScene.class]) {
        [scenes addObject:(UIWindowScene *) scene];
      }
    }

    result = bare_ui_kit__from_objects(env, state, scenes);
  }

  return result;
}

static js_value_t *
bare_ui_kit_scene_screen(js_env_t *env, js_callback_info_t *info) {
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
    UIWindowScene *scene = (__bridge UIWindowScene *) handle;

    result = bare_foundation_bridge(env, state->registry, scene.screen);
  }

  return result;
}

static js_value_t *
bare_ui_kit_scene_activation_state(js_env_t *env, js_callback_info_t *info) {
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
    UIScene *scene = (__bridge UIScene *) handle;

    err = js_create_int32(env, (int32_t) scene.activationState, &result);
    assert(err == 0);
  }

  return result;
}

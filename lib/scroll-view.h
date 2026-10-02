#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

enum {
  bare_ui_kit_scroll_view_event_did_scroll = 1 << 0,
};

// A scroll view is its own delegate, like every other class here.
@interface BareScrollView : UIScrollView <UIScrollViewDelegate, BareEventTarget> {
@public
  js_env_t *env;
  js_ref_t *ctx;

  int32_t mask;
}

@end

@implementation BareScrollView

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

- (void)scrollViewDidScroll:(UIScrollView *)view {
  if ((mask & bare_ui_kit_scroll_view_event_did_scroll) == 0) return;

  CGPoint offset = view.contentOffset;

  bare_ui_kit__emit_point(env, ctx, "_ondidscroll", offset.x, offset.y);
}

@end

static js_value_t *
bare_ui_kit_scroll_view_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
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
    BareScrollView *handle = [[[BareScrollView alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation_bridge(env, state->registry, handle);

    handle->env = env;

    err = js_create_reference(env, argv[4], 0, &handle->ctx);
    assert(err == 0);

    handle.delegate = handle;
  }

  return result;
}

static js_value_t *
bare_ui_kit_scroll_view_content_size(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIScrollView *view = (__bridge UIScrollView *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_size(env, view.contentSize);
    } else {
      double width;
      if (!bare_ui_kit__read_double(env, argv[1], "width", &width)) return NULL;

      double height;
      if (!bare_ui_kit__read_double(env, argv[2], "height", &height)) return NULL;

      view.contentSize = CGSizeMake(width, height);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_scroll_view_content_inset_adjustment_behavior(js_env_t *env, js_callback_info_t *info) {
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
    UIScrollView *view = (__bridge UIScrollView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) view.contentInsetAdjustmentBehavior, &result);
      assert(err == 0);
    } else {
      double behavior;
      if (!bare_ui_kit__read_double(env, argv[1], "behavior", &behavior)) return NULL;

      view.contentInsetAdjustmentBehavior = (UIScrollViewContentInsetAdjustmentBehavior) behavior;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_scroll_view_adjusted_content_inset_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  uint32_t offset;
  if (!bare_ui_kit__read_uint32(env, argv[2], "offset", &offset)) return NULL;

  double *out;

  if (!bare_ui_kit__buffer(env, argv[1], offset, 4, &out)) {
    err = js_throw_type_error(env, NULL, "Expected an array buffer with room for 4 doubles");
    assert(err == 0);

    return NULL;
  }

  @autoreleasepool {
    UIScrollView *view = (__bridge UIScrollView *) handle;

    UIEdgeInsets value = view.adjustedContentInset;

    out[0] = value.top;
    out[1] = value.left;
    out[2] = value.bottom;
    out[3] = value.right;
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_scroll_view_content_offset(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIScrollView *view = (__bridge UIScrollView *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_point(env, view.contentOffset);
    } else {
      double x;
      if (!bare_ui_kit__read_double(env, argv[1], "x", &x)) return NULL;

      double y;
      if (!bare_ui_kit__read_double(env, argv[2], "y", &y)) return NULL;

      view.contentOffset = CGPointMake(x, y);
    }
  }

  return result;
}

#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

enum {
  bare_ui_kit_scroll_view_event_did_scroll = 1 << 0,
};

// A scroll view reports through its delegate, and it is its own, which is what
// every other class here does with the protocol it answers to.
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

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
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

    result = bare_foundation__bridge(env, handle);

    handle->env = env;

    err = js_create_reference(env, argv[4], 0, &handle->ctx);
    assert(err == 0);

    handle.delegate = handle;
  }

  return result;
}

// A `UIScrollView` does not take its content size from its subviews, unlike
// the `NSScrollView` it corresponds to, so what there is to scroll has to be
// stated.
static js_value_t *
bare_ui_kit_scroll_view_content_size(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

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
bare_ui_kit_scroll_view_content_offset(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

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

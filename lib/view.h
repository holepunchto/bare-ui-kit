#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// A plain `UIView` cannot report touches, so the base view is a subclass. The
// delivery mask keeps a view that listens to nothing as cheap as the plain one
// it replaces.
enum {
  bare_ui_kit_view_event_touches_began = 1 << 0,
  bare_ui_kit_view_event_touches_moved = 1 << 1,
  bare_ui_kit_view_event_touches_ended = 1 << 2,
  bare_ui_kit_view_event_touches_cancelled = 1 << 3,
};

// UIKit hands back the same `UITouch` for as long as a finger is down but
// never a number for it, where every other platform reports one. The slot a
// touch occupies here is that number.
enum {
  bare_ui_kit_touches_max = 16
};

static UITouch *bare_ui_kit__touches[bare_ui_kit_touches_max];

static int32_t
bare_ui_kit__touch(UITouch *touch) {
  int32_t vacant = -1;

  for (int32_t i = 0; i < bare_ui_kit_touches_max; i++) {
    if (bare_ui_kit__touches[i] == touch) return i;

    if (bare_ui_kit__touches[i] == nil && vacant == -1) vacant = i;
  }

  if (vacant != -1) bare_ui_kit__touches[vacant] = touch;

  return vacant;
}

static void
bare_ui_kit__touch_release(UITouch *touch) {
  for (int32_t i = 0; i < bare_ui_kit_touches_max; i++) {
    if (bare_ui_kit__touches[i] == touch) {
      bare_ui_kit__touches[i] = nil;
      return;
    }
  }
}

@interface BareView : UIView <BareEventTarget> {
@public
  js_env_t *env;
  js_ref_t *ctx;

  int32_t mask;
}

@end

@implementation BareView

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

// UIKit reports the touches that changed together, and every other platform
// reports one pointer at a time, so the set is unrolled here.
- (void)bareEmit:(int32_t)type touches:(NSSet<UITouch *> *)touches {
  bool last = type == bare_ui_kit_view_event_touches_ended || type == bare_ui_kit_view_event_touches_cancelled;

  for (UITouch *touch in touches) {
    CGPoint point = [touch locationInView:self];

    int32_t pointer = bare_ui_kit__touch(touch);

    if (last) bare_ui_kit__touch_release(touch);

    bare_ui_kit__emit_event(env, ctx, "_onevent", type, point.x, point.y, pointer);
  }
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  if ((mask & bare_ui_kit_view_event_touches_began) == 0) return [super touchesBegan:touches withEvent:event];

  [self bareEmit:bare_ui_kit_view_event_touches_began touches:touches];
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  if ((mask & bare_ui_kit_view_event_touches_moved) == 0) return [super touchesMoved:touches withEvent:event];

  [self bareEmit:bare_ui_kit_view_event_touches_moved touches:touches];
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  if ((mask & bare_ui_kit_view_event_touches_ended) == 0) return [super touchesEnded:touches withEvent:event];

  [self bareEmit:bare_ui_kit_view_event_touches_ended touches:touches];
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
  if ((mask & bare_ui_kit_view_event_touches_cancelled) == 0) return [super touchesCancelled:touches withEvent:event];

  [self bareEmit:bare_ui_kit_view_event_touches_cancelled touches:touches];
}

@end

static js_value_t *
bare_ui_kit_view_init(js_env_t *env, js_callback_info_t *info) {
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
    BareView *handle = [[[BareView alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation__bridge(env, handle);

    handle->env = env;

    // Weak, so that the native object does not keep its own JS wrapper
    // alive. Events are dropped once the wrapper has been collected.
    err = js_create_reference(env, argv[4], 0, &handle->ctx);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_frame(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 5);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_rect(env, view.frame);
    } else {
      double x;
      if (!bare_ui_kit__read_double(env, argv[1], "x", &x)) return NULL;

      double y;
      if (!bare_ui_kit__read_double(env, argv[2], "y", &y)) return NULL;

      double width;
      if (!bare_ui_kit__read_double(env, argv[3], "width", &width)) return NULL;

      double height;
      if (!bare_ui_kit__read_double(env, argv[4], "height", &height)) return NULL;

      view.frame = CGRectMake(x, y, width, height);
    }
  }

  return result;
}

static void
bare_ui_kit_view_frame_typed(js_value_t *receiver, int32_t bare_tag, double x, double y, double width, double height, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.frame = CGRectMake(x, y, width, height);
  }
}

static js_value_t *
bare_ui_kit_view_frame_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  uint32_t offset;
  if (!bare_ui_kit__read_uint32(env, argv[2], "offset", &offset)) return NULL;

  double *out;

  if (!bare_ui_kit__buffer(env, argv[1], offset, 4, &out)) {
    err = js_throw_type_error(env, NULL, "Expected an array buffer with room for 4 doubles");
    assert(err == 0);

    return NULL;
  }

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    CGRect value = view.frame;

    out[0] = value.origin.x;
    out[1] = value.origin.y;
    out[2] = value.size.width;
    out[3] = value.size.height;
  }

  return NULL;
}

static void
bare_ui_kit_view_frame_into_typed(js_value_t *receiver, int32_t bare_tag, js_value_t *bare_buffer, uint32_t bare_offset, js_typed_callback_info_t *info) {
  int err;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, NULL);
  assert(err == 0);

  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  double *out;

  if (!bare_ui_kit__buffer(env, bare_buffer, bare_offset, 4, &out)) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    CGRect value = view.frame;

    out[0] = value.origin.x;
    out[1] = value.origin.y;
    out[2] = value.size.width;
    out[3] = value.size.height;
  }
}

static js_value_t *
bare_ui_kit_view_bounds(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 5);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_rect(env, view.bounds);
    } else {
      double x;
      if (!bare_ui_kit__read_double(env, argv[1], "x", &x)) return NULL;

      double y;
      if (!bare_ui_kit__read_double(env, argv[2], "y", &y)) return NULL;

      double width;
      if (!bare_ui_kit__read_double(env, argv[3], "width", &width)) return NULL;

      double height;
      if (!bare_ui_kit__read_double(env, argv[4], "height", &height)) return NULL;

      view.bounds = CGRectMake(x, y, width, height);
    }
  }

  return result;
}

static void
bare_ui_kit_view_bounds_typed(js_value_t *receiver, int32_t bare_tag, double x, double y, double width, double height, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.bounds = CGRectMake(x, y, width, height);
  }
}

static js_value_t *
bare_ui_kit_view_bounds_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  uint32_t offset;
  if (!bare_ui_kit__read_uint32(env, argv[2], "offset", &offset)) return NULL;

  double *out;

  if (!bare_ui_kit__buffer(env, argv[1], offset, 4, &out)) {
    err = js_throw_type_error(env, NULL, "Expected an array buffer with room for 4 doubles");
    assert(err == 0);

    return NULL;
  }

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    CGRect value = view.bounds;

    out[0] = value.origin.x;
    out[1] = value.origin.y;
    out[2] = value.size.width;
    out[3] = value.size.height;
  }

  return NULL;
}

static void
bare_ui_kit_view_bounds_into_typed(js_value_t *receiver, int32_t bare_tag, js_value_t *bare_buffer, uint32_t bare_offset, js_typed_callback_info_t *info) {
  int err;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, NULL);
  assert(err == 0);

  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  double *out;

  if (!bare_ui_kit__buffer(env, bare_buffer, bare_offset, 4, &out)) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    CGRect value = view.bounds;

    out[0] = value.origin.x;
    out[1] = value.origin.y;
    out[2] = value.size.width;
    out[3] = value.size.height;
  }
}

static js_value_t *
bare_ui_kit_view_hidden(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, view.hidden, &result);
      assert(err == 0);
    } else {
      bool hidden;
      if (!bare_ui_kit__read_bool(env, argv[1], "hidden", &hidden)) return NULL;

      view.hidden = hidden;
    }
  }

  return result;
}

static void
bare_ui_kit_view_hidden_typed(js_value_t *receiver, int32_t bare_tag, bool hidden, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.hidden = hidden;
  }
}

static js_value_t *
bare_ui_kit_view_alpha(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      err = js_create_double(env, view.alpha, &result);
      assert(err == 0);
    } else {
      double alpha;
      if (!bare_ui_kit__read_double(env, argv[1], "alpha", &alpha)) return NULL;

      view.alpha = alpha;
    }
  }

  return result;
}

static void
bare_ui_kit_view_alpha_typed(js_value_t *receiver, int32_t bare_tag, double alpha, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.alpha = alpha;
  }
}

// Focus is a view's to take on UIKit, where on AppKit it is the window's to
// give, so these are methods rather than something done to the view.
// What a view would like to be at most this big, which is what a measure
// function asks a control that lays its own content out.
static js_value_t *
bare_ui_kit_view_size_that_fits_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 5);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  double width;
  if (!bare_ui_kit__read_double(env, argv[1], "width", &width)) return NULL;

  double height;
  if (!bare_ui_kit__read_double(env, argv[2], "height", &height)) return NULL;

  uint32_t offset;
  if (!bare_ui_kit__read_uint32(env, argv[4], "offset", &offset)) return NULL;

  double *out;

  if (!bare_ui_kit__buffer(env, argv[3], offset, 2, &out)) {
    err = js_throw_type_error(env, NULL, "Expected an array buffer with room for 2 doubles");
    assert(err == 0);

    return NULL;
  }

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    CGSize size = [view sizeThatFits:CGSizeMake(width, height)];

    out[0] = size.width;
    out[1] = size.height;
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_become_first_responder(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    err = js_get_boolean(env, [view becomeFirstResponder], &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_resign_first_responder(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    err = js_get_boolean(env, [view resignFirstResponder], &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_is_first_responder(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    err = js_get_boolean(env, view.isFirstResponder, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_content_mode(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) view.contentMode, &result);
      assert(err == 0);
    } else {
      int32_t content_mode;
      if (!bare_ui_kit__read_int32(env, argv[1], "content_mode", &content_mode)) return NULL;

      view.contentMode = content_mode;
    }
  }

  return result;
}

static void
bare_ui_kit_view_content_mode_typed(js_value_t *receiver, int32_t bare_tag, int32_t content_mode, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.contentMode = content_mode;
  }
}

static js_value_t *
bare_ui_kit_view_clips_to_bounds(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, view.clipsToBounds, &result);
      assert(err == 0);
    } else {
      bool clips_to_bounds;
      if (!bare_ui_kit__read_bool(env, argv[1], "clips_to_bounds", &clips_to_bounds)) return NULL;

      view.clipsToBounds = clips_to_bounds;
    }
  }

  return result;
}

static void
bare_ui_kit_view_clips_to_bounds_typed(js_value_t *receiver, int32_t bare_tag, bool clips_to_bounds, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.clipsToBounds = clips_to_bounds;
  }
}

static js_value_t *
bare_ui_kit_view_user_interaction_enabled(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, view.userInteractionEnabled, &result);
      assert(err == 0);
    } else {
      bool enabled;
      if (!bare_ui_kit__read_bool(env, argv[1], "enabled", &enabled)) return NULL;

      view.userInteractionEnabled = enabled;
    }
  }

  return result;
}

static void
bare_ui_kit_view_user_interaction_enabled_typed(js_value_t *receiver, int32_t bare_tag, bool enabled, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.userInteractionEnabled = enabled;
  }
}

static js_value_t *
bare_ui_kit_view_background_color(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, view.backgroundColor);
    } else {
      view.backgroundColor = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_superview(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    result = bare_foundation__bridge(env, view.superview);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_subviews(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    result = bare_ui_kit__from_objects(env, view.subviews);
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
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  void *subview_handle;
  if (!bare_foundation__read_tag(env, argv[1], "subview_handle", &subview_handle)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;
    UIView *subview = (__bridge UIView *) subview_handle;

    [view addSubview:subview];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_insert_subview_at_index(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  void *subview_handle;
  if (!bare_foundation__read_tag(env, argv[1], "subview_handle", &subview_handle)) return NULL;

  int32_t index;
  if (!bare_ui_kit__read_int32(env, argv[2], "index", &index)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;
    UIView *subview = (__bridge UIView *) subview_handle;

    [view insertSubview:subview atIndex:index];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_insert_subview_below_subview(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  void *subview_handle;
  if (!bare_foundation__read_tag(env, argv[1], "subview_handle", &subview_handle)) return NULL;

  void *sibling_handle;
  if (!bare_foundation__read_tag(env, argv[2], "sibling_handle", &sibling_handle)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;
    UIView *subview = (__bridge UIView *) subview_handle;
    UIView *sibling = (__bridge UIView *) sibling_handle;

    [view insertSubview:subview belowSubview:sibling];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_insert_subview_above_subview(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  void *subview_handle;
  if (!bare_foundation__read_tag(env, argv[1], "subview_handle", &subview_handle)) return NULL;

  void *sibling_handle;
  if (!bare_foundation__read_tag(env, argv[2], "sibling_handle", &sibling_handle)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;
    UIView *subview = (__bridge UIView *) subview_handle;
    UIView *sibling = (__bridge UIView *) sibling_handle;

    [view insertSubview:subview aboveSubview:sibling];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_remove_from_superview(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    [view removeFromSuperview];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_set_needs_layout(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    [view setNeedsLayout];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_set_needs_display(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    [view setNeedsDisplay];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_size_to_fit(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    [view sizeToFit];
  }

  return NULL;
}

static js_value_t *
bare_ui_kit_view_safe_area_insets_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (!bare_foundation__read_tag(env, argv[0], "handle", &handle)) return NULL;

  uint32_t offset;
  if (!bare_ui_kit__read_uint32(env, argv[2], "offset", &offset)) return NULL;

  double *out;

  if (!bare_ui_kit__buffer(env, argv[1], offset, 4, &out)) {
    err = js_throw_type_error(env, NULL, "Expected an array buffer with room for 4 doubles");
    assert(err == 0);

    return NULL;
  }

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    UIEdgeInsets value = view.safeAreaInsets;

    out[0] = value.top;
    out[1] = value.left;
    out[2] = value.bottom;
    out[3] = value.right;
  }

  return NULL;
}

static void
bare_ui_kit_view_safe_area_insets_into_typed(js_value_t *receiver, int32_t bare_tag, js_value_t *bare_buffer, uint32_t bare_offset, js_typed_callback_info_t *info) {
  int err;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, NULL);
  assert(err == 0);

  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  double *out;

  if (!bare_ui_kit__buffer(env, bare_buffer, bare_offset, 4, &out)) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    UIEdgeInsets value = view.safeAreaInsets;

    out[0] = value.top;
    out[1] = value.left;
    out[2] = value.bottom;
    out[3] = value.right;
  }
}

#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

// A plain `UIView` cannot report touches, so the base view is a subclass. The
// mask keeps a view that nobody listens to as cheap as a plain one.
enum {
  bare_ui_kit_view_event_touches_began = 1 << 0,
  bare_ui_kit_view_event_touches_moved = 1 << 1,
  bare_ui_kit_view_event_touches_ended = 1 << 2,
  bare_ui_kit_view_event_touches_cancelled = 1 << 3,
  bare_ui_kit_view_event_trait_change = 1 << 4,
};

static int32_t
bare_ui_kit__touch(bare_ui_kit_state_t *state, UITouch *touch) {
  int32_t vacant = -1;

  for (int32_t i = 0; i < bare_ui_kit_touches_max; i++) {
    if (state->touches[i] == touch) return i;

    if (state->touches[i] == nil && vacant == -1) vacant = i;
  }

  if (vacant != -1) state->touches[vacant] = touch;

  return vacant;
}

static void
bare_ui_kit__touch_release(bare_ui_kit_state_t *state, UITouch *touch) {
  for (int32_t i = 0; i < bare_ui_kit_touches_max; i++) {
    if (state->touches[i] == touch) {
      state->touches[i] = nil;
      return;
    }
  }
}

@interface BareView : UIView <BareEventTarget> {
@public
  bare_ui_kit_state_t *state;
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

// UIKit reports the touches that changed together, and other platforms report
// one pointer at a time, so they are sent one by one.
- (void)bareEmit:(int32_t)type touches:(NSSet<UITouch *> *)touches {
  bool last = type == bare_ui_kit_view_event_touches_ended || type == bare_ui_kit_view_event_touches_cancelled;

  for (UITouch *touch in touches) {
    CGPoint point = [touch locationInView:self];

    int32_t pointer = bare_ui_kit__touch(state, touch);

    if (last) bare_ui_kit__touch_release(state, touch);

    bare_ui_kit__emit_event(env, ctx, "_onevent", type, point.x, point.y, pointer);
  }
}

// From iOS 17 a view registers for the traits it cares about, once. Before
// that, overriding is the only hook, so both are here and the version decides
// which one reports.
- (void)bareRegisterForTraitChanges {
  if (@available(iOS 17.0, *)) {
    [self registerForTraitChanges:@[ UITraitUserInterfaceStyle.class, UITraitPreferredContentSizeCategory.class ]
                      withHandler:^(BareView *view, UITraitCollection *previous) {
                        if ((view->mask & bare_ui_kit_view_event_trait_change) == 0) return;

                        bare_ui_kit__emit(view->env, view->ctx, "_ontraitchange");
                      }];
  }
}

- (void)traitCollectionDidChange:(UITraitCollection *)previous {
  [super traitCollectionDidChange:previous];

  if (@available(iOS 17.0, *)) return;

  if ((mask & bare_ui_kit_view_event_trait_change) == 0) return;

  if (previous.userInterfaceStyle == self.traitCollection.userInterfaceStyle &&
      [previous.preferredContentSizeCategory isEqualToString:self.traitCollection.preferredContentSizeCategory]) return;

  bare_ui_kit__emit(env, ctx, "_ontraitchange");
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
    BareView *handle = [[[BareView alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation_bridge(env, state->registry, handle);

    handle->state = state;
    handle->env = env;

    // Weak, so the native object does not keep its own wrapper alive. Events
    // are dropped once the wrapper is collected.
    err = js_create_reference(env, argv[4], 0, &handle->ctx);
    assert(err == 0);

    [handle bareRegisterForTraitChanges];
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_override_user_interface_style(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, view.overrideUserInterfaceStyle, &result);
      assert(err == 0);
    } else {
      int32_t style;
      if (!bare_ui_kit__read_int32(env, argv[1], "style", &style)) return NULL;

      view.overrideUserInterfaceStyle = style;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_user_interface_style(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    err = js_create_int32(env, view.traitCollection.userInterfaceStyle, &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_frame(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 5);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_typed_callback_info(info, NULL, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 5);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_typed_callback_info(info, NULL, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_typed_callback_info(info, NULL, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_typed_callback_info(info, NULL, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UIView *view = (UIView *) bare_object;

    view.alpha = alpha;
  }
}

// Asks whatever is editing inside this view to stop, which puts the keyboard
// away without knowing which field showed it.
static js_value_t *
bare_ui_kit_view_end_editing(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  bool force;
  if (!bare_ui_kit__read_bool(env, argv[1], "force", &force)) return NULL;

  js_value_t *result;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    err = js_get_boolean(env, [view endEditing:force], &result);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_convert_point_to_view(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 4);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  double x;
  if (!bare_ui_kit__read_double(env, argv[1], "x", &x)) return NULL;

  double y;
  if (!bare_ui_kit__read_double(env, argv[2], "y", &y)) return NULL;

  id target = bare_foundation_to_object(env, state->registry, argv[3]);

  js_value_t *result;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    result = bare_ui_kit__from_point(env, [view convertPoint:CGPointMake(x, y) toView:target]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_convert_point_from_view(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 4);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  double x;
  if (!bare_ui_kit__read_double(env, argv[1], "x", &x)) return NULL;

  double y;
  if (!bare_ui_kit__read_double(env, argv[2], "y", &y)) return NULL;

  id target = bare_foundation_to_object(env, state->registry, argv[3]);

  js_value_t *result;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    result = bare_ui_kit__from_point(env, [view convertPoint:CGPointMake(x, y) fromView:target]);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_size_that_fits_into(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 5;
  js_value_t *argv[5];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 5);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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

// On UIKit a view takes focus itself, where on AppKit the window gives it.
static js_value_t *
bare_ui_kit_view_become_first_responder(js_env_t *env, js_callback_info_t *info) {
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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_typed_callback_info(info, NULL, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_typed_callback_info(info, NULL, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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
  int err;

  bare_ui_kit_state_t *state;
  err = js_get_typed_callback_info(info, NULL, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  js_value_t *result = NULL;

  @autoreleasepool {
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      result = bare_foundation_bridge(env, state->registry, view.backgroundColor);
    } else {
      view.backgroundColor = bare_foundation_to_object(env, state->registry, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_tint_color(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    if (argc == 1) {
      result = bare_foundation_bridge(env, state->registry, view.tintColor);
    } else {
      view.tintColor = bare_foundation_to_object(env, state->registry, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_superview(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    result = bare_foundation_bridge(env, state->registry, view.superview);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_subviews(js_env_t *env, js_callback_info_t *info) {
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
    UIView *view = (__bridge UIView *) handle;

    result = bare_ui_kit__from_objects(env, state, view.subviews);
  }

  return result;
}

static js_value_t *
bare_ui_kit_view_add_subview(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  void *subview_handle;
  if (bare_foundation_read_tag(env, state->registry, argv[1], "subview_handle", &subview_handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  void *subview_handle;
  if (bare_foundation_read_tag(env, state->registry, argv[1], "subview_handle", &subview_handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  void *subview_handle;
  if (bare_foundation_read_tag(env, state->registry, argv[1], "subview_handle", &subview_handle) < 0) return NULL;

  void *sibling_handle;
  if (bare_foundation_read_tag(env, state->registry, argv[2], "sibling_handle", &sibling_handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

  void *subview_handle;
  if (bare_foundation_read_tag(env, state->registry, argv[1], "subview_handle", &subview_handle) < 0) return NULL;

  void *sibling_handle;
  if (bare_foundation_read_tag(env, state->registry, argv[2], "sibling_handle", &sibling_handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;
  err = js_get_callback_info(env, info, &argc, argv, NULL, (void **) &state);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  if (bare_foundation_read_tag(env, state->registry, argv[0], "handle", &handle) < 0) return NULL;

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

  bare_ui_kit_state_t *state;

  js_env_t *env;
  err = js_get_typed_callback_info(info, &env, (void **) &state);
  assert(err == 0);

  id bare_object = bare_foundation_object(state->registry, bare_tag);

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

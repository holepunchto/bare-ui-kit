#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

enum {
  bare_ui_kit_text_view_event_did_begin_editing = 1 << 0,
  bare_ui_kit_text_view_event_did_end_editing = 1 << 1,
  bare_ui_kit_text_view_event_did_change = 1 << 2,
  bare_ui_kit_text_view_event_should_change_text = 1 << 3,
  bare_ui_kit_text_view_event_did_change_selection = 1 << 4,
};

@interface BareTextView : UITextView <UITextViewDelegate, BareEventTarget> {
@public
  js_env_t *env;
  js_ref_t *ctx;

  int32_t mask;
}

@end

@implementation BareTextView

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

- (void)textViewDidBeginEditing:(UITextView *)view {
  if ((mask & bare_ui_kit_text_view_event_did_begin_editing) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidbeginediting");
}

- (void)textViewDidEndEditing:(UITextView *)view {
  if ((mask & bare_ui_kit_text_view_event_did_end_editing) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidendediting");
}

- (void)textViewDidChange:(UITextView *)view {
  if ((mask & bare_ui_kit_text_view_event_did_change) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidchange");
}

- (void)textViewDidChangeSelection:(UITextView *)view {
  if ((mask & bare_ui_kit_text_view_event_did_change_selection) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidchangeselection");
}

- (BOOL)textView:(UITextView *)view shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)string {
  if ((mask & bare_ui_kit_text_view_event_should_change_text) != 0) {
    bare_ui_kit__emit_replacement(env, ctx, "_onshouldchangetext", (int32_t) range.location, (int32_t) range.length, string);
  }

  return YES;
}

@end

static js_value_t *
bare_ui_kit_text_view_init(js_env_t *env, js_callback_info_t *info) {
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
    BareTextView *handle = [[[BareTextView alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation__bridge(env, handle);

    handle->env = env;

    err = js_create_reference(env, argv[4], 0, &handle->ctx);
    assert(err == 0);

    handle.delegate = handle;
  }

  return result;
}

// A text view keeps an `NSRange` where a field keeps opaque positions, so this
// is the one of the two that needs no conversion.
static js_value_t *
bare_ui_kit_text_view_selected_range(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_range(env, text_view.selectedRange);
    } else {
      int32_t location;
      if (!bare_ui_kit__read_int32(env, argv[1], "location", &location)) return NULL;

      int32_t length;
      if (!bare_ui_kit__read_int32(env, argv[2], "length", &length)) return NULL;

      text_view.selectedRange = NSMakeRange(location, length);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_text(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_string(env, text_view.text);
    } else {
      text_view.text = bare_ui_kit__to_string(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_font(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, text_view.font);
    } else {
      text_view.font = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_text_color(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, text_view.textColor);
    } else {
      text_view.textColor = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_text_alignment(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_view.textAlignment, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "text_alignment", &value)) return NULL;

      text_view.textAlignment = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_editable(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, text_view.editable, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "editable", &value)) return NULL;

      text_view.editable = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_keyboard_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_view.keyboardType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "keyboard_type", &value)) return NULL;

      text_view.keyboardType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_return_key_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_view.returnKeyType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "return_key_type", &value)) return NULL;

      text_view.returnKeyType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_autocapitalization_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_view.autocapitalizationType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "autocapitalization_type", &value)) return NULL;

      text_view.autocapitalizationType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_autocorrection_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_view.autocorrectionType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "autocorrection_type", &value)) return NULL;

      text_view.autocorrectionType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_view_scroll_enabled(js_env_t *env, js_callback_info_t *info) {
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
    UITextView *text_view = (__bridge UITextView *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, text_view.scrollEnabled, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "scroll_enabled", &value)) return NULL;

      text_view.scrollEnabled = value;
    }
  }

  return result;
}

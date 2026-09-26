#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

enum {
  bare_ui_kit_text_field_event_did_begin_editing = 1 << 0,
  bare_ui_kit_text_field_event_did_end_editing = 1 << 1,
  bare_ui_kit_text_field_event_did_change = 1 << 2,
  bare_ui_kit_text_field_event_should_change_characters = 1 << 3,
  bare_ui_kit_text_field_event_should_return = 1 << 4,
  bare_ui_kit_text_field_event_did_change_selection = 1 << 5,
};

@interface BareTextField : UITextField <UITextFieldDelegate, BareEventTarget> {
@public
  js_env_t *env;
  js_ref_t *ctx;

  int32_t mask;
}

@end

@implementation BareTextField

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

- (void)textFieldDidBeginEditing:(UITextField *)field {
  if ((mask & bare_ui_kit_text_field_event_did_begin_editing) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidbeginediting");
}

- (void)textFieldDidEndEditing:(UITextField *)field {
  if ((mask & bare_ui_kit_text_field_event_did_end_editing) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidendediting");
}

- (void)textFieldDidChangeSelection:(UITextField *)field {
  if ((mask & bare_ui_kit_text_field_event_did_change_selection) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidchangeselection");
}

// UIKit reports an edit before it happens and a change after it, and both are
// wanted: the first is what is about to replace what, the second is that the
// text now differs.
- (BOOL)textField:(UITextField *)field shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string {
  if ((mask & bare_ui_kit_text_field_event_should_change_characters) != 0) {
    bare_ui_kit__emit_replacement(env, ctx, "_onshouldchangecharacters", (int32_t) range.location, (int32_t) range.length, string);
  }

  return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)field {
  if ((mask & bare_ui_kit_text_field_event_should_return) != 0) {
    bare_ui_kit__emit(env, ctx, "_onshouldreturn");
  }

  return YES;
}

- (void)bareEditingChanged:(id)sender {
  if ((mask & bare_ui_kit_text_field_event_did_change) == 0) return;

  bare_ui_kit__emit(env, ctx, "_ondidchange");
}

@end

static js_value_t *
bare_ui_kit_text_field_init(js_env_t *env, js_callback_info_t *info) {
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
    BareTextField *handle = [[[BareTextField alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation__bridge(env, handle);

    handle->env = env;

    err = js_create_reference(env, argv[4], 0, &handle->ctx);
    assert(err == 0);

    handle.delegate = handle;

    // The delegate hears about an edit before it happens; that the text has
    // changed is a control event, as it is for every other `UIControl`.
    [handle addTarget:handle
               action:@selector(bareEditingChanged:)
     forControlEvents:UIControlEventEditingChanged];
  }

  return result;
}

// A field's selection is a pair of opaque positions, and `UITextInput` is what
// turns them into offsets, so the conversion happens here rather than leaving
// a caller holding positions it can do nothing else with.
static js_value_t *
bare_ui_kit_text_field_selected_range(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      UITextRange *range = text_field.selectedTextRange;

      NSInteger location = 0;
      NSInteger length = 0;

      if (range != nil) {
        location = [text_field offsetFromPosition:text_field.beginningOfDocument toPosition:range.start];
        length = [text_field offsetFromPosition:range.start toPosition:range.end];
      }

      result = bare_ui_kit__from_range(env, NSMakeRange(location, length));
    } else {
      int32_t location;
      if (!bare_ui_kit__read_int32(env, argv[1], "location", &location)) return NULL;

      int32_t length;
      if (!bare_ui_kit__read_int32(env, argv[2], "length", &length)) return NULL;

      UITextPosition *start = [text_field positionFromPosition:text_field.beginningOfDocument offset:location];
      UITextPosition *end = [text_field positionFromPosition:start offset:length];

      if (start != nil && end != nil) {
        text_field.selectedTextRange = [text_field textRangeFromPosition:start toPosition:end];
      }
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_text(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_string(env, text_field.text);
    } else {
      text_field.text = bare_ui_kit__to_string(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_placeholder(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_string(env, text_field.placeholder);
    } else {
      text_field.placeholder = bare_ui_kit__to_string(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_font(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, text_field.font);
    } else {
      text_field.font = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_text_color(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, text_field.textColor);
    } else {
      text_field.textColor = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_text_alignment(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_field.textAlignment, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "text_alignment", &value)) return NULL;

      text_field.textAlignment = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_secure_text_entry(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, text_field.secureTextEntry, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "secure_text_entry", &value)) return NULL;

      text_field.secureTextEntry = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_keyboard_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_field.keyboardType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "keyboard_type", &value)) return NULL;

      text_field.keyboardType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_return_key_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_field.returnKeyType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "return_key_type", &value)) return NULL;

      text_field.returnKeyType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_autocapitalization_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_field.autocapitalizationType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "autocapitalization_type", &value)) return NULL;

      text_field.autocapitalizationType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_autocorrection_type(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) text_field.autocorrectionType, &result);
      assert(err == 0);
    } else {
      int32_t value;
      if (!bare_ui_kit__read_int32(env, argv[1], "autocorrection_type", &value)) return NULL;

      text_field.autocorrectionType = value;
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_text_field_enabled(js_env_t *env, js_callback_info_t *info) {
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
    UITextField *text_field = (__bridge UITextField *) handle;

    if (argc == 1) {
      err = js_get_boolean(env, text_field.enabled, &result);
      assert(err == 0);
    } else {
      bool value;
      if (!bare_ui_kit__read_bool(env, argv[1], "enabled", &value)) return NULL;

      text_field.enabled = value;
    }
  }

  return result;
}

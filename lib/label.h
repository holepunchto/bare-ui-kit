#import <assert.h>
#import <bare.h>
#import <js.h>

#import <UIKit/UIKit.h>

#import "bridging.h"

static js_value_t *
bare_ui_kit_label_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 4;
  js_value_t *argv[4];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 4);

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
    UILabel *handle = [[[UILabel alloc]
      initWithFrame:CGRectMake(x, y, width, height)] autorelease];

    result = bare_foundation__bridge(env, handle);
  }

  return result;
}

static js_value_t *
bare_ui_kit_label_text(js_env_t *env, js_callback_info_t *info) {
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
    UILabel *label = (__bridge UILabel *) handle;

    if (argc == 1) {
      result = bare_ui_kit__from_string(env, label.text);
    } else {
      label.text = bare_ui_kit__to_string(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_label_font(js_env_t *env, js_callback_info_t *info) {
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
    UILabel *label = (__bridge UILabel *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, label.font);
    } else {
      label.font = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_label_text_color(js_env_t *env, js_callback_info_t *info) {
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
    UILabel *label = (__bridge UILabel *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, label.textColor);
    } else {
      label.textColor = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

static js_value_t *
bare_ui_kit_label_text_alignment(js_env_t *env, js_callback_info_t *info) {
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
    UILabel *label = (__bridge UILabel *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) label.textAlignment, &result);
      assert(err == 0);
    } else {
      int32_t text_alignment;
      if (!bare_ui_kit__read_int32(env, argv[1], "text_alignment", &text_alignment)) return NULL;

      label.textAlignment = text_alignment;
    }
  }

  return result;
}

static void
bare_ui_kit_label_text_alignment_typed(js_value_t *receiver, int32_t bare_tag, int32_t text_alignment, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UILabel *label = (UILabel *) bare_object;

    label.textAlignment = text_alignment;
  }
}

static js_value_t *
bare_ui_kit_label_number_of_lines(js_env_t *env, js_callback_info_t *info) {
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
    UILabel *label = (__bridge UILabel *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) label.numberOfLines, &result);
      assert(err == 0);
    } else {
      int32_t number_of_lines;
      if (!bare_ui_kit__read_int32(env, argv[1], "number_of_lines", &number_of_lines)) return NULL;

      label.numberOfLines = number_of_lines;
    }
  }

  return result;
}

static void
bare_ui_kit_label_number_of_lines_typed(js_value_t *receiver, int32_t bare_tag, int32_t number_of_lines, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UILabel *label = (UILabel *) bare_object;

    label.numberOfLines = number_of_lines;
  }
}

static js_value_t *
bare_ui_kit_label_line_break_mode(js_env_t *env, js_callback_info_t *info) {
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
    UILabel *label = (__bridge UILabel *) handle;

    if (argc == 1) {
      err = js_create_int32(env, (int32_t) label.lineBreakMode, &result);
      assert(err == 0);
    } else {
      int32_t line_break_mode;
      if (!bare_ui_kit__read_int32(env, argv[1], "line_break_mode", &line_break_mode)) return NULL;

      label.lineBreakMode = line_break_mode;
    }
  }

  return result;
}

static void
bare_ui_kit_label_line_break_mode_typed(js_value_t *receiver, int32_t bare_tag, int32_t line_break_mode, js_typed_callback_info_t *info) {
  id bare_object = bare_foundation__object(bare_tag);

  if (bare_object == nil) return;

  @autoreleasepool {
    UILabel *label = (UILabel *) bare_object;

    label.lineBreakMode = line_break_mode;
  }
}


static js_value_t *
bare_ui_kit_label_attributed_text(js_env_t *env, js_callback_info_t *info) {
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
    UILabel *label = (__bridge UILabel *) handle;

    if (argc == 1) {
      result = bare_foundation__bridge(env, label.attributedText);
    } else {
      label.attributedText = bare_foundation__to_object(env, argv[1]);
    }
  }

  return result;
}

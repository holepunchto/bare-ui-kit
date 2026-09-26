#pragma once

#import <assert.h>
#import <js.h>
#import <stdlib.h>
#import <utf.h>

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

// The tag registry and the protocol for passing an object to another addon.
#import "registry.h"

@protocol BareEventTarget

- (int32_t)eventMask;
- (void)setEventMask:(int32_t)mask;

@end

// A value of the wrong type would otherwise reach V8's `Cast()` and abort the
// whole process, which leaves no JS stack to debug from. These raise an
// ordinary JS exception instead; every caller returns NULL so it propagates.
static bool
bare_ui_kit__read_number(js_env_t *env, js_value_t *value, const char *name) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type == js_number) return true;

  err = js_throw_type_errorf(env, NULL, "Expected a number for '%s'", name);
  assert(err == 0);

  return false;
}

static bool
bare_ui_kit__read_bool(js_env_t *env, js_value_t *value, const char *name, bool *result) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type != js_boolean) {
    err = js_throw_type_errorf(env, NULL, "Expected a boolean for '%s'", name);
    assert(err == 0);

    return false;
  }

  err = js_get_value_bool(env, value, result);
  assert(err == 0);

  return true;
}

static bool
bare_ui_kit__read_int32(js_env_t *env, js_value_t *value, const char *name, int32_t *result) {
  if (!bare_ui_kit__read_number(env, value, name)) return false;

  int err = js_get_value_int32(env, value, result);
  assert(err == 0);

  return true;
}

static bool
bare_ui_kit__read_uint32(js_env_t *env, js_value_t *value, const char *name, uint32_t *result) {
  if (!bare_ui_kit__read_number(env, value, name)) return false;

  int err = js_get_value_uint32(env, value, result);
  assert(err == 0);

  return true;
}

static bool
bare_ui_kit__read_double(js_env_t *env, js_value_t *value, const char *name, double *result) {
  if (!bare_ui_kit__read_number(env, value, name)) return false;

  int err = js_get_value_double(env, value, result);
  assert(err == 0);

  return true;
}

// A getter that returns a struct has to build a JS object, which is the most
// expensive thing a binding does and cannot happen in a typed callback. The
// caller supplies the storage instead, and the components are written into it.
// Reading an array buffer's backing store needs no handle scope.
static bool
bare_ui_kit__buffer(js_env_t *env, js_value_t *value, uint32_t offset, uint32_t count, double **result) {
  int err;

  bool is_arraybuffer;
  err = js_is_arraybuffer(env, value, &is_arraybuffer);
  assert(err == 0);

  if (!is_arraybuffer) return false;

  void *data;
  size_t len;
  err = js_get_arraybuffer_info(env, value, &data, &len);
  assert(err == 0);

  if ((uint64_t) offset + (uint64_t) count * sizeof(double) > len) return false;

  *result = (double *) ((uint8_t *) data + offset);

  return true;
}

static js_value_t *
bare_ui_kit__from_string(js_env_t *env, NSString *string) {
  int err;

  js_value_t *result;

  if (string == nil) {
    err = js_get_null(env, &result);
    assert(err == 0);
  } else {
    err = js_create_string_utf8(env, (const utf8_t *) [string UTF8String], -1, &result);
    assert(err == 0);
  }

  return result;
}

static NSString *
bare_ui_kit__to_string(js_env_t *env, js_value_t *value) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type == js_null || type == js_undefined) return nil;

  size_t len;
  err = js_get_value_string_utf8(env, value, NULL, 0, &len);
  assert(err == 0);

  len += 1 /* NULL */;

  char *data = malloc(len);

  err = js_get_value_string_utf8(env, value, (utf8_t *) data, len, &len);
  assert(err == 0);

  NSString *result = [NSString stringWithUTF8String:data];

  free(data);

  return result;
}

static js_value_t *
bare_ui_kit__from_rect(js_env_t *env, CGRect rect) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("x", rect.origin.x)
  V("y", rect.origin.y)
  V("width", rect.size.width)
  V("height", rect.size.height)
#undef V

  return result;
}

static js_value_t *
bare_ui_kit__from_range(js_env_t *env, NSRange range) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, (int32_t) n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("location", range.location)
  V("length", range.length)
#undef V

  return result;
}

static js_value_t *
bare_ui_kit__from_size(js_env_t *env, CGSize size) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("width", size.width)
  V("height", size.height)
#undef V

  return result;
}

static js_value_t *
bare_ui_kit__from_point(js_env_t *env, CGPoint point) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("x", point.x)
  V("y", point.y)
#undef V

  return result;
}

// The attributes a run of text can carry. Only the keys bound here cross the
// boundary; anything else in the object is left where it is.
#define BARE_UI_KIT_ATTRIBUTES(V) \
  V("font", NSFontAttributeName) \
  V("foregroundColor", NSForegroundColorAttributeName) \
  V("backgroundColor", NSBackgroundColorAttributeName) \
  V("paragraphStyle", NSParagraphStyleAttributeName)

// The ones that are numbers rather than objects, which cross as numbers and
// would otherwise be dropped on the way.
#define BARE_UI_KIT_ATTRIBUTES_NUMBER(V) \
  V("kern", NSKernAttributeName) \
  V("baselineOffset", NSBaselineOffsetAttributeName) \
  V("underlineStyle", NSUnderlineStyleAttributeName) \
  V("strikethroughStyle", NSStrikethroughStyleAttributeName)

static js_value_t *
bare_ui_kit__from_attributes(js_env_t *env, NSDictionary<NSAttributedStringKey, id> *attributes) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(key, name) \
  if (attributes[name] != nil) { \
    err = js_set_named_property(env, result, key, bare_foundation__bridge(env, attributes[name])); \
    assert(err == 0); \
  }
  BARE_UI_KIT_ATTRIBUTES(V)
#undef V

  return result;
}

static NSDictionary<NSAttributedStringKey, id> *
bare_ui_kit__to_attributes(js_env_t *env, js_value_t *value) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  NSMutableDictionary<NSAttributedStringKey, id> *result = [NSMutableDictionary dictionary];

  if (type != js_object) return result;

#define V(key, name) \
  { \
    js_value_t *val; \
    err = js_get_named_property(env, value, key, &val); \
    assert(err == 0); \
    id object = bare_foundation__to_object(env, val); \
    if (object != nil) result[name] = object; \
  }
  BARE_UI_KIT_ATTRIBUTES(V)
#undef V

#define V(key, name) \
  { \
    js_value_t *val; \
    err = js_get_named_property(env, value, key, &val); \
    assert(err == 0); \
    js_value_type_t t; \
    err = js_typeof(env, val, &t); \
    assert(err == 0); \
    if (t == js_number) { \
      double n; \
      err = js_get_value_double(env, val, &n); \
      assert(err == 0); \
      result[name] = @(n); \
    } \
  }
  BARE_UI_KIT_ATTRIBUTES_NUMBER(V)
#undef V

  return result;
}

static js_value_t *
bare_ui_kit__from_objects(js_env_t *env, NSArray *objects) {
  int err;

  js_value_t *result;
  err = js_create_array_with_length(env, objects.count, &result);
  assert(err == 0);

  for (NSUInteger i = 0, n = objects.count; i < n; i++) {
    err = js_set_element(env, result, (uint32_t) i, bare_foundation__bridge(env, objects[i]));
    assert(err == 0);
  }

  return result;
}

static bool
bare_ui_kit__handler(js_env_t *env, js_value_t *receiver, const char *name, js_value_t **result) {
  int err = js_get_named_property(env, receiver, name, result);
  assert(err == 0);

  js_value_type_t type;
  err = js_typeof(env, *result, &type);
  assert(err == 0);

  return type == js_function;
}

static void
bare_ui_kit__emit(js_env_t *env, js_ref_t *ctx, const char *name) {
  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  // The context is weak, so it is empty once the JS wrapper has been collected.
  js_value_t *fn;

  if (receiver != NULL && bare_ui_kit__handler(env, receiver, name, &fn)) {
    err = js_call_function(env, receiver, fn, 0, NULL, NULL);
    (void) err;
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

// An event carries its payload positionally rather than as an object, because
// a move reports on every frame of a gesture and an object per report is an
// allocation per frame.
static void
bare_ui_kit__emit_event(js_env_t *env, js_ref_t *ctx, const char *name, int32_t type, double x, double y, int32_t pointer) {
  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *fn;

  if (receiver != NULL && bare_ui_kit__handler(env, receiver, name, &fn)) {
    js_value_t *argv[4];

    err = js_create_int32(env, type, &argv[0]);
    assert(err == 0);

    err = js_create_double(env, x, &argv[1]);
    assert(err == 0);

    err = js_create_double(env, y, &argv[2]);
    assert(err == 0);

    err = js_create_int32(env, pointer, &argv[3]);
    assert(err == 0);

    err = js_call_function(env, receiver, fn, 4, argv, NULL);
    (void) err;
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

// A range and the string about to replace it, which is what the text objects
// report about an edit before it happens.
static void
bare_ui_kit__emit_replacement(js_env_t *env, js_ref_t *ctx, const char *name, int32_t location, int32_t length, NSString *string) {
  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *fn;

  if (receiver != NULL && bare_ui_kit__handler(env, receiver, name, &fn)) {
    js_value_t *argv[3];

    err = js_create_int32(env, location, &argv[0]);
    assert(err == 0);

    err = js_create_int32(env, length, &argv[1]);
    assert(err == 0);

    argv[2] = bare_ui_kit__from_string(env, string);

    err = js_call_function(env, receiver, fn, 3, argv, NULL);
    (void) err;
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

static void
bare_ui_kit__emit_rect(js_env_t *env, js_ref_t *ctx, const char *name, CGRect rect) {
  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *fn;

  if (receiver != NULL && bare_ui_kit__handler(env, receiver, name, &fn)) {
    js_value_t *argv[4];

    err = js_create_double(env, rect.origin.x, &argv[0]);
    assert(err == 0);

    err = js_create_double(env, rect.origin.y, &argv[1]);
    assert(err == 0);

    err = js_create_double(env, rect.size.width, &argv[2]);
    assert(err == 0);

    err = js_create_double(env, rect.size.height, &argv[3]);
    assert(err == 0);

    err = js_call_function(env, receiver, fn, 4, argv, NULL);
    (void) err;
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

static void
bare_ui_kit__emit_point(js_env_t *env, js_ref_t *ctx, const char *name, double x, double y) {
  int err;

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *fn;

  if (receiver != NULL && bare_ui_kit__handler(env, receiver, name, &fn)) {
    js_value_t *argv[2];

    err = js_create_double(env, x, &argv[0]);
    assert(err == 0);

    err = js_create_double(env, y, &argv[1]);
    assert(err == 0);

    err = js_call_function(env, receiver, fn, 2, argv, NULL);
    (void) err;
  }

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

static js_value_t *
bare_ui_kit_event_mask(js_env_t *env, js_callback_info_t *info) {
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
    id object = (__bridge id) handle;

    // An object backed by a plain framework class has nothing to deliver;
    // listening to it is silently inert.
    if (![object conformsToProtocol:@protocol(BareEventTarget)]) return NULL;

    id<BareEventTarget> target = object;

    if (argc == 1) {
      err = js_create_int32(env, target.eventMask, &result);
      assert(err == 0);
    } else {
      int32_t mask;
      if (!bare_ui_kit__read_int32(env, argv[1], "mask", &mask)) return NULL;

      target.eventMask = mask;
    }
  }

  return result;
}

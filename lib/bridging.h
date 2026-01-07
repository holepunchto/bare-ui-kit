#import <js.h>

#import <Foundation/Foundation.h>

static void
bare_ui_kit__on_bridged_release(js_env_t *env, void *data, void *finalize_hint) {
  CFBridgingRelease(data);
}

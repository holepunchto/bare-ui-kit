#import <assert.h>
#import <bare.h>
#import <js.h>
#import <utf.h>

#import <UIKit/UIKit.h>

#import "lib/view.h"
#import "lib/view-controller.h"
#import "lib/window.h"

static js_value_t *
bare_ui_kit_exports(js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("viewHidden", bare_ui_kit_view_hidden)
  V("viewAddSubview", bare_ui_kit_view_add_subview)

  V("viewControllerInit", bare_ui_kit_view_controller_init)
  V("viewControllerView", bare_ui_kit_view_controller_view)

  V("windowInit", bare_ui_kit_window_init)
  V("windowRootViewController", bare_ui_kit_window_root_view_controller)
  V("windowMakeKeyWindow", bare_ui_kit_window_make_key_window)
#undef V

  return exports;
}

BARE_MODULE(bare_ui_kit, bare_ui_kit_exports)

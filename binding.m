#import <assert.h>
#import <bare.h>
#import <js.h>
#import <utf.h>

#import <UIKit/UIKit.h>

#import "lib/attributed-string.h"
#import "lib/color.h"
#import "lib/font.h"
#import "lib/font-descriptor.h"
#import "lib/image.h"
#import "lib/image-view.h"
#import "lib/bezier-path.h"
#import "lib/label.h"
#import "lib/paragraph-style.h"
#import "lib/scene.h"
#import "lib/screen.h"
#import "lib/scroll-view.h"
#import "lib/activity-indicator-view.h"
#import "lib/switch.h"
#import "lib/text-field.h"
#import "lib/text-view.h"
#import "lib/view.h"
#import "lib/view-controller.h"
#import "lib/window.h"

static js_value_t *
bare_ui_kit_exports(js_env_t *env, js_value_t *exports) {
  int err;

#define T(name, fn, signature, typed) \
  { \
    js_value_t *val; \
    err = js_create_typed_function(env, name, -1, fn, signature, typed, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("claim", bare_foundation_claim)
  V("wrapper", bare_foundation_wrapper)
  V("registrySize", bare_foundation_registry_size)
  V("eventMask", bare_ui_kit_event_mask)
  V("handle", bare_foundation_handle)
  V("adopt", bare_foundation_adopt)

  V("colorSystem", bare_ui_kit_color_system)
  V("colorRGB", bare_ui_kit_color_rgb)
  V("colorHSB", bare_ui_kit_color_hsb)
  V("colorWhite", bare_ui_kit_color_white)
  V("colorWithAlphaComponent", bare_ui_kit_color_with_alpha_component)
  T(
    "colorComponentsInto",
    bare_ui_kit_color_components_into,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 4,
      .args = (int[]){js_object, js_int32, js_object, js_uint32},
    }),
    bare_ui_kit_color_components_into_typed
  )

  V("fontSystemFont", bare_ui_kit_font_system_font)
  V("fontBoldSystemFont", bare_ui_kit_font_bold_system_font)
  V("fontMonospacedSystemFont", bare_ui_kit_font_monospaced_system_font)
  V("fontWithName", bare_ui_kit_font_with_name)
  V("fontFontName", bare_ui_kit_font_font_name)
  V("fontFamilyName", bare_ui_kit_font_family_name)
  V("fontPointSize", bare_ui_kit_font_point_size)
  V("fontAscender", bare_ui_kit_font_ascender)
  V("fontDescender", bare_ui_kit_font_descender)
  V("fontCapHeight", bare_ui_kit_font_cap_height)
  V("fontXHeight", bare_ui_kit_font_x_height)
  V("fontLeading", bare_ui_kit_font_leading)
  V("fontLineHeight", bare_ui_kit_font_line_height)
  V("fontWithDescriptor", bare_ui_kit_font_with_descriptor)
  V("fontFontDescriptor", bare_ui_kit_font_font_descriptor)

  V("fontDescriptorSymbolicTraits", bare_ui_kit_font_descriptor_symbolic_traits)
  V("fontDescriptorWithSymbolicTraits", bare_ui_kit_font_descriptor_with_symbolic_traits)
  V("fontDescriptorWithFamily", bare_ui_kit_font_descriptor_with_family)

  V("attributedStringInit", bare_ui_kit_attributed_string_init)
  V("attributedStringString", bare_ui_kit_attributed_string_string)
  V("attributedStringLength", bare_ui_kit_attributed_string_length)
  V("attributedStringAttributesAt", bare_ui_kit_attributed_string_attributes_at)
  V("attributedStringAppend", bare_ui_kit_attributed_string_append)
  V("attributedStringAppendString", bare_ui_kit_attributed_string_append_string)

  V("paragraphStyleInit", bare_ui_kit_paragraph_style_init)
  V("paragraphStyleAlignment", bare_ui_kit_paragraph_style_alignment)
  V("paragraphStyleLineBreakMode", bare_ui_kit_paragraph_style_line_break_mode)

  V("imageWithContentsOfFile", bare_ui_kit_image_with_contents_of_file)
  V("imageNamed", bare_ui_kit_image_named)
  T(
    "imageSizeInto",
    bare_ui_kit_image_size_into,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 4,
      .args = (int[]){js_object, js_int32, js_object, js_uint32},
    }),
    bare_ui_kit_image_size_into_typed
  )
  V("imageScale", bare_ui_kit_image_scale)

  V("imageViewInit", bare_ui_kit_image_view_init)
  V("imageViewImage", bare_ui_kit_image_view_image)

  V("activityIndicatorViewInit", bare_ui_kit_activity_indicator_view_init)
  V("activityIndicatorViewStyle", bare_ui_kit_activity_indicator_view_style)
  V("activityIndicatorViewColor", bare_ui_kit_activity_indicator_view_color)
  V("activityIndicatorViewHidesWhenStopped", bare_ui_kit_activity_indicator_view_hides_when_stopped)
  V("activityIndicatorViewAnimating", bare_ui_kit_activity_indicator_view_animating)

  V("switchInit", bare_ui_kit_switch_init)
  V("switchOn", bare_ui_kit_switch_on)
  V("switchEnabled", bare_ui_kit_switch_enabled)

  V("textFieldInit", bare_ui_kit_text_field_init)
  V("textFieldSelectedRange", bare_ui_kit_text_field_selected_range)
  V("textFieldText", bare_ui_kit_text_field_text)
  V("textFieldPlaceholder", bare_ui_kit_text_field_placeholder)
  V("textFieldFont", bare_ui_kit_text_field_font)
  V("textFieldTextColor", bare_ui_kit_text_field_text_color)
  V("textFieldTextAlignment", bare_ui_kit_text_field_text_alignment)
  V("textFieldSecureTextEntry", bare_ui_kit_text_field_secure_text_entry)
  V("textFieldKeyboardType", bare_ui_kit_text_field_keyboard_type)
  V("textFieldReturnKeyType", bare_ui_kit_text_field_return_key_type)
  V("textFieldAutocapitalizationType", bare_ui_kit_text_field_autocapitalization_type)
  V("textFieldAutocorrectionType", bare_ui_kit_text_field_autocorrection_type)
  V("textFieldEnabled", bare_ui_kit_text_field_enabled)

  V("textViewInit", bare_ui_kit_text_view_init)
  V("textViewSelectedRange", bare_ui_kit_text_view_selected_range)
  V("textViewText", bare_ui_kit_text_view_text)
  V("textViewFont", bare_ui_kit_text_view_font)
  V("textViewTextColor", bare_ui_kit_text_view_text_color)
  V("textViewTextAlignment", bare_ui_kit_text_view_text_alignment)
  V("textViewEditable", bare_ui_kit_text_view_editable)
  V("textViewKeyboardType", bare_ui_kit_text_view_keyboard_type)
  V("textViewReturnKeyType", bare_ui_kit_text_view_return_key_type)
  V("textViewAutocapitalizationType", bare_ui_kit_text_view_autocapitalization_type)
  V("textViewAutocorrectionType", bare_ui_kit_text_view_autocorrection_type)
  V("textViewScrollEnabled", bare_ui_kit_text_view_scroll_enabled)

  V("bezierPathInit", bare_ui_kit_bezier_path_init)
  V("bezierPathMoveTo", bare_ui_kit_bezier_path_move_to)
  V("bezierPathLineTo", bare_ui_kit_bezier_path_line_to)
  V("bezierPathCurveTo", bare_ui_kit_bezier_path_curve_to)
  V("bezierPathAddArc", bare_ui_kit_bezier_path_add_arc)
  V("bezierPathClose", bare_ui_kit_bezier_path_close)

  V("labelInit", bare_ui_kit_label_init)
  V("labelText", bare_ui_kit_label_text)
  V("labelAttributedText", bare_ui_kit_label_attributed_text)
  V("labelFont", bare_ui_kit_label_font)
  V("labelTextColor", bare_ui_kit_label_text_color)
  T(
    "labelTextAlignment",
    bare_ui_kit_label_text_alignment,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_int32},
    }),
    bare_ui_kit_label_text_alignment_typed
  )
  T(
    "labelNumberOfLines",
    bare_ui_kit_label_number_of_lines,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_int32},
    }),
    bare_ui_kit_label_number_of_lines_typed
  )
  T(
    "labelLineBreakMode",
    bare_ui_kit_label_line_break_mode,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_int32},
    }),
    bare_ui_kit_label_line_break_mode_typed
  )

  V("sceneConnected", bare_ui_kit_scene_connected)
  V("sceneScreen", bare_ui_kit_scene_screen)
  V("sceneActivationState", bare_ui_kit_scene_activation_state)

  V("screenMainScreen", bare_ui_kit_screen_main_screen)
  T(
    "screenBoundsInto",
    bare_ui_kit_screen_bounds_into,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 4,
      .args = (int[]){js_object, js_int32, js_object, js_uint32},
    }),
    bare_ui_kit_screen_bounds_into_typed
  )
  V("screenNativeBoundsInto", bare_ui_kit_screen_native_bounds_into)
  V("screenScale", bare_ui_kit_screen_scale)
  V("screenNativeScale", bare_ui_kit_screen_native_scale)

  V("scrollViewInit", bare_ui_kit_scroll_view_init)
  V("scrollViewContentSize", bare_ui_kit_scroll_view_content_size)
  V("scrollViewContentOffset", bare_ui_kit_scroll_view_content_offset)

  V("viewSizeThatFitsInto", bare_ui_kit_view_size_that_fits_into)
  V("viewBecomeFirstResponder", bare_ui_kit_view_become_first_responder)
  V("viewResignFirstResponder", bare_ui_kit_view_resign_first_responder)
  V("viewIsFirstResponder", bare_ui_kit_view_is_first_responder)

  V("viewInit", bare_ui_kit_view_init)
  T(
    "viewFrame",
    bare_ui_kit_view_frame,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 6,
      .args = (int[]){js_object, js_int32, js_float64, js_float64, js_float64, js_float64},
    }),
    bare_ui_kit_view_frame_typed
  )
  T(
    "viewFrameInto",
    bare_ui_kit_view_frame_into,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 4,
      .args = (int[]){js_object, js_int32, js_object, js_uint32},
    }),
    bare_ui_kit_view_frame_into_typed
  )
  T(
    "viewBounds",
    bare_ui_kit_view_bounds,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 6,
      .args = (int[]){js_object, js_int32, js_float64, js_float64, js_float64, js_float64},
    }),
    bare_ui_kit_view_bounds_typed
  )
  T(
    "viewBoundsInto",
    bare_ui_kit_view_bounds_into,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 4,
      .args = (int[]){js_object, js_int32, js_object, js_uint32},
    }),
    bare_ui_kit_view_bounds_into_typed
  )
  T(
    "viewHidden",
    bare_ui_kit_view_hidden,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_boolean},
    }),
    bare_ui_kit_view_hidden_typed
  )
  T(
    "viewAlpha",
    bare_ui_kit_view_alpha,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_float64},
    }),
    bare_ui_kit_view_alpha_typed
  )
  T(
    "viewContentMode",
    bare_ui_kit_view_content_mode,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_int32},
    }),
    bare_ui_kit_view_content_mode_typed
  )
  T(
    "viewClipsToBounds",
    bare_ui_kit_view_clips_to_bounds,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_boolean},
    }),
    bare_ui_kit_view_clips_to_bounds_typed
  )
  T(
    "viewUserInteractionEnabled",
    bare_ui_kit_view_user_interaction_enabled,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 3,
      .args = (int[]){js_object, js_int32, js_boolean},
    }),
    bare_ui_kit_view_user_interaction_enabled_typed
  )
  V("viewBackgroundColor", bare_ui_kit_view_background_color)
  V("viewSuperview", bare_ui_kit_view_superview)
  V("viewSubviews", bare_ui_kit_view_subviews)
  V("viewAddSubview", bare_ui_kit_view_add_subview)
  V("viewInsertSubviewAtIndex", bare_ui_kit_view_insert_subview_at_index)
  V("viewInsertSubviewBelowSubview", bare_ui_kit_view_insert_subview_below_subview)
  V("viewInsertSubviewAboveSubview", bare_ui_kit_view_insert_subview_above_subview)
  V("viewRemoveFromSuperview", bare_ui_kit_view_remove_from_superview)
  V("viewSetNeedsLayout", bare_ui_kit_view_set_needs_layout)
  V("viewSetNeedsDisplay", bare_ui_kit_view_set_needs_display)
  V("viewSizeToFit", bare_ui_kit_view_size_to_fit)
  T(
    "viewSafeAreaInsetsInto",
    bare_ui_kit_view_safe_area_insets_into,
    &((js_callback_signature_t){
      .version = 0,
      .result = js_undefined,
      .args_len = 4,
      .args = (int[]){js_object, js_int32, js_object, js_uint32},
    }),
    bare_ui_kit_view_safe_area_insets_into_typed
  )

  V("viewControllerInit", bare_ui_kit_view_controller_init)
  V("viewControllerView", bare_ui_kit_view_controller_view)

  V("windowInit", bare_ui_kit_window_init)
  V("windowWindowScene", bare_ui_kit_window_window_scene)
  V("windowRootViewController", bare_ui_kit_window_root_view_controller)
  V("windowKeyWindow", bare_ui_kit_window_key_window)
  V("windowMakeKeyWindow", bare_ui_kit_window_make_key_window)
  V("windowMakeKeyAndVisible", bare_ui_kit_window_make_key_and_visible)
#undef V
#undef T

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

#define V_COLOR(name, selector) V("COLOR_" #name, bare_ui_kit_color_##name)
  BARE_UI_KIT_COLORS(V_COLOR)
#undef V_COLOR

  V("LINE_BREAK_MODE_WORD_WRAPPING", NSLineBreakByWordWrapping)
  V("LINE_BREAK_MODE_CHAR_WRAPPING", NSLineBreakByCharWrapping)
  V("LINE_BREAK_MODE_CLIPPING", NSLineBreakByClipping)
  V("LINE_BREAK_MODE_TRUNCATING_HEAD", NSLineBreakByTruncatingHead)
  V("LINE_BREAK_MODE_TRUNCATING_TAIL", NSLineBreakByTruncatingTail)
  V("LINE_BREAK_MODE_TRUNCATING_MIDDLE", NSLineBreakByTruncatingMiddle)

  V("SCENE_ACTIVATION_STATE_UNATTACHED", UISceneActivationStateUnattached)
  V("SCENE_ACTIVATION_STATE_FOREGROUND_ACTIVE", UISceneActivationStateForegroundActive)
  V("SCENE_ACTIVATION_STATE_FOREGROUND_INACTIVE", UISceneActivationStateForegroundInactive)
  V("SCENE_ACTIVATION_STATE_BACKGROUND", UISceneActivationStateBackground)

  V("FONT_DESCRIPTOR_TRAIT_ITALIC", UIFontDescriptorTraitItalic)
  V("FONT_DESCRIPTOR_TRAIT_BOLD", UIFontDescriptorTraitBold)
  V("FONT_DESCRIPTOR_TRAIT_EXPANDED", UIFontDescriptorTraitExpanded)
  V("FONT_DESCRIPTOR_TRAIT_CONDENSED", UIFontDescriptorTraitCondensed)
  V("FONT_DESCRIPTOR_TRAIT_MONO_SPACE", UIFontDescriptorTraitMonoSpace)

  V("TEXT_ALIGNMENT_LEFT", NSTextAlignmentLeft)
  V("TEXT_ALIGNMENT_RIGHT", NSTextAlignmentRight)
  V("TEXT_ALIGNMENT_CENTER", NSTextAlignmentCenter)
  V("TEXT_ALIGNMENT_JUSTIFIED", NSTextAlignmentJustified)
  V("TEXT_ALIGNMENT_NATURAL", NSTextAlignmentNatural)

  V("VIEW_CONTENT_MODE_SCALE_TO_FILL", UIViewContentModeScaleToFill)
  V("VIEW_CONTENT_MODE_SCALE_ASPECT_FIT", UIViewContentModeScaleAspectFit)
  V("VIEW_CONTENT_MODE_SCALE_ASPECT_FILL", UIViewContentModeScaleAspectFill)
  V("VIEW_CONTENT_MODE_REDRAW", UIViewContentModeRedraw)
  V("VIEW_CONTENT_MODE_CENTER", UIViewContentModeCenter)
  V("VIEW_CONTENT_MODE_TOP", UIViewContentModeTop)
  V("VIEW_CONTENT_MODE_BOTTOM", UIViewContentModeBottom)
  V("VIEW_CONTENT_MODE_LEFT", UIViewContentModeLeft)
  V("VIEW_CONTENT_MODE_RIGHT", UIViewContentModeRight)
  V("VIEW_CONTENT_MODE_TOP_LEFT", UIViewContentModeTopLeft)
  V("VIEW_CONTENT_MODE_TOP_RIGHT", UIViewContentModeTopRight)
  V("VIEW_CONTENT_MODE_BOTTOM_LEFT", UIViewContentModeBottomLeft)
  V("VIEW_CONTENT_MODE_BOTTOM_RIGHT", UIViewContentModeBottomRight)

  V("TEXT_FIELD_EVENT_DID_BEGIN_EDITING", bare_ui_kit_text_field_event_did_begin_editing)
  V("TEXT_FIELD_EVENT_DID_END_EDITING", bare_ui_kit_text_field_event_did_end_editing)
  V("ACTIVITY_INDICATOR_VIEW_STYLE_MEDIUM", UIActivityIndicatorViewStyleMedium)
  V("ACTIVITY_INDICATOR_VIEW_STYLE_LARGE", UIActivityIndicatorViewStyleLarge)

  V("SWITCH_EVENT_VALUE_CHANGED", bare_ui_kit_switch_event_value_changed)

  V("TEXT_FIELD_EVENT_DID_CHANGE", bare_ui_kit_text_field_event_did_change)
  V("TEXT_FIELD_EVENT_SHOULD_CHANGE_CHARACTERS", bare_ui_kit_text_field_event_should_change_characters)
  V("TEXT_FIELD_EVENT_SHOULD_RETURN", bare_ui_kit_text_field_event_should_return)
  V("TEXT_FIELD_EVENT_DID_CHANGE_SELECTION", bare_ui_kit_text_field_event_did_change_selection)

  V("TEXT_VIEW_EVENT_DID_BEGIN_EDITING", bare_ui_kit_text_view_event_did_begin_editing)
  V("TEXT_VIEW_EVENT_DID_END_EDITING", bare_ui_kit_text_view_event_did_end_editing)
  V("TEXT_VIEW_EVENT_DID_CHANGE", bare_ui_kit_text_view_event_did_change)
  V("TEXT_VIEW_EVENT_SHOULD_CHANGE_TEXT", bare_ui_kit_text_view_event_should_change_text)
  V("TEXT_VIEW_EVENT_DID_CHANGE_SELECTION", bare_ui_kit_text_view_event_did_change_selection)

  V("KEYBOARD_TYPE_DEFAULT", UIKeyboardTypeDefault)
  V("KEYBOARD_TYPE_NUMBER_PAD", UIKeyboardTypeNumberPad)
  V("KEYBOARD_TYPE_DECIMAL_PAD", UIKeyboardTypeDecimalPad)
  V("KEYBOARD_TYPE_NUMBERS_AND_PUNCTUATION", UIKeyboardTypeNumbersAndPunctuation)
  V("KEYBOARD_TYPE_EMAIL_ADDRESS", UIKeyboardTypeEmailAddress)
  V("KEYBOARD_TYPE_PHONE_PAD", UIKeyboardTypePhonePad)
  V("KEYBOARD_TYPE_URL", UIKeyboardTypeURL)

  V("RETURN_KEY_TYPE_DEFAULT", UIReturnKeyDefault)
  V("RETURN_KEY_TYPE_DONE", UIReturnKeyDone)
  V("RETURN_KEY_TYPE_GO", UIReturnKeyGo)
  V("RETURN_KEY_TYPE_NEXT", UIReturnKeyNext)
  V("RETURN_KEY_TYPE_SEARCH", UIReturnKeySearch)
  V("RETURN_KEY_TYPE_SEND", UIReturnKeySend)

  V("AUTOCAPITALIZATION_TYPE_NONE", UITextAutocapitalizationTypeNone)
  V("AUTOCAPITALIZATION_TYPE_WORDS", UITextAutocapitalizationTypeWords)
  V("AUTOCAPITALIZATION_TYPE_SENTENCES", UITextAutocapitalizationTypeSentences)
  V("AUTOCAPITALIZATION_TYPE_ALL_CHARACTERS", UITextAutocapitalizationTypeAllCharacters)

  V("AUTOCORRECTION_TYPE_DEFAULT", UITextAutocorrectionTypeDefault)
  V("AUTOCORRECTION_TYPE_NO", UITextAutocorrectionTypeNo)
  V("AUTOCORRECTION_TYPE_YES", UITextAutocorrectionTypeYes)

  V("WINDOW_EVENT_KEYBOARD_WILL_CHANGE_FRAME", bare_ui_kit_window_event_keyboard_will_change_frame)

  V("VIEW_EVENT_TOUCHES_BEGAN", bare_ui_kit_view_event_touches_began)
  V("VIEW_EVENT_TOUCHES_MOVED", bare_ui_kit_view_event_touches_moved)
  V("VIEW_EVENT_TOUCHES_ENDED", bare_ui_kit_view_event_touches_ended)
  V("VIEW_EVENT_TOUCHES_CANCELLED", bare_ui_kit_view_event_touches_cancelled)

  V("SCROLL_VIEW_EVENT_DID_SCROLL", bare_ui_kit_scroll_view_event_did_scroll)
#undef V

  // The font weights are fractions of the way from thin to black, not an
  // enumeration, so they cross as doubles.
#define W(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  W("FONT_WEIGHT_ULTRA_LIGHT", UIFontWeightUltraLight)
  W("FONT_WEIGHT_THIN", UIFontWeightThin)
  W("FONT_WEIGHT_LIGHT", UIFontWeightLight)
  W("FONT_WEIGHT_REGULAR", UIFontWeightRegular)
  W("FONT_WEIGHT_MEDIUM", UIFontWeightMedium)
  W("FONT_WEIGHT_SEMIBOLD", UIFontWeightSemibold)
  W("FONT_WEIGHT_BOLD", UIFontWeightBold)
  W("FONT_WEIGHT_HEAVY", UIFontWeightHeavy)
  W("FONT_WEIGHT_BLACK", UIFontWeightBlack)
#undef W

  return exports;
}

BARE_MODULE(bare_ui_kit, bare_ui_kit_exports)

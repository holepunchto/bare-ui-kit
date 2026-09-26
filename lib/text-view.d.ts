import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitScrollView = require('./scroll-view')
import UIKitView = require('./view')
import UIKitColor = require('./color')
import UIKitFont = require('./font')

/** Several lines of editable text, as a `UITextView`. */
interface UIKitTextView extends UIKitScrollView<UIKitTextView.Events> {
  text: string | null

  get font(): UIKitFont | null
  set font(font: Wrapper | null)

  get textColor(): UIKitColor | null
  set textColor(color: Wrapper | null)

  /** A `UIKitLabel.TEXT_ALIGNMENT` constant. */
  textAlignment: number

  /** A `UIKitTextField.KEYBOARD_TYPE` constant. */
  keyboardType: number

  /** A `UIKitTextField.RETURN_KEY_TYPE` constant. */
  returnKeyType: number

  /** A `UIKitTextField.TEXT_AUTOCAPITALIZATION_TYPE` constant. */
  autocapitalizationType: number

  /** A `UIKitTextField.TEXT_AUTOCORRECTION_TYPE` constant. */
  autocorrectionType: number

  /** The selected range, as UTF-16 offsets. With nothing selected, `length` is 0. */
  get selectedRange(): { location: number; length: number }
  set selectedRange(range: { location?: number; length?: number })

  editable: boolean

  scrollEnabled: boolean
}

declare class UIKitTextView {
  constructor(frame?: Partial<UIKitView.Rect>)
}

declare namespace UIKitTextView {
  export interface Events {
    didBeginEditing: []
    didEndEditing: []

    /** The text changed. */
    didChange: []
    didChangeSelection: []

    /** The `length` characters at `location` are about to be replaced by `string`. */
    shouldChangeText: [edit: { location: number; length: number; string: string }]
  }
}

export = UIKitTextView

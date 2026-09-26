import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitView = require('./view')
import UIKitColor = require('./color')
import UIKitFont = require('./font')

/** A single line of editable text, as a `UITextField`. */
interface UIKitTextField extends UIKitView<UIKitTextField.Events> {
  text: string | null

  /** Text shown while the field is empty. */
  placeholder: string | null

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

  /** Whether the text is hidden, as for a password. */
  secureTextEntry: boolean

  enabled: boolean
}

declare class UIKitTextField {
  constructor(frame?: Partial<UIKitView.Rect>)

  static readonly KEYBOARD_TYPE: {
    readonly DEFAULT: number
    readonly NUMBER_PAD: number
    readonly DECIMAL_PAD: number
    readonly NUMBERS_AND_PUNCTUATION: number
    readonly EMAIL_ADDRESS: number
    readonly PHONE_PAD: number
    readonly URL: number
  }

  static readonly RETURN_KEY_TYPE: {
    readonly DEFAULT: number
    readonly DONE: number
    readonly GO: number
    readonly NEXT: number
    readonly SEARCH: number
    readonly SEND: number
  }

  static readonly TEXT_AUTOCAPITALIZATION_TYPE: {
    readonly NONE: number
    readonly WORDS: number
    readonly SENTENCES: number
    readonly ALL_CHARACTERS: number
  }

  static readonly TEXT_AUTOCORRECTION_TYPE: {
    readonly DEFAULT: number
    readonly NO: number
    readonly YES: number
  }
}

declare namespace UIKitTextField {
  export interface Edit {
    location: number
    length: number
    string: string
  }

  export interface Events {
    didBeginEditing: []
    didEndEditing: []

    /** The text changed. */
    didChange: []
    didChangeSelection: []

    /** The `length` characters at `location` are about to be replaced by `string`. */
    shouldChangeCharacters: [edit: Edit]

    /** The user pressed the return key. */
    shouldReturn: []
  }
}

export = UIKitTextField

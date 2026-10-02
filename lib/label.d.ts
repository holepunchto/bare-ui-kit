import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitView = require('./view')
import UIKitAttributedString = require('./attributed-string')
import UIKitColor = require('./color')
import UIKitFont = require('./font')

/** A view that shows text, as a `UILabel`. */
interface UIKitLabel extends UIKitView {
  text: string | null

  get attributedText(): UIKitAttributedString | null
  set attributedText(text: Wrapper | null)

  get font(): UIKitFont | null
  set font(font: Wrapper | null)

  get textColor(): UIKitColor | null
  set textColor(color: Wrapper | null)

  /** A `TEXT_ALIGNMENT` constant. */
  textAlignment: number

  /** The most lines to show. 0 means no limit. */
  numberOfLines: number

  /** How lines wrap or are cut off, as a `LINE_BREAK_MODE` constant. */
  lineBreakMode: number
}

declare class UIKitLabel {
  constructor(frame?: Partial<UIKitView.Rect>)

  static readonly TEXT_ALIGNMENT: {
    readonly LEFT: number
    readonly RIGHT: number
    readonly CENTER: number
    readonly JUSTIFIED: number
    readonly NATURAL: number
  }

  static readonly LINE_BREAK_MODE: {
    readonly WORD_WRAPPING: number
    readonly CHAR_WRAPPING: number
    readonly CLIPPING: number
    readonly TRUNCATING_HEAD: number
    readonly TRUNCATING_TAIL: number
    readonly TRUNCATING_MIDDLE: number
  }
}

export = UIKitLabel

import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'

/** How paragraphs in an attributed string are laid out, as an `NSMutableParagraphStyle`. */
interface UIKitParagraphStyle {
  /** A `UIKitLabel.TEXT_ALIGNMENT` constant. */
  alignment: number

  /** A `UIKitLabel.LINE_BREAK_MODE` constant. */
  lineBreakMode: number

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitParagraphStyle {
  constructor()
}

export = UIKitParagraphStyle

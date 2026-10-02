import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitColor = require('./color')
import UIKitFont = require('./font')
import UIKitParagraphStyle = require('./paragraph-style')

/** Text with styles, as an `NSMutableAttributedString`. */
interface UIKitAttributedString {
  readonly string: string

  readonly length: number

  /** The styles of the character at `location`. */
  attributesAt(location: number): UIKitAttributedString.Attributes

  /** Add another attributed string at the end. It can come from another addon. */
  append(other: Wrapper): this

  /** Add `string` at the end, with `attributes`. */
  appendString(string: string, attributes?: UIKitAttributedString.Attributes): this

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitAttributedString {
  constructor(opts?: { string?: string; attributes?: UIKitAttributedString.Attributes })
}

declare namespace UIKitAttributedString {
  export interface Attributes {
    font?: UIKitFont | null
    foregroundColor?: UIKitColor | null
    backgroundColor?: UIKitColor | null
    paragraphStyle?: UIKitParagraphStyle | null

    /** Extra space between characters, in points. */
    kern?: number
    baselineOffset?: number
    underlineStyle?: number
    strikethroughStyle?: number
  }
}

export = UIKitAttributedString

import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitFont = require('./font')

/** Scales fonts and lengths for the text size the user chose, as a `UIFontMetrics`. */
interface UIKitFontMetrics {
  scaledValueForValue(value: number): number

  scaledFontForFont(font: UIKitFont): UIKitFont

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class UIKitFontMetrics {
  protected constructor()

  static readonly defaultMetrics: UIKitFontMetrics

  /** The metrics for a `FONT_TEXT_STYLE`. */
  static metricsForTextStyle(textStyle: string): UIKitFontMetrics

  static readonly FONT_TEXT_STYLE: {
    readonly LARGE_TITLE: string
    readonly TITLE1: string
    readonly TITLE2: string
    readonly TITLE3: string
    readonly HEADLINE: string
    readonly SUBHEADLINE: string
    readonly BODY: string
    readonly CALLOUT: string
    readonly FOOTNOTE: string
    readonly CAPTION1: string
    readonly CAPTION2: string
  }
}

export = UIKitFontMetrics

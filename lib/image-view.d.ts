import { tag, handle, Handle, Wrapper } from 'bare-foundation-registry'
import UIKitView = require('./view')
import UIKitImage = require('./image')

/** A view that shows an image, as a `UIImageView`. */
interface UIKitImageView extends UIKitView {
  get image(): UIKitImage | null
  set image(image: Wrapper | null)
}

declare class UIKitImageView {
  constructor(frame?: Partial<UIKitView.Rect>)
}

export = UIKitImageView

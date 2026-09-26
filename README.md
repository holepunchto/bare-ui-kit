# bare-ui-kit

UIKit for Bare on iOS. It gives you UIKit's views in JavaScript, along with a runtime that starts the app for you, so a Bare app can have a native iOS screen.

```
npm i bare-ui-kit
```

## Usage

```js
const { Scene, Window, ViewController, Label, Color, Font } = require('bare-ui-kit')

// A window is only shown in a scene, and the runtime connects one at launch.
const [scene] = Scene.connected

const window = new Window({ scene })

const controller = new ViewController()

window.rootViewController = controller
window.makeKeyAndVisible()

const root = controller.view

root.backgroundColor = Color.systemBackgroundColor

const label = new Label()

label.text = 'Hello'
label.font = Font.systemFont(34, Font.WEIGHT.BOLD)
label.textAlignment = Label.TEXT_ALIGNMENT.CENTER

root.addSubview(label)

// The controller lays its view out to fill the window, so the label follows it.
controller.on('didLayoutSubviews', () => {
  label.frame = root.bounds
})
```

Build the app with `bare-build` and this runtime:

```console
bare-build --host ios-arm64 --runtime bare-ui-kit/runtime --identifier com.example.Hello index.js
```

Objects that have events, such as views, text fields and switches, emit them as ordinary events. UIKit is only asked to report an event while something listens to it.

## License

Apache-2.0

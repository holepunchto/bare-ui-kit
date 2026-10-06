const { Scene, ViewController, Window } = require('..')

exports.open = function open(t) {
  const window = new Window({ scene: Scene.connected[0] })
  const controller = new ViewController()

  window.rootViewController = controller
  window.makeKeyAndVisible()

  t.teardown(() => {
    window.hidden = true
  })

  return { window, controller }
}

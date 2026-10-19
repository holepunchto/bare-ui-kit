const { test } = require('bare-tap')
const Pointer = require('bare-pointer')
const { ActivityIndicatorView, Label, Switch, TextField, TextView } = require('..')
const { open } = require('./helpers')

test('toggles a switch', (t) => {
  const control = new Switch()

  t.equal(control.isOn, false, 'off')

  control.isOn = true

  t.equal(control.isOn, true, 'on')

  control.enabled = false

  t.equal(control.enabled, false, 'disabled')
})

test('toggles a switch with the pointer', async (t) => {
  const { controller } = open(t)
  const root = controller.view
  const pointer = new Pointer()

  t.teardown(() => pointer.destroy())

  const control = new Switch({ x: 40, y: 100 })

  root.addSubview(control)

  let changes = 0

  control.on('valueChanged', () => changes++)

  await pointer.tap(control)

  t.equal(changes, 1, 'changed')
  t.equal(control.isOn, true, 'on')
})

test('does not toggle a disabled switch with the pointer', async (t) => {
  const { controller } = open(t)
  const root = controller.view
  const pointer = new Pointer()

  t.teardown(() => pointer.destroy())

  const control = new Switch({ x: 40, y: 100 })

  control.enabled = false

  root.addSubview(control)

  let changes = 0

  control.on('valueChanged', () => changes++)

  await pointer.tap(control)

  t.equal(changes, 0, 'not changed')
  t.equal(control.isOn, false, 'off')
})

test('round trips the text of a field', (t) => {
  const field = new TextField()

  t.equal(field.placeholder, null, 'no placeholder')

  field.placeholder = 'Name'
  field.text = 'Grüße 👋'

  t.equal(field.placeholder, 'Name', 'placeholder')
  t.equal(field.text, 'Grüße 👋', 'text')
})

test('selects text in a field', (t) => {
  const field = new TextField()

  field.text = 'Hello world'
  field.selectedRange = { location: 6, length: 5 }

  t.deepStrictEqual(field.selectedRange, { location: 6, length: 5 })
})

test('focuses a text field with the pointer', async (t) => {
  const { controller } = open(t)
  const root = controller.view
  const pointer = new Pointer()

  t.teardown(() => pointer.destroy())

  const field = new TextField({ x: 40, y: 100, width: 200, height: 40 })

  root.addSubview(field)

  let edits = 0

  field.on('didBeginEditing', () => edits++)

  await pointer.tap(field)

  t.equal(edits, 1)
})

test('round trips the text of a text view', (t) => {
  const view = new TextView()

  view.text = 'One\nTwo'
  view.editable = false

  t.equal(view.text, 'One\nTwo', 'text')
  t.equal(view.editable, false, 'not editable')
})

test('grows a label with its text', (t) => {
  const label = new Label()

  label.text = 'A'
  label.sizeToFit()

  const short = label.frame.width

  label.text = 'A much longer string'
  label.sizeToFit()

  t.ok(short > 0, 'sized')
  t.ok(label.frame.width > short, 'grown')
})

test('animates an activity indicator', (t) => {
  const indicator = new ActivityIndicatorView()

  indicator.hidesWhenStopped = true

  t.equal(indicator.hidden, true, 'hidden while stopped')

  indicator.animating = true

  t.equal(indicator.animating, true, 'animating')
  t.equal(indicator.hidden, false, 'shown while animating')
})

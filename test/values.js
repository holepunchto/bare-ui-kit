const { test } = require('bare-tap')
const { Color, Font, Image, Label, Screen } = require('..')

test('creates a color', (t) => {
  t.deepStrictEqual(
    Color.rgb(1, 0.5, 0, 0.25).components,
    { red: 1, green: 0.5, blue: 0, alpha: 0.25 },
    'components'
  )

  t.equal(Color.white(0.5).components.alpha, 1, 'opaque by default')
  t.equal(Color.rgb(1, 0, 0).withAlphaComponent(0.75).components.alpha, 0.75, 'with alpha')
})

test('returns the same system color', (t) => {
  t.ok(Color.systemRedColor === Color.systemRedColor)
})

test('returns the wrapper of an assigned color', (t) => {
  const label = new Label()
  const color = Color.rgb(0, 0, 1)

  label.textColor = color

  t.ok(label.textColor === color)
})

test('creates a font', (t) => {
  const font = Font.systemFont(13)

  t.equal(font.pointSize, 13, 'size')
  t.ok(font.ascender > 0, 'ascender')
  t.ok(font.descender < 0, 'descender')
  t.ok(font.lineHeight > 13, 'line height')

  t.equal(Font.boldSystemFont(20).pointSize, 20, 'bold')
})

test('finds a font by name', (t) => {
  t.equal(Font.withName('Helvetica', 12).familyName, 'Helvetica', 'found')
  t.equal(Font.withName('No Such Font', 12), null, 'missing')
})

test('does not find a missing image', (t) => {
  t.equal(Image.named('no-such-image'), null, 'by name')
  t.equal(Image.withContentsOfFile('/no/such/file.png'), null, 'by path')
})

test('describes the main screen', (t) => {
  const screen = Screen.mainScreen

  t.ok(screen.bounds.width > 0 && screen.bounds.height > 0, 'has a size')
  t.ok(screen.scale >= 1, 'scale')
  t.equal(screen.nativeBounds.width, screen.bounds.width * screen.nativeScale, 'native size')
})

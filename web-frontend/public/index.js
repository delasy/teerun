/* eslint-env browser */

const container = document.getElementById('root')
const canvas = document.createElement('canvas')

const { height, width } = container.getBoundingClientRect()
const scaleFactor = window.devicePixelRatio

canvas.height = height * scaleFactor
canvas.style.height = height + 'px'
canvas.style.width = width + 'px'
canvas.width = width * scaleFactor

container.appendChild(canvas)

let frameId = null
let map = null
let player = null
let players = null

const settings = {
  size: 26
}

class Context {
  constructor (canvas) {
    this._ctx = canvas.getContext('2d')
  }

  clear () {
    this._ctx.clearRect(0, 0, width * scaleFactor, height * scaleFactor)
    return this
  }

  fillCircle (color, x, y, radius) {
    this._ctx.beginPath()

    this._ctx.arc(
      x * scaleFactor,
      y * scaleFactor,
      radius * scaleFactor,
      0,
      2 * Math.PI
    )

    this._ctx.fillStyle = color
    this._ctx.fill()

    return this
  }

  fillRect (color, x, y, width, height) {
    this._ctx.fillStyle = color

    this._ctx.fillRect(
      x * scaleFactor,
      y * scaleFactor,
      width * scaleFactor,
      height * scaleFactor
    )

    return this
  }
}

const ctx = new Context(canvas)

function drawMap (ctx) {
  for (const tile of map.tiles) {
    ctx.fillRect(
      'gray',
      width / 2 + tile.x * settings.size - settings.size / 2,
      height / 2 - tile.y * settings.size,
      settings.size,
      settings.size
    )
  }
}

function drawPlayer (ctx) {
  const x = width / 2 + player.position.x * settings.size
  const y = height / 2 - player.position.y * settings.size - settings.size / 2

  ctx.fillCircle(player.color, x, y, 18)
}

function drawPlayers (ctx) {
  for (const player of players) {
    const x = width / 2 + player.position.x * settings.size
    const y = height / 2 - player.position.y * settings.size - settings.size / 2

    ctx.fillCircle(player.color, x, y, 18)
  }
}

function draw () {
  ctx.clear()

  if (map !== null) {
    drawMap(ctx)
  }

  if (players !== null) {
    drawPlayers(ctx)
  }

  if (player !== null) {
    drawPlayer(ctx)
  }

  frameId = window.requestAnimationFrame(draw)
}

function parsePayload (buf) {
  try {
    return JSON.parse(buf)
  } catch {
  }
}

function connect () {
  const ws = new WebSocket('ws://localhost:8081')

  function send (action, data = null) {
    return ws.send(JSON.stringify({ action, data }))
  }

  window.wsSend = send

  function handleKeyDown (e) {
    switch (e.code) {
      case 'KeyA': {
        send('start-going-left')
        break
      }
      case 'KeyD': {
        send('start-going-right')
        break
      }
      case 'Space': {
        send('jump')
        break
      }
    }
  }

  function handleKeyUp (e) {
    switch (e.code) {
      case 'KeyA': {
        send('stop-going-left')
        break
      }
      case 'KeyD': {
        send('stop-going-right')
        break
      }
    }
  }

  ws.onopen = () => {
    draw()
    window.addEventListener('keydown', handleKeyDown)
    window.addEventListener('keyup', handleKeyUp)

    ws.send('Hello, World!')
  }

  ws.onmessage = (msg) => {
    const payload = parsePayload(msg.data)

    switch (payload.action) {
      case 'map': {
        map = payload.data
        break
      }
      case 'player': {
        player = payload.data
        break
      }
      case 'players': {
        players = payload.data
        break
      }
    }

    draw()
  }

  ws.onclose = () => {
    window.removeEventListener('keydown', handleKeyDown)
    window.removeEventListener('keyup', handleKeyUp)

    if (frameId !== null) {
      window.cancelAnimationFrame(frameId)
      frameId = null
    }

    setTimeout(connect, 1000)
  }

  ws.onerror = () => {
    ws.close()
  }
}

connect()

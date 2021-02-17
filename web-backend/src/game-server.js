const http = require('http')
const WebSocket = require('ws')

const Map = require('./map')
const Player = require('./player')

class GameServer {
  constructor () {
    this.map = new Map()
    this.players = []

    this._httpServer = null
    this._previousTick = Date.now()
    this._running = false
    this._socketServer = null

    this._settings = {
      fps: 30,
      port: parseInt(process.env.PORT) || 8081
    }
  }

  start () {
    this._running = true

    this._socketServer = new WebSocket.Server({
      noServer: true
    })

    this._socketServer.on(
      'connection',
      this._handleConnectSocketServer.bind(this)
    )

    this._httpServer = http.createServer()
    this._httpServer.on('upgrade', this._handleUpgradeHttpServer.bind(this))
    this._httpServer.listen(this._settings.port, this._loop.bind(this))

    return this
  }

  stop () {
    this._running = false
    this._socketServer.close()
    this._httpServer.close()

    return this
  }

  _handleConnectSocketServer (socket, player) {
    socket.on('close', this._handleDisconnectPlayer.bind(this, player))
    socket.on('message', this._handleMessagePlayer.bind(this, player))

    return this
  }

  _handleDisconnectPlayer (player) {
    player.disconnect()
    return this
  }

  _handleMessagePlayer (player, buf) {
    let payload

    try {
      payload = JSON.parse(buf)
    } catch {
      return
    }

    switch (payload.action) {
      case 'jump': {
        player.jump()
        break
      }
      case 'start-going-left': {
        player.startGoingLeft()
        break
      }
      case 'start-going-right': {
        player.startGoingRight()
        break
      }
      case 'stop-going-left': {
        player.stopGoingLeft()
        break
      }
      case 'stop-going-right': {
        player.stopGoingRight()
        break
      }
    }

    return this
  }

  _handleUpgradeHttpServer (req, socket, head) {
    this._socketServer.handleUpgrade(
      req,
      socket,
      head,
      this._handleUpgradeSocketServer.bind(this)
    )

    return this
  }

  _handleUpgradeSocketServer (socket) {
    const player = new Player(this, socket)

    this.players.push(player)
    this._socketServer.emit('connection', socket, player)

    return this
  }

  _loop () {
    if (!this._running) {
      return
    }

    const tickLengthMs = 1000 / this._settings.fps
    const now = Date.now()

    if (this._previousTick + tickLengthMs <= now) {
      const delta = (now - this._previousTick) / 1000

      this._previousTick = now
      this._update(delta)
    }

    if (Date.now() - this._previousTick < tickLengthMs - 16) {
      setTimeout(this._loop.bind(this))
    } else {
      setImmediate(this._loop.bind(this))
    }

    return this
  }

  _update () {
    for (const player of this.players) {
      player.update()
    }

    this.players.forEach((player) => {
      player.send('players', player.getOtherPlayers())
    })

    return this
  }
}

module.exports = GameServer

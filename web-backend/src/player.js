const { v4: uuidv4 } = require('uuid')

function randomColor () {
  const chars = '0123456789ABCDEF'
  let color = '#'

  for (let i = 0; i < 6; i++) {
    color += chars[Math.floor(Math.random() * 16)]
  }

  return color
}

class Player {
  constructor (gameServer, socket) {
    this.color = randomColor()

    this.position = {
      x: 0,
      y: 0
    }

    this._gameServer = gameServer

    this._goingLeft = false
    this._goingRight = false
    this._goingStartPosition = null
    this._goingStartTime = null
    this._id = uuidv4()
    this._socket = socket

    this.send('map', this._gameServer.map)
    this.send('player')
  }

  disconnect () {
    for (let i = 0; i < this._gameServer.players.length; i++) {
      if (this._gameServer.players[i].isSame(this)) {
        this._gameServer.players.splice(i, 1)
        break
      }
    }

    return this
  }

  getOtherPlayers () {
    return this._gameServer.players.filter(it => !it.isSame(this))
  }

  isSame (player) {
    return this._id === player._id
  }

  jump () {
    return this
  }

  send (action, data = null) {
    if (action === 'player') {
      data = this
    }

    const payload = JSON.stringify({ action, data })
    this._socket.send(payload)

    return this
  }

  startGoingLeft () {
    this._goingLeft = true
    this._goingStartPosition = { ...this.position }
    this._goingStartTime = Date.now()

    return this
  }

  startGoingRight () {
    this._goingRight = true
    this._goingStartPosition = { ...this.position }
    this._goingStartTime = Date.now()

    return this
  }

  stopGoingLeft () {
    this._goingLeft = false
    this._goingStartPosition = null
    this._goingStartTime = null

    return this
  }

  stopGoingRight () {
    this._goingRight = false
    this._goingStartPosition = null
    this._goingStartTime = null

    return this
  }

  toJSON () {
    return {
      color: this.color,
      position: this.position
    }
  }

  update () {
    const now = Date.now()

    if (this._goingStartTime) {
      const goingDelta = now - this._goingStartTime

      if (this._goingLeft) {
        this.position.x = this._goingStartPosition.x - 0.01 * goingDelta
      } else if (this._goingRight) {
        this.position.x = this._goingStartPosition.x + 0.01 * goingDelta
      }

      this.send('player')
    }
  }
}

module.exports = Player

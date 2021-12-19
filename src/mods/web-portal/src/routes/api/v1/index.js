var express = require('express')
var router = express.Router()

const PREFIX = '/api/v1/'
const r = uri => PREFIX + uri

/* Login route */
router.post(r('/login'), (req, res, next) => {
	console.log(req)
  res.send('respond with a resource')
})

module.exports = router

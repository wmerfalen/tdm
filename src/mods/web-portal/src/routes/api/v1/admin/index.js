module.exports = {
	routes: (router) => {
		const { uriPrefix, uri } = require('../util')
		uriPrefix('admin');

		/* Login route */
		router.post(uri('login'), (req, res, next) => {
			console.log(req)
			res.send('respond with a resource')
		})

		/* Login route */
		router.get(uri('login'), (req, res, next) => {
			console.log(req)
			res.send('respond with a resource')
		})
	}
};

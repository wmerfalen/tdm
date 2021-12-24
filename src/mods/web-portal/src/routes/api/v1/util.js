const PREFIX = '/api/v1/'
const r = uri => PREFIX + uri

const RouteUrl = {};

RouteUrl.prefix = PREFIX;
RouteUrl.setPrefix = p => RouteUrl.prefix = p.replace(/^\//,'');
RouteUrl.uri = uri => PREFIX + RouteUrl.prefix.replace(/\/$/,'') + '/' + uri.replace(/^\//,'');

module.exports = {
	RouteUrl,
	uriPrefix: p => RouteUrl.setPrefix(p),
	uri: ur => RouteUrl.uri(ur),
}

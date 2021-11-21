# Components
- iptables rule runner
	- setuid script that calls iptables via shell
- zeromq server
	- listens for requests to ban via ip
- zeromq client
	- sends ban/unban requests to zeromq server

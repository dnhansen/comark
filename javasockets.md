# Java socket programming (WIP)

Classes for socket programming can be found in the `java.net` package. All class names below are relative to this package.

## The `Socket` class

The [`Socket`](https://docs.oracle.com/javase/8/docs/api/java/net/Socket.html) class represents an endpoint of a TCP connection between two machines (for UDP connections there is the [`DatagramSocket`](https://docs.oracle.com/javase/8/docs/api/java/net/DatagramSocket.html) class). Sockets are created using one of the [below constructors](#constructors), and they are also returned by various methods, see [TODO serversocket methods]


### Constructors

| Constructor | Description |
| - | - |
| `Socket()` | Creates an unconnected socket. See [below](#instance-methods) for how to connect it to a server. |
| `Socket(InetAddress address, int port)` | Connect to a server with IP address `address` on port `port`. See [below](#the-inetaddress-class) for the `InetAddress` class. |
| `Socket(String host, int port)` | Connect to a server with hostname `host` (e.g. `www.google.com`) on port `port`.|


### Instance methods

| Type | Method | Description |
| - | - | - |
| `void` | `close()` | Closes the socket. |
| `void` | `bind(SocketAddress bindpoint)` | Binds the socket to a local address. We describe the `SocketAddress` class [below](#the-socketaddress-class). It is not advised to manually bind client sockets. |
| `void` | `connect(SocketAddress endpoint)` | Connects the socket to the server with address `endpoint`. |
| `InputStream` | `getInputStream()` | Returns an input stream for the socket. |
| `OutputStream` | `getOutputStream()` | Returns an output stream for the socket.


## The `InetAddress` class

The [`InetAddress`](https://docs.oracle.com/javase/8/docs/api/java/net/InetAddress.html) class represents an IP address. IPv4 (IPv6) addresses are represented using the subclass `Inet4Address` (`Inet6Address`). None of these classes have constructors, but various static methods can be used to construct `InetAddress` objects, as seen [below](#static-methods).

### Static methods

| Type | Method | Description |
| - | - | - |
| `InetAddress` | `getByAddress(byte[] addr)` | Returns an `InetAddress` object given the raw IP address `addr`, e.g. `{127, 0, 0, 1}`. |
| `InetAddress` | `getByName(String host)` | Returns an `InetAddress` object given the host name `host`. This can either be a domain name (e.g. `"www.google.com"`) or an IP address (e.g. `"127.0.0.1"`). |


## The `InetSocketAddress` class

The [`InetSocketAddress`](https://docs.oracle.com/javase/8/docs/api/java/net/InetSocketAddress.html) class is a direct subclass of the [`SocketAddress`](https://docs.oracle.com/javase/8/docs/api/java/net/SocketAddress.html) abstract class, and instances can thus e.g. be passed to the `connect` method of the `Socket` class (see [above](#instance-methods)). It represents an IP socket address, consisting of a port number and either an IP address or a hostname (in which case an attempt will be made to resolve the hostname).

[TODO wildcard address]

### Constructors

| Constructor | Description |
| - | - |
| `InetSocketAddress(int port)` | Creates a socket address from a port number, where the IP address is the wildcard address.
| `InetSocketAddress(InetAddress addr, int port)` | Creates a socket address from an IP address and a port number. |
| `InetSocketAddress(String hostname, int port)` | Creates a socket address from a hostname and a port number. |


## The `ServerSocket` class

The [`ServerSocket`](https://docs.oracle.com/javase/8/docs/api/java/net/ServerSocket.html) class represents TCP server sockets. A server socket waits for an incoming request over the network.


### Constructors

| Constructor | Description |
| - | - |
| `ServerSocket()` | Creates an unbound server socket. |
| `ServerSocket(int port)` | Creates a server socket, bound to the specified port. |


### Instance methods

| Type | Method | Description |
| - | - | - |
| `Socket` | `accept()` | Listens for a connection to be made to the server socket and accepts it. Blocks until a connection is made, after which it returns. |
| `void` | `bind(SocketAddress endpoint)` | Binds the ServerSocket to a specific address. TODO wildcard IP |
| `void` | `close()` | Closes the socket. |


## The `DatagramPacket` class

The [`DatagramPacket`](https://docs.oracle.com/javase/8/docs/api/java/net/DatagramPacket.html) class represents UDP packets that are either to be sent or received. To construct a `DatagramPacket` object for sending packets, we pass an address to the constructor. To construct an object for receiving packets, we omit the address.

### Constructors

| Constructor | Description |
| - | - |
| `DatagramPacket(byte[] buf, int length)` | Constructs a packet for receiving packets of length `length` into the buffer `buf`. |
| `DatagramPacket(byte[] buf, int length, InetAddress address, int port)` | Constructs a datagram packet for sending packets to the address `address:port`. |
| `DatagramPacket(byte[] buf, int length, SocketAddress address)` | Constructs a datagram packet for sending packets to the address `address`. |

### Getter methods

| Type | Method | Description |
| - | - | - |
| `InetAddress` | `getAddress()` | Returns the IP address of the machine to/from which this datagram is being sent/was received. |
| `byte[]` | `getData()` | Returns the data buffer. |
| `int` | `getLength()` | Returns the length of the data in the buffer. |
| `int` | `getPort()` | Returns the port number on the remote host to/from which this datagram is being sent/was received. |
| `SocketAddress` | `getSocketAddress()` | Returns the socket address of the remote host to/from which this datagram is being sent/was received. |


### Setter methods

| Type | Method | Description |
| - | - | - |
| `void` | `setAddress(InetAddress address)` | Sets the IP address of the machine to which this datagram is being sent. |
| `void` | `setData(byte[] buf)` | Set the data buffer for this packet. |
| `void` | `setLength(int length)` | Set the length for this packet.
| `void` | `setPort(int port)` | Sets the port number on the  |remote host to which this datagram is being sent. |
| `void` | `setSocketAddress(SocketAddress address)` | Sets the SocketAddress (usually IP address + port number) of the remote host to which this datagram is being sent. |



## The `DatagramSocket` class

The [`DatagramSocket`](https://docs.oracle.com/javase/8/docs/api/java/net/DatagramSocket.html) class represents endpoints for sending and receiving UDP packets.

### Constructors

| Constructor | Description |
| - | - |
| `DatagramSocket()` | Constructs a datagram socket bound to the wildcard address and any available port. |
| `DatagramSocket(int port)` | Constructs a datagram socket bound to the wildcard address and the port `port`. |
| `DatagramSocket(int port, InetAddress address)` | Constructs a datagram socket bound to `address:port`. |
| `DatagramSocket(SocketAddress address)` | Constructs a datagram socket bound to `address`. |


### Getter methods

| Type | Method | Description |
| - | - | - |
| `InetAddress` | `getInetAddress()` | Returns the remote address to which the socket is connected. |
| `InetAddress` | `getLocalAddress()` | Returns the local address to which the socket is bound. |
| `int` | `getLocalPort()` | Returns the local port number to which the socket is bound. |
| `SocketAddress` | `getLocalSocketAddress()` | Returns the local address to which socket is bound. |
| `int` | `getPort()` | Returns the remote port number to which the socket is connected. |
| `SocketAddress` | `getRemoteSocketAddress()` | Returns the remote address the socket is connected to, or null if it is unconnected. |


### Action methods

| Type | Method | Description |
| - | - | - |
| `void` | `bind(SocketAddress address)` | Binds the socket to `address`. |
| `void` | `close()` | Closes the socket. |
| `void` | `connect(InetAddress address, int port)` | Connects the socket to the remote address `address:port`. |
| `void` | `connect(SocketAddress address)` | Connects the socket to the remove address `address`. |
| `void` | `disconnect()` | Disconnects the socket. |
| `void` | `receive(DatagramPacket p)` | Receives a datagram packet from this socket and saves it in `p`. |
| `void` | `send(DatagramPacket p)` | Sends the datagram packet `p` from this socket. |
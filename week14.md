# Week 14 (calendar week 19)

## Hand-in

See [my notes](javasockets.md) on socket programming in Java.

### Task 1

There are various ways of implementing the connections:

- One is to have a single connection $\mathcal{C}'$ between the server $S$ and the destination $D$. All threads must then have access to the `Socket` object corresponding to $\mathcal{C}'$. One potential issue with this approach is that each thread might read from the input stream independently of the other threads, which means that each byte sent by $D$ will only arrive at a single client. A potential fix is to maintain a list of connections and have a single thread monitor input from $D$, and upon input iterate through the above list and send the message to all clients.
- Another is to have multiple connections between $S$ and $D$, one for each connection between a client and $S$. This introduces some redundancy but is much simpler to implement. An issue is that, contrary to in last week's exercises, we allow *both* clients and $D$ to send messages. Care must then be taken that each client and each connection to $D$ have a thread that takes care of input from the corresponding connection.


### Task 4

The sample execution works as follows: The role of $D$ is played by the SSH server running on the Raspberry Pi. Traffic directed to `localhost:5555` is redirected (using **port forwarding**) to port `22` on the Raspberry Pi (recall the option `hostfwd=tcp::5555-:22` given to the `qemu-system-aarch64` command when booting up the Raspberry Pi), which is the standard SSH server port.

Another way to test this would have been to use netcat to listen on some port (playing the role of $D$), and then connecting to this port using `ServerTunnel`. Sadly, netcat does not support multiple ingoing connections, so this is not an option.

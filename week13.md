# Week 13 (calendar week 18)

## Exercises

### Task 2

When running `ifconfig` (or `ip addr`) I variously see the interface `eth0` or an interface called `usb0`. I am not quite sure why.

(On my personal Ubuntu VM the corresponding interface is called `enp0s3`. Apparently this [has something to do](https://askubuntu.com/questions/704035/no-eth0-listed-in-ifconfig-a-only-enp0s3-and-lo) with the [systemd](https://wiki.debian.org/systemd) manager that Debian, on which both Ubuntu and Raspberry Pi OS are based, uses.)


### Task 3

Instead of the command `netstat -t -l` you can also just run `netstat -tl`. When running this command you will see that the machine is listening on `0.0.0.0:ssh`. Here `ssh` just stands for the standard SSH port 22, and the IP address `0.0.0.0` means that it is listening on all IP addresses on the machine.

Running `netstat -t` on my machine I see a connection with local address `10.0.2.15:ssh` and foreign address `10.0.2.2:1136`, both of which are [private network addresses](https://en.wikipedia.org/wiki/Private_network). (Indeed, from running `ifconfig` we see that the network mask is `255.255.255.0`, so they are indeed on the same network, namely `10.0.2.0/24`.) Hence our IP address, and the IP address we are connected to, are not public, indicating that [NAT](https://en.wikipedia.org/wiki/Network_address_translation) is used by the host machine to create a private network on which the Raspberry Pi is located.

Running `netstat` (or `netstat -n`) on my Windows host tells me that I am connected on `localhost:5555` to `localhost:1136` (recall that `localhost` is `127.0.0.1`). That is, the ports are the same but the IP addresses are different: Recall that an IP address is associated with a network layer *network interface*, not the machine itself. The host machine thus creates a (virtual) interface with IP address `10.0.2.2` to which the Raspberry Pi can connect.

(On my personal Ubuntu VM I use port forwarding to redirect data from host to guest, and I see exactly the same behaviour. So presumably that's what's happening?)


### Task 7

Here you will probably see *two* connections, one with local address `localhost:4321` and one with foreign address `localhost:4321`. This is simply because the machine is connected to itself on two different ports, both of which are both local and foreign.


### Task 8

On Windows, instead of Telnet you can also install and use [Nmap](https://nmap.org/download.html#windows), which ships with the tool Ncat (command `ncat`) corresponding to netcat on UNIX. Apparently there is [an issue](https://github.com/openssl/openssl/issues/19191) with the latest version of Nmap, so installing the previous version 7.92 (download link [here](https://nmap.org/dist/nmap-7.92-setup.exe)) is recommended.


### Task 9

On Windows you can again use Ncat instead of Telnet.

Note the behaviour of the program so far: When a client $C$ connects to the server $S$ and sends a message, the message is sent back to $C$. In Task 10 we will modify this behaviour.


### Task 10

The desired behaviour is as follows: The server $S$ listens for connections, and an arbitrary number of clients $C_1, \ldots, C_n$ can connect to $S$ simultaneously. When a client $C_i$ sends a message to $S$, this is just sent back to $C_i$. That is, each client is oblivious to the fact that other clients are connected to $S$. In particular, $S$ only sends a message to $C_i$ if $C_i$ has just sent a message to $S$, so there is no fear of blocking, and we can reuse almost all of the given code.

In the tunneling hand-in the desired behaviour is slightly different, so don't be confused by the comments regarding the hand-in. (In the hand-in we *do* need to worry about blocking.)


## Hand-in

No hand-in this week.


## Further reading

For more on how virtual machines deal with networking, the [VirtualBox manual](https://www.virtualbox.org/manual/ch06.html) covers many topics that are relevant for other virtualisation software packages such as QEMU and VMWare Workstation (Player).

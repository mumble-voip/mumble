Mumble - A voicechat utility for gamers
=======================================

> *http://mumble.info/*  
> *#mumble on freenode*

Mumble is a voicechat program for gamers written on top of Qt and Opus.

There are two modules in Mumble; the client (mumble) and the server
(murmur). The client works on Win32/64, Linux and Mac OS X, while the
server should work on anything Qt can be installed on.

Note that when we say Win32, we mean Windows XP or newer.

## Setting up Mutter on Ubuntu

First, log into your Ubuntu machine. It is recommended that you just change
your user to root rather than typing sudo at the start of every command.

`sudo -s`

This will change your user to root. It is considered good practice to type
`exit` as soon as you're done using any kind of commands that require root.

You should update your package manager and make sure all installed packages 
are up-to-date. Type 'y' if prompted to.

```
apt-get update
apt-get upgrade
```

Now it's time to get all the necessary dependencies. Note that this list of
dependencies it NOT identical to the list of mumble dependencies; two
additional packages are needed to be installed in order to complete a later
qt-related command.

Just copy and paste the following into your linux
terminal (inside 'real' linux distros, not just windows subsystems, to paste 
into terminal, you need to use CTRL+SHIFT+v).

```
apt-get install build-essential pkg-config qt5-default libqt5svg5-dev \
                libboost-dev libasound2-dev libssl-dev \
                libspeechd-dev libzeroc-ice-dev libpulse-dev \
                libcap-dev libprotobuf-dev protobuf-compiler \
                libogg-dev libavahi-compat-libdnssd-dev libsndfile1-dev \
                libg15daemon-client-dev libxi-dev qttools5-dev-tools libminiupnpc-dev
```

The next step is obtaining the source code from the git repository. Install
git in case you don't already have it with `apt-get install git`. 

```
git clone https://github.com/2uh/Mutter.git
cd Mutter
git submodule init
git submodule update
```

You now should have all the necessary files on your machine. These next
couple steps might take a little bit, so keep that in mind.

To speed up the process `make`, it's recommended that you instead run the 
command `make -j$(nproc)`, which will run `make` but using multiple threads,
provided that you have a cpu that supports multithreading. You don't have
to know if your cpu does support it though, the command calculates how
many threads your computer can use for `make` and uses those.
 
If for whatever reason you don't feel comfortable using multiple threads 
or you get some kind of error, you can just use `make` instead of 
`make -j$(nproc)`, though compiling will likely take significantly longer.

```
qmake -recursive main.pro
make -j$(nproc)
```

From here, you should be able to run Mutter locally by simply going into
`./Mutter/release` via `cd release` and running `./mumble`.

## Running Mumble

On Windows, after installation, you should have a new Mumble folder in your
Start Menu, from which you can start Mumble.

On Mac OS X, to install Mumble, drag the application from the downloaded
disk image into your `/Applications` folder.

Once Mumble is launched, you need a server to connect to. Either create your
own or join a friend's.

## Running Murmur on Unix-like systems

Murmur should be run from the command line, so start a shell (command prompt)
and go to wherever you installed Mumble. Run murmur as

```
murmurd [-supw <password>] [-ini <inifile>] [-fg] [v]

-supw   Set new password for the user SuperUser, which is hardcoded to
        bypass ACLs. Keep this password safe. Until you set a password,
        the SuperUser is disabled. If you use this option, murmur will
        set the password in the database and then exit.

-ini    Use a inifile other than murmur.ini, use this to run several instances
        of murmur from the same directory. Make sure each instance is using
        a separate database.

-fg     Run in the foreground, logging to standard output.

-v      More verbose logging.
```

## Running Murmur on Mac OS X

Murmur is distributed seperately from the Mumble client on Mac OS X.
It is called Static OS X Server and can be downloaded from the main webpage.

Once downloaded it can be run in the same way as on any other Unix-like system.
For more information please see the 'Running Murmur on Unix-like systems' above.

## Running Murmur on Win32

Doubleclick the Murmur icon to start murmur. There will be a small icon on your
taskbar from which you can view the log.

To set the superuser password, run murmur with the parameters `-supw <password>`.

## Bandwidth usage

Mumble will use 10-40 kbit/s outgoing, and the same incoming for each user.
So if there are 10 other users on the server with you, your incoming
bandwidth requirement will be 100-400 kbit/s if they all talk at the same time.


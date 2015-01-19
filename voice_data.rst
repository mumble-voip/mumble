.. _voice-data:

Voice data
==========

.. _enable-udp-channel:

Enabling the UDP channel
------------------------

Before the UDP channel can reliably be used both sides should
be certain that the connection works. Before the server may use
the UDP connection to the client the client must first open a UDP
socket and communicate its address to the server by sending a packet
over UDP. Once the server has received an UDP transmission the server
should start using the UDP channel for the voice packets. Respectively 
he client should not use the UDP channel for voice data until it is
certain that the packets go through to the server.

In practice these requirements are filled with UDP ping. When the server
receives a UDP ping packet from the client it echoes the packet back.
When the client receives this packet it can ascertain that the UDP channel
works for two-way communication.

.. _udp-ping-packet:

.. table:: UDP ping packet
   
   +---------------------------------------------------------+
   | UDP ping packet                                         |
   +======================+==================================+
   | byte                 | type/flags (0010 0000 for Ping)  |
   +----------------------+----------------------------------+
   | varint               | timestamp                        |
   +----------------------+----------------------------------+

If the client stops receiving replies to the UDP packets at some point or never
receives the first one it should immediately start tunneling the voice communication
through TCP as described in the *UDP tunnel* section. When the server
receives a tunneled packet over the TCP connection it must also stop using
the UDP for communication. The client may continue sending UDP ping packets
over the UDP channel and the server must echo these if it receives them. If
the client later receives these echoes it may switch back to the UDP channel
for voice communication. When the server receives an UDP voice communication
packet from the client it should stop tunneling the packets as well.


.. _udp-data:

Data
----

The voice data is transmitted in variable length packets that consist of header portion,
followed by repeated data segments and an optional position part. The full packet
structure is shown in the figure below, and consists of three parts. The decrypted
data should never be longer than 1020 bytes, this allows the use of 1024 byte UDP
buffer even after the 4-byte encryption header is added to the packet during the
encryption. The protocol transfers 64-bit integers using variable length encoding.
This encoding is specified in the *varint* section.

   A voice packet starts with a header:

   +------------------------------------------------------------------------------+
   | Voice packet header                                                          |
   +----------------------+---------------+---------------------------------------+
   | Type                 | Field         | Description                           |
   +======================+===============+=======================================+
   | byte                 | Type/Flags    | Bitfield  **76543210**   ,            |
   |                      |               | 7-5 Type(*), 4-0 Target               |
   +----------------------+---------------+---------------------------------------+
   | varint               | Session       | The session number of the source user |
   |                      |               | (only from server)                    |
   +----------------------+---------------+---------------------------------------+
   | varint               | Sequence      |                                       |
   +----------------------+---------------+---------------------------------------+

   Followed by one or more audio data segments:

   +--------------------------------------------------------------------------------+
   | Voice packet audio data                                                        |
   +----------------------+---------------+-----------------------------------------+
   | Type                 | Field         | Description                             |
   +==============+=======+===============+=========================================+
   | Header       | byte  | Header (CELT) | Bitfield **76543210**,                  |
   |              |       |               | Bit 7: Terminator, Bit 6-0: Data length |
   | depends on   +-------+---------------+-----------------------------------------+
   |              | varint| Header (OPUS) | Bitfield **FEDCBA9876543210**           |
   | packet type  | int16 |               | Bit D: Terminator, Bit C-0: Data length |
   +--------------+-------+---------------+-----------------------------------------+
   | byte[]               | Data          | Encoded voice frames                    |
   +----------------------+---------------+-----------------------------------------+
  
   Followed by an optional set of positional audio coordinates:

   +--------------------------------------------------------------------------------+
   | Voice packet positional audio data                                             |
   +----------------------+---------------+-----------------------------------------+
   | Type                 | Field         | Description                             |
   +======================+===============+=========================================+
   | float                | Position 1    |                                         |
   +----------------------+---------------+-----------------------------------------+
   | float                | Position 2    |                                         |
   +----------------------+---------------+-----------------------------------------+
   | float                | Position 3    |                                         |
   +----------------------+---------------+-----------------------------------------+

The first byte of the header contains the packet type and additional target specifier.
The format of this byte is described below. If the voice packet comes from the server,
the type is followed by a *varint* encoded value that specifies the session this
voice packet originated from -- this information is added by the server and the client
omits this field. The last segment in the header is a sequence number for the first
audio frame of the packet. If there are for example two frames in the packet, the sequence
field of the next packet should be incremented by two.

The type is stored in the first three bits and specifies the type and encoding of the packet.
Current types are listed in *UDP Types* table. The remaining 5 bits specify additional
packet-wide options. For voice packets the values specify the voice target as listed in the
table below:

.. _udp-types:

.. table:: UDP Types

   +--------------------------+--------------------------------------------+
   | Type      Bitfield       | Description                                |
   +==========================+============================================+
   | 0        [000xxxxx]      | CELT Alpha encoded voice data              |
   +--------------------------+--------------------------------------------+
   | 1        [001xxxxx]      | Ping packet                                |
   +--------------------------+--------------------------------------------+
   | 2        [010xxxxx]      | Speex encoded voice data                   |
   +--------------------------+--------------------------------------------+
   | 3        [011xxxxx]      | CELT Beta encoded voice data               |
   +--------------------------+--------------------------------------------+
   | 4        [100xxxxx]      | OPUS encoded voice data                    |
   +--------------------------+--------------------------------------------+
   | 5-7                      | Unused                                     |
   +--------------------------+--------------------------------------------+

.. _udp-targets:

.. table:: UDP targets

   +-----------+--------------------------------------------+
   | Target    | Description                                |
   +===========+============================================+
   | 0         | Normal talking                             |
   +-----------+--------------------------------------------+
   | 1         | Whisper to channel                         |
   +-----------+--------------------------------------------+
   | 2-30      | Direct whisper (always 2 for incoming      |
   |           | whisper)                                   |
   +-----------+--------------------------------------------+
   | 31        | Server loopback                            |
   +-----------+--------------------------------------------+

The audio frames consist of one byte long header and up to 127 bytes long data portion.
The first bit in the header is the *terminator bit* which informs the receiver
whether there are more audio frames after this one. This bit is turned on (value *1*)
for all but the last frame in the current UDP packet. Rest of the seven bits in the header
specify the length of the data portion. The data portion is encoded using one of the
supported codecs. The exact codec is specified in the type portion of the whole packet
(See the UDP types table). *The data in each frame is encoded separately.*

Codecs
------

Mumble supports two distinct codecs; Low bit rate audio uses Speex and higher quality
audio is encoded with CELT. Both of these codecs must be supported for full support
of the Mumble protocol. Furthermore, as the CELT bitstream has not been frozen yet
which places requirements for the exact CELT version: The clients must support
CELT 0.7.1 bitstream. The protocol includes codec negotiation which allows clients
to support other codec versions as well, in which case the server should attempt
to negotiate a version that all clients support. The clients must respect the
server resolution.

Whispering
----------

Normal talking can be heard by the users of the current channel and all linked
channels as long as the speaker has Talk permission on these channels. If the
speaker wishes to broadcast the voice to specific users or channels, he may
use whispering. This is achieved by registering a voice target using the
VoiceTarget message and specifying the target ID as the target in the first
byte of the UDP packet.

Varint and 64-bit integer encoding
----------------------------------

The variable length integer encoding is used to encode long, 64-bit,
integers so that short values do not need the full 8 bytes to be transferred.
The basic idea behind the encoding is prefixing the value with a length prefix
and then removing the leading zeroes from the value. The positive numbers are
always right justified. That is to say that the least significant bit in the
encoded presentation matches the least significant bit in the decoded presentation.
The *varint prefixes* table contains the definitions of the different length
prefixes. The encoded **x** bits are part of the decoded number while the **_**
signifies a unused bit. Encoding should be done by searching the first decoded
description that fits the number that should be decoded, truncating it to the
required bytes and combining it with the defined encoding prefix. 

See the *quint64* shift operators in
https://github.com/mumble-voip/mumble/blob/master/src/PacketDataStream.h
for a reference implementation.

.. table:: Varint prefixes

   +-----------------------------------+--------------------------------------------------------+
   | Encoded                           | Decoded                                                |
   +===================================+========================================================+
   | **0xxxxxxx**                      | 1 byte with :math:`7 \cdot 8 + 1` leading zeroes       |
   +-----------------------------------+--------------------------------------------------------+
   | **10xxxxxx** + 1 byte             | 2 bytes with :math:`6 \cdot 8 + 2` leading zeroes      |
   +-----------------------------------+--------------------------------------------------------+
   | **110xxxxx** + 2 bytes            | 3 bytes with :math:`5 \cdot 8 + 3` leading zeroes      |
   +-----------------------------------+--------------------------------------------------------+
   | **1110xxxx** + 3 bytes            | 4 bytes with :math:`4 \cdot 8 + 4` leading zeroes      |
   +-----------------------------------+--------------------------------------------------------+
   | **111100__** + **int** (4 bytes)  | 32-bit positive number                                 |
   +-----------------------------------+--------------------------------------------------------+
   | **111101__** + **long** (8 bytes) | 64-bit number                                          |
   +-----------------------------------+--------------------------------------------------------+
   | **111110__** + **varint**         | Negative varint                                        |
   +-----------------------------------+--------------------------------------------------------+
   | **111111xx**                      | Byte-inverted negative two byte number (~xx)           |
   +-----------------------------------+--------------------------------------------------------+

The variable length integer encoding is used to encode long (64-bit) integers so that
short values do not need the full 8 bytes to be transferred. The encoding function is
given below. While it might seem complex it is worth noting that the
:math:`(a_v, a_p) \append (b_v, b_p)` function equals appending the :math:`a_p` bits
long value :math:`a_v` to a byte stream that already has the :math:`b_p` bits long
value :math:`b_v`.

.. % Encoding function
.. % \begin{align*}
.. % 	(a_v, a_p) \append (b_v, b_p) &= (2^{b_p} a_v + b_v, a_p + b_p) \\
.. % %
.. % 	e &: \mathbb{N} \rightarrow \mathbb{N}_{\geq0}^2 \\
.. % 	e(x) &= \begin{dcases*}
.. % 			e_+(x, 1)										& when $ 0 \leq x < 2^{28} $ \\
.. % 			\left((2^8 - 2^4) \cdot {2^8}^4 + x, 2^{40}\right)			& when $ 2^{28} \leq x < 2^{32} $ \\
.. % 			\left((2^8 - 2^4 + 2^2) \cdot {2^8}^8 + x, 2^{72}\right)	& when $ 2^{32} \leq x $ \\
.. % 			(2^8 - 2^2 - x, 8)								& when $ -4 < x < 0 $ \\
.. % 			(2^8 - 2^3, 8) \append e(-x)					& when $ x \leq -4 $ \\
.. % 		\end{dcases*} \\
.. % %
.. % 	e_+(x, b) &= \begin{dcases*}
.. % 			(p(b) + x, 8)												& when $ r < 2^(8-b) $ \\
.. % 			e_+\left(\left\lfloor \frac{x}{2^8} \right\rfloor, b + 1\right) \append (x \bmod 2^8, 8)	& when $ r \geq 2^(8-b) $
.. % 		\end{dcases*} \\
.. % %
.. % 	p(b) &= 2^8 - 2^{9-b}
.. % \end{align*}
.. 
.. % Decoding is performed by analyzing the first byte after which the rest of the number can be read from the byte stream.
.. 
.. % Decoding function
.. % \begin{align*}
.. % 	s_0(x) &= 8 - \left\lfloor log_2(2^8-1 - x) \right\rfloor \\
.. % %
.. % 	f_x &: \mathbb{N}_{\geq0} \rightarrow [0, 2^8) \\
.. % 	d &: f \rightarrow \mathbb{N}, f = \{ f_1, f_2, f_3, ... \} \\
.. % 	d(f) &= \begin{dcases*}
.. % 			d_+\Big(f, s_0\big(f(0)\big)\Big)													& when $f(0) \leq 2^8 - 2^4 $ \\
.. % 			\sum_{i=0}^4 2^{32-8i}f(i)								& when $f(0) = 2^8 - 2^4 $ \\
.. % 			\sum_{i=0}^8 2^{64-8i}f(i)								& when $f(0) = 2^8 - 2^4 + 2^2 $ \\
.. % 			-d(g : g(n) = f(n+1))									& when $f(0) = 2^8 - 2^3 $ \\
.. % 			(2^8 - 2^2) - f(0)										& when $f(0) \geq 2^8 - 2^2 $ \\
.. % 		\end{dcases*} \\
.. % %
.. % 	d_+(f, z) &= -2^{8z - 7z} + \sum_{i=1}^z 2^{8z-8i}f(i-1)
.. % \end{align*}

.. _tcp-tunnel:

TCP tunnel
----------

If the UDP channel isn't available the voice packets must be transmitted
through the TCP socket. These messages use the normal TCP prefixing, as seen in shown in
figure :ref:`mumble-packet`: 16-bit message type followed by 32-bit message
length. However unlike other TCP messages, the UDP packets are not encoded as
protocol buffer messages but instead the raw UDP packet described in section :ref:`udp-data`
should be written to the TCP socket directly.

When the packets are received it is safe to parse the type and length fields normally.
If the type matches that of the UDP tunnel the rest of the message should be processed
as an UDP packet without attempting a protocol buffer decoding.

Encryption
----------

All the packets are encrypted once during transfer. The actual encryption depends on the
used transport layer. If the packets are tunneled through TCP they are encrypted using the
TLS that encrypts the whole TCP connection and if they are sent directly using UDP they must
be encrypted using the OCB-AES128 encryption.

Implementation notes
--------------------

When implementing the protocol it is easier to ignore the UDP transfer layer at
first and just tunnel the UDP data through the TCP tunnel. The TCP layer must be implemented
for authentication in any case. Making sure that the voice transmission works before
implementing the UDP protocol simplifies debugging greatly. The UDP protocol is a required
part of the specification though.

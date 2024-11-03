Overview
========

Mumble is based on a standard server-client communication model. It
utilizes two channels of communication, the first one is a TCP connection
which is used to reliably transfer control data between the client and the
server. The second one is a UDP connection which is used for unreliable,
low latency transfer of voice data.

.. figure:: resources/mumble_system_overview.png
   :alt: Mumble system overview
   :align: center

   Mumble system overview

Both are protected by strong cryptography, this encryption is mandatory and cannot be disabled. The TCP control channel uses TLSv1 AES256-SHA [#f1]_ while the voice channel is encrypted with OCB-AES128 [#f2]_.

.. figure:: resources/mumble_crypt_types.png
   :alt: Mumble crypt types
   :align: center

   Mumble crypto types

While the TCP connection is mandatory the UDP connection can be compensated by tunnelling the UDP packets through the TCP connection as described in the protocol description later.

.. rubric:: Footnotes

.. [#f1] http://en.wikipedia.org/wiki/Transport_Layer_Security
.. [#f2] http://www.cs.ucdavis.edu/~rogaway/ocb/ocb-back.htm
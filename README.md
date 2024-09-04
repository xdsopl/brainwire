This is my poke at the [Neuralink Compression Challenge](https://content.neuralink.com/compression-challenge/README.html)

My goal was to see if my implementation of [Rice coding](https://en.wikipedia.org/wiki/Golomb_coding#Rice_coding) lifted from my lossless [DWT](https://github.com/xdsopl/dwt) based image compression project could be used to beat the compression ratio of 2.2 by simply using zip, while having a low latency of only one sample and a lower CPU usage:

```
*snip*
compression ratio = 316%
data/ff8b34c6-6b6c-4f74-957c-299954a1b247.wav losslessly compressed from 197542 bytes to 62508 bytes
Processing data/ff970660-0ffd-461f-93de-379e95cd784a.wav
compression ratio = 303%
data/ff970660-0ffd-461f-93de-379e95cd784a.wav losslessly compressed from 197422 bytes to 65153 bytes
Processing data/ffb6837e-be2b-474f-bdd0-3c9cd631f39d.wav
compression ratio = 301%
data/ffb6837e-be2b-474f-bdd0-3c9cd631f39d.wav losslessly compressed from 197442 bytes to 65383 bytes
All recordings successfully compressed.
Original size (bytes): 146800526
Compressed size (bytes): 47841362
Compression ratio: 3.06
```
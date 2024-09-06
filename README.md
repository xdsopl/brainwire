This is my poke at the [Neuralink Compression Challenge](https://content.neuralink.com/compression-challenge/README.html)

My goal was to see if my implementation of [Rice coding](https://en.wikipedia.org/wiki/Golomb_coding#Rice_coding) lifted from my lossless [DWT](https://github.com/xdsopl/dwt) based image compression project could be used to beat the compression ratio of 2.2 by simply using zip, while having a low latency of only one sample and a lower CPU usage:

```
./eval.sh
Archive:  data.zip
   creating: data/
  inflating: data/102b47d9-371e-412a-8995-0dc6115ab2bb.wav
  inflating: data/2eef5d4d-93d1-4c0e-9d23-0989abaa34d0.wav
  inflating: data/fa2c5efb-cc0d-4292-ab99-91d345cf17d9.wav
*snip*
compression ratio = 358%
data/fefade45-a947-43b3-8b55-578b1c7c74ed.wav losslessly compressed from 197528 bytes to 55079 bytes
Processing data/ff07d2c2-51a1-49ec-85ab-80df95461ce5.wav
compression ratio = 367%
data/ff07d2c2-51a1-49ec-85ab-80df95461ce5.wav losslessly compressed from 197496 bytes to 53703 bytes
Processing data/ff8b34c6-6b6c-4f74-957c-299954a1b247.wav
compression ratio = 343%
data/ff8b34c6-6b6c-4f74-957c-299954a1b247.wav losslessly compressed from 197542 bytes to 57464 bytes
Processing data/ff970660-0ffd-461f-93de-379e95cd784a.wav
compression ratio = 329%
data/ff970660-0ffd-461f-93de-379e95cd784a.wav losslessly compressed from 197422 bytes to 59911 bytes
Processing data/ffb6837e-be2b-474f-bdd0-3c9cd631f39d.wav
compression ratio = 328%
data/ffb6837e-be2b-474f-bdd0-3c9cd631f39d.wav losslessly compressed from 197442 bytes to 60064 bytes
All recordings successfully compressed.
Original size (bytes): 146800526
Compressed size (bytes): 44022077
Compression ratio: 3.33
```

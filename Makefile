test: encode decode
	./encode input.wav output.brainwire
	./decode output.brainwire output.wav
	diff -q -s input.wav output.wav
encode: encode.c
decode: decode.c
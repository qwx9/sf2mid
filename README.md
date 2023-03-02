# sf2mid: TinySoundFont-based midi player using soundfont2 banks

## Usage

Recommended soundfont: Patch93's Roland SC55 font.

Example usage: playing a doom midi file

	; games/wadfs /sys/games/lib/doom/doom1.wad
	createfile SW18_7: file already exists
	; games/mus </mnt/wad/d_e1m3 \
		| games/sf2mid /lib/midi/sf2/patch93.sc-55.sf2 >/dev/audio

See: [mus(1)](http://man.9front.org/1/mus).

Example usage: using sf2mid for playback in doom(1):

	Edit dmus(1):
	; cat /bin/dmus
	#!/bin/rc
	#sf2=sc55.v3.7.sf2
	sf2=patch93.sc-55.sf2
	if(test -f /lib/midi/sf2/$sf2)
		c=(games/sf2mid /lib/midi/sf2/$sf2)
	if not if(test -f /tmp/genmidi.*)
		c=(games/dmid -i /tmp/genmidi.* '|' games/opl3)
	if not
		c=(games/midi -c)
	if(~ `{file -m $1} audio/mus)
		c=(games/mus '<' $1 '|' $c)
	if not
		c=('<' $1 $c)
	eval $c

Current port-specific issues: performance is not great;
doom will hang while sf2mid unscrews itself.

## Library

See [TinySoundFont library](https://github.com/schellingb/TinySoundFont)
for more information.

License: MIT

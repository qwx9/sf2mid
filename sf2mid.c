/* adapted from examples */
#define TSF_RENDER_EFFECTSAMPLEBLOCK 32
#define TSF_IMPLEMENTATION
#include "tsf.h"
#define TML_IMPLEMENTATION
#include "tml.h"

#include <unistd.h>

tsf *ttsf;
double T;	/* msec */
tml_message* next;

enum{
	Nsamp = 8192,
	Rate = 44100,
};

static int
samp(void*, tsf_s16 *stream, int samp)
{
	int chunk;

	for(chunk=512; samp!=0; samp-=chunk, stream+=chunk*2){
		if(chunk > samp)
			chunk = samp;
		T += (1000.0 / Rate) * chunk;
		for(; next && T >= next->time; next = next->next){
			switch(next->type){
				case TML_PROGRAM_CHANGE:
					tsf_channel_set_presetnumber(ttsf, next->channel,
						next->program, next->channel == 9);
					break;
				case TML_NOTE_ON:
					tsf_channel_note_on(ttsf, next->channel,
						next->key, next->velocity / 127.0);
					break;
				case TML_NOTE_OFF:
					tsf_channel_note_off(ttsf, next->channel,
						next->key);
					break;
				case TML_PITCH_BEND:
					tsf_channel_set_pitchwheel(ttsf, next->channel,
						next->pitch_bend);
					break;
				case TML_CONTROL_CHANGE:
					tsf_channel_midi_control(ttsf, next->channel,
						next->control, next->control_value);
					break;
			}
		}
		if(next == NULL)
			return -1;
		tsf_render_short(ttsf, stream, chunk, 0);
	}
	return 0;
}

int
main(int argc, char **argv)
{
	FILE *out;
	char *sf, *mid;
	tml_message* tml;
	tsf_s16 obuf[2 * Nsamp];

	sf = NULL;
	mid = "/fd/0";
	out = stdout;
	if(argc < 2){
		fprintf(stderr, "usage: %s SF2BANK [FILE]\n", argv[0]);
		return 1;
	}
	if(argc >= 2)
		sf = argv[1];
	if(argc >= 3)
		mid = argv[2];
	if((tml = tml_load_filename(mid)) == NULL){
		fprintf(stderr, "error reading midi file\n");
		return 1;
	}
	if((ttsf = tsf_load_filename(sf)) == NULL){
		fprintf(stderr, "error loading soundfont\n");
		return 2;
	}
	next = tml;
	// arm 10th channel for percussion sound bank (128) if available
	tsf_channel_set_bank_preset(ttsf, 9, 128, -3.0);
	tsf_set_output(ttsf, TSF_STEREO_INTERLEAVED, Rate, 0.0);
	for(;;){
		if(samp(NULL, obuf, Nsamp) < 0)
			break;
		fwrite(obuf, Nsamp, 4, out);
	}
	return 0;
}

#include "MIDIFormater.h"


MIDIFormater::MIDIFormater() {
}

MIDIFormater::~MIDIFormater()
{
}

void MIDIFormater::konvertuj(Mapa & mapa, Kompozicija * kompozicija) {
	konvertujMIDI(kompozicija->getMelody(&mapa), kompozicija->getRhithm());
}

void MIDIFormater::konvertujMIDI(std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> melody, std::pair<std::vector<int>, std::vector<int>> rhythm) {
	
	smf::MidiFile outputfile;
	outputfile.absoluteTicks();
	std::vector<unsigned char> midievent;
	midievent.resize(3);
	int tpq = 48;
	outputfile.setTicksPerQuarterNote(tpq);
	outputfile.addTrack(1);
	
	std::vector<std::vector<int>> melodyLeva = melody.first;
	//int melody[50] =
	//{ 72,72,79,79,81,81,79,77,77,76,76,74,74,72,-1 };

	std::vector<int> ritamLeva = rhythm.first;
	//int mrhythm[50] = { 1,1,1,1,1,1,2,1,1,1,1,1,1,2,-1 };
	
	std::vector<std::vector<int>> melodyDesna = melody.second;
	//int bass[50] =
	//{ 48,60,64,60,65,60,64,60,62,59,60,57,53,55,48,-1 };
	
	std::vector<int> ritamDesna = rhythm.second;
	//int brhythm[50] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,-1 };
	
	int i = 0;
	int actiontime = 0;
	midievent[2] = 64;

	std::for_each(melodyLeva.begin(), melodyLeva.end(), [&i, &midievent, &actiontime, &outputfile, &tpq, &ritamLeva](std::vector<int>& simboli) {
		int pomocna;
		std::for_each(simboli.begin(), simboli.end(), [&tpq, &i, &pomocna, &midievent, &actiontime, &outputfile, &ritamLeva](int &midiBroj) {
			if (midiBroj != 999) {
				if(ritamLeva[i]!=3)
					pomocna = tpq / 2 * ritamLeva[i];
				else 
					pomocna = tpq / 2 * 2;
				midievent[0] = 0x90;
				midievent[1] = midiBroj;
				outputfile.addEvent(1, actiontime, midievent); //1 leva, 0 desna
				midievent[0] = 0x80;
				outputfile.addEvent(1, actiontime + pomocna, midievent);
			}
		});
		if (ritamLeva[i] != 3)
			actiontime += tpq / 2 * ritamLeva[i];
		else 
			actiontime += tpq / 2 * 2;
		i++;
		

	});

	i = 0;
	actiontime = 0;

	std::for_each(melodyDesna.begin(), melodyDesna.end(), [&i, &midievent, &actiontime, &outputfile, &tpq, &ritamDesna](std::vector<int>& simboli) {
		int pomocna;
		std::for_each(simboli.begin(), simboli.end(), [&tpq, &i, &pomocna, &midievent, &actiontime, &outputfile, &ritamDesna](int &midiBroj) {
			if (midiBroj != 999) {
				if (ritamDesna[i] != 3)
					pomocna = tpq / 2 * ritamDesna[i];
				else
					pomocna = tpq / 2 * 2;
				midievent[0] = 0x90;
				midievent[1] = midiBroj;
				outputfile.addEvent(0, actiontime, midievent); //1 leva, 0 desna
				midievent[0] = 0x80;
				outputfile.addEvent(0, actiontime + pomocna, midievent);
			}
		});
		if (ritamDesna[i] != 3)
			actiontime += tpq / 2 * ritamDesna[i];
		else
			actiontime += tpq / 2 * 2;
		i++;
	});

	outputfile.sortTracks();
	outputfile.write("out.mid");
	return;
}


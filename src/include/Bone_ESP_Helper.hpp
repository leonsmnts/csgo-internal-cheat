#pragma once

#include "csgo_header.hpp"

// helper funcs to differentiate different models with different bone structures; defined below
bool isSAS(char* s);
bool isIDF(char* s);
bool isGIGN(char* s);
bool isFBI(char* s);
bool isST6(char* s);
bool isGSG9(char* s);
bool isSWAT(char* s);
bool isSEPARATIST(char* s);
bool isLEET(char* s);
bool isPIRATE(char* s);
bool isPHOENIX(char* s);
bool isPROFESSIONAL(char* s);
bool isBALKAN(char* s);
bool isANARCHIST(char* s);
bool isJUMPSUIT(char* s);

// bone index arrays
const int a[5][4] = { {8, 7, 0, 0}, {7, 10, 11, 12}, {7, 38, 39, 40}, {0, 65, 66, 67}, {0, 72, 73, 74} };

const int b[5][4] = { {8, 7, 0, 0}, {7, 11, 12, 13}, {7, 39, 40, 41}, {0, 66, 67, 68}, {0, 73, 74, 75} };
const int c[5][4] = { {8, 7, 0, 0}, {7, 11, 12, 13}, {7, 39, 40, 41}, {0, 70, 71, 72}, {0, 77, 78, 79} };
const int d[5][4] = { {8, 7, 0, 0}, {7, 11, 12, 13}, {7, 39, 40, 41}, {0, 71, 72, 73}, {0, 79, 80, 81} };
const int e[5][4] = { {8, 7, 0, 0}, {7, 11, 12, 13}, {7, 39, 40, 41}, {0, 71, 72, 73}, {0, 80, 81, 82} };

const int f[5][4] = { {8, 7, 0, 0}, {7, 11, 12, 13}, {7, 40, 41, 42}, {0, 69, 70, 71}, {0, 76, 77, 78} };
const int g[5][4] = { {8, 7, 0, 0}, {7, 11, 12, 13}, {7, 40, 41, 42}, {0, 72, 73, 74}, {0, 81, 82, 83} };

const int h[5][4] = { {8, 7, 0, 0}, {7, 11, 12, 13}, {7, 41, 42, 43}, {0, 70, 71, 72}, {0, 77, 78, 79} };

const int i[5][4] = { {8, 7, 0, 0}, {7, 12, 13, 14}, {7, 40, 41, 42}, {0, 67, 68, 69}, {0, 74, 75, 76} };


intptr_t getCorrectBoneIndexMatrix(Ent* entPtr) {
	if (!entPtr) {
		return 0;
	}

	char* modelName = (char*)(*(intptr_t*)((int)entPtr + 0x6c) + 0x04);
	modelName += 35; // skip "models/player/custom_player/legacy/"

	if (isST6(modelName) || isGSG9(modelName) || isSEPARATIST(modelName) || isPHOENIX(modelName) || isBALKAN(modelName)) {
		return (intptr_t)&a;
	}
	else if (isLEET(modelName) || (isJUMPSUIT(modelName) && *(modelName + 19) != 'a') || (isSWAT(modelName) && *(modelName + 16) == 'B')) {
		return (intptr_t)&b;
	}
	else if (isJUMPSUIT(modelName) || isANARCHIST(modelName) || isPIRATE(modelName) || isSWAT(modelName)) {
		return (intptr_t)&i;
	}
	else if (isFBI(modelName) && *(modelName + 15) != 'E') {
		return (intptr_t)&e;
	}
	else if (isFBI(modelName)) {
		return (intptr_t)&d;
	}
	else if (isPROFESSIONAL(modelName)) {
		return (intptr_t)&c;
	}
	else if (isGIGN(modelName)) {
		return (intptr_t)&f;
	} 
	else if (isIDF(modelName)) {
		return (intptr_t)&h;
	} 
	else { // isSAS
		return (intptr_t) &g;
	}
}

bool isTeamCT(int team) {
	return team == 3;
}

// some of the helpers dont check full name because no other model to match the sequence.
// e.g. only one model-name with sequence 's', 'a'. it's the SAS model.

bool isSAS(char* s) {
	return *(s + 4) == 's' && *(s + 5) == 'a';
}

bool isIDF(char* s) {
	return *(s + 4) == 'i' && *(s + 5) == 'd';
}

bool isGIGN(char* s) {
	return *(s + 4) == 'g' && *(s + 5) == 'i';
}

bool isFBI(char* s) {
	return *(s + 4) == 'f';
}

bool isST6(char* s) {
	return *(s + 4) == 's' && *(s + 5) == 't';
}

bool isGSG9(char* s) {
	return *(s + 4) == 'g' && *(s + 5) == 's';
}

bool isSWAT(char* s) {
	return *(s + 4) == 's' && *(s + 5) == 'w';
}

bool isSEPARATIST(char* s) {
	return *(s + 3) == 's';
}

bool isLEET(char* s) {
	return *(s + 3) == 'l';
}

bool isPIRATE(char* s) {
	return *(s + 3) == 'p' && *(s + 4) == 'i';
}

bool isPHOENIX(char* s) {
	return *(s + 3) == 'p' && *(s + 4) == 'h';
}

bool isPROFESSIONAL(char* s) {
	return *(s + 3) == 'p' && *(s + 4) == 'r';
}

bool isBALKAN(char* s) {
	return *(s + 3) == 'b';
}

bool isANARCHIST(char* s) {
	return *(s + 3) == 'a';
}

bool isJUMPSUIT(char* s) {
	return *(s + 3) == 'j';
}



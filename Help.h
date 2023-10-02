#pragma once
#ifndef  HELP_H
#define HELP_H

#include "LTexture.h"

class Help {
	LTexture HTexture;
	bool isRunning;
public:
	Help() : isRunning(false) {}
	void run();
};

#endif // ! HELP_H

/*
 *  SonosFileDialog.h
 *  videograb
 *
 *  Created by fabrizio on 28/03/11.
 *  Copyright 2011 Abstract Instruments. All rights reserved.
 *
 */


#ifndef _SONOS_FILE_DIALOG_
#define _SONOS_FILE_DIALOG_

#include "ofMain.h"


class SonosFileDialog {
public:
	static int openFile(string& URL);
	static int saveFile(string& URL, string& fileURL);	
	
};

#endif //_SONOS_FILE_DIALOG_

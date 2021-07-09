/************************************************************
URL
	http://www.slideshare.net/tado/media-art-ii-2013
************************************************************/


/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboSet.h"


/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
	/*
		PARTICLE_WIDTH	= 512,
		PARTICLE_HEIGHT	= 100,
	*/
		PARTICLE_WIDTH	= 200,
		PARTICLE_HEIGHT	= 200,
		
		NUM_PARTICLES	= PARTICLE_WIDTH * PARTICLE_HEIGHT,
	};
	
	/****************************************
	****************************************/
	ofx__VBO_SET VboSet;
	
	ofEasyCam cam;
	ofImage img;
	
	/********************
	********************/
	ofxPanel gui;
	ofxFloatSlider gui__particleSize;
	ofxFloatSlider gui__space_of_dot_x;
	ofxFloatSlider gui__space_of_dot_y;
	ofxFloatSlider gui__waveType;
	ofxFloatSlider gui__waveAmp;
	ofxColorSlider gui__col_particle;
	ofxToggle gui__useCam;
	ofxToggle gui__depthTest;
	ofxToggle gui__draw_BackImg;
	ofxToggle gui__draw_sphere;
	
	bool b_DispGui = false;
	
	ofColor LastCol_Particle;
	
	/****************************************
	****************************************/
	void clear_VboSetting_gl();
	void Refresh_vboVerts();
	void Refresh_vboColor();
	
public:
	/****************************************
	****************************************/
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};

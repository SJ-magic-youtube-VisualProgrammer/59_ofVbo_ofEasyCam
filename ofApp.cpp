/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
void ofApp::setup(){
	/********************
	ofSetVerticalSync();
		true
			60fps
			
		false
			trueの10倍くらい出る
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("study_vbo");
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	/*
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
	*/

	ofSetWindowShape(1280, 720);
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	gui.setup("GUI", "gui.xml", 10, 100);
	
	gui.add(gui__particleSize.setup("particleSize", 2.0, 1.0, 10.0));
	gui.add(gui__space_of_dot_x.setup("space_of_dot_x", 1.0, 1.0, 20));
	gui.add(gui__space_of_dot_y.setup("space_of_dot_y", 1.0, 1.0, 20));
	gui.add(gui__waveType.setup("waveType", 0.0, 0.0, 3.0));
	gui.add(gui__waveAmp.setup("waveAmp", 10.0, 0.0, 50.0));
	{
		ofColor initColor = ofColor(50, 127, 255, 255);
		ofColor minColor = ofColor(0, 0, 0, 0);
		ofColor maxColor = ofColor(255, 255, 255, 255);
		gui.add(gui__col_particle.setup("col_particle", initColor, minColor, maxColor));
		
		LastCol_Particle = gui__col_particle;
	}
	gui.add(gui__useCam.setup("useCam", true));
	gui.add(gui__depthTest.setup("depthTest", true));
	gui.add(gui__draw_BackImg.setup("draw_BackImg", true));
	gui.add(gui__draw_sphere.setup("draw_sphere", false));
	
	/********************
	********************/
	img.load("img/img.png");
	
	/********************
	********************/
	VboSet.setup(NUM_PARTICLES);
	Refresh_vboVerts();
	Refresh_vboColor();
	VboSet.update_vertex_color();
	
	
	cam.setDistance(100);
}

/******************************
1--2
|  |
0--3
******************************/
void ofApp::Refresh_vboVerts()
{
	float time_f = ofGetElapsedTimef();
	
	for(int x = 0; x < PARTICLE_WIDTH; x++){
		for(int y = 0; y < PARTICLE_HEIGHT; y++){
			float sin_x = gui__waveAmp * sin(x * 0.10 + time_f);
			float sin_y = gui__waveAmp * sin(y * 0.15 + time_f);
			
			float sin_z;
			if((int)gui__waveType == 0)			sin_z = 0.0;
			else if((int)gui__waveType == 1)	sin_z = sin_x;
			else if((int)gui__waveType == 2)	sin_z = sin_y;
			else if((int)gui__waveType == 3)	sin_z = sin_x + sin_y;
			
			VboSet.set_vboVerts(y * PARTICLE_WIDTH + x, (x - PARTICLE_WIDTH/2) * gui__space_of_dot_x, (y - PARTICLE_HEIGHT/2) * gui__space_of_dot_y , sin_z);
		}
	}
}

/******************************
******************************/
void ofApp::Refresh_vboColor()
{
	// ofColor col(50, 200, 255, 255);
	VboSet.set_singleColor(gui__col_particle);
	
	return;
}

/******************************
******************************/
void ofApp::update(){
	Refresh_vboVerts();
	VboSet.update_vertex();
	
	ofColor col_gui = gui__col_particle;
	if(col_gui != LastCol_Particle){
		Refresh_vboColor();
		VboSet.update_color();
	
		LastCol_Particle = gui__col_particle;
	}
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofBackground(0);
	
	/********************
	********************/
	if(gui__draw_BackImg){
		ofDisableDepthTest();
		
		ofSetColor(255, 255, 255, 100);
		img.draw(0, 0);
	}
	
	/********************
	********************/
	if(gui__depthTest)	ofEnableDepthTest();
	else				ofDisableDepthTest();
	
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing(); // OF_BLENDMODE_ADDとofEnableSmoothingは同時に有効化できない
	
	/********************
	********************/
	// ofSetHexColor(0xffffff);
	ofSetColor(255);
	
	/********************
	********************/
	if(gui__useCam){
		cam.begin();
	}else{
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	}
	
	glPointSize(gui__particleSize);
	VboSet.draw(GL_POINTS);
	
	if(gui__draw_sphere){
		ofSetColor(255, 0, 0, 200);
		ofNoFill();
		ofDrawSphere(ofPoint(0, 0, 0), 100);
	}
	
	if(gui__useCam)	cam.end();
	else			ofPopMatrix();
	
	
	/********************
	********************/
	// clear_VboSetting_gl();
	
	/********************
	********************/
	ofSetColor(255);
	string info;
	info = "Vertex num = " + ofToString(PARTICLE_WIDTH * PARTICLE_HEIGHT, 0) + "\n";
	info += "FPS = " + ofToString(ofGetFrameRate(), 2) + "\n";
	info += "cam distance = " + ofToString(cam.getDistance(), 2);
	ofDrawBitmapString(info, 30, 30);
	
	/********************
	********************/
	ofDisableDepthTest(); // need this to draw gui properly.
	if(b_DispGui) gui.draw();
}

/******************************
descrition
	ofVboで描画すると、openGlの設定が何らか変わるようだ。
	この結果、次に来る描画が所望の動作とならないケース多数。
		次のfunctionが描画されないなど
			ofDrawBitmapString()
			image.draw()
			
	この対応として、
		ofCircle(). ofRect().
	等を1発いれてやることで、OKとなった。
	おそらく、この関数内で、openGl設定が、また変わるのだろう。
		α = 0;
	にて描画する。
******************************/
void ofApp::clear_VboSetting_gl()
{
	ofSetColor(255, 255, 255, 0);
	ofDrawCircle(0, 0, 1);
	
	ofSetColor(255, 255, 255, 255);
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case 'r':
			cam.reset();
			cam.setDistance(100);
			break;
			
		case 'd':
			b_DispGui = !b_DispGui;
			break;
	}
}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxHistogram.h"

class ofApp : public ofBaseApp{

	public:
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
		
    void drawHistogram(vector<float> & h);
    
    int width, height;
    float fr;
    
    ofVideoGrabber cam;
    ofImage cap, spectrum;
    ofxCvGrayscaleImage gg;
    ofxHistogram histogram;
    vector<float> histogramG;
};

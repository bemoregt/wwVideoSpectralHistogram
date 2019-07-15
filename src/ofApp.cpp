#include "ofApp.h"

using namespace ofxCv;

void fftshift(Mat & in, Mat & out);
//--------------------------------------------------------------
void ofApp::setup(){

    width = 256;
    height = 256;
    
    cam.initGrabber(width, height);
    
    ofBackground(50);
}

//--------------------------------------------------------------
void ofApp::update(){

    cam.update();
    if(cam.isFrameNew()) {
        cap.setFromPixels(cam.getPixels());
        cap.update();
    
        cv::Mat img = toCv(cap);
        cv::Mat img2, img3;
        cv::cvtColor(img, img2, CV_RGB2GRAY);
        // resize
        float ratio = 256.0 / img2.cols;
        cv::resize(img2, img3, cv::Size(img2.cols*ratio, img2.rows*ratio));
        
        // 2d DFT
        cv::Mat planes[] = {cv::Mat_<float>(img3), cv::Mat::zeros(img3.size(), CV_32F)};
        cv::Mat complexImg;
        cv::merge(planes, 2, complexImg);
        cv::dft(complexImg, complexImg);
        // added 190709
        fftshift(complexImg, complexImg);
        
        cv::split(complexImg, planes);
        
        cv::Mat mag, logmag;
        cv::Mat mag1;
        
        cv::magnitude(planes[0], planes[1], mag);
        cv::log(mag, logmag);
        
        cv::normalize(logmag, mag1, 255, 0, cv::NORM_MINMAX, CV_8U);
        toOf(mag1, spectrum);
        spectrum.update();
        
        gg.setFromPixels(cam.getPixels().getChannel(1));
        
        histogramG = histogram.getHistogram(gg, 256);
    }//if
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    cap.draw(0,0, 512, 512);
    spectrum.draw(512,0, 512, 512);
    
    fr= ofGetFrameRate();
    // text
    ofSetColor(0);
    ofDrawBitmapString("Fr/sec= " + ofToString(fr), 20, 30);
    
    ofSetColor(0, 255, 0);
    //gg.draw(256, 0);
    drawHistogram(histogramG);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------
void fftshift(Mat & in, Mat & out) {
    out = in.clone();
    int mx1, my1, mx2, my2;
    mx1 = out.cols / 2;
    my1 = out.rows / 2;
    mx2 = int(ceil(out.cols / 2.0));
    my2 = int(ceil(out.rows / 2.0));
    Mat q0(out, cv::Rect(0, 0, mx2, my2));
    Mat q1(out, cv::Rect(mx2, 0, mx1, my2));
    Mat q2(out, cv::Rect(0, my2, mx2, my1));
    Mat q3(out, cv::Rect(mx2, my2, mx1, my1));
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q2.copyTo(tmp);
    q1.copyTo(q2);
    tmp.copyTo(q1);
    vconcat(q1, q3, out);
    vconcat(q0, q2, tmp);
    hconcat(tmp, out, out);
}

//--------------------------------------------------------------
void ofApp::drawHistogram(vector<float> & h) {
    ofBeginShape();
    ofNoFill();
    ofSetLineWidth(3);
    for (int i=0; i<h.size(); i++) {
        float x = ofMap(i, 0, h.size(), 0, 320);
        float y = ofMap(h[i], 0, 0.3, 240, 0);
        ofVertex(x, y);
    }
    ofEndShape();
}

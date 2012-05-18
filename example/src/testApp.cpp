#include "testApp.h"
#include <gl/glut.h>

float lightPos0[] = {100, 150, -200, 1};

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
   
    setLight();
      
    glClearColor(0.3, 0.3, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    
    moBlur.setup();
    
}

//--------------------------------------------------------------
void testApp::update(){ 
    moBlur.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
       
    moBlur.begin();
        xPos+=0.5;
        if(xPos>ofGetWidth()) xPos = 0;
        ofPushMatrix();
            ofBox(xPos, ofGetHeight()/2, 0, 50);
        ofPopMatrix();
    moBlur.end();
    
    moBlur.draw();    
}


void testApp::setLight(){
    float   lightAmbient0[] =   {0.5, 0.5, 0.5, 1.0};
    float   lightDiffuse0[] =   {1.0, 1.0, 1.0, 1.0};
    float   lightSpecular0[] =   {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    switch (key) {
        case 'l':
            lightPos0[0] = ofRandom(ofGetWidth());
            lightPos0[1] = ofRandom(ofGetHeight());
            lightPos0[2] = ofRandom(-100,100);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
            break;
            
        default:
            break;
    }

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
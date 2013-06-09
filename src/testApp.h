#pragma once

#include "ofMain.h"
#include "ofxVideoBufferPlayer.h"
#include "ofxVideoBuffer.h"

class testApp : public ofBaseApp{
    
public:
    
    ~testApp();
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void resized(int w, int h);
    
    vector<ofxSharedVideoBuffer> buffers;
    vector<ofxSharedVideoBufferPlayer> players;
    vector<ofRectangle> rects;
    
    bool useBuffer;
    
    int rows;
    int cols;
    
    int currentPlayer;
    
    ofFbo fbo;
    ofFbo maskFbo;
    
    ofFbo moviesFbo; 

    
    
    ofImage     dstImg;
    ofImage     brushImg;    
    
    
    ofShader    shader;
    
    bool        bBrushDown;
    
    int state;
    ofVideoPlayer intro; 
    
    
};
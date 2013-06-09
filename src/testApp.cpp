#include "testApp.h"


testApp::~testApp() {
    players.clear();
    buffers.clear();
}

//--------------------------------------------------------------
void testApp::setup(){
	
    /*
     state 0 = intro
     state 1 = drawing
     */
    
    state = 0;
    

    rows = 3;
    cols = 3;
    useBuffer = true;
    

    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    intro.loadMovie("intro 2.mov");

    //ofSetFrameRate(24);
    
    fbo.allocate(ofGetWidth() , ofGetHeight(),GL_RGB);

    
    //shaders
    dstImg.loadImage("birdman_black&white.jpg");
    brushImg.loadImage("brush.png");
    
    int width = dstImg.getWidth();
    int height = dstImg.getHeight();
    
    maskFbo.allocate(width,height);
    fbo.allocate(width,height);
    moviesFbo.allocate(width, height);
    
    
    
    string shaderProgram = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect tex0;\
    uniform sampler2DRect maskTex;\
    \
    void main (void){\
    vec2 pos = gl_TexCoord[0].st;\
    \
    vec3 src = texture2DRect(tex0, pos).rgb;\
    float mask = texture2DRect(maskTex, pos).r;\
    \
    gl_FragColor = vec4( src , mask);\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();
    

    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    moviesFbo.begin();
    ofClear(0,0,0,255);
    moviesFbo.end();
    
    bBrushDown = false;

    
    
    ///////Video buffers////////////////////////////////////////////////////////
    
    ofxSharedVideoBuffer buffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("1.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("2.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("3.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("4.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("5.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("6.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("7.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("8.mov");
    buffers.push_back(buffer);
    buffer = ofxSharedVideoBuffer(new ofxVideoBuffer());
    buffer->loadMovieAsync("9.mov");
    buffers.push_back(buffer);



    
    for(int i = 0; i < rows * cols; i++) {
        ofxSharedVideoBufferPlayer player(new ofxVideoBufferPlayer());
        if(useBuffer) {
            player->loadVideoBuffer(buffers[i]);
            player->start();
            player->setPaused(true);
        } else {
            player->loadMovie("fingers_pjpeg.mov");
            player->start();
        }
        player->setLoopType(OF_LOOP_PALINDROME);
        players.push_back(player);
        
    }
    currentPlayer = 0;
    

    
    for (int y = 0; y < rows; y ++) {
    
        for (int x = 0; x < cols; x ++){
        
            int xstep = floor(ofGetWidth() / cols);
            int ystep = floor(ofGetHeight() / rows);
        
        
        
            ofRectangle thisRect;
            //thisRect.set(x * (ofGetWidth()/cols), y * (ofGetHeight()/rows), ofGetWidth()/cols, ofGetHeight()/rows);
            thisRect.set(x * xstep , y * ystep, ofGetWidth()/cols, ofGetHeight()/rows);
            rects.push_back(thisRect);
        }
    }
}

//--------------------------------------------------------------
void testApp::update() {
    
    switch (state) {
        case 0:
            intro.update();
            break;
            
        case 1:
            // MASK (frame buffer object)

            maskFbo.begin();
            if (bBrushDown){
                ofEnableAlphaBlending();
                ofFill();
                brushImg.draw(mouseX-25,mouseY-25,50,50);
            }
            maskFbo.end();
            
            // HERE the shader-masking happends
            //
            
            ofEnableAlphaBlending();
            fbo.begin();
            
            dstImg.draw(0,0);
            shader.begin();
            shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
            
            moviesFbo.draw(0, 0);
            
            shader.end();
            
            fbo.end();
            
            
            for(int i = 0; i < buffers.size(); i++) {
                buffers[i]->update();
            }
            
            for(int i = 0; i < rows * cols; i++) {
                players[i]->update();
            }

            break;
    }
    
  }

//--------------------------------------------------------------
void testApp::draw(){
    

    ofBackground(0,0,0);
    
    
    switch (state) {
        case 0:
            intro.play();
            intro.draw(0,0);
            break;
            
        case 1:
            ofEnableAlphaBlending();
            
            moviesFbo.begin();//put the drawing into fbo
            
            ofClear(0);//cleaning the fbo before drawing again
   
            float w = float(ofGetWidth()) / cols;
            float h = float(ofGetHeight()) / rows;
            float r = 0;
            float c = 0;
            
            float mx = ofGetMouseX();
            float my = ofGetMouseY();
            
            float tx,ty,xx,yy,ss,rr;
            
            for(int i = 0; i < rows * cols; i++) {
                ofSetColor(255);
                
                float x = c * w;
                float y = r * h;
                
                float dist = ofDist(x,y,mx,my);
                float scaleScaler = ofMap(dist,0,ofGetWidth(),5,1,true);
                float speedScaler = ofMap(dist,0,ofGetWidth()/4,-2,2,true);
                float rotScaler = ofMap(dist,0,ofGetWidth()/4,360,0,true);
                ofPushMatrix();
                ofTranslate(x + w/2,y + h/2);
                //ofScale(scaleScaler,scaleScaler);
                //ofRotateZ(rotScaler);
                //players[i]->setSpeed(speedScaler);
                //players[i]->draw(-w/2,-h /2,rects[0].width,rects[0].height);
                players[i]->draw(-w/2,-h /2, w,h);
                
                ofSetColor(255);
                if(rows < 4) {
                   // ofDrawBitmapString(players[i]->toString(), -w/2 + 10,-h /2 + 20);
                }
                if(i == currentPlayer) {
                    xx = -w/2;
                    yy = -h/2;
                    tx = x + w/2;
                    ty = y + h/2;
                    ss = speedScaler;
                    rr = rotScaler;
                }
                
                c++;
                if(c >= cols) {
                    r++;
                    c = 0;
                }
                
                ofPopMatrix();
            }
            
            ofPushMatrix();
            ofTranslate(tx,ty);

            ofNoFill();
            //ofSetColor(255,255,0);
            ofRect(xx,yy,w,h);
            
            ofPopMatrix();
            
            ofDisableAlphaBlending();
            
            moviesFbo.end();
            
            
            
            ofSetColor(255,255);
            
            
            
            fbo.draw(0,0);
            
//ofSetColor(255, 0, 255);
//            //ofFill();
//            ofRect(rects[3]);
//            
//            for (int i = 0; i < rects.size(); i++) {
//                ofSetColor(255, 0, 0);
//                ofRect(rects[i]);
//            }
            
            
            //maskFbo.draw(0,0);

            break;
    }
    
       
    
}



//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    if (key == '0') {
        state = 0;
    }
    
    if (key == '1') {
        state = 1;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
    cout << rects.size() << endl;
    
    for (int i = 0; i < rects.size(); i++) {
        //ofRect(rects[i]);

        
        if (rects[i].inside(x, y)) {
            if (players[i]->isPaused()) players[i]->setPaused(false);
            cout << i << "is inside" << endl;
            
        } else {
            if (!players[i]->isPaused())players[i]->setPaused(true);
            cout << i << "is OUTSIDE" << endl;

            
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    bBrushDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    bBrushDown = false;


}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
    
}

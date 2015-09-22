// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


void ofApp::setup()
{
    ofSetVerticalSync(false);

	camWidth = WIDTH;
	camHeight = HEIGHT;
    camFrameRate = FPS;
    current_cam = 0;
    frame_ms = 50;
    current_frame = 0;
    wiggle_dir = true;
    picture_counter = 1;

    //we can now get back a list of devices.
    std::vector<ofVideoDevice> devices = ofxPS3EyeGrabber().listDevices();

    for(std::size_t i = 0; i < devices.size(); ++i)
    {
        std::stringstream ss;

        ss << devices[i].id << ": " << devices[i].deviceName;

        if(!devices[i].bAvailable)
        {
            ss << " - unavailable ";
        }
        else
        {
            std::shared_ptr<ofxPS3EyeGrabber> videoGrabber = std::shared_ptr<ofxPS3EyeGrabber>(new ofxPS3EyeGrabber());

            videoGrabber->setDeviceID(i);
            videoGrabber->setDesiredFrameRate(camFrameRate);
            videoGrabber->initGrabber(camWidth, camHeight);

            videoGrabbers.push_back(videoGrabber);

            // Add a texture.
            videoTextures.push_back(ofTexture());
        }

        ofLogNotice("ofApp::setup") << ss.str();
	}


}


void ofApp::update(){
    
    if(bTakePhoto==true){
       for(std::size_t i = 0; i < NUM_FRAMES; ++i){
           videoGrabbers[i]->update();
           while(!videoGrabbers[i]->isFrameNew()){
               cout << "Waiting for camera " << i << "..." << endl;
               videoGrabbers[i]->update();
           }
           cout << "New frame " << i << "..." << endl;
           string frame = "frames/" + ofToString(picture_counter) + "/" + ofToString(i) + ".png";
           aFrameImage[i].clear();
           aFrameImage[i].allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
           aFrameImage[i].setFromPixels(videoGrabbers[i]->getPixelsRef());
           aFrameImage[i].saveImage(frame);
       }
        bTakePhoto = false;
        picture_counter++;
    }else{
        for(std::size_t i = 0; i < NUM_FRAMES; ++i){
            videoGrabbers[i]->update();
            if (videoGrabbers[i]->isFrameNew()){
                videoTextures[i].loadData(videoGrabbers[i]->getPixelsRef());
            }
        }
    }
    
}

void ofApp::draw()
{
    
    /*
    ofBackground(0);
    ofSetColor(255);

    int x = 0;
    int y = 0;

    for(std::size_t i = 0; i < videoGrabbers.size(); ++i)
    {
        ofPushMatrix();
        ofTranslate(x, y);

        videoTextures[i].draw(0, 0);

        std::stringstream ss;

        ss << "App FPS: " << ofGetFrameRate() << std::endl;
        ss << "Cam FPS: " << videoGrabbers[i]->getFPS();

        ofDrawBitmapStringHighlight(ss.str(), ofPoint(10, 15));

        ofPopMatrix();

        if (x + camWidth >= ofGetWidth())
        {
            y += camHeight;
            x = 0;
        }
        else
        {
            x += camWidth;
        }
    }
     */

    ofBackground(0);
    if(ofGetElapsedTimeMillis() - timer > frame_ms){
        if(wiggle_dir==true){
            current_frame++;
        }else{
            current_frame--;
        }
        if(current_frame>NUM_FRAMES-1){
            wiggle_dir = false;
            current_frame = NUM_FRAMES-2;
        }
        if(current_frame<0){
            wiggle_dir = true;
            current_frame = 1;
        }
        timer = ofGetElapsedTimeMillis();
        cout << current_frame << endl;
    }
    if(picture_counter-1<=0){
        picture_counter=1;
    }
    frame.loadImage("/Applications/of_v0.8.4_osx_release/apps/myApps/wiggle_cam/bin/data/frames/"+ofToString(picture_counter-1)+"/"+ofToString(current_frame)+".png");
    frame.draw((ofGetWidth()-WIDTH)/2.0,(ofGetHeight()-HEIGHT)/2.0,WIDTH,HEIGHT);
}

void ofApp::keyPressed(int key){
    
    /*
    if (key == '1'){
        current_cam = 0;
    }
    else if (key == '2'){
        current_cam = 1;
    }
    else if (key == '3'){
        current_cam = 2;
    }
    else if (key == '4'){
        current_cam = 3;
    }
    else if (key == '5'){
        current_cam = 4;
    }
    else if (key == '6'){
        current_cam = 5;
    }
    else if (key == '7'){
        current_cam = 6;
    }
    else{
        bTakePhoto = true;
    }
    */
    bTakePhoto = true;
    
}

void ofApp::exit()
{
    for(std::size_t i = 0; i < videoGrabbers.size(); ++i){
        videoGrabbers[i]->close();
    }
}

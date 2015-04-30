#include "HearThereOSCVizApp.h"

// account for the fact that the sensor's global frame has east=x, up=z, north=y
const ofMatrix4x4 HearThereOSCVizApp::worldTransform = ofMatrix4x4(
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 1);

// account for the body rotation because of how the board is physically oriented.
// We want forward=-z, up=-x, right=y
// Note that this is a pure rotation with no axes swaps
const ofMatrix4x4 HearThereOSCVizApp::bodyTransform = ofMatrix4x4(
         0,  1,  0,  0,
        -1,  0,  0,  0,
         0,  0, -1,  0,
         0,  0,  0,  1);

//--------------------------------------------------------------
void HearThereOSCVizApp::setup(){
    cout << "Listening for OSC on port 10001\n";
    oscReceiver.setup(10001);
    cam.setPosition(ofVec3f(0, 0, 9));
    cam.setDistance(3);
    cam.setTarget(ofVec3f(0, 0, 0));
    cam.setNearClip(0.1);
    int i = 0;
    for(int x = -2; x <= 2; x += 4) {
        for(int y = -2; x <= 2; x += 4) {
            for(int z = -2; x <= 2; x += 4) {
                lights[i].setPointLight();
                lights[i].setPosition(ofVec3f(x, y, z));
                lights[i].setPointLight();
                lights[i].setDiffuseColor(ofColor(255, 255, 255));
                lights[i].enable();
                ++i;
            }
        }
    }
    ofBackground(150,150,150);
    remoteForward = ofVec3f(0, 0, 0);
    remoteUp = ofVec3f(0, 0, 0);
    //cam.lookAt(0, 0, 0);

    displayAxes = true;
    displayRaw = false;
    displayRemoteCooked = false;
    displayLocalCooked = true;
}

//--------------------------------------------------------------
void HearThereOSCVizApp::update(){
    while(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);

        // check for mouse moved message
        if(m.getAddress() == "/orientation"){
            float w = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1);
            float y = m.getArgAsFloat(2);
            float z = m.getArgAsFloat(3);
            orientation = ofQuaternion(x, y, z, w);
            //cout << w << ", " << x << ", " << y << ", " << z << endl;
        }
        else if(m.getAddress() == "/forward"){
            float x = m.getArgAsFloat(0);
            float y = m.getArgAsFloat(1);
            float z = m.getArgAsFloat(2);
            remoteForward = ofVec3f(x, y, z);
            displayRemoteCooked = true;
        }
        else if(m.getAddress() == "/up"){
            float x = m.getArgAsFloat(0);
            float y = m.getArgAsFloat(1);
            float z = m.getArgAsFloat(2);
            remoteUp = ofVec3f(x, y, z);
            displayRemoteCooked = true;
        }
        else{
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            for(int i = 0; i < m.getNumArgs(); i++){
                // get the argument type
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                switch(m.getArgType(i)) {
                    case OFXOSC_TYPE_INT32:
                        msg_string += ofToString(m.getArgAsInt32(i));
                        break;
                    case OFXOSC_TYPE_FLOAT:
                        msg_string += ofToString(m.getArgAsFloat(i));
                        break;
                    case OFXOSC_TYPE_STRING:
                        msg_string += m.getArgAsString(i);
                        break;
                    default:
                        msg_string += "unknown";
                        break;
                }
            }
            cout << msg_string << endl;
        }
    }
}

//--------------------------------------------------------------
void HearThereOSCVizApp::draw(){
    cam.begin();
    ofSetDepthTest(true);
    //ofEnableAlphaBlending();
    ofEnableLighting();
    // switch to left-handed coordinates
    ofScale(1, 1, -1);
    ofSetLineWidth(5);

    if(displayAxes) {
        ofSetColor(255, 0, 0, axisAlpha);
        ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(1, 0, 0));
        ofSetColor(0, 255, 0, axisAlpha);
        ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
        ofSetColor(0, 0, 255, axisAlpha);
        ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 1));
    }
    if(displayRaw) {
        float w;
        ofVec3f xyz;

        ofPushMatrix();
        orientation.getRotate(w, xyz);
        ofRotate(w, xyz.x, xyz.y, xyz.z);
        ofSetColor(255, 255, 0, axisAlpha);
        ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0.75));
        ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0.25, 0));
        ofPopMatrix();
    }
    if(displayLocalCooked) {
        float w_sensor;
        ofVec3f xyz_sensor;
        float w_correction;
        ofVec3f xyz_correction;
        orientation.getRotate(w_sensor, xyz_sensor);
        correction.getRotate(w_correction, xyz_correction);

        ofPushMatrix();
        // Correct for the world coordinate system that the algorithm uses
        ofMultMatrix(worldTransform);

        //ofRotate(90, 1, 0, 0);
        ofRotate(w_sensor, xyz_sensor.x, xyz_sensor.y, xyz_sensor.z);
        ofRotate(w_correction, xyz_correction.x, xyz_correction.y, xyz_correction.z);
        //ofMultMatrix(bodyTransform);
        ofSetColor(0, 255, 255, axisAlpha);
        // draw forward vector (in body frame)
        ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, -0.75));
        // draw up vector (in body frame)
        ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(-0.25, 0, 0));
        ofPopMatrix();
    }
    if(displayRemoteCooked) {
        ofVec3f forward = remoteForward / remoteForward.length() * 0.75;
        ofVec3f up = remoteUp / remoteUp.length() * 0.25;
        ofSetColor(0, 255, 255, axisAlpha);
        ofDrawArrow(ofVec3f(0, 0, 0), remoteForward);
        ofDrawArrow(ofVec3f(0, 0, 0), remoteUp, 0.1);
    }
    cam.end();
}


//--------------------------------------------------------------
void HearThereOSCVizApp::keyPressed(int key) {
    if(key == ' ') {
        cout << "RECALIBRATE" << endl;
        correction = orientation.inverse();
    }
    else if(key == 'a') {
        displayAxes = !displayAxes;
    }
    else if(key == 'l') {
        displayLocalCooked = !displayLocalCooked;
    }
    else if(key == 'r') {
        displayRaw = !displayRaw;
    }
    else {
        cout << "UNRECOGNIZED KEY: " << key << endl;
    }
}

//--------------------------------------------------------------
void HearThereOSCVizApp::keyReleased(int key){

}

//--------------------------------------------------------------
void HearThereOSCVizApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void HearThereOSCVizApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void HearThereOSCVizApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void HearThereOSCVizApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void HearThereOSCVizApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void HearThereOSCVizApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void HearThereOSCVizApp::dragEvent(ofDragInfo dragInfo){ 

}

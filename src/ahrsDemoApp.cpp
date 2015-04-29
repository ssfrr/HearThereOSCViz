#include "ahrsDemoApp.h"

//--------------------------------------------------------------
void ahrsDemoApp::setup(){
    cout << "Listening for OSC on port 10001\n";
    oscReceiver.setup(10001);
    cam.setPosition(ofVec3f(0, 0, 9));
    cam.setNearClip(0);
    light1.setPosition(ofVec3f(3, 3, 40));
    light1.setPointLight();
    light2.setPosition(ofVec3f(-10, -2, 20));
    light2.setPointLight();
    ofBackground(15,15,15);
    ofSetDepthTest(true);
    remoteForward = ofVec3f(0, 0, 0);
    remoteUp = ofVec3f(0, 0, 0);
    //cam.lookAt(0, 0, 0);
}

//--------------------------------------------------------------
void ahrsDemoApp::update(){
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
            //cout << w << ", " << x << ", " << y << ", " << z << endl;
        }
        else if(m.getAddress() == "/up"){
            float x = m.getArgAsFloat(0);
            float y = m.getArgAsFloat(1);
            float z = m.getArgAsFloat(2);
            remoteUp = ofVec3f(x, y, z);
            //cout << w << ", " << x << ", " << y << ", " << z << endl;
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
void ahrsDemoApp::draw(){
    float w_sensor;
    ofVec3f xyz_sensor;
    float w_correction;
    ofVec3f xyz_correction;

    cam.begin();
    // switch to left-handed coordinates
    ofScale(1, 1, -1);
    ofVec3f forward = ofVec3f(0, 0, 1);
    orientation.getRotate(w_sensor, xyz_sensor);
    //correction.getRotate(w_correction, xyz_correction);

    // apply the head-tracking rotation
    forward = forward.rotate(w_sensor, xyz_sensor);
    //forward = forward.rotate(w_correction, xyz_correction);
    //forward = forward.rotate(90, ofVec3f(0, 0, 1));

    // rotate so Z is pointing up
    //ofRotate(90, 1, 0, 0);
    //ofRotate(w, xyz.x, xyz.y, xyz.z);
    //ofDrawAxis(200);
    //ofSetColor(255, 0, 0);
    //ofDrawBox(ofVec3f(102, 1, 1), 200, 10, 10);
    //ofSetColor(0, 255, 0);
    //ofDrawBox(ofVec3f(1, 102, 1), 10, 200, 10);
    //ofSetColor(0, 0, 255);
    //ofDrawBox(ofVec3f(1, 1, 102), 10, 10, 200);
    ofEnableLighting();
    light1.enable();
    light2.enable();
    ofSetLineWidth(2);
    ofSetColor(255, 0, 0);
    ofDrawArrow(ofVec3f(0, 0, 0), forward, 0.2);
    if(remoteForward.length() > 0.01) {
        ofSetColor(0, 255, 0);
        ofDrawArrow(ofVec3f(0, 0, 0), remoteForward, 0.2);
        ofSetColor(0, 100, 100);
        ofDrawArrow(ofVec3f(0, 0, 0), remoteUp, 0.1);
    }
    cam.end();
}


//--------------------------------------------------------------
void ahrsDemoApp::keyPressed(int key) {
    if(key == 'r') {
        cout << "RECALIBRATE" << endl;
        float w_sensor;
        ofVec3f xyz_sensor;
        orientation.getRotate(w_sensor, xyz_sensor);
        correction = orientation.inverse();
    }
    else {
        cout << "UNRECOGNIZED KEY: " << key << endl;
    }
    cout << "======================================================\n";
    cout << "======================================================\n";
    cout << "======================================================\n";
}

//--------------------------------------------------------------
void ahrsDemoApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ahrsDemoApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ahrsDemoApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ahrsDemoApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ahrsDemoApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ahrsDemoApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ahrsDemoApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ahrsDemoApp::dragEvent(ofDragInfo dragInfo){ 

}

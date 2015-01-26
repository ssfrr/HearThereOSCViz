#include "ahrsDemoApp.h"

//--------------------------------------------------------------
void ahrsDemoApp::setup(){
    cout << "Listening for OSC on port 10001\n";
    oscReceiver.setup(10001);
}

//--------------------------------------------------------------
void ahrsDemoApp::update(){
    while(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);

        // check for mouse moved message
        if(m.getAddress() == "/orientation"){
            // both the arguments are int32's
            float w = m.getArgAsFloat(0);
            float x = m.getArgAsFloat(1);
            float y = m.getArgAsFloat(2);
            float z = m.getArgAsFloat(3);
            orientation = ofQuaternion(x, y, z, w);
            cout << w << ", " << x << ", " << y << ", " << z << endl;
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
    float w;
    ofVec3f xyz;

    orientation.getRotate(w, xyz);
    ofBackgroundGradient(ofColor(15, 15, 15), ofColor(5, 5, 5), OF_GRADIENT_LINEAR);
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 0);
    ofRotate(w, xyz.x, xyz.y, xyz.z);
    ofDrawAxis(200);
}

//--------------------------------------------------------------
void ahrsDemoApp::keyPressed(int key){

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

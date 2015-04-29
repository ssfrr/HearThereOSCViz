#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class HearThereOSCVizApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

    private:
        ofVec3f remoteForward;
        ofVec3f remoteUp;
        ofQuaternion orientation;
        ofQuaternion correction;
        ofxOscReceiver oscReceiver;
        ofCamera cam;
        ofLight light1;
        ofLight light2;
        bool displayAxes;
        bool displayRaw;
        bool displayRemoteCooked;
        bool displayLocalCooked;
};

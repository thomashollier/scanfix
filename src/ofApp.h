#pragma once

#include "ofMain.h"
#include "ofxInfiniteCanvas.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

class offsetCurve{
public:
    offsetCurve();
    
    void init(string);
    
    vector <ofPoint> srcPoints;
    vector <ofPoint> dstPoints;
    ofPath srcPath;
    ofPath dstPath;
    string imageFilePath;
    string sessionFile;
    vector <ofPoint> srcPolyline;
    vector <ofPoint> dstPolyline;

    
    void srcPrint();
    void dstPrint();

    void updatePathFromPoints();
    void updatePointsFromPath();

    int selectedP;
    void selectP(ofPoint p);
    void deselectP();
    void moveSelectedP(ofPoint p);
    void insertP(ofPoint p);
    void deleteSelectedP();
    
    void draw();

    ofImage image;
    string getImage();

    void saveSession(string filePath, string imagePath);
    void readSession(string filePath);
    void readSession();
    void newSession();
    void processNewFileSelection(ofFileDialogResult);
    void processOpenFileSelection(ofFileDialogResult);
    void setSessionFile(string f);

    void createOffsets();
    float getOffsetFromIndex(int index);
    
    void drawOffsets();
};


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
        void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
    void openSessionRequester();
    void newSessionRequester();
    void loadImageAndBuildScanlines();

    void applyOffset();
    
    
    string imageFile;
        void zoom(float zm);
        vector<ofPlanePrimitive> planes;
        ofImage image;
        ofxInfiniteCanvas myCanvas;
    
        ofSpherePrimitive sph;
    
        offsetCurve session;
    
    enum appMode{MOVE=0, EDIT=1, INSERT=2, DELETE=3};
    
    appMode mode;
    
    int selectedPoint;

    bool DRAWOFFSET;
    bool DRAWCURVES;
};

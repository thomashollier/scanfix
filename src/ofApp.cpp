#include "ofApp.h"

offsetCurve::offsetCurve(){
}

void offsetCurve::init(string imagePath){
    imageFilePath = imagePath;
    image.load(imagePath);
    //image.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

    float x = image.getWidth()/2;
    float y=0;
    int step = x/8;
    int n = 8;
    
    for(int i=-1; i<= n+1; i++){
        srcPoints.push_back(ofPoint(x+step*i, y));
        dstPoints.push_back(ofPoint(x+step*i, y+ofRandom(-100,100)));
    }
    updatePathFromPoints();
}

void offsetCurve::draw(){
    dstPath.draw();
    srcPath.draw();
    for(int i=1; i< dstPath.getCommands().size(); i++){
        ofPoint p = dstPath.getCommands()[i].to;
        ofDrawCircle(p, 3);
    }

}

void offsetCurve::updatePathFromPoints(){
    srcPath.clear();
    dstPath.clear();
    for(int i=0; i<srcPoints.size();i++){
        srcPath.curveTo(ofPoint(srcPoints[i].x, srcPoints[i].y));
    }
    for(int i=0; i<dstPoints.size();i++){
        dstPath.curveTo(ofPoint(dstPoints[i].x, dstPoints[i].y));
    }
    srcPath.setFilled(false);
    srcPath.setStrokeWidth(3);
    srcPath.setCurveResolution(30);
    dstPath.setFilled(false);
    dstPath.setStrokeWidth(3);
    dstPath.setCurveResolution(30);
}

void offsetCurve::updatePointsFromPath(){
    srcPoints.clear();
    dstPoints.clear();
    ofPoint p;
    for(int i=1; i<srcPath.getCommands().size(); i++){
        p = srcPath.getCommands()[i].to;
        srcPoints.push_back(ofPoint(p.x, p.y));
        p = dstPath.getCommands()[i].to;
        dstPoints.push_back(ofPoint(p.x, p.y));
    }
}

void offsetCurve::dstPrint(){
    for(int i=0; i<dstPoints.size();i++){
        cout << "points: " << dstPoints[i] << "\tpath: " << dstPath.getCommands()[i+1].to << "\n";
    }
}

void offsetCurve::srcPrint(){
    for(int i=0; i<srcPoints.size();i++){
        cout << "points: " << srcPoints[i] << "\tpath: " << srcPath.getCommands()[i+1].to << "\n";
    }
}

void offsetCurve::selectP(ofPoint p){
    float shortestDistance = 100000000;
    float d;
    for(int i=1; i< dstPath.getCommands().size(); i++){
        d = ofPoint(p.x,p.y).distance(dstPath.getCommands()[i].to);
        if(d<shortestDistance && d < 10){
            shortestDistance = d;
            selectedP = i;
        }
    }
}

void offsetCurve::deselectP(){
    selectedP = -1;
}

void offsetCurve::moveSelectedP(ofPoint p){
    if(selectedP > -1){
        dstPath.getCommands()[selectedP].to.set(round(p.x), p.y);
        srcPath.getCommands()[selectedP].to.x = round(p.x);
    }
}

void offsetCurve::insertP(ofPoint p){
    for(int i = 1; i < dstPoints.size(); i++){
        if (dstPoints[i-1].x < p.x && dstPoints[i].x > p.x){
            dstPoints.emplace(dstPoints.begin()+i, ofPoint(p.x,p.y));
            srcPoints.emplace(srcPoints.begin()+i, ofPoint(p.x,0));
            selectedP = i+1;
            break;
        }
    }
}

void offsetCurve::deleteSelectedP(){
    srcPoints.erase(srcPoints.begin()+(selectedP-1));
    dstPoints.erase(dstPoints.begin()+(selectedP-1));
}

string offsetCurve::getImage(){
    return imageFilePath;
}

//--------------------------------------------------------------
void offsetCurve::saveSession(string filePath, string imagePath){
    cout << "saving \n";
    ofxXmlSettings positions;
    positions.addTag("image");
    positions.pushTag("image");
    positions.addValue("path", imageFilePath);
    positions.popTag();
    positions.addTag("source");
    positions.pushTag("source");
    //points is a vector<ofPoint> that we want to save to a file
    for(int i = 0; i < srcPoints.size(); i++){
        //each position tag represents one point
        positions.addTag("point");
        positions.pushTag("point",i);
        //so set the three values in the file
        positions.addValue("X", srcPoints[i].x);
        positions.addValue("Y", srcPoints[i].y);
        positions.popTag();//pop position
    }
    positions.popTag(); //pop position
    positions.addTag("destination");
    positions.pushTag("destination");
    //points is a vector<ofPoint> that we want to save to a file
    for(int i = 0; i < dstPoints.size(); i++){
        //each position tag represents one point
        positions.addTag("point");
        positions.pushTag("point",i);
        //so set the three values in the file
        positions.addValue("X", dstPoints[i].x);
        positions.addValue("Y", dstPoints[i].y);
        positions.popTag();//pop position
    }
    positions.popTag(); //pop position
    positions.saveFile(filePath);
}

void offsetCurve::readSession(){
    readSession(sessionFile);
}

void offsetCurve::readSession(string filePath){
    cout << "loading session file " << filePath << "\n";
    ofxXmlSettings settings;
    if(settings.loadFile(filePath)){
        settings.pushTag("image");
        imageFilePath = settings.getValue("path", "foo");
        image.load(imageFilePath);
        image.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        cout << "\timage " << imageFilePath << "\n";
        settings.popTag();
        srcPoints.clear();
        settings.pushTag("source");
        int numberOfSavedPoints = settings.getNumTags("point");
        for(int i = 0; i < numberOfSavedPoints; i++){
            settings.pushTag("point", i);
            float x = settings.getValue("X", 0);
            float y = settings.getValue("Y", 0);
            srcPoints.push_back(ofPoint(x,y));
            settings.popTag();
        }
        settings.popTag(); //pop position
        cout << "\t" << numberOfSavedPoints << " source points\n";
        dstPoints.clear();
        settings.pushTag("destination");
        numberOfSavedPoints = settings.getNumTags("point");
        for(int i = 0; i < numberOfSavedPoints; i++){
            settings.pushTag("point", i);
            float x = settings.getValue("X", 0);
            float y = settings.getValue("Y", 0);
            dstPoints.push_back(ofPoint(x,y));
            settings.popTag();
        }
        settings.popTag(); //pop position
        cout << "\t" << numberOfSavedPoints << " destination points\n";
    }
    else{
        ofLogError("Position file did not load!");
    }
    updatePathFromPoints();
}

void offsetCurve::newSession(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("load image");
    
    //Check if the user opened a file
    if (openFileResult.bSuccess){
        
        ofLogVerbose("User selected a file");
        
        //We have a file, check it and process it
        processNewFileSelection(openFileResult);
        
    }else {
        ofLogVerbose("User hit cancel");
    }
}

void offsetCurve::processNewFileSelection(ofFileDialogResult openFileResult){
    cout << openFileResult.getName() << "\n";
    cout << openFileResult.getPath() << "\n";
}

void offsetCurve::setSessionFile(string f){
    sessionFile = f;
}

void offsetCurve::createOffsets(){
    ofPolyline srcTmpPolyline, dstTmpPolyline;
    for(int i=0;i<dstPoints.size(); i++){
        ofPoint cv = dstPoints[i];
        dstTmpPolyline.curveTo(ofPoint(cv.x, cv.y));
        srcTmpPolyline.curveTo(ofPoint(cv.x, 0));
    }
    srcPolyline.clear();
    dstPolyline.clear();

    float length = srcTmpPolyline.getLengthAtIndex(srcTmpPolyline.getVertices().size()+1);
    float numSamples = length;
    for(int i = 0;i<numSamples+1;i++){
        float idx = srcTmpPolyline.getIndexAtLength(i*length/numSamples);
        ofPoint pp = srcTmpPolyline.getPointAtLength(i*length/numSamples);
        ofPoint pp2 = dstTmpPolyline.getPointAtIndexInterpolated(idx);
        if(dstPolyline.size() > 0){
            if(pp2.x > dstPolyline.back().x){
                srcPolyline.push_back(ofPoint(pp2.x, 0));
                dstPolyline.push_back(ofPoint(pp2.x, pp2.y));
            }
        }else{
            srcPolyline.push_back(ofPoint(pp2.x, 0));
            dstPolyline.push_back(ofPoint(pp2.x, pp2.y));
        }
    }
}

void offsetCurve::drawOffsets(){
    for(int i = 0 ;i<dstPolyline.size(); i++ ){
        ofDrawCircle(srcPolyline[i],.5);
        ofDrawCircle(dstPolyline[i],.5);
    }
}

float offsetCurve::getOffsetFromIndex(int index){
    //Build a polyline from the ofPath commands and set the resolution
    //based on x distance of current point to next curveTo position
    //Assume the curve goes across the full image and return the Y value
    //of the vertex "current x location + image width / 2 "
    return -dstPolyline[index].y;
}

//--------------------------------------------------------------
//
//  MAIN APP
//
//--------------------------------------------------------------
void ofApp::setup(){
    //openSessionRequester();
    session.setSessionFile("/Users/hollt054/Desktop/IMG_0968.xml");
    session.readSession();
    loadImageAndBuildScanlines();

    selectedPoint = -1;
    mode = MOVE;
    DRAWOFFSET = true;
    DRAWCURVES = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    if(DRAWOFFSET){
        for(int i=0; i<planes.size(); i++){
            float x = planes[i].getPosition().x;
            float y = session.getOffsetFromIndex(i);
            planes[i].setPosition(x,y,0);
        }
    }else{
        for(int i=0; i<planes.size(); i++){
            float x = planes[i].getPosition().x;
            float y = 0;
            planes[i].setPosition(x,y,0);
        }
    }
    switch (mode){
        case MOVE:
            myCanvas.enableMouseInput(true);
            break;
        case EDIT:
            myCanvas.enableMouseInput(false);
            break;
        case INSERT:
            myCanvas.enableMouseInput(false);
            break;
        case DELETE:
            myCanvas.enableMouseInput(false);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    session.image.bind();
    myCanvas.begin();
    int n = session.image.getWidth(); // loop total
    for(int i = 0; i< n; i++){
        planes[i].draw();
    }
    session.image.unbind();
    session.dstPath.setColor(0);
    ofSetColor(ofColor(255,255,100,128));
    if(DRAWCURVES){
        session.draw();
        session.drawOffsets();
    }
    myCanvas.end();
    ofSetColor(ofColor(255));
}

void ofApp::loadImageAndBuildScanlines(){
    planes.clear();
    int n = session.image.getWidth();
    int w = session.image.getWidth();
    int h = session.image.getHeight();
    session.image.getTexture().disableAlphaMask();
    int gap = 0;
    for(int i = 0; i< n; i++){
        planes.push_back(ofPlanePrimitive(1, h, 2, 2));
        planes[i].setPosition(-(w/2)+i*(gap+w/n), 0, 0);
        planes[i].mapTexCoords(i*w/n, 0, (i+1)*w/n, h);
    }
    session.createOffsets();
}

void ofApp::zoom(float newScale){
    myCanvas.begin();
    float currentScale = myCanvas.getScale();
    ofVec3f currentTranslation = myCanvas.getTranslation();
    ofVec2f screenCenter = ofVec2f(ofGetWidth()/2,ofGetHeight()/2);
    ofVec2f pivot = ofVec2f(screenCenter - currentTranslation);
    pivot /= currentScale;
    ofVec3f translation = currentTranslation - pivot*(newScale - currentScale);
    
    myCanvas.setScale(newScale);
    myCanvas.setTranslation(translation);
    myCanvas.end();
}

//--------------------------------------------------------------
void ofApp::openSessionRequester(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("select xml file");
    if (openFileResult.bSuccess){
        
        ofLogVerbose("User selected a file");
        
        //We have a file, check it and process it
        session.sessionFile = openFileResult.getPath();
    }else {
        ofLogVerbose("User hit cancel");
    }
}

//--------------------------------------------------------------
void ofApp::newSessionRequester(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("select image file");
    if (openFileResult.bSuccess){
        
        ofLogVerbose("User selected a file");
        
        //We have a file, check it and process it
        session.imageFilePath = openFileResult.getPath();
        session.init(session.imageFilePath);
    }else {
        ofLogVerbose("User hit cancel");
    }
}

//--------------------------------------------------------------
void ofApp::applyOffset(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    myCanvas.begin();
    switch(key){
        case '=':
            myCanvas.reset();
            break;
        case '1':
            zoom(1);
            break;
        case '2':
            zoom(2);
            break;
        case '3':
            zoom(4);
            break;
        case '4':
            zoom(8);
            break;
        case '-':
            zoom(myCanvas.getScale()*.5);
            break;
        case '+':
            zoom(myCanvas.getScale()*2);
            break;
        case 1281:
            myCanvas.enableMouseInput(!myCanvas.getMouseInputEnabled());
            break;
        case ',':
            switch (mode){
                case MOVE: mode=EDIT;break;
                case EDIT: mode=INSERT;break;
                case INSERT: mode=DELETE;break;
                case DELETE: mode=MOVE;break;
            }
            break;
        case 's':
            session.saveSession("/tmp/poo.xml", imageFile);
        case 'a':
            session.readSession("/tmp/poo.xml");
        case 'n':
            session.newSession();
        case ' ':
            DRAWOFFSET = !DRAWOFFSET;
        case 'h':
            DRAWCURVES = !DRAWCURVES;
    }
    myCanvas.end();
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(mode == EDIT || mode == INSERT){
        session.moveSelectedP(myCanvas.screenToWorld(ofPoint(x,y)));
        session.updatePointsFromPath();
        session.createOffsets();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(mode == EDIT){
        session.selectP(myCanvas.screenToWorld(ofPoint(x,y)));
    }else if(mode == INSERT){
        session.insertP(myCanvas.screenToWorld(ofPoint(x,y)));
        session.updatePathFromPoints();
        session.createOffsets();
    }else if(mode == DELETE){
        session.selectP(myCanvas.screenToWorld(ofPoint(x,y)));
        session.deleteSelectedP();
        session.updatePathFromPoints();
        session.createOffsets();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(mode == EDIT || mode == INSERT){
        session.updatePointsFromPath();
        session.updatePointsFromPath();
        session.createOffsets();
        cout << "released\n";
    }
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

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
}

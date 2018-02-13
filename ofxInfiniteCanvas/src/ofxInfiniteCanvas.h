//
//  2DCam.h
//  This addon is for navigating through a 3D scene using an orthographic projection, so that it looks like 2D, or any 2D scene.
//  This should be used instead of ofEasyCam when using an ortho projection or when drawing 2D, as ofEasyCam's controls are not well suited for such task.
//  The user can change the position from where the scene is being looked. TOP, BOTTOM, LEFT, RIGHT, FRONT (default), BACK.
//
//
//  Created by Roy Macdonald on 27-06-15.
//
//

#pragma once
#include "ofMain.h"


class ofxInfiniteCanvas {
    public:
    
        enum LookAt{
            OFX2DCAM_FRONT =0,
            OFX2DCAM_BACK,
            OFX2DCAM_LEFT,
            OFX2DCAM_RIGHT,
            OFX2DCAM_TOP,
            OFX2DCAM_BOTTOM
        };
        ofxInfiniteCanvas();
        ~ofxInfiniteCanvas();
    
        virtual void begin(ofRectangle viewport = ofGetCurrentViewport());
        virtual void end();
        void reset();
        //-------   mouse
        void enableMouseInput(bool e = true);
        void disableMouseInput();
        bool getMouseInputEnabled();
    
        void mousePressed(ofMouseEventArgs & mouse);
        void mouseReleased(ofMouseEventArgs & mouse);
        void mouseDragged(ofMouseEventArgs & mouse);
        void mouseScrolled(ofMouseEventArgs & mouse);
        //    bool mousePressed(ofMouseEventArgs & mouse);
        //    bool mouseReleased(ofMouseEventArgs & mouse);
        //    bool mouseDragged(ofMouseEventArgs & mouse);
        //    bool mouseScrolled(ofMouseEventArgs & mouse);
    
        //-------   getters/setters
        void setFlipY(bool bFlipped);
        bool getYFlipped(){return bFlipY;}
        ofVec3f getTranslation(){return translation;}
        void setTranslation(ofVec3f t);
        float getScale(){return scale;}
        void setScale(float s);
    
        void setLookAt(LookAt l);
        LookAt getLookAt();
    
        void update();
        void drawDebug();
    
        void setDragSensitivity(float s);
        float getDragSensitivity(){return dragSensitivity;}
    
        void  setScrollSensitivity(float s);
        float getScrollSensitivity(){return scrollSensitivity;}
    
        void setDrag(float drag);
        float getDrag() const;
    
        void setNearClip(float nc);
        float getNearClip(){return nearClip;}
    
        void setFarClip(float fc);
        float getFarClip(){return farClip;}
    
        void setOverrideMouse(bool b);
        bool isMouseOverride(){return bMouseOverride;}
        //-------   parameters
        ofParameterGroup parameters;
    
        //-------   utils
        ofVec3f screenToWorld(ofVec3f screen);
        ofVec3f worldToScreen(ofVec3f world);
    
        void save(string path);
        bool load(string path);
    
    
    protected:
    
        ofVec3f orientation;
        void enableMouseInputCB(bool &e);
        ofRectangle viewport;
        bool bApplyInertia;
        bool bDoTranslate;
        bool bDoScale;
        bool bDoScrollZoom;
        bool bMouseInputEnabled;
        bool bDistanceSet;
        bool bEventsSet;
        ofVec3f move;
        ofParameter<float> scale;
        float clicScale;
    
        ofParameterGroup protectedParameters;
        ofParameter<ofVec3f>translation;
        ofVec3f clicTranslation;
        ofParameter<bool> bEnableMouse,bFlipY;
        ofParameter<float> dragSensitivity, scrollSensitivity, drag, farClip, nearClip;
    
        bool bMouseOverride;
        ofMouseEventArgs lastMouseDragged, lastMousePressed, lastMouseReleased, lastMouseScrolled;
        bool bNotifyMouseDragged, bNotifyMousePressed, bNotifyMouseReleased, bNotifyMouseScrolled;
    
    
        void enableMouseListeners(bool e = true);
        bool bMouseListenersEnabled;
        ofVec2f prevMouse, clicPoint;
    
        ofVec2f mouseVel;
    
        void update(ofEventArgs & args);
    
        void updateMouse();
    
        ofMatrix4x4 orientationMatrix;
    
        unsigned long lastTap;
    
        ofParameter<int> lookAt;
    
        static ofMatrix4x4 FM, BM, LM, RM, TM, BoM;
    
    private:

};

#pragma once

#include "AudioVisualizer.hpp"
#include "ofMain.h"
#include <ofPath.h>

class ofApp : public ofBaseApp {

  private:
    ofSoundPlayer sound;
    AudioVisualizer visualizer;

    bool playing = false;
    bool paused = false;
    int crtsong = 0;
    int number;
    float volume = 1.0; 
    char mode = '1';
    int cur_x, cur_y = 0;
    float progress;
    int percent;
    float pos;
    float lastPos = 0;
    vector<string> musics {"beat.wav", "geesebeat.wav", "pigeon-coo.wav" , "rock-song.wav"};
    int currentSongIndex;



    //Progress bar 
    float borderRadius = 5.0; // border radius 
    float barHeight = 10.0; // height of the progress bar 
    float barWidth = 200.0; // width of the progress bar
    float x = (ofGetWidth() - barWidth - 10); // x-coordinate of the progress bar
    float y = 10.0; // y-coordinate of the progress bar
    bool dragging = false;

    //Indicator Modes
    bool repeat = false;
    bool shuffle = false;
    bool looping = false;

  public:
    void setup();
    void update();
    void draw();

    void drawMode1(vector<float> amplitudes);
    void drawMode2(vector<float> amplitudes);
    void drawMode3(vector<float> amplitudes);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void setBackgroundColor();

};


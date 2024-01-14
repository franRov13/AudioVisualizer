#include "ofApp.hpp"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetBackgroundColor(36, 32, 56); // Sets the Background Color
}
//--------------------------------------------------------------
void ofApp::update() {
    /* The update method is called muliple times per second
    It's in charge of updating variables and the logic of our app */
    if (paused || dragging) { return; } 
    ofSoundUpdate();               // Updates all sound players
    visualizer.updateAmplitudes(); // Updates Amplitudes for visualizer
    progress = sound.getPosition();

    //Looping button Mode
    if (looping)
    {
        if (!sound.isPlaying()) {
            currentSongIndex = (currentSongIndex + 1) % musics.size();
            if (currentSongIndex == 0 && looping) {
                sound.load(musics[currentSongIndex]);
                sound.play();
            }
            else if (currentSongIndex < musics.size()) {
                sound.load(musics[currentSongIndex]);
                sound.play();
            }
        }
    }

    //Shuffle button Mode
    if (shuffle)
    {
        if (!sound.isPlaying()) 
        {
            currentSongIndex = ofRandom(musics.size());
            sound.load(musics[currentSongIndex]);
            sound.play();
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw() {
    /* The update method is called muliple times per second
    It's in charge of drawing all figures and text on screen */
    ofSetColor(256);
    ofFill(); 
    pos = playing ? progress : lastPos;
    percent = pos * 100;
    ofDrawBitmapString("Song Progress: " + ofToString(percent) + "%", 0, 30);
    // Mode Selection
    if (!playing) 
    {
        ofDrawBitmapString("Press 'p' to play some music!", ofGetWidth() / 2 - 50, ofGetHeight() / 2);
    }
    vector<float> amplitudes = visualizer.getAmplitudes();
    if (mode == '1') 
    {
        drawMode1(amplitudes);
    } else if (mode == '2') 
    {
        drawMode2(amplitudes);
    } else if (mode == '3')
    {
        drawMode3(amplitudes);
    }

    // Key mappings
    ofSetColor(255);
    ofDrawBitmapString("KEY MAPPINGS: [1]Rectangle Height Visualizer, [2]Circle Radius Visualizer, [3]Multi-Color Wave Visualizer, [a]Pause, [l]Loop", 10, ofGetHeight() - 30);
    ofDrawBitmapString("------------  [w]song#1, [e]song#2, [s]song#3, [d]song#4, [+]Volume Up, [-]Volume Down, [r]Repeat, [b]Shuffle ", 10, ofGetHeight() - 15);


    //Progress bar
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Progress Bar", x, y + 25);
    ofDrawRectRounded(x, y, barWidth * progress, barHeight, borderRadius);
    //Mouse click behaviors on the progress bar 
    if (ofGetMouseX() >= x && ofGetMouseX() <= x + barWidth &&
        ofGetMouseY() >= y && ofGetMouseY() <= y + barHeight) 
    {
        if (ofGetMousePressed()) 
        {
            float newProgress = (ofGetMouseX() - x) / static_cast<float>(barWidth);
            newProgress = ofClamp(newProgress, 0, 1);
            sound.setPosition(newProgress);
            progress = newProgress;
            dragging = true;
        } else 
        {
            dragging = false;
        }
    }

    int rectX = 425;
    int rectY = 0;
    int rectWidth = 200;
    int rectHeight = 50;
    int borderRadius = 10;
    ofSetColor(255, 255, 255);
    ofDrawRectRounded(rectX, rectY, rectWidth, rectHeight, borderRadius);
    // //Mode Indicator Section
    // Draw the active modes inside the rectangle
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("Active Mode:", rectX + 10, rectY + 20);
    if (repeat) {
        ofDrawBitmapString("Repeat Mode!", rectX + 110, rectY + 20);
    }
    if (shuffle) {
        ofDrawBitmapString("Shuffle Mode!", rectX + 110, rectY + 20);
    }
    if (looping) {
        ofDrawBitmapString("Looping Mode!", rectX + 110, rectY + 20);
    }
}
void ofApp::drawMode1(vector<float> amplitudes) {
    ofFill();        // Drawn Shapes will be filled in with color
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Rectangle Height Visualizer", 0, 15);
    setBackgroundColor();
    //Phase 2, Feature #3
    float barWidth = ofGetWidth() / static_cast<float>(amplitudes.size());
    for (int i = 0; i < amplitudes.size(); i++) 
    {
        float barHeight = ofMap(amplitudes[i], 0, 1, 0, 50);
        ofSetColor(ofColor(0, 0, ofRandom(128, 255)));
        ofDrawRectRounded(i * barWidth, ofGetHeight() - 50, barWidth, barHeight, 20);
    }
}
void ofApp::drawMode2(vector<float> amplitudes) {
    ofSetLineWidth(5); // Sets the line width
    ofFill();        // Only the outline of shapes will be drawn
    ofSetColor(256);   // This resets the color of the "brush" to white
    ofDrawBitmapString("Circle Radius Visualizer", 0, 15);
    setBackgroundColor();
    int bands = amplitudes.size();
    for (int i = 0; i < bands; i++) 
    {
        ofSetColor((bands - i) * 32 % 256, 18, 144); // Color varies between frequencies
        ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, amplitudes[0] / (i + 1));
    }
 
}

void ofApp::drawMode3(vector<float> amplitudes) {
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Multi-Color Wave Visualizer", 0, 15);
    setBackgroundColor();

    //Polyline object to draw the waves in our cool mode
    ofPolyline line; // Creates a polyline object to draw the wave
    float xSpacing = ofGetWidth() / static_cast<float>(amplitudes.size());
    float ySpacing = ofGetHeight();
    float amplitudeScale = ofGetHeight() / 2;
    // Change line width based on the progress of the song
    float lineWidth = ofMap(progress, 0, 1, 1, 10); 
    // This loop iterates through the amplitude values in the input vector, and calculates the x and y coordinates for each point of the waveform
    for (int i = 0; i < amplitudes.size(); i++) {
        float x = i * xSpacing;
        float y = amplitudes[i] * amplitudeScale + 325;
        y = ofMap(y, 0, ofGetHeight(), ySpacing - amplitudeScale, ySpacing + amplitudeScale);
        line.addVertex(x, y);
    }
    // Color of the waves change based on the amplitudes
    int r = ofMap(amplitudes[0], 0, 1, 100, 255);
    int g = ofMap(amplitudes[1], 0, 1, 100, 255);
    int b = ofMap(amplitudes[2], 0, 1, 100, 255);
    ofColor waveColor(r, g, b);
    // Waves are drawn
    ofSetLineWidth(lineWidth);
    ofSetColor(waveColor);
    line.draw();
}   

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    // This method is called automatically when any key is pressed
    switch (key) {
    case 'p':
        if (playing) {
            sound.stop();
        } else if(crtsong == 0) 
        {
            sound.load("beat.wav");
            sound.play();
        } else if(crtsong == 1) 
        {
            sound.load("geesebeat.wav");
            sound.play();
        } else if(crtsong == 2) 
        {
            sound.load("pigeon-coo.wav");
            sound.play();
        } else if(crtsong == -1) 
        {
            sound.load("rock-song.wav");
            sound.play();
        }
        playing = !playing;
        break;
    case 'a':
        paused = !paused;
        break;
    // 4 different keys for a specific song
    case 'w':
        sound.load("beat.wav");
        sound.play();
        break;
    case 'e':
        sound.load("geesebeat.wav");
        sound.play();
        break;
    case 's':
        sound.load("pigeon-coo.wav");
        sound.play();
        break;
    case 'd':
        sound.load("rock-song.wav");
        sound.play();
        break;
    //Volume increase
    case '=':
        volume += 0.2;
        if (volume > 1.0) 
        {
            volume = 1.0;
        }
        sound.setVolume(volume);
        break;
    //Volume decrease
    case '-':
        volume -= 0.2;
        if (volume < 0.0) 
        {
            volume = 0.0;
        }
        sound.setVolume(volume);
        break;
    //Shuffle mode
    case 'b':
        if (shuffle)
        {
            shuffle = false;
        }else
        {
            shuffle = true;
            repeat = false;
            looping = false;
        }
        break; 
    //Repeat Mode  
    case 'r':
        if (repeat)
        {
            repeat = false;
            sound.setLoop(false);
            break;
        }else
        {
            repeat = true;
            shuffle = false;
            looping = false;
            if (playing) 
            {
                sound.setLoop(true);
            }else
            {
                sound.setLoop(true);
                sound.play();
            } 
        }
        sound.play();
        break;    
    case 'l':
        if (looping)
        {
            looping = false;
        }else
        {
            looping = true;
            repeat = false;
            shuffle = false;
        }
        break;
    //Change mode feature
    case '1':
        mode = '1';
        break;
    case '2':
        mode = '2';
        break;
    case '3':
        mode = '3';
        break;
    default:
        break;
    }

}

void ofApp::setBackgroundColor() 
{
    if (mode == '1') {
        //dark green
        ofSetBackgroundColor(50, 50, 50);
    } else if (mode == '2') {
        //wine red 
        ofSetBackgroundColor(56, 32, 36);
    } else if (mode == '3') {
        //navy blue
        ofSetBackgroundColor(0);
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    cur_x = x;
    cur_y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
//--------------------------------------------------------------
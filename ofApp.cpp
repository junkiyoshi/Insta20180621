#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(39);
	ofSetWindowTitle("Insta");

	ofSetLineWidth(3);

	this->cap.open("D:\\video\\image2.mp4");
	this->cap_size = cv::Size(1280, 720);
	this->cap_image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->cap_frame = cv::Mat(this->cap_image.getHeight(), this->cap_image.getWidth(), CV_MAKETYPE(CV_8UC3, this->cap_image.getPixels().getNumChannels()), this->cap_image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat origin;
	this->cap >> origin;
	if (origin.empty()) {

		this->cap.set(CV_CAP_PROP_POS_FRAMES, 1);
		return;
	}

	cv::resize(origin, this->cap_frame, this->cap_size);
	cv::cvtColor(this->cap_frame, this->cap_frame, CV_BGR2RGB);
	
	this->cap_image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (int radius = 0; radius < 720; radius += 10) {

		float noise_value = ofMap(ofNoise(radius * 0.005 + ofGetFrameNum() * 0.05), 0, 1, -60, 60);
		ofPushMatrix();
		ofRotate(noise_value);

		for (int deg = 0; deg < 360; deg++) {

			int x = radius * cos(deg * DEG_TO_RAD);
			int y = radius * sin(deg * DEG_TO_RAD);

			int x_index = x + this->cap_size.width * 0.5;
			int y_index = y + this->cap_size.height * 0.5;

			if (x_index < 0 || x_index >= this->cap_size.width || y_index < 0 || y_index >= this->cap_size.height) {

				continue;
			}

			ofColor color(this->cap_frame.at<cv::Vec3b>(y_index, x_index)[0], this->cap_frame.at<cv::Vec3b>(y_index, x_index)[1], this->cap_frame.at<cv::Vec3b>(y_index, x_index)[2]);
			ofSetColor(color);

			int next_x = radius * cos((deg + 1) * DEG_TO_RAD);
			int next_y = radius * sin((deg + 1) * DEG_TO_RAD);

			ofDrawLine(x, y, next_x, next_y);
		}

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
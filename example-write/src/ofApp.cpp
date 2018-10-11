#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);
	
	
	string path = "box.abc";
	
	{
		ofCamera outcam;
		ofxAlembic::Writer writer;

		if (writer.open(path, 30, Alembic::AbcCoreFactory::IFactory::kOgawa)) // export at 30fps
		{
			for (int f = 0; f < 60; f++)
			{
				// points
				{
					std::vector<glm::vec3> points;
					
					for (int i = 0; i < 10; i++)
					{
						glm::vec3 p;
						p.x = ofRandom(-300, 300);
						p.y = ofRandom(-300, 300);
						p.z = ofRandom(-300, 300);
						points.push_back(p);
					}
					
					writer.addPoints("/points", points);
				}
				
				// curves
				{
					std::vector<ofPolyline> curves;
					
					for (int i = 0; i < 10; i++)
					{
						ofPolyline poly;
						
						for (int n = 0; n < 100; n++)
						{
							glm::vec3 v;
							v.x = ofSignedNoise(1, 0, 0, n * 0.01 + f * 10 + i) * 300;
							v.y = ofSignedNoise(0, 1, 0, n * 0.01 + f * 10 + i) * 300;
							v.z = ofSignedNoise(0, 0, 1, n * 0.01 + f * 10 + i) * 300;
							poly.addVertex(v);
						}
						
						curves.push_back(poly);
					}
					
					writer.addCurves("/curves", curves);
				}
				
				// mesh
				{
					ofMesh mesh;
					
					int num = ofRandom(1, 10) * 3;
					
					for (int i = 0; i < num; i++)
					{
						glm::vec3 p;
						p.x = ofRandom(-300, 300);
						p.y = ofRandom(-300, 300);
						p.z = ofRandom(-300, 300);
						mesh.addVertex(p);
					}
					
					writer.addPolyMesh("/polymesh", mesh);
				}
				
				// mesh with xform
				{
					glm::mat4 mat = glm::translate(glm::vec3(0, 0, 0));
					writer.addXform("/box", mat);
					
					if (f == 0)
					{
						// write only first frame
						
						ofBoxPrimitive box;
						box.set(100);
						writer.addPolyMesh("/box/boxShape", box.getMesh());
					}
				}
				
				// camera
				{
					outcam.setFov(20 + f * 0.5);
					outcam.orbit(f, 50.0, 600.0 + 600.0 * ofNoise(f / 60.0 + 10.0));
					outcam.begin();
					outcam.end();
					
					// implicitly add xform node
					writer.addCamera("/of_camera", outcam);
				}
			}
		}
	}
	
	abc.open(path);
	abc.dumpNames();
}

void ofApp::exit()
{
	abc.close();
}

//--------------------------------------------------------------
void ofApp::update()
{
	float t = fmodf(ofGetElapsedTimef(), abc.getMaxTime());
	abc.setTime(t);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	
	abc.get("/of_camera/cameraShape", cam);
	
	cam.begin();
	{
		ofMesh mesh;
		abc.get("/polymesh", mesh);
		
		ofSetColor(255, 0, 0);
		mesh.draw();
	}

	{
		std::vector<glm::vec3> points;
		abc.get("/points", points);
		
		ofSetColor(0, 255, 0);
		for (int i = 0; i < points.size(); i++) {
			ofDrawCircle(points[i].x, points[i].y, points[i].z, 5);
		}
	}

	{
		std::vector<ofPolyline> curves;
		abc.get("/curves", curves);
		
		ofSetColor(0, 0, 255);
		for (int i = 0; i < curves.size(); i++)
			curves[i].draw();
	}

	{
		ofSetColor(255, 255, 0);
		abc.get("/box")->draw(); // draw box with xform
	}

	// or simply, abc.draw();

	cam.end();
	
	ofSetColor(255);
	
	ofDrawBitmapString(ofToString(abc.getTime()) + "/" + ofToString(abc.getMaxTime()), 10, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
#ifndef DRONE_H
#define DRONE_H

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#if defined(Bool)
#undef Bool
#endif
#if defined(True)
#undef True
#endif
#if defined(False)
#undef False
#endif

#include "Network/Address.h"
#include "Network/Device.h"
#include "Network/Protocol/Local.h"
#include "Network/Protocol/Minuit.h"

using namespace OSSIA;
using namespace std;

#define PATH ""

//Point(0,0,0) is at the back left corner

/*
* Drone class
* */
class Drone
{
public:
    Drone() = delete; //forbids default constructor
    Drone(const Drone&) = delete; //forbids copy constructor

    Drone(Drone&&) = default; //move constructor : ctrl-X
    Drone(int id, std::shared_ptr<Node> parentNode, ofVec3f size, ofVec3f pos = ofVec3f(0), string modelName = "Sphere");

    // Load the 3d model
    bool load();

    // Update 3D model
    void updateBot(){_loader.update();}

    // Moves to the next position
    void move(ofVec3f speed);

    // Modifies the model to the default one (=Sphere)
    void modelToDefault(){_modelName = defaultModel();}

    // Set inZone to false : bot out of zone
    void outOfZone(){_inZone = false;}

    // Set collision to true : bot in collision with another
    void collision(){_collision = true;}

    // Returns information on the drone
    string info() const;

    // Returns classname (=Drone)
    string className() const {return "Drone";}

    // Returns the default model (=sphere)
    string defaultModel(){return "Sphere";}

    // Returns id
    int id() const {return _id;}

    // Returns size
    ofVec3f size() const {return _size;}

    // Returns current position
    ofVec3f position() const {return _position;}

    // Returns color
    ofVec3f color() const {return _color;}

    // Returns if the bot is in zone or not
    bool isInZone() const {return _inZone;}

    // Returns if the bot is in collision or not
    bool isInCollision() const {return _collision;}

    // Returns 3D model object name
    string modelName() const {return _modelName;}

    // Returns 3D model object loader
    ofxAssimpModelLoader& loader() {return _loader;}

    // Share the drone with i-score
    void shareDrone(std::shared_ptr<Node> parentNode);

private:
    // Drone individual id
    int _id;

    // Drone size
    ofVec3f _size;

    // Drone current position
    ofVec3f _position;

    // Drone color (temporary used in selecting in ofApp)
    ofVec3f _color;

    // boolean to check if the position is inbound or not
    bool _inZone = true;

    // boolean for the collision
    bool _collision = false;

    // 3D model name
    string _modelName;

    // 3D object model loader
    ofxAssimpModelLoader _loader;

    // node in the network
    //std::shared_ptr<Node> droneNode;
    //std::shared_ptr<Address> idAddress;
};

#endif // DRONE_H

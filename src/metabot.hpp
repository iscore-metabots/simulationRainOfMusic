#ifndef METABOT_H
#define METABOT_H

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

//Point(0,0,0) is at the back left corner

/*
* Metabot class
*
* 3d Model tends to slow down the movement
* */
class Metabot
{
public:
    Metabot() = delete; //forbids default constructor
    Metabot(const Metabot&) = delete; //forbids copy constructor

    Metabot(Metabot&&) = default; //move constructor : ctrl-X
    Metabot(int id,
            std::shared_ptr<Node> parentNode,
            ofVec3f size = ofVec3f(30),
            ofVec3f pos = ofVec3f(0),
            string modelName = "Square",
            float freq = 2);

    // Load the 3d model
    bool load();

    // Update 3D model
    void updateBot(){_loader.update();}

    // Moves to the next position
    void move(ofVec3f speed); // the speed should be automatically updated,
                              // so move() will not take argument (attribute _speed to ba added)

    // Modifies the model to the default one (=Square)
    void modelToDefault(){_modelName = defaultModel();}

    // Set inZone to false : bot out of zone
    void outOfZone(){_inZone = false;}

    // Set collision to true : bot in collision with another
    void collision(){_collision = true;}

    // Returns information on the metabot
    string info() const;

    // Returns classname (=Metabot)
    string className() const {return "Metabot";}

    // Returns the default model (=square)
    string defaultModel(){return "Square";}

    // Returns id
    int id() const {return _id;}

    // Returns size
    ofVec3f size() const {return _size;}

    // Returns current position
    ofVec3f position() const {return _position;}

    // Returns color
    ofVec3f color() const {return _color;}

    // Returns walking frequency
    float frequency() const {return _frequency;}

    // Returns if the bot is in zone or not
    bool isInZone() const {return _inZone;}

    // Returns if the bot is in collision or not
    bool isInCollision() const {return _collision;}

    // Returns 3D model object name
    string modelName() const {return _modelName;}

    // Returns 3D model object loader
    ofxAssimpModelLoader& loader() {return _loader;}

    // Share the metabot with i-score
    void shareMetabot(std::shared_ptr<Node> parentNode);

private:
    // Metabot individual id
    int _id;

    // Metabot size
    ofVec3f _size;

    // Metabot current position
    ofVec3f _position;

    // Metabot color (temporary used in selecting in ofApp)
    ofVec3f _color;

    // Frequency of the walk, in Hz (default = 2Hz)
    float _frequency;

    // boolean to check if the position is inbound or not
    bool _inZone = true;

    // boolean for the collision
    bool _collision = false;

    // 3D model name
    string _modelName;

    // 3D object model loader
    ofxAssimpModelLoader _loader;

    // node in the network
    //std::shared_ptr<Node> metabotNode;
    //std::shared_ptr<Address> idAddress;
};

#endif // METABOT_H

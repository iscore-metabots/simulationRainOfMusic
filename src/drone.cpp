#include "drone.hpp"

Drone::Drone(int id , std::shared_ptr<Node> parentNode, ofVec3f size, ofVec3f pos, string modelName):
    _id(id),
    _size(size),
    _position(pos),
    _color(ofVec3f(0,200+id,0)),
    _modelName(modelName){

    // adds the drone to the node Scene
    shareDrone(parentNode);

    // creates parameters to be published
    _collision = Parameter<Bool>(_droneNode,
                                 Value::Type::BOOL,
                                 Bool(false),
                                 string("collision"));
    _inZone = Parameter<Bool>(_droneNode,
                              Value::Type::BOOL,
                              Bool(true),
                              string("inZone"));

}

//--------------------------------------------------------------

void Drone::shareDrone(std::shared_ptr<Node> parentNode){
    _droneNode = *(parentNode->emplace(parentNode->children().cend(), "Drone "+ to_string(id())));
}
//--------------------------------------------------------------

bool Drone::load(){
    if(_loader.loadModel(_modelName)){
        return true;
    }
    else{
        modelToDefault();
        return false;
    }
}

//--------------------------------------------------------------

void Drone::move(ofVec3f speed){
     if(isInCollision()==Bool(false)){
        _position += speed;
     }
}
//--------------------------------------------------------------

string Drone::info() const
{
    string msg;
    msg = "Robot selected : "
            + className() + " "
            + std::to_string( id()) + "\n";
    msg += "at position ("
            + std::to_string((int)position().x)+", "
            + std::to_string((int)position().y)+", "
            + std::to_string((int)position().z)+") \n";

    return msg;
}


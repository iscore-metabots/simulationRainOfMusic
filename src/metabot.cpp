#include "metabot.hpp"

Metabot::~Metabot(){
    _collision.removeListener(&_collision,&Parameter<bool>::listen);
    _inZone.removeListener(&_inZone,&Parameter<bool>::listen);

    _frequency.removeListener(&_frequency,&Parameter<float>::listen);

    _speed_x.removeListener(&_speed_x,&Parameter<float>::listen);
    _speed_y.removeListener(&_speed_y,&Parameter<float>::listen);

    _angle.removeListener(&_angle,&Parameter<float>::listen);
}

Metabot::Metabot(int id , std::shared_ptr<Node> parentNode, ofVec3f zonedim,
                 float proba, ofVec3f pos, ofVec3f size, string modelName, float freq, float batt):
    _id(id),
    _zoneDim(zonedim),
    _size(size),
    _initialPos(ofVec3f(pos.x,pos.z,pos.y)),
    _color(ofVec3f(0,0,200+id)),
    _modelName(modelName),
    _initialFreq(freq),
    _initialBatt(batt){
    /* if(string::npos != modelName.find(".")){
        _3dmodel = PATH+modelName;
    }*/
    // adds the metabot to the node Scene
    shareMetabot(parentNode);

    setup(proba,parentNode);
}


//--------------------------------------------------------------

void Metabot::shareMetabot(std::shared_ptr<Node> parentNode){
    _metabotNode = *(parentNode->emplace(parentNode->children().cend(), "Metabot."+ to_string(id())));
}

//--------------------------------------------------------------
void Metabot::setup(float proba,std::shared_ptr<Node> parentNode){
    // initialize the parameters groups
    _parameters.setName(this->className()+std::to_string(_id));
    //_parameters.
    _simulation.setName(this->className()+std::to_string(_id));

    // creates parameters to be published
    _collision.setup(_metabotNode,"collision",false);

    _inZone.setup(_metabotNode,"inZone",true);

    // probability
    _probability.setupNoPublish(parentNode,"PacketLoss",proba,0,100);

    if(_probability.getAddress() == NULL){
        // with getAddress if the adress is null, it explores the tree
        //  std::cout << "adress null"<<std::endl;
    }
    else{ _probability.getAddress()->addCallback([&](const Value *v){
            OSSIA::Float * val= (OSSIA::Float *)v;
            if(val->value != _probability.get()){
                _probability.set(val->value);
            }

        });
    }
    // _proba.addListener(&_proba,&Parameter<float>::listen);

    // Battery set up
    _simulation.add(_battery.setup(_metabotNode,"battery",_initialBatt,0,100));
    _battery.getAddress()->addCallback([&](const Value *v){
        OSSIA::Float * val= (OSSIA::Float *)v;
        if(val->value !=_battery.get()){
            _battery.set(val->value);
        }

    });
    _battery.addListener(&_battery,&Parameter<float>::listen);

    // creates parameters to be published and listened
    // Frequency set up
    _parameters.add(_frequency.setup(_metabotNode,"freq",_initialFreq,0,3));
    _frequency.getAddress()->addCallback([&](const Value *v){

        // if there is a packet loss
        if(random()%100 <= _probability.get()){
            // do nothing
        }
        else{ OSSIA::Float * val= (OSSIA::Float *)v;
            if(val->value !=_frequency.get()){
                _frequency.set(val->value);
            }
        }
    });
    _frequency.addListener(&_frequency,&Parameter<float>::listen);

    // Angle set up
    _parameters.add(_angle.setup(_metabotNode,"turn",0,-300,300));
    _angle.getAddress()->addCallback([&](const Value *v){
        // if there is a packet loss
        if(random()%100 <= _probability.get()){
            // do nothing
        }
        else{ OSSIA::Float * val= (OSSIA::Float *)v;
            if(val->value !=_angle.get()){
                _angle.set(val->value);
            }
        }
    });
    _angle.addListener(&_angle,&Parameter<float>::listen);


    // Speed set up
    _parameters.add(_speed_x.setup(_metabotNode,"dx",0,-300,300));
    _speed_x.getAddress()->addCallback([&](const Value *v){
        // if there is a packet loss
        if(random()%100 <= _probability.get()){
            // do nothing
        }
        else{
            OSSIA::Float * val= (OSSIA::Float *)v;
            if(val->value !=_speed_x){
                _speed_x.set(val->value);
            }
        }
    });
    _speed_x.addListener(&_speed_x,&Parameter<float>::listen);


    _parameters.add(_speed_y.setup(_metabotNode,"dy",0,-300,300));
    _speed_y.getAddress()->addCallback([&](const Value *v){
        // if there is a packet loss
        if(random()%100 <= _probability.get()){
            // do nothing
        }
        else{
            OSSIA::Float * val= (OSSIA::Float *)v;
            if(val->value !=_speed_y){
                _speed_y.set(val->value);
            }
        }
    });
    _speed_y.addListener(&_speed_y,&Parameter<float>::listen);

    _parameters.add( _position.setup(_metabotNode,"position",_initialPos,ofVec2f(0),ofVec3f(_zoneDim.x,_zoneDim.z,_zoneDim.y)));//ofVec2f(500,300)));

    _position.getAddress()->addCallback([&](const Value *v){
        // if there is a packet loss
        if(random()%100 <= _probability.get()){
            // do nothing
        }
        else{
            OSSIA::Tuple * val = (OSSIA::Tuple *) v;
            val->value.resize(2);
            OSSIA::Float * valx = (OSSIA::Float *) val->value[0];
            OSSIA::Float * valy = (OSSIA::Float *) val->value[1];

            if(valx->value != _position.get().x
                    || valy->value != _position.get().y){
                _position.set(ofVec2f(valx->value,valy->value));
            }
        }
    });
    _position.addListener(&_position,&Parameter<ofVec2f>::listen);
}

//--------------------------------------------------------------

bool Metabot::load(){
    if(_loader.loadModel(_modelName)){
        _loader.setScale(_size.x/100,_size.y/100,_size.z/100);
        _size = ofVec3f(30);
        return true;
    }
    else{
        modelToDefault();
        return false;
    }
}
//--------------------------------------------------------------
void Metabot::move(ofVec3f speed){
    if(!isInCollision()){
        _position += ofVec2f(speed.x,speed.z);
    }
}
void Metabot::move(){
    // defined in ofApp using setFrameRate()
    if(!isInCollision() && _battery.get()!= 0 ){
        // a simple equation is used here but
        // it can be changed to a more complex one if needed
        float dx = _speed_x.get() * (0.1/_frameRate); // mm/s
        float dy = _speed_y.get() * (0.1/_frameRate);
        float ang = ofDegToRad(_loader.getRotationAngle(0) );

        ofVec2f speedtmp = ofVec2f(dx * cos(ang) - dy * sin(ang),
                                   (dx * sin(ang) + dy * cos(ang)));
        ofVec2f newpos = _position.get() + speedtmp;

        float distance = newpos.distance(_position.get());
        _position.set(newpos);

        // battery consumption model
        float battmp = _battery.get() - distance/60;
        // /60 because speed is in mm/s and battery in min
        _battery.set(battmp);
    }
}

//--------------------------------------------------------------

void Metabot::reset(){
    _position.set(_initialPos);
    _frequency.set(_initialFreq);
    _speed_x.set(0);
    _speed_y.set(0);
    _angle.set(0);
    _inZone.update(true);
    _collision.update(false);
    _loader.setRotation(0,0,0,0,0);
    _battery.set(90);
}


//--------------------------------------------------------------
string Metabot::info() const
{
    string msg;
    msg = "Robot selected : "
            + className() + " "
            + std::to_string( id()) + "\n";
    msg += "at position ("
            + std::to_string((int)position().x)+", "
            + std::to_string((int)position().z)+") \n";
    msg += "Walking frequency: "
            + std::to_string(frequency()) + " Hz";
    return msg;
}





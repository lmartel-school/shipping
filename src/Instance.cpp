#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#include "Instance.h"
#include "Engine.h"
#include "Location.h"
#include "Statistics.h"
#include "Connectivity.h"


namespace Shipping {

using namespace std;

//
// Rep layer classes
//
class ManagerImpl;

class LocationRep : public Instance {
public:

    LocationRep(const string& name, ManagerImpl* manager, Location::Ptr location) :
        Instance(name), manager_(manager), location_(location)
    {
        // Nothing else to do.
    }

    string attribute(const string& name) {
        int i = segmentNumber(name);
        if (i != 0) {
            return location_->segment(i)->name();
        }
        return "";
    }


    void attributeIs(const string& name, const string& v) {
        // Mothing to do
    }

protected:
    Ptr<ManagerImpl> manager_;
    Location::Ptr location_;

    int segmentNumber(const string& name);

};
                                                                                                  
class TerminalRep : public LocationRep {
public:

    TerminalRep(const string& name, ManagerImpl *manager, Terminal::Ptr location) :
        LocationRep(name, manager, location), segmentType_(location->segmentType())
    {
        // Nothing else to do.
    }

private:
    Segment::Type segmentType_;

};

class SegmentRep : public Instance {
public:
    SegmentRep(const string& name, ManagerImpl* manager, Segment::Ptr segment) :
        Instance(name), manager_(manager), segment_(segment)
    {
        // Nothing else to do.
    }

    string attribute(const string& name){
        // TODO parser
    }

    void attributeIs(const string& name, const string& v){
        // TODO parser
    }

protected:
    Ptr<ManagerImpl> manager_;
    Segment::Ptr segment_;
};

class StatisticsRep : public Instance {
public:
    StatisticsRep(const string& name, ManagerImpl* manager, Statistics::Ptr statistics) :
        Instance(name), manager_(manager), statistics_(statistics)
    {
        // Nothing else to do.
    }

    string attribute(const string& name){
        // TODO parser
    }

    void attributeIs(const string& name, const string& v){
        // TODO parser
    }

protected:
    Ptr<ManagerImpl> manager_;
    Statistics::Ptr statistics_;
};

class ConnectivityRep : public Instance {
public:
    ConnectivityRep(const string& name, ManagerImpl* manager, Connectivity::Ptr connectivity) :
        Instance(name), manager_(manager), connectivity_(connectivity)
    {
        // Nothing else to do.
    }

    string attribute(const string& name){
        // TODO parser
    }

    void attributeIs(const string& name, const string& v){
        // TODO parser
    }

protected:
    Ptr<ManagerImpl> manager_;
    Connectivity::Ptr connectivity_;
};

class FleetRep : public Instance {
public:
    FleetRep(const string& name, ManagerImpl* manager, Fleet::Ptr fleet) :
        Instance(name), manager_(manager), fleet_(fleet)
    {
        // Nothing else to do.
    }

    string attribute(const string& name){
        // TODO parser
    }

    void attributeIs(const string& name, const string& v){
        // TODO parser
    }

protected:
    Ptr<ManagerImpl> manager_;
    Fleet::Ptr fleet_;
};

class ManagerImpl : public Instance::Manager {
public:
    ManagerImpl(){
        engine_ = Engine::engineNew(string("engine"));
    }

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    // Manager method
    void instanceDel(const string& name);

private:
    map<string,Ptr<Instance> > instance_;
    Engine::Ptr engine_;

    Ptr<FleetRep> fleetRep_;
    Ptr<StatisticsRep> statisticsRep_;
    Ptr<ConnectivityRep> connectivityRep_;
};

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
    if(instance(name) != NULL){
        cerr << "Instance name already in use." << endl;
        return NULL;
    }

    Ptr<Instance> t;
    if(type == "Customer"){ // locations
        t = new LocationRep(name, this, engine_->customerLocationNew(name));
    } else if(type == "Port") {
        t = new LocationRep(name, this, engine_->portLocationNew(name));
    } else if(type == "Truck terminal") {
        t = new TerminalRep(name, this, engine_->terminalLocationNew(name, Segment::truck()));
    } else if(type == "Boat terminal"){
        t = new TerminalRep(name, this, engine_->terminalLocationNew(name, Segment::boat()));
    } else if(type == "Plane terminal"){
        t = new TerminalRep(name, this, engine_->terminalLocationNew(name, Segment::plane()));
    } else if(type == "Truck segment"){ // segments
        t = new SegmentRep(name, this, engine_->truckSegmentNew(name));
    } else if(type == "Boat segment"){
        t = new SegmentRep(name, this, engine_->boatSegmentNew(name));
    } else if(type == "Plane segment"){
        t = new SegmentRep(name, this, engine_->planeSegmentNew(name));
    } else if(type == "Fleet"){
        if(fleetRep_) return fleetRep_;
        fleetRep_ = new FleetRep(name, this, engine_->fleetNew(name));
        t = fleetRep_;
    } else if(type == "Stats"){
        if(statisticsRep_) return statisticsRep_;
        statisticsRep_ = new StatisticsRep(name, this, Statistics::statisticsNew(name, engine_));
        t = statisticsRep_;
    } else if(type == "Conn"){
        if(connectivityRep_) return connectivityRep_;
        connectivityRep_ = new ConnectivityRep(name, this, Connectivity::connectivityNew(name, engine_));
        t = connectivityRep_;
    } else {
        cerr << "Invalid instance type." << endl;
        return NULL;
    }

    instance_[name] = t;
    return t;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);

    return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
    if (name.substr(0, segmentStrlen) == segmentStr) {
        const char* t = name.c_str() + segmentStrlen;
        return atoi(t);
    }
    return 0;
}


}

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
    return new Shipping::ManagerImpl();
}

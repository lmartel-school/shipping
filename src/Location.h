#ifndef LOCATION_H
#define LOCATION_H

#include "Segment.h"
#include "Typedef.h"
#include "Entity.h"
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include "ActivityManager.h"
#include "Activity.h"
#include "TransferRate.h"
#include "ShipmentCount.h"
#include "PackageCount.h"
#include "PathData.h"
#include "Shipment.h"

namespace Shipping {
	class Connectivity;
	class Location : public Entity<Location> {
	public:
		typedef Fwk::Ptr<Location> Ptr;

		enum class Type {
			CUSTOMER,
		    PORT,
		    BOAT_TERMINAL,
		    TRUCK_TERMINAL,
        PLANE_TERMINAL
        };

		static Type customer(){ return Type::CUSTOMER; } 
		static Type port(){ return Type::PORT; }
		static Type boatTerminal(){ return Type::BOAT_TERMINAL; }
    static Type planeTerminal(){ return Type::PLANE_TERMINAL; }
    static Type truckTerminal(){ return Type::TRUCK_TERMINAL; }

		Type type(){ return type_; }

    SegmentPtr segment(int index){ if (index < segments_.size()) { return segments_[index]; } else { return NULL; } }
    virtual void segmentIs(int index, SegmentPtr segment){ segments_[index] = segment; }
    virtual void segmentAdd(SegmentPtr segment) { segments_.push_back(segment); }
    virtual void segmentDel(SegmentPtr segment) { segments_.erase(std::find(segments_.begin(), segments_.end(), segment)); }
    EntityCount segmentCount() { return segments_.size(); }

    void shipmentAdd(Shipment::Ptr shipment) { shipments_.push_back(shipment); }
    void shipmentDel(Shipment::Ptr shipment) { shipments_.erase(find(shipments_.begin(), shipments_.end(), shipment)); }
    EntityCount shipmentCount() { return shipments_.size(); }

    void routeIs(EntityName dest, PathData route){
    	std::vector<PathData> paths = routes_[dest];
    	paths.push_back(route);
    }

    PathData route(EntityName dest){
    	auto it = routes_.find(dest);
    	if(it == routes_.end()) throw new InvalidAttributeException("No routes found");
    	std::vector<PathData> paths = (*it).second;
    	return paths[0]; // TODO: choose best path?
    }

    ShipmentCount shipmentsReceived() { return shipmentsReceived_; }
    Hour averageLatency() { return averageLatency_; }
    Dollar totalCost() { return totalCost_; }


	protected:
    std::vector<SegmentPtr> segments_;
    std::map<EntityName, std::vector<PathData>> routes_; // Name of destination
    Type type_;

    ShipmentCount shipmentsReceived_;
    Hour averageLatency_;
    Dollar totalCost_;
    vector<Shipment::Ptr> shipments_;

		static Location::Ptr locationNew(EntityName name, Type type){
			Ptr ptr = new Location(name, type);
      return ptr;
		}

    #include "ForwardingActivityReactor.h"

    Location(EntityName name, Type type): Entity(name), type_(type), shipmentsReceived_(0), averageLatency_(0), totalCost_(0) {}
	};

	class Customer : public Location {
	public:
		static Location::Ptr customerNew(EntityName name){ return locationNew(name, customer()); }
    void transferRateIs(TransferRate transferRate) { transferRate_ = transferRate; }
    void shipmentSizeIs(PackageCount shipmentSize) { shipmentSize_ = shipmentSize; }
    void destinationIs(Location::Ptr);

    TransferRate transferRate() { return transferRate_; }
    PackageCount shipmentSize() { return shipmentSize_; }
    Location::Ptr destination() { return destination_; }

  private:
    TransferRate transferRate_;
    PackageCount shipmentSize_;
    Location::Ptr destination_;
	};


	class Port : public Location {
	public:
		static Location::Ptr portNew(EntityName name){ return locationNew(name, port()); }
	};
}

#endif

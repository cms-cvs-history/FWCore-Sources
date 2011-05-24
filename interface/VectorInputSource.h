#ifndef FWCore_Sources_VectorInputSource_h
#define FWCore_Sources_VectorInputSource_h

/*----------------------------------------------------------------------
VectorInputSource: Abstract interface for vector input sources.
----------------------------------------------------------------------*/

#include "FWCore/Sources/interface/EDInputSource.h"

#include "boost/shared_ptr.hpp"

#include <memory>
#include <string>
#include <vector>

namespace edm {
  class EventPrincipal;
  struct InputSourceDescription;
  class ParameterSet;
  class VectorInputSource : public EDInputSource {
  public:
    typedef boost::shared_ptr<EventPrincipal> EventPrincipalVectorElement;
    typedef std::vector<EventPrincipalVectorElement> EventPrincipalVector;
    explicit VectorInputSource(ParameterSet const& pset, InputSourceDescription const& desc);
    virtual ~VectorInputSource();

    template<typename T>
    void loopRandom(int number, T& eventOperator);
    template<typename T>
    void loopSequential(int number, T& eventOperator);
    template<typename T>
    void loopSpecified(std::vector<EventID> const& events, T& eventOperator);

    void dropUnwantedBranches(std::vector<std::string> const& wantedBranches);

  private:

    virtual std::auto_ptr<EventPrincipal> readOneRandom() = 0;
    virtual std::auto_ptr<EventPrincipal> readOneSequential() = 0;
    virtual std::auto_ptr<EventPrincipal> readOneSpecified(EventID const& event) = 0;

    virtual void dropUnwantedBranches_(std::vector<std::string> const& wantedBranches) = 0;
  };

  template<typename T>
  void VectorInputSource::loopRandom(int number, T& eventOperator) {
    for(int i = 0; i < number; ++i) {
      std::auto_ptr<EventPrincipal> ep = readOneRandom();
      eventOperator(*ep);
    }
  }

  template<typename T>
  void VectorInputSource::loopSequential(int number, T& eventOperator) {
    for(int i = 0; i < number; ++i) {
      std::auto_ptr<EventPrincipal> ep = readOneSequential();
      eventOperator(*ep);
    }
  }

  template<typename T>
  void VectorInputSource::loopSpecified(std::vector<EventID> const& events, T& eventOperator) {
    for(std::vector<EventID>::const_iterator it = events.begin(), itEnd = events.end(); it != itEnd; ++it) {
      std::auto_ptr<EventPrincipal> ep = readOneSpecified(*it);
      eventOperator(*ep);
    }
  }
}
#endif

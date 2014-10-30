#include "FWCore/Framework/interface/Event.h"

#include "CaloCollections.h"

namespace l1t {
   namespace stage1 {
      CaloCollections::~CaloCollections()
      {
         event_.put(towers_);
         event_.put(egammas_);
         event_.put(etsums_);
         event_.put(jets_);
         event_.put(taus_);
      }
   }
}

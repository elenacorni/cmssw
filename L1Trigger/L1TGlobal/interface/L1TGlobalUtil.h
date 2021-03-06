// L1TGlobalUtil:  Utility class for parsing the L1 Trigger Menu 

#ifndef L1TGlobal_L1TGlobalUtil_h
#define L1TGlobal_L1TGlobalUtil_h

// system include files
#include <vector>

#include "CondFormats/L1TObjects/interface/L1TUtmTriggerMenu.h"

// Objects to produce for the output record.
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "DataFormats/L1TGlobal/interface/GlobalExtBlk.h"
#include "CondFormats/L1TObjects/interface/L1TUtmAlgorithm.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/EventSetup.h"

#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtilHelper.h"

// forward declarations

// class declaration

namespace l1t {

class L1TGlobalUtil {

public:

  // Using this constructor will require InputTags to be specified in the configuration
  L1TGlobalUtil(edm::ParameterSet const& pset,
		edm::ConsumesCollector&& iC);

  L1TGlobalUtil(edm::ParameterSet const& pset,
		edm::ConsumesCollector& iC);

  // Using this constructor will cause it to look for valid InputTags in
  // the following ways in the specified order until they are found.
  //   1. The configuration
  //   2. Search all products from the preferred input tags for the required type
  //   3. Search all products from any other process for the required type
  template <typename T>
  L1TGlobalUtil(edm::ParameterSet const& pset,
		edm::ConsumesCollector&& iC,
		T& module);
  
  template <typename T>
  L1TGlobalUtil(edm::ParameterSet const& pset,
		edm::ConsumesCollector& iC,
		T& module);

  // Using this constructor will cause it to look for valid InputTags in
  // the following ways in the specified order until they are found.
  //   1. The constructor arguments
  //   2. The configuration
  //   3. Search all products from the preferred input tags for the required type
  //   4. Search all products from any other process for the required type
  template <typename T>
  L1TGlobalUtil(edm::ParameterSet const& pset,
		edm::ConsumesCollector&& iC,
		T& module,
		edm::InputTag const& l1tAlgBlkInputTag,
		edm::InputTag const& l1tExtBlkInputTag);

  template <typename T>
  L1TGlobalUtil(edm::ParameterSet const& pset,
		edm::ConsumesCollector& iC,
		T& module,
		edm::InputTag const& l1tAlgBlkInputTag,
		edm::InputTag const& l1tExtBlkInputTag);

  /// destructor
  virtual ~L1TGlobalUtil();  
  
  static void fillDescription(edm::ParameterSetDescription & desc) {
    L1TGlobalUtilHelper::fillDescription(desc);
  }

    // OverridePrescalesAndMasks
    // The ability to override the prescale/mask file will not be part of the permanent interface of this class.
    // It is provided only until prescales and masks are available as CondFormats...
    // Most users should simply ignore this method and use the default ctor only!
    // Will look for prescale csv file in L1Trigger/L1TGlobal/data/Luminosity/startup/<filename>
    void OverridePrescalesAndMasks(std::string filename, unsigned int psColumn=1);

    /// initialize the class (mainly reserve)
    void retrieveL1(const edm::Event& iEvent, const edm::EventSetup& evSetup); // using helper
    void retrieveL1(const edm::Event& iEvent, const edm::EventSetup& evSetup, edm::EDGetToken gtAlgToken);
    void retrieveL1Setup(const edm::EventSetup& evSetup);
    void retrieveL1Event(const edm::Event& iEvent, const edm::EventSetup& evSetup); // using helper
    void retrieveL1Event(const edm::Event& iEvent, const edm::EventSetup& evSetup, edm::EDGetToken gtAlgToken);

    inline void setVerbosity(const int verbosity) {
        m_verbosity = verbosity;
    }
 
    inline bool getFinalOR() const {return m_finalOR;}
    
    // get the trigger bit from the name
    const bool getAlgBitFromName(const std::string& AlgName, int& bit) const;

    // get the name from the trigger bit
    const bool getAlgNameFromBit(int& bit, std::string& AlgName) const;
				 
    // Access results for particular trigger bit 
    const bool getInitialDecisionByBit(int& bit,   bool& decision) const;
    const bool getIntermDecisionByBit(int& bit,    bool& decision) const;
    const bool getFinalDecisionByBit(int& bit,     bool& decision) const;

    // Access Prescale
    const bool getPrescaleByBit(int& bit,           int& prescale) const;

    // Access Masks:
    // follows logic of uGT board:
    //       finalDecision[AlgBit] 
    //           Final word is after application of prescales.
    //           A prescale = 0 effectively masks out the algorithm in the final decision word
    //
    //    If vetoMask = true and Algorithm is true, the FINOR (final global decision) is forced to false (ie. event is vetoed)
    //    If vetoMask = false, algorithm cannot veto FINOR (final global decision)
    const bool getMaskByBit(int& bit,              bool&     mask) const;
    const bool getVetoMaskByBit(int& bit,          bool&     veto) const;

    // Access results for particular trigger name
    const bool getInitialDecisionByName(const std::string& algName,   bool& decision) const;
    const bool getIntermDecisionByName(const std::string& algName,    bool& decision) const;
    const bool getFinalDecisionByName(const std::string& algName,     bool& decision) const;

    // Access Prescales
    const bool getPrescaleByName(const std::string& algName,           int& prescale) const;

    // Access Masks (see note) above
    const bool getMaskByName(const std::string& algName,              bool&     mask) const;
    const bool getVetoMaskByName(const std::string& algName,          bool&     veto) const;

    // Some inline commands to return the full vectors
    inline const std::vector<std::pair<std::string, bool> >& decisionsInitial()   { return m_decisionsInitial; }
    inline const std::vector<std::pair<std::string, bool> >& decisionsInterm()    { return m_decisionsInterm; }
    inline const std::vector<std::pair<std::string, bool> >& decisionsFinal()     { return m_decisionsFinal; }

    // Access all prescales
    inline const std::vector<std::pair<std::string, int> >&  prescales()          { return m_prescales; }

    // Access Masks (see note) above
    inline const std::vector<std::pair<std::string, bool> >& masks()              { return m_masks; }
    inline const std::vector<std::pair<std::string, bool> >& vetoMasks()          { return m_vetoMasks; }

    // Menu names
    inline const std::string& gtTriggerMenuName()    const {return m_l1GtMenu->getName();}
    inline const std::string& gtTriggerMenuVersion() const {return m_l1GtMenu->getVersion();}
    inline const std::string& gtTriggerMenuComment() const {return m_l1GtMenu->getComment();}

    // Prescale Column
    inline unsigned int prescaleColumn() const {return m_PreScaleColumn;}

private:

    L1TGlobalUtil();

    /// clear decision vectors on a menu change
    void resetDecisionVectors();
    void resetPrescaleVectors();
    void resetMaskVectors();
    void loadPrescalesAndMasks();

    // trigger menu
    const L1TUtmTriggerMenu* m_l1GtMenu;
    unsigned long long m_l1GtMenuCacheID;

    // prescales and masks
    bool m_filledPrescales;

    // algorithm maps
    //const AlgorithmMap* m_algorithmMap;
    const std::map<std::string, L1TUtmAlgorithm>* m_algorithmMap;
    
    // Number of physics triggers
    unsigned int m_numberPhysTriggers;
    
    //file  and container for prescale factors
    std::string m_preScaleFileName;
    unsigned int m_PreScaleColumn;
    
    std::vector<std::vector<int> > m_initialPrescaleFactorsAlgoTrig;
    const std::vector<std::vector<int> >* m_prescaleFactorsAlgoTrig;
    std::vector<unsigned int>  m_initialTriggerMaskAlgoTrig;
    const std::vector<unsigned int>*  m_triggerMaskAlgoTrig;
    std::vector<unsigned int>   m_initialTriggerMaskVetoAlgoTrig;
    const std::vector<unsigned int>*  m_triggerMaskVetoAlgoTrig;
    
    // access to the results block from uGT 
    edm::Handle<BXVector<GlobalAlgBlk>>  m_uGtAlgBlk;

    // final OR
    bool m_finalOR;

    // Vectors containing the trigger name and information about that trigger
    std::vector<std::pair<std::string, bool> > m_decisionsInitial;
    std::vector<std::pair<std::string, bool> > m_decisionsInterm;
    std::vector<std::pair<std::string, bool> > m_decisionsFinal;
    std::vector<std::pair<std::string, int> >  m_prescales;
    std::vector<std::pair<std::string, bool> > m_masks;
    std::vector<std::pair<std::string, bool> > m_vetoMasks;
    
    /// verbosity level
    int m_verbosity;

    std::unique_ptr<L1TGlobalUtilHelper> m_l1tGlobalUtilHelper;

};

template <typename T>
L1TGlobalUtil::L1TGlobalUtil(edm::ParameterSet const& pset,
			     edm::ConsumesCollector&& iC,
			     T& module) :
 L1TGlobalUtil(pset, iC, module) { }

template <typename T>
L1TGlobalUtil::L1TGlobalUtil(edm::ParameterSet const& pset,
			     edm::ConsumesCollector& iC,
			     T& module) :
 L1TGlobalUtil() {
   m_l1tGlobalUtilHelper.reset(new L1TGlobalUtilHelper(pset,
						       iC,
						       module));
 }
 
template <typename T>
L1TGlobalUtil::L1TGlobalUtil(edm::ParameterSet const& pset,
			     edm::ConsumesCollector&& iC,
			     T& module,
			     edm::InputTag const& l1tAlgBlkInputTag,
			     edm::InputTag const& l1tExtBlkInputTag) :
 L1TGlobalUtil(pset, iC, module, l1tAlgBlkInputTag, l1tExtBlkInputTag) { }

template <typename T>
L1TGlobalUtil::L1TGlobalUtil(edm::ParameterSet const& pset,
			     edm::ConsumesCollector& iC,
			     T& module,
			     edm::InputTag const& l1tAlgBlkInputTag,
			     edm::InputTag const& l1tExtBlkInputTag) :
 L1TGlobalUtil() {
   m_l1tGlobalUtilHelper.reset(new L1TGlobalUtilHelper(pset,
						       iC,
						       module,
						       l1tAlgBlkInputTag,
						       l1tExtBlkInputTag));
 }
}
#endif

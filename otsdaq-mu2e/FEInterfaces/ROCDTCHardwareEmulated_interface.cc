#include "otsdaq-mu2e/FEInterfaces/ROCDTCHardwareEmulated.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"

using namespace ots;

#undef __MF_SUBJECT__
#define __MF_SUBJECT__ "FE-ROCDTCHardwareEmulated"

//=========================================================================================
ROCDTCHardwareEmulated::ROCDTCHardwareEmulated(
    const std::string&       rocUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       theConfigurationPath)
    : ROCCoreVInterface(rocUID, theXDAQContextConfigTree, theConfigurationPath)
{
	INIT_MF("." /*directory used is USER_DATA/LOG/.*/);

	__MCOUT_INFO__("ROCDTCHardwareEmulated instantiated with link: "
	               << linkID_ << " and EventWindowDelayOffset = " << delay_ << __E__);
}  // end constructor()

//==========================================================================================
ROCDTCHardwareEmulated::~ROCDTCHardwareEmulated(void)
{
	// NOTE:: be careful not to call __FE_COUT__ decoration because it uses the
	// tree and it may already be destructed partially
	__COUT__ << FEVInterface::interfaceUID_ << "Destructed." << __E__;
}  // end destructor()

//==================================================================================================
uint16_t ROCDTCHardwareEmulated::readEmulatorRegister(uint16_t address)
{
	__FE_COUT__ << "Calling read emulator ROC register: link number " << std::dec
	            << linkID_ << ", address = " << address << __E__;
	if(address == 6)
		return 4860;
	else if(address == 7)
		return delay_;
	else
		return address + (time(NULL)&0xF);
	return -1;
}  // end readEmulatorRegister
//==================================================================================================
void ROCDTCHardwareEmulated::readEmulatorBlock(std::vector<DTCLib::roc_data_t>& 	data,
                                             DTCLib::roc_address_t  	   	address,
                                             uint16_t               		numberOfReads,
                                             bool                   		incrementAddress)
{
	__FE_COUT__ << "Calling read emulator BLOCK: link number " << std::dec << linkID_
	            << ", address = " << address << ", numberOfReads = " << numberOfReads
	            << __E__;
	//	if(address == 6)
	//		return 4860;
	//	else if(address == 7)
	//		return delay_;
	return;
}  // end readEmulatorBlock

//==================================================================================================
void ROCDTCHardwareEmulated::universalBlockRead(char* address, char* returnValue, unsigned int numberOfBytes)
{
	uint16_t numberOfReads = numberOfBytes/sizeof(uint16_t);
	bool incrementAddress = true;
	std::vector<DTCLib::roc_data_t> data;
	readROCBlock(data,*((DTCLib::roc_address_t*)address),numberOfReads,incrementAddress);

	std::memcpy(&data[0],returnValue,numberOfBytes);
	__SS__;
	for(unsigned int i = 0 ; i < numberOfReads; ++i)
		ss << std::hex << std::setfill('0') << std::setw(2) << data[i] << __E__;
	__FE_COUT__ << ss.str();
	
}  // end universalBlockRead()


//==================================================================================================
void ROCDTCHardwareEmulated::GetStatus() { __SS__ << "TODO"; __SS_THROW__; }

//==================================================================================================
void ROCDTCHardwareEmulated::GetFirmwareVersion() { __SS__ << "TODO"; __SS_THROW__; }

//==================================================================================================
int ROCDTCHardwareEmulated::readInjectedPulseTimestamp() { return this->readRegister(12); }

//==================================================================================================
void ROCDTCHardwareEmulated::writeDelay(uint16_t delay)
{
	this->writeRegister(21, delay);
	return;
}

//==================================================================================================
int ROCDTCHardwareEmulated::readDelay() { return this->readRegister(7); }

//==================================================================================================
int ROCDTCHardwareEmulated::readDTCLinkLossCounter() { return this->readRegister(8); }

//==================================================================================================
void ROCDTCHardwareEmulated::resetDTCLinkLossCounter()
{
	this->writeRegister(24, 0x1);
	return;
}

//==================================================================================================
void ROCDTCHardwareEmulated::configure(void) try
{
	if(emulatorMode_)
	{
		__FE_COUT__ << "Emulator ROC configuring..." << __E__;
		return;
	}

}  // end configure()
catch(const std::runtime_error& e)
{
	__FE_MOUT__ << "Error caught: " << e.what() << __E__;
	throw;
}
catch(...)
{
	__FE_SS__ << "Unknown error caught. Check printouts!" << __E__;
	try	{ throw; } //one more try to printout extra info
	catch(const std::exception &e)
	{
		ss << "Exception message: " << e.what();
	}
	catch(...){}
	__FE_SS_THROW__;
}  // end configure() catch

//==============================================================================
void ROCDTCHardwareEmulated::halt(void) {}

//==============================================================================
void ROCDTCHardwareEmulated::pause(void) {}

//==============================================================================
void ROCDTCHardwareEmulated::resume(void) {}

//==============================================================================
void ROCDTCHardwareEmulated::start(std::string)  // runNumber)
{
}

//==============================================================================
void ROCDTCHardwareEmulated::stop(void) {}

//==============================================================================
bool ROCDTCHardwareEmulated::running(void) { return false; }

DEFINE_OTS_INTERFACE(ROCDTCHardwareEmulated)

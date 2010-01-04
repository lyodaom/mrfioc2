
#ifndef EVR_HPP_INC
#define EVR_HPP_INC

#include "evr/util.h"

#include <epicsTypes.h>
#include <epicsTime.h>

class Pulser;
class Output;
class PreScaler;

enum OutputType {
  OutputInt=0, //! Internal
  OutputFP=1,  //! Front Panel
  OutputFPUniv=2, //! FP Universal
  OutputRB=3 //! Rear Breakout
};

enum TSSource {
  TSSourceInternal=0,
  TSSourceEvent=1,
  TSSourceDBus4=2
};

enum TSMode {
  TSModeFree,
  TSModeLatch
};

/**@brief Base interface for EVRs.
 *
 * This is the interface which the generic EVR device support
 * will use to interact with an EVR.
 *
 * Device support can use one of the functions returning IOSCANPVT
 * to impliment get_ioint_info().
 */
class EVR
{
public:

  virtual ~EVR()=0;

  //! Hardware model
  virtual epicsUInt32 model() const=0;

  //! Firmware Version
  virtual epicsUInt32 version() const=0;

  /**\defgroup ena Enable/disable pulser output.
   */
  /*@{*/
  virtual bool enabled() const=0;
  virtual void enable(bool)=0;
  /*@}*/

  //! Pulser id number is device specific
  virtual Pulser* pulser(epicsUInt32)=0;
  virtual const Pulser* pulser(epicsUInt32) const=0;

  //! Output id number is device specific
  virtual Output* output(OutputType otype,epicsUInt32 idx)=0;
  virtual const Output* output(OutputType,epicsUInt32) const=0;

  //! Prescaler id number is device specific
  virtual PreScaler* prescaler(epicsUInt32)=0;
  virtual const PreScaler* prescaler(epicsUInt32) const=0;

  /** Hook to handle general event mapping table manipulation.
   *  Allows 'special' events only (ie heartbeat, log, led, etc)
   *  Normal mappings (pulsers, outputs) must be made through the
   *  appropriate class (Pulser, Output).
   *
   * Note: this is one place where Device Support will have some depth.
   */
  virtual bool specialMapped(epicsUInt32 code, epicsUInt32 func) const=0;
  virtual void specialSetMap(epicsUInt32 code, epicsUInt32 func,bool set)=0;

  /** Return a human readable string describing 'func'.
   * 'func' is any mapping code which can be a special mapping.
   */
  virtual const char* idName(epicsUInt32 func) const=0;

  /**\defgroup pll Module reference clock
   *
   *  Conversion between frequency and clock word
   *  Should happen outside.
   */
  /*@{*/

  virtual double clock() const=0;
  virtual void clockSet(double)=0;

  //! Internal PLL Status
  virtual bool pllLocked() const=0;

  virtual epicsUInt32 uSecDiv() const=0;
  /*@}*/

  /**\defgroup ts Time Stamp
   *
   * Configuration and access to the hardware timestamp
   */
  /*@{*/
  virtual void setSourceTS(TSSource)=0;
  virtual TSSource SourceTS() const=0;

  virtual double clockTS() const=0;
  virtual void clockTSSet(double)=0;

  virtual epicsUInt32 tsDiv() const=0;

  /** Gives the current time stamp as sec+nsec
   *@param ts This pointer will be filled in with the current time
   *@param mo Which access mode to use.
   *@return true When ts was updated
   *@return false When ts could not be updated
   */
  virtual bool getTimeStamp(epicsTimeStamp *ts,TSMode mode)=0;

  /** Latch and reset the Timestamp
   *@param latch true  - Latch
   *@param latch false - Reset
   */
  virtual void tsLatch(bool latch)=0;
  /*@}*/

  /**\defgroup linksts Event Link Status
   */
  /*@{*/
  virtual bool linkStatus() const=0;
  virtual IOSCANPVT linkChanged()=0;
  virtual epicsUInt32 recvErrorCount() const=0;
  /*@}*/

  virtual epicsUInt16 dbus() const=0;

  virtual void enableHeartbeat(bool)=0;
  virtual IOSCANPVT heartbeatOccured()=0;


  /**\defgroup devhelp Device Support Helpers
   *
   * These functions exists to make life easier for device support
   */
  /*@{*/
  void setSourceTSraw(epicsUInt32 r){setSourceTS((TSSource)r);};
  epicsUInt32 SourceTSraw() const{return (TSSource)SourceTS();};
  /*@}*/

}; // class EVR

#endif // EVR_HPP_INC

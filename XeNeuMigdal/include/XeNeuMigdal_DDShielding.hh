//////////////////////////////////////////////////////
//                                                  //
//  XeNeuMigdal_DDShielding.hh                //
//                                                  //
//  This is the class definition for the actve RF   //
//  region of liquid xenon that is in the LZ        //
//  System Test (as of 3 Nov 2016).                 //
//                                                  //
//////////////////////////////////////////////////////
/*
********************************************************************************
*
*	Change log
*	
*	2016-11-7 - Initial submission (Ryan Linehan)
*
*
********************************************************************************
*/

#ifndef XeNeuMigdal_DDShielding_HH
#define XeNeuMigdal_DDShielding_HH 1

//
//	GEANT4 includes
//

#include "globals.hh"

//
//	Project includes
//
#include "BaccDetectorComponent.hh"

//
//	Class forwarding
//

class G4Material;
class G4LogicalVolume;
//class BaccDetectorComponent;
class BaccMaterials;
class XeNeuMigdalMessenger;
class XeNeuMigdalMaterials;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class XeNeuMigdal_DDShielding
{
    
public:
  
  XeNeuMigdal_DDShielding();
  ~XeNeuMigdal_DDShielding();
  
  inline G4LogicalVolume *GetLogicalVolume() { return full_shield_assembly_log; }
  
private:
  
  BaccMaterials *BACCmaterials;
  XeNeuMigdalMaterials *XeNeumaterials;
  G4LogicalVolume *full_shield_assembly_log;
};

#endif

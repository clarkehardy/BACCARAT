
/////////////////////////////////////////////////////
//                                                  //
//  XeNeuMigdal_DDShielding.cc                             //
//                                                  //
//  This is the class implementation for the XeNeu  //
//  xenon detector, for use in the LLNL xenon       //
//  recoil experiments                              //
//  Copied from the DT shielding model, with a few  //
//  modifications. 
//                                                  //
//////////////////////////////////////////////////////
/*
********************************************************************************
*
*	Change log
*	
*	2019-06-20 - Initial submission (Daniel Naim)
* 2022-10-06 - Copied from DTShielding
*
*
********************************************************************************
*/

//
//	C/C++ includes
//

#include <sstream>
#include <string>

//
//	GEANT4 includes
//

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4Ellipsoid.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

//
//      BACCARAT includes
//
#include "G4PVPlacement.hh"
#include "BaccDetectorComponent.hh"

//
//	LZSystemTest includes
//
//#include "XeNeu_XeDetectorParameters.hh"
#include "XeNeuMigdal_DDShielding.hh"
#include "G4NistManager.hh"
#include "XeNeuMigdalMaterials.hh"
#include "XeNeuMigdalMessenger.hh"
//
//		Definitions
//
using namespace std;

#define PI 3.14159265358979312


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				LZSystemTestActiveLXeRegion
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuMigdal_DDShielding::XeNeuMigdal_DDShielding()
{

  double DT_Shift_y_Position = -30* cm;
  double inch = 2.54*cm;

  //this is for the DT part -- we keep the lead shielding in place
  double DT_container_half_width_y = 38*cm;
  double  Lead_Box_Thickness = 25.4*cm; 

  // water dimensions
  double water_half_length_x = 119.38*cm;
  double water_half_width_y  = 96.52*cm;
  double water_half_height_z = 83.82*cm;
  double brick_half_length_y = 18.*inch/2;
  double brick_half_width_x  = 9.*inch/2;
  double brick_half_height_z = 6.*inch/2;
  double bpe_half_length_y   = 2.*brick_half_length_y;

  double center_bpe_collimator = -water_half_length_x + brick_half_width_x;
 

  //  double measurement_box_side = 10*cm;

  //  double Dimension_of_World = water_tank_radius + 100*cm;

  //	Get the Materials pointer
  G4NistManager * matman = G4NistManager::Instance();
  //Get the predefined materials
  XeNeuMigdalMaterials *Xmat = XeNeuMigdalMaterials::GetInstance();
  if(!Xmat) Xmat = new XeNeuMigdalMaterials();

  // Create the volume that the DT shielding will occupy 
  G4Box * full_shield_assembly_box = new G4Box("full_shield_assembly_box",water_half_length_x + inch,water_half_width_y,water_half_height_z);
  full_shield_assembly_log = new G4LogicalVolume( full_shield_assembly_box,
						  matman->FindOrBuildMaterial("G4_AIR"),
						  "full_shield_assembly_log");
  full_shield_assembly_log->SetVisAttributes( G4VisAttributes::GetInvisible() );

  //Create Water Shielding
  G4Box * water_tank_shielding = new G4Box("water_tank_shielding",water_half_length_x, water_half_width_y,water_half_height_z);
  G4LogicalVolume * Water_Tank_Shielding_log = new G4LogicalVolume(water_tank_shielding,
								   //XeNeumaterials->BoratedWater(),
								   Xmat->BoratedWater(),
								   "Water_Tank_Shielding_log");
  // Water_Tank_Shielding_log->SetVisAttributes( BACCmaterials->WaterVis() );
  Water_Tank_Shielding_log->SetVisAttributes( Xmat->GetVisAttributesByName("lblue") );

  //Create the BPE30 that the collimator will sit in
  G4Box * BPEShielding = new G4Box("BPEShielding", 
				   brick_half_width_x, 
				   bpe_half_length_y, 
				   brick_half_height_z);
  G4LogicalVolume * BPEShielding_log = new G4LogicalVolume(BPEShielding, 
							   //XeNeumaterials->BPE30(),
							   Xmat->BPE30(),
							   "BPEShielding_log");
  BPEShielding_log->SetVisAttributes(Xmat->GetVisAttributesByName("pink"));
  // BPEShielding_log->SetVisAttributes(BACCmaterials->TestRedVis());
  new G4PVPlacement(0,
		    G4ThreeVector(center_bpe_collimator, 0., -0.5*inch), //make it 0.5 inch below 0 so the collimator is at 0 in z
		    BPEShielding_log,
		    "BPEShielding_object",
		    Water_Tank_Shielding_log,
		    0,0,0);

  //comment out the BPE 5 slab -- not sure why we need this? Do we have BPE5 right beloe the center?
  /*
    G4Box * BPE_5_slab = new G4Box("BPE_5_slab_box",
    brick_half_width_x,
    bpe_half_length_y,
    0.5* inch);
    G4LogicalVolume * BPE_5_slab_log = new G4LogicalVolume(BPE_5_slab, 
    //XeNeumaterials->BPE5(), 
    Xmat->GetMaterialByName("BPE30"),
    "BPE_5_slab_log");
    //BPE_5_slab_log->SetVisAttributes( BACCmaterials->TestGreenVis() );
    BPE_5_slab_log->SetVisAttributes( Xmat->GetVisAttributesByName("pink") );
    //G4PVPlacement * BPE_5_slab_object = 
    new G4PVPlacement(0,
    G4ThreeVector( 0, 0, -0.5*inch),
    BPE_5_slab_log,
    "BPE_5_slab",
    BPEShielding_log,
    0,0,0);
  */ 

  //Create the collimator
  G4Box * Collimator = new G4Box("Collimator", brick_half_width_x, inch/2., inch/2.);
  G4LogicalVolume * Collimator_log = new G4LogicalVolume(Collimator, 
							 matman->FindOrBuildMaterial("G4_AIR"), 
							 "Collimator_log");
  Collimator_log->SetVisAttributes( Xmat->GetVisAttributesByName("green") );
  new G4PVPlacement( 0,
		     //		    G4ThreeVector(0, DT_Shift_y_Position, 0.), //this is center in z
		     G4ThreeVector(0, DT_Shift_y_Position, 0.5*inch), //this is center in z
		     Collimator_log, 
		     "Collimator_object", 
		     //		    BPE_5_slab_log,
		     BPEShielding_log,                                                                                                                                                                                                                                                                                                                   		     0,0,true );
 
  //------------------------------------------------------------------------------------------------
  // Create the box with the DD generator
  double top_al_sheet_thickness = 3./8.* inch;
  double bottom_al_sheet_thickness = 5./8.* inch;
  double ddgen_box_x_offset = center_bpe_collimator + brick_half_width_x*2.;

  G4LogicalVolume * ddgen_box_log = new G4LogicalVolume(BPEShielding, 
							matman->FindOrBuildMaterial("G4_AIR"),
							"ddgen_box_log");
  ddgen_box_log->SetVisAttributes(G4VisAttributes::GetInvisible());
  new G4PVPlacement(0,
		    G4ThreeVector(ddgen_box_x_offset, 0., -0.5*inch), //make it 0.5 inch above 0 so the collimator is at 0 in z
		    ddgen_box_log,
		    "ddgen_box",
		    Water_Tank_Shielding_log,
		    0,0,0);

  G4Box * top_al_sheet_box = new G4Box("top_al_sheet_box", brick_half_width_x, bpe_half_length_y, top_al_sheet_thickness/2.);
  G4LogicalVolume * top_al_sheet_log = new G4LogicalVolume( top_al_sheet_box,
							    //BACCmaterials->Aluminum(),
							    matman->FindOrBuildMaterial("G4_Al"),
							    "top_al_sheet_log");
  top_al_sheet_log->SetVisAttributes( Xmat->GetVisAttributesByName("lgrey") );
  new G4PVPlacement( 0, 
		     G4ThreeVector(0, 0, brick_half_height_z - top_al_sheet_thickness/2. ),
		     top_al_sheet_log,
		     "top_al_sheet",
		     //		     Water_Tank_Shielding_log,
		     ddgen_box_log,
		     0,0,0);

  G4Box * bottom_al_sheet_box = new G4Box("bottom_al_sheet_box", brick_half_width_x, bpe_half_length_y, bottom_al_sheet_thickness/2.);
  G4LogicalVolume * bottom_al_sheet_log = new G4LogicalVolume( bottom_al_sheet_box,
							       //BACCmaterials->Aluminum(),
							       matman->FindOrBuildMaterial("G4_Al"),
							       "bottom_al_sheet_log");
  bottom_al_sheet_log->SetVisAttributes( Xmat->GetVisAttributesByName("lgrey") );
  new G4PVPlacement( 0, 
		     G4ThreeVector(0, 0, -brick_half_height_z + bottom_al_sheet_thickness/2. ),
		     bottom_al_sheet_log,
		     "bottom_al_sheet",
		     //Water_Tank_Shielding_log,
		     ddgen_box_log,
		     0,0,0);

  G4Box * dd_bpe_shield_box = new G4Box("dd_bpe_shield_box", 2.*inch, bpe_half_length_y, 2.5*inch); //this is 5 inch tall, so looks good
  G4LogicalVolume * dd_bpe_shield_log = new G4LogicalVolume( dd_bpe_shield_box,
							     //XeNeumaterials->BPE5(),
							     Xmat->BPE30(),
							     "dd_bpe_shield_log");
  dd_bpe_shield_log->SetVisAttributes( Xmat->GetVisAttributesByName("pink") );
  new G4PVPlacement( 0, 
		     G4ThreeVector(-2.5*inch, 0, 0.125*inch), 
		     dd_bpe_shield_log,
		     "dd_BPE_shield",
		     //Water_Tank_Shielding_log,
		     ddgen_box_log,
		     0,0,0);
   
  G4Box * dd_short_collimator_box = new G4Box("dd_short_collimator_box", 2.*inch, 0.5*inch, inch);
  G4LogicalVolume * dd_short_collimator_log = new G4LogicalVolume( dd_short_collimator_box,
								   matman->FindOrBuildMaterial("G4_AIR"),
								   "dd_short_collimator_log");
  //  dd_short_collimator_log->SetVisAttributes( G4VisAttributes::GetInvisible() );
  dd_short_collimator_log->SetVisAttributes( Xmat->GetVisAttributesByName("green"));
  new G4PVPlacement( 0, 
		     G4ThreeVector(0, DT_Shift_y_Position, 0.5*inch),
		     dd_short_collimator_log,
		     "dd_short_collimator",
		     dd_bpe_shield_log,
		     0,0,0);

  //Create The Lead Shielding Surrounding the DT generator
  double DD_Tube_Radius = 10.16/2*cm;
  G4Box * Lead_DT_Box = new G4Box("Lead_DT_Box", 
				  DD_Tube_Radius + Lead_Box_Thickness, 
				  DD_Tube_Radius + Lead_Box_Thickness + 3.*inch, 
				  DD_Tube_Radius + Lead_Box_Thickness);
  G4LogicalVolume * Lead_Box_log = new G4LogicalVolume(Lead_DT_Box, 
						       //BACCmaterials->Lead(), 
						       matman->FindOrBuildMaterial("G4_Pb"),
						       "Lead_Box_log");
  Lead_Box_log->SetVisAttributes(Xmat->GetVisAttributesByName("purple"));
  //Finally Create the Lead Box
  //new G4PVPlacement(0, G4ThreeVector(0,0,0), Lead_Box_log, "Lead_Box_object", Water_Tank_Shielding_log,0,0,0);
  new BaccDetectorComponent(0, G4ThreeVector(0,0,0), Lead_Box_log, "Lead_Box_object", Water_Tank_Shielding_log,0,0,true);
 
  G4RotationMatrix* rm1 = new G4RotationMatrix();
  rm1->rotateX(90.*deg); 
  //Tube
  G4Tubs * DD_Tube = new G4Tubs("DD_Tube", DD_Tube_Radius-2.54/16.*cm, DD_Tube_Radius, (DT_container_half_width_y), 0,2*PI);
  G4LogicalVolume * DD_Tube_log = new G4LogicalVolume(DD_Tube,matman->FindOrBuildMaterial("G4_STAINLESS-STEEL"),"DD_Tube_log"); 
  DD_Tube_log->SetVisAttributes(Xmat->GetVisAttributesByName("grey"));
  new G4PVPlacement(rm1,
		    G4ThreeVector(1.5*inch, 0,0.5*inch),
		    DD_Tube_log, 
		    "DD_Tube", 
		    ddgen_box_log,
		    0,0,0);



  /*
  //Create the BPE5 Shielding (5% boron concentration 5 cm thickness)

  G4Tubs * BPE5_Shielding = new G4Tubs("BPE5_Shielding", (water_tank_radius - BPE_Thickness), water_tank_radius, water_tank_half_height, 0 , 2*PI);
 
  G4LogicalVolume * BPE5_Shielding_log = new G4LogicalVolume(BPE5_Shielding, XeNeumaterials->BPE5(), "BPE5_Shielding_log");
 
  G4PVPlacement * BPE5_Shielding_object = new G4PVPlacement(0,
  G4ThreeVector(0., 0.,0.),
  BPE5_Shielding_log,
  "BPE5_Shielding_object",
  Water_Tank_Shielding_log,
  0,0,0);

  */


  /*
  //Create a layer of BPE5 inside the water tank 
  double distance_of_layer_of_BPE5 = 50.*cm;

  G4Tubs * Layer_of_BPE5 = new G4Tubs("Layer_of_BPE5", distance_of_layer_of_BPE5, distance_of_layer_of_BPE5 + BPE_Thickness,water_tank_half_height, 0, 2*PI);

  G4LogicalVolume * Layer_of_BPE5_log = new G4LogicalVolume(Layer_of_BPE5, XeNeumaterials->BPE5(),"Layer_of_BPE5_log");

 
  G4PVPlacement * Layer_of_BPE5_object = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.), Layer_of_BPE5_log, "Layer_of_BPE5_object", subtract_log,0,0,0);

  */
  /*
  //Create the top and bottom BPE5 covers for the water tank
  G4Tubs * BPE5_Top_Shielding = new G4Tubs("BPE5_Top_Shielding", 0, water_tank_radius,1*cm, 0 , 2*PI);

  //G4Tubs * BPE5_Bottom_Shielding = new G4Tubs("BPE5_Bottom_Shielding", 0, water_tank_radius,BPE_Thickness/2.0, 0 , 2*PI);

  G4LogicalVolume * BPE5_Top_Shielding_log = new G4LogicalVolume( BPE5_Top_Shielding, matman->FindOrBuildMaterial("G4_AIR"), "BPE5_Top_Shielding_log");

  //G4LogicalVolume * BPE5_Bottom_Shielding_log = new G4LogicalVolume( BPE5_Bottom_Shielding, XeNeumaterials->BPE5(), "BPE5_Bottom_Shielding_log");


  BPE5_Top_Shielding_log->SetVisAttributes(XeNeumaterials->BPE5Vis());
  //BPE5_Bottom_Shielding_log->SetVisAttributes(XeNeumaterials->BPE5Vis());



  G4PVPlacement * BPE5_Top_Shielding_object = new G4PVPlacement(0,G4ThreeVector(0.,0.,water_tank_half_height + 1*cm),BPE5_Top_Shielding_log,"BPE5_Top_Shielding_object", full_shield_assembly_log,0,0,0);

  //G4PVPlacement * BPE5_Bottom_Shielding_object = new G4PVPlacement(0,G4ThreeVector(0.,0.,-(water_tank_half_height + (BPE_Thickness/2.))),BPE5_Bottom_Shielding_log,"BPE5_Bottom_Shielding_object", full_shield_assembly_log,0,0,0);
  */
  /*
  //Create thin sheet of Lead Shielding outside collimator
  double Lead_Sheet_Thickness = 1.*cm;

  G4Box * Lead_Sheet_Collimator = new G4Box("Lead_Sheet_Collimator", Lead_Sheet_Thickness/2., 10*cm, 10*cm);

  G4LogicalVolume * Lead_Sheet_Collimator_log = new G4LogicalVolume(Lead_Sheet_Collimator, BACCmaterials->Lead(), "Lead_Sheet_Collimator_log");

  Lead_Sheet_Collimator_log->SetVisAttributes(BACCmaterials->SteelVis());

  G4PVPlacement * Lead_Sheet_Collimator_object = new G4PVPlacement(0,G4ThreeVector(-(water_tank_radius + BPE_Thickness + Lead_Sheet_Thickness ),0.,0.),Lead_Sheet_Collimator_log, "Lead_Sheet_Collimator_object", full_shield_assembly_log,0,0,0);





  */ 
  //Create the volume that will measure and stop any particles/gammas entering its volume
  /*
    double Measurement_Volume_Thickness = 0.5*cm;

    G4Box * Measurement_Volume = new G4Box("Measurement_Volume",Measurement_Volume_Thickness/2., water_tank_half_height, water_tank_radius);
 
    G4LogicalVolume * Measurement_Volume_log = new G4LogicalVolume( Measurement_Volume, matman->FindOrBuildMaterial("G4_AIR"), "Measurement_Volume_log");

    Measurement_Volume_log->SetVisAttributes(BACCmaterials->WaterVis());

    G4PVPlacement * Measurement_Volume_object = new G4PVPlacement(0, 
    G4ThreeVector(-115.3*cm, 0.,0.), 
    Measurement_Volume_log, 
    "Measurement_Volume_object",
    full_shield_assembly_log,
    0,0,0);	
  */
  //Finally Create the entire WaterTank Shielding
  //  G4PVPlacement * Water_Tank_object = 
  //new G4PVPlacement(0,G4ThreeVector(0,0,0),Water_Tank_Shielding_log,"Water_Tank_object",full_shield_assembly_log,0,0,0);
  //new BaccDetectorComponent(0,G4ThreeVector(0,0,0),Water_Tank_Shielding_log,"Water_Tank_object",full_shield_assembly_log,0,0,0);
  //BaccDetectorComponent * Water_Tank_object = 
  new BaccDetectorComponent(0,G4ThreeVector(0,0,0),Water_Tank_Shielding_log,"Water_Tank_object",full_shield_assembly_log,0,0,true);

  //Lead shielding outside collimator

  G4Box * LeadOutside_Left = new G4Box("LeadOutside_Left", (2.54*0.275)/2*cm,34.4/2*cm,22.62/2*cm);

  G4LogicalVolume * LeadOutside_Left_log = new G4LogicalVolume(LeadOutside_Left, 
							       //BACCmaterials->Lead(),
							       matman->FindOrBuildMaterial("G4_Pb"),
							       "LeadOutside_Left_log");

  //LeadOutside_Left_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());
  LeadOutside_Left_log->SetVisAttributes(Xmat->GetVisAttributesByName("purple"));

  //G4PVPlacement * LeadOutside_Left_object = 
  new G4PVPlacement(0,G4ThreeVector(-water_half_length_x - inch/2.,DT_Shift_y_Position - 34.4/2*cm - inch/2.,0),LeadOutside_Left_log,"LeadOutside_Left_object",full_shield_assembly_log,0,0,0); 

  // Commented out for DT Migdal measurements, August 2022
  //G4Box * LeadOutside_Right = new G4Box("LeadOutside_Right", inch/2.,24.4/2*cm,22.62/2*cm);
  G4Box * LeadOutside_Right = new G4Box("LeadOutside_Right", (2.54*0.275)/2*cm,24.4/2*cm,22.62/2*cm);

  G4LogicalVolume * LeadOutside_Right_log = new G4LogicalVolume(LeadOutside_Right, 
								//BACCmaterials->Lead(),
								matman->FindOrBuildMaterial("G4_Pb"),
								"LeadOutside_Right_log");

  //LeadOutside_Right_log->SetVisAttributes(XeNeumaterials->BoratedWaterVis());
  LeadOutside_Right_log->SetVisAttributes(Xmat->GetVisAttributesByName("purple"));

  //G4PVPlacement * LeadOutside_Right_object = 
  new G4PVPlacement(0,G4ThreeVector(-water_half_length_x - inch/2.,DT_Shift_y_Position + 24.4/2*cm + inch/2.,0),LeadOutside_Right_log,"LeadOutside_Right_object",full_shield_assembly_log,0,0,0);
 

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~XeNeuMigdal_DDShielding
//------++++++------++++++------++++++------++++++------++++++------++++++------
XeNeuMigdal_DDShielding::~XeNeuMigdal_DDShielding(){}

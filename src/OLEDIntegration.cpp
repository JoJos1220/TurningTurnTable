/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     OLEDIntegration.cpp
  Purpose:      See OLEDIntegration.h
  Autor:        Jojo1220
  Created:      05.06.2024
  Modified:     05.06.2024
  Lizenz:       CC BY-NC-ND 4.0
  Notification: See OLEDIntegration.h
******************************************v1.0.0****************************************************/

#include "OLEDIntegration.h"

/*-----------------------------------------------------------------------------
 * Setup Global Variables
 * ----------------------------------------------------------------------------
*/
// Setup OLED Display
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

// Setup Global Menu Variable
int selectedMenu = MENUMAIN;

// Extern Defined char* Variable because of Change during Runtime to Actual Value
const char* menuSTEPaCC_items[NUM_MENUSTEPACC_ITEMS]{
    placeHolder,
    "<< BACK",
    "- - - - - - - - - -"
};

const char* menuSTEPvEL_items[NUM_MENUSTEPVEL_ITEMS]{
  placeHolder,
  "<< BACK",
  "- - - - - - - - - -" 
};

const char* menuPicDelay_items[NUM_MENUPICDEL_ITEMS]{
  placeHolder,
  "<< BACK",
  "- - - - - - - - - -" 
};

/*-----------------------------------------------------------------------------
 * OLEDSetup() Function with Logo and "Hello" call at Startup 
 * ----------------------------------------------------------------------------
*/
void OLEDSetup(void){
  /* Display Specific Sourcecode */
  u8g2.begin();
  u8g2.clearBuffer();

  u8g2.firstPage();
  do {
      u8g2.drawHLine(10,0,108);
      u8g2.drawHLine(10,63,108);
  
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(10,24,"TurnTable");
      u8g2.drawStr(10,24+12, "by");
      u8g2.drawStr(10,24+24, "Jojo1220");

  } while ( u8g2.nextPage() );

} // OLEDSetup()

/*-----------------------------------------------------------------------------
 * OLEDMainMenu() Function for Menu Navigation called with seperate Task Function
 * ----------------------------------------------------------------------------
*/
void OLEDMainMenu(int _selected, float* _parMaxSpeed, float* _parAccel){
  bool updated = false;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);

    // Set MAX Shown Menu Entrys depending on MaxMenuCount() Elements from given const list
    int i_max = 0;
    if(getMaxMenuCount() < MAX_SHOWN_MENU_Entrys){
      i_max = getMaxMenuCount();
    }else{
      i_max = MAX_SHOWN_MENU_Entrys;
    }

    // within the For LOOP the Menu Items are placed dynamically!
    // Menu Header & Design is done after this for loop!
    if((getMenuSelected() != MENUACCSETTING) && (getMenuSelected() != MENUSPEEDSETTING) && (getMenuSelected() != MENUPICDELAYSETTING)){
      for(int i = 0; i<i_max; i++){
        int menuItemIndex = (_selected +i) % getMaxMenuCount();
        u8g2.setFont(u8g2_font_ncenB10_tr);
        // If Selected menu, is current Menu -->> Draw a Frame on the Menu-Item
        if(menuItemIndex == _selected) {
          // set Font == BOLT
          u8g2.setFontMode(1);
          u8g2.drawFrame(0, 20 + 16 * i, u8g2.getDisplayWidth(), 16);
        }else{
          // set Font to NORMAL
          u8g2.setFontMode(0);
        }
        if(getMenuSelected() == MENUMAIN){
            u8g2.drawStr(5, 32 + 16 * i, menuMAIN_items[menuItemIndex]);           
        }else if(getMenuSelected() == MENUAUTOMODE){
            u8g2.drawStr(5, 32 + 16 * i, menuAutoMode_items[menuItemIndex]);   
        }else if(getMenuSelected() == MENUHANDMODE){
            u8g2.drawStr(5, 32 + 16 * i, menuHandMode_items[menuItemIndex]);                          
        }else if(getMenuSelected() == MENUSETTINGS){
            u8g2.drawStr(5, 32 + 16 * i, menuSETTINGS_items[menuItemIndex]);             
        }else if(getMenuSelected() == MENUAUTOMODESETTINGS){
            u8g2.drawStr(5, 32 + 16 * i, menuAtuoModeSettings_items[menuItemIndex]);   
        }/*else if(getMenuSelected() == MENUACCSETTING){
            Not in Menu Settings Included!
        }*//*else if(getMenuSelected() == MENUSPEEDSETTING){
            Not in Menu Settings Included!
        }*/else if(getMenuSelected() == MENUTESTSCREENSHOT){
            u8g2.drawStr(5, 32 + 16 * i, menuTestBLEScreens_items[menuItemIndex]);              
        }else if(getMenuSelected() == MENUAUTOMODESTEPPERANGLE){
            u8g2.drawStr(5, 32 + 16 * i, menuStepperAngle_items[menuItemIndex]);  
        }else if(getMenuSelected() == MENUPICTUREDELAY){

          if((strcmp(menuPicDelay_items[menuItemIndex], placeHolder) == 0)){
            // Set the Cursor, based on Menu Item Number called
            u8g2.setCursor(5, 32 + 16 * i);
            u8g2.print(systemParameters.pictureDelay,10);
            u8g2.print(" ms");
          }else{
            u8g2.drawStr(5, 32 + 16 * i, menuPicDelay_items[menuItemIndex]);                          
          }

        }else if(getMenuSelected() == MENUACCELERATION){

          if((strcmp(menuSTEPaCC_items[menuItemIndex], placeHolder) == 0)){
            // Set the Cursor, based on Menu Item Number called
            u8g2.setCursor(5, 32 + 16 * i);
            u8g2.print(systemParameters.maxStepperAccel,1);
            u8g2.print(" steps/s2");
          }else{
            u8g2.drawStr(5, 32 + 16 * i, menuSTEPaCC_items[menuItemIndex]);                          
          }

        }else if(getMenuSelected() == MENUSPEED){
          if((strcmp(menuSTEPvEL_items[menuItemIndex], placeHolder) == 0)){
            // Set the Cursor, based on Menu Item Number called
            u8g2.setCursor(5, 32 + 16 * i);
            u8g2.print(systemParameters.maxStepperSpeed,1);
            u8g2.print(" steps/s");
          }else{
            u8g2.drawStr(5, 32 + 16 * i, menuSTEPvEL_items[menuItemIndex]);                          
          }
        }
      }
    }
    // Set Display Header & Parameter Changes here:
    if(getMenuSelected() == MENUMAIN){
      u8g2.drawStr(5, 12, menuMAIN_Header);           
    }else if(getMenuSelected() == MENUSETTINGS){
      u8g2.drawStr(5, 12, menuSETTINGS_Header);             
    }else if(getMenuSelected() == MENUAUTOMODE){
      u8g2.drawStr(5, 12, menuAutoMode_Header);    
    }else if(getMenuSelected() == MENUHANDMODE){
      u8g2.drawStr(5, 12, menuHandMode_Header);          
    } else if(getMenuSelected() == MENUACCSETTING){
      u8g2.drawStr(5, 12, menuAccSettings_Header);
      if(!updated){   
        systemParameters.maxStepperAccel = systemParameters.maxStepperAccel + _selected;
        if(systemParameters.maxStepperAccel < MINSTEPPERACCELERATION){
          systemParameters.maxStepperAccel = MINSTEPPERACCELERATION;
        }else if(systemParameters.maxStepperAccel > MAXSTEPPERACCELERATION){
          systemParameters.maxStepperAccel = MAXSTEPPERACCELERATION;
        }
        updated = true; // Set Parameter Update to TRUE
      }
      u8g2.setCursor(5,32);
      u8g2.print(systemParameters.maxStepperAccel,1);
      u8g2.print(" steps/s2");
    }else if(getMenuSelected() == MENUSPEEDSETTING){
      u8g2.drawStr(5, 12, menuVelSettings_Header);
      if(!updated){   
        systemParameters.maxStepperSpeed = systemParameters.maxStepperSpeed + _selected;
        if(systemParameters.maxStepperSpeed < MINSTEPPERSPEED){
          systemParameters.maxStepperSpeed = MINSTEPPERSPEED;
        }else if(systemParameters.maxStepperSpeed > MAXSTEPPERSPEED){
          systemParameters.maxStepperSpeed = MAXSTEPPERSPEED;
        }
        updated = true; // Set Parameter Update to TRUE
      }
      u8g2.setCursor(5,32);
      u8g2.print(systemParameters.maxStepperSpeed,1);
      u8g2.print(" steps/s");        
    }else if(getMenuSelected() == MENUTESTSCREENSHOT){
      u8g2.drawStr(5, 12, menuTestBLEScreens_Header);           
    }else if(getMenuSelected() == MENUSPEED){
      u8g2.drawStr(5, 12, menuSpeed_Header);    
    }else if(getMenuSelected() == MENUAUTOMODESETTINGS){
      u8g2.drawStr(5, 12, menuAutoModeSettings_Header);    
    }else if(getMenuSelected() == MENUACCELERATION){
      u8g2.drawStr(5, 12, menuAccSettings_Header); 
    }else if(getMenuSelected() == MENUAUTOMODESTEPPERANGLE){
      u8g2.drawStr(5, 12, menuStepperAngle_Header);    
    }else if(getMenuSelected() == MENUPICTUREDELAY){
      u8g2.drawStr(5,12, menuPicDelay_Header);
    }else if(getMenuSelected() == MENUPICDELAYSETTING){
      u8g2.drawStr(5, 12, menuPictureDelay_Header);
      if(!updated){   
        systemParameters.pictureDelay = systemParameters.pictureDelay + _selected;
        if(systemParameters.pictureDelay < MINPICTUREDELAY){
          systemParameters.pictureDelay = MINPICTUREDELAY;
        }else if(systemParameters.pictureDelay > MAXDPICTUREDELAY){
          systemParameters.pictureDelay = MAXDPICTUREDELAY;
        }
        updated = true; // Set Parameter Update to TRUE
      }
      u8g2.setCursor(5,32);
      u8g2.print(systemParameters.pictureDelay,10);
      u8g2.print(" ms");        
    }
    // Corner Illumination Effect:
    // Left UP
    u8g2.drawLine(0, 0, 5, 0);
    u8g2.drawLine(0, 0, 0, 5);

    // Right UP
    u8g2.drawLine(122, 0, 127, 0);
    u8g2.drawLine(127, 0, 127, 5);

    // Left DOWN
    u8g2.drawLine(0, 58, 0, 63);
    u8g2.drawLine(0, 63, 5, 63);

    // Right DOWN
    u8g2.drawLine(122, 63, 127, 63);
    u8g2.drawLine(127, 63, 127, 58);

  } while (u8g2.nextPage());

} // OLEDMainMenu()

/*-----------------------------------------------------------------------------
 * MenuSelector() Function for determining which Menu Item is selected
 * ----------------------------------------------------------------------------
*/
void MenuSelector(int _value, bool* _spacingTriggerd, long* _StepperWay, bool* _StartAutoMode, bool* _hitBLEScreenshot, long* _way, bool* _StepperRunning, int* _InfiniteMode){
    
  /* Definition if MAINMenu has changed
  */
  if((getMenuSelected() == MENUMAIN) && _value == 0){
    setMenuSelected(MENUAUTOMODE);
  }else if((getMenuSelected() == MENUMAIN)&& _value == 1){
    setMenuSelected(MENUHANDMODE);
  }else if((getMenuSelected() == MENUMAIN)&& _value == 2){
    setMenuSelected(MENUSETTINGS);
  }else if((getMenuSelected() == MENUMAIN)&& _value == 3){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;
  }
  /* Definition if SubMenu MainMenu -->> MENUAUTOMODE has changed
  */
  else if((getMenuSelected() == MENUAUTOMODE) && _value == 0){
    DEBUG_SERIAL.println("Start Auto-Prozess");
    *_InfiniteMode = 3;
    *_StartAutoMode = true;
  }else if((getMenuSelected() == MENUAUTOMODE) && _value == 1){
    DEBUG_SERIAL.println("STOPP Auto-Prozess");
    *_InfiniteMode = 3;
    *_StartAutoMode = false;
  }else if((getMenuSelected() == MENUAUTOMODE) && _value == 2){
    setMenuSelected(MENUMAIN);
  }else if((getMenuSelected() == MENUAUTOMODE) && _value == 3){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;
  }
  /* Definition if Submenu MainMenu -->> HAND-Mode has changed
  */
  else if((getMenuSelected() == MENUHANDMODE) && _value == 0){
    //DEBUG_SERIAL.println("Positive Infinite Mode started!");
    *_InfiniteMode = 1;
    *_StepperRunning = false;
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode  
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 1){
    *_way = TURNQUART;
    *_StepperRunning = false;
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 2){
    *_way = TURNSIXTE;
    *_StepperRunning = false; 
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode         
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 3){
    *_way = TURNSIXTY;
    *_StepperRunning = false;
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode          
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 4){
    *_way = -TURNSIXTY;
    *_StepperRunning = false;
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode          
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 5){
    *_way = -TURNSIXTE;
    *_StepperRunning = false;
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode         
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 6){
    *_way = -TURNQUART;
    *_StepperRunning = false; 
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode         
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 7){
    //DEBUG_SERIAL.println("Negative Infinite Mode started!");
    *_InfiniteMode = 2;
    *_StepperRunning = false; 
    *_spacingTriggerd = true;   // Quite annoying solution to avoid display Update during Hand Mode   
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 8){
    *_InfiniteMode = 3;
    setMenuSelected(MENUMAIN);
  }else if((getMenuSelected() == MENUHANDMODE) && _value == 9){
     // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;       
  }
  /* Definition if SubMenu MainMenu -->> SETTINGS has changed
  */
  else if((getMenuSelected() == MENUSETTINGS) && _value == 0){
    setMenuSelected(MENUAUTOMODESETTINGS);
  }else if((getMenuSelected() == MENUSETTINGS) && _value == 1){
    setMenuSelected(MENUACCELERATION);
  }else if((getMenuSelected() == MENUSETTINGS) && _value == 2){
    setMenuSelected(MENUSPEED);
  }else if((getMenuSelected() == MENUSETTINGS) && _value == 3){
    setMenuSelected(MENUMAIN);
  }else if((getMenuSelected() == MENUSETTINGS) && _value == 4){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;
  }
  /* Definition if SubMenu MainMenu -->> Settings -->> MENUAUTOMODESETTINGS has changed
  */
  else if((getMenuSelected() == MENUAUTOMODESETTINGS) && _value == 0){
    setMenuSelected(MENUAUTOMODESTEPPERANGLE);
  }else if((getMenuSelected() == MENUAUTOMODESETTINGS) && _value == 1){
    setMenuSelected(MENUTESTSCREENSHOT);
  }else if((getMenuSelected() == MENUAUTOMODESETTINGS) && _value == 2){
    setMenuSelected(MENUPICTUREDELAY);
  } else if((getMenuSelected() == MENUAUTOMODESETTINGS) && _value == 3){
    setMenuSelected(MENUSETTINGS);           
  }else if((getMenuSelected() == MENUAUTOMODESETTINGS) && _value == 4){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;      
  }
  /* Definition if SubMenu MainMenu -->> Settings -->> MENUACCELERATION has changed
  */
  else if((getMenuSelected() == MENUACCELERATION) && _value == 0){
    setMenuSelected(MENUACCSETTING);
  }else if((getMenuSelected() == MENUACCELERATION) && _value == 1){
    setMenuSelected(MENUSETTINGS);       
  }else if((getMenuSelected() == MENUACCELERATION) && _value == 2){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;          
  } 
  /* Definition if SubMenu MainMenu -->> Settings -->> MENUSPEED has changed
  */
  else if((getMenuSelected() == MENUSPEED) && _value == 0){
    setMenuSelected(MENUSPEEDSETTING);
  }else if((getMenuSelected() == MENUSPEED) && _value == 1){
    setMenuSelected(MENUSETTINGS);       
  }else if((getMenuSelected() == MENUSPEED) && _value == 2){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;          
  }
  /* Set Stepper Acc. if Submenu MainMenu -->> Settings -->> MENUACCELERATION -->> MENUACCSETTINGS
  */
  else if((getMenuSelected() == MENUACCSETTING)){

    preferencesBegin();
    preferences.putFloat(MAXACC_KEY, systemParameters.maxStepperAccel);
    preferences.end();

    setMenuSelected(MENUACCELERATION);
  }
  /* Set Stepper Velocity if Submenu MainMenu -->> Settings -->> MENUSPEED -->> MENUSPEEDSETTING
  */
  else if((getMenuSelected() == MENUSPEEDSETTING)){

    preferencesBegin();
    preferences.putFloat(MAXVEL_KEY, systemParameters.maxStepperSpeed);
    preferences.end();

    setMenuSelected(MENUSPEED);
  }
  /* Definition if Submenu MainMenu ->> Settings -->> MenuAutoModeSettings -->> TEST BLE Screenshot Function
  */
  else if((getMenuSelected() == MENUTESTSCREENSHOT) && _value == 0){
    *_hitBLEScreenshot = true;
  }else if((getMenuSelected() == MENUTESTSCREENSHOT) && _value == 1){
    setMenuSelected(MENUAUTOMODESETTINGS);      
  }else if((getMenuSelected() == MENUTESTSCREENSHOT) && _value == 2){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;        
  }
  /* Definition if Submenu MainMenu ->> Settings -->> MenuAutoModeSettings -->> AutoModeStepperAngle
  */
  else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 0){
    *_StepperWay = TURNFULL; // Stepper Angle Selected: 1: 360°

    preferencesBegin();
    preferences.putLong(STEPCOUNT_KEY, TURNFULL);
    preferences.end();

    DEBUG_SERIAL.println("TURNFULL");     
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 1){
    *_StepperWay = TURNHALF; // Stepper Angle Selected: 2: 180° 

    preferencesBegin();
    preferences.putLong(STEPCOUNT_KEY, TURNHALF);
    preferences.end();

    DEBUG_SERIAL.println("TURNHALF");          
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 2){
    *_StepperWay = TURNQUART; // Stepper Angle Selected: 4: 90°    

    preferencesBegin();
    preferences.putLong(STEPCOUNT_KEY, TURNQUART);
    preferences.end();

    DEBUG_SERIAL.println("TURNQUART");        
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 3){
    *_StepperWay = TURNEIGHT; // Stepper Angle Selected: 8: 45°  
        
    preferencesBegin();
    preferences.putLong(STEPCOUNT_KEY, TURNEIGHT);
    preferences.end();

    DEBUG_SERIAL.println("TURNEIGHT");         
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 4){
    *_StepperWay = TURNSIXTE; // Stepper Angle Selected: 16: 22,5°  
        
    preferencesBegin();
    preferences.putLong(STEPCOUNT_KEY, TURNSIXTE);
    preferences.end();

    DEBUG_SERIAL.println("TURNSIXTE");
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 5){
    *_StepperWay = TURNTHIRT; // Stepper Angle Selected: 32: 11,25°  

    preferencesBegin();
    preferences.putLong(STEPCOUNT_KEY, TURNTHIRT);
    preferences.end();

    DEBUG_SERIAL.println("TURNTHIRT");         
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 6){
    *_StepperWay = TURNSIXTY; // Stepper Angle Selected: 64: 5,625°

    preferencesBegin();
    preferences.putLong(STEPCOUNT_KEY, TURNSIXTY);
    preferences.end();

    DEBUG_SERIAL.println("TURNSIXTY") ;           
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 7){
    setMenuSelected(MENUAUTOMODESETTINGS);         
  }else if((getMenuSelected() == MENUAUTOMODESTEPPERANGLE) && _value == 8){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;              
  }
  /* Definition if Submenu MainMenu ->> Settings -->> MenuAutoModeSettings -->> PictureDelay
  */else if((getMenuSelected() == MENUPICTUREDELAY) && _value == 0){
    setMenuSelected(MENUPICDELAYSETTING);
  }else if((getMenuSelected() == MENUPICTUREDELAY) && _value == 1){
    setMenuSelected(MENUAUTOMODESETTINGS);      
  }else if((getMenuSelected() == MENUPICTUREDELAY) && _value == 2){
    // Do Nothing -->> It is only for spacing!
    *_spacingTriggerd = true;        
  }
  /* Set Picture Delay if Submenu MainMenu -->> Settings -->> AutoModeSettings -->> MENUPICTUREDELAY -->> MENUPICDELAYSETTING
  */
  else if((getMenuSelected() == MENUPICDELAYSETTING)){

    preferencesBegin();
    preferences.putULong(PICDELAY_KEY, systemParameters.pictureDelay);
    preferences.end();

    setMenuSelected(MENUPICTUREDELAY);
  }
} // MenuSelector()


/*-----------------------------------------------------------------------------
 * setMenuSelected() Function to set selected Item:
 * After this function call, the OLED needs to be actualized by calling Task
 * Function MenuSelector()
 * ----------------------------------------------------------------------------
*/
void setMenuSelected(int _value){
  selectedMenu = _value;

  // Refresh OLED Menu after slectedMenu has been changed
}

/*-----------------------------------------------------------------------------
 * getMenuSelected() Function give back the selected Item
 * ----------------------------------------------------------------------------
*/
int getMenuSelected(void){
  return selectedMenu;
}

/*-----------------------------------------------------------------------------
 * getMaxMenuCount() Function for getting Back MAX. Count of Menu Items selected
 * ----------------------------------------------------------------------------
*/
int getMaxMenuCount(void){
  if(selectedMenu == MENUMAIN){
    return NUM_MENUMAIN_ITEMS;
  }else if(selectedMenu == MENUAUTOMODE){
    return NUM_MENUAutoMode_ITEMS;
  }else if(selectedMenu == MENUHANDMODE){
    return NUM_MENUHandMode_ITEMS;
  }else if(selectedMenu == MENUSETTINGS){
    return NUM_MENUSETTINGS_ITEMS;
  }else if(selectedMenu == MENUACCELERATION){
    return NUM_MENUSTEPACC_ITEMS;
  }else if(selectedMenu == MENUAUTOMODESETTINGS){
    return NUM_MENUAutoModeSettings_ITEMS;
  }else if(selectedMenu == MENUACCSETTING){
    return NUM_MENUAccSettings_ITEMS;
  }else if(selectedMenu == MENUSPEEDSETTING){
    return NUM_MENUVelSettings_ITEMS;
  }else if(selectedMenu == MENUTESTSCREENSHOT){
    return NUM_MENUTestBLEScreens_ITEMS;
  }else if(selectedMenu == MENUAUTOMODESTEPPERANGLE){
    return NUM_MENUStepperAngle_ITEMS;
  }else if(selectedMenu == MENUSPEED){
    return NUM_MENUSTEPVEL_ITEMS;
  }else if(selectedMenu == MENUPICTUREDELAY){
    return NUM_MENUPICDEL_ITEMS;
  }else if(selectedMenu == MENUPICDELAYSETTING){
    return NUM_MENUPICTUREDELAY_ITEMS;
  }
  return 0;
}

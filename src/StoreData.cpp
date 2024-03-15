/*****************************************************************************
* | File      	:	StoreData.cpp
* | Function    :	Storing data on QSPI for C33 on LittleFileSystem
* | Info        : JV 2024
*----------------
* |	This version:   V2.0
* | Date        :   2024-01-16
* | Info        :   Basic version
*
* Game data store / read functions based on LittleFile system in QSPI
*
******************************************************************************/
#include "Debug.h"
#include "StoreData.h"

#include <vector>
#include <string>
#include "QSPIFlashBlockDevice.h"
#include <MBRBlockDevice.h>
#include "LittleFileSystem.h"


//File system structures
  BlockDevice* block_device = BlockDevice::get_default_instance();
  MBRBlockDevice mbr(block_device, 2);
  LittleFileSystem fs(FS_NAME);
  FILE* fp;
  DIR *dir;
// Folder and file name strings
  std::string root_folder       = std::string("/") + std::string(FS_NAME);
  std::string folder_test_name  = root_folder + std::string("/") + std::string(FOLDER_NAME);
  std::string file_test_name    = folder_test_name + std::string("/") + std::string(FILEHS); 
  std::vector<std::string> dir_list;
// Global Variable
  int err;
  struct dirent *ent;
  int dirIndex = 0;
  int numbytes;
  EEData IV_Object = {99999999,100,1,EEData_ID,1,0}; // Global Variable Object structure example
  
// Mount QSPI and read directiry to check file structure /qspi/C33/highscore.txt
int Mount_Qspi(){
        Debugln(" * Mounting QSPI FLASH...");
        err =  fs.mount(&mbr);
        if (err) {
          err = fs.reformat(&mbr);
          Debug(" *! No filesystem found, formatting error#0x");Debug(err,HEX);
        }
      if (err) {
        Debug(" *! Error mounting File system, error#0x");Debug(err,HEX);
        while(1);
        }

  // Run Through Root Directory
  if ((dir = opendir(root_folder.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      if(ent->d_type == DT_REG) ; //Debug("- [File]: ");
      else if(ent->d_type == DT_DIR) {
        //Debug("- [Fold]: ");
        if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..")) {
          dir_list.push_back(ent->d_name);
        }
      }
      //Debugln(ent->d_name);
      dirIndex++;
    }
    closedir (dir); // moved th
  } 
  else {
    // Could not open directory
    Debugln(" *! Error opening Qspi\n");
    while(1);
  }
  if(dirIndex == 0) {
    Debugln(" *! Empty Qspi");
  }

  // Check if our directory is there, if not , create it.
  bool found_test_folder = false;
  for(unsigned int i = 0; i < dir_list.size(); i++) { // find your subdirectory
    if(dir_list[i] == FOLDER_NAME) found_test_folder = true;
  }
      err = 0;
      if(!found_test_folder) {
        Debugln(" * FOLDER NOT FOUND... creating folder"); 
        err = mkdir(folder_test_name.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
        if(err != 0) {
            Debug(" *! FAILED folder creation with error#0x ");Debugln(err,HEX);
            }
      }
  return(err);
}


// DELETE FILE 
int Delete_File()
{
      if(remove(file_test_name.c_str()) == 0) {
        Debugln(" *!FILE HAS BEEN DELETED!");
        return(1);
      }
    else return(0);
}

int Check_Filesize()
{
    int size; 
    fp = fopen(file_test_name.c_str(), "r");
    if(fp != NULL) {
      fseek(fp, 0L, SEEK_END);
      size = ftell(fp);
      fseek(fp, 0L, SEEK_SET);  
      return size;
    }
    else return(0);
}

/* Read Data and verify ID */
uint8_t Read_Data(struct EEData *O)
{
  fp = fopen(file_test_name.c_str(), "r");
  if(fp != NULL) {
    uint8_t* ptr = (uint8_t*)O;       // make pointer to structure t
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);  
    Debug(" * Found file :size ");Debugln(size);
    fread(ptr, 1 , sizeof(EEData), fp);     fclose(fp); 
    Debug(" * Data read from File system, with ID 0x");Debugln(O->identity,HEX);
    return(1);
  } // check file        
  else { 
    Debugln(" * File Read Failed : No File system");
    fclose(fp); 
    return(0);
    }         
}

/* Print debug information of Credentaisl structure,*/
uint8_t Debug_Data(struct EEData *O)
{
     Debug(" * Data:[");Debug(O->highlevel);Debug(",");Debug(O->highscore);
     Debug("],[");Debug(O->time); Debug("], id=");Debug(O->identity);Debug(", count=");Debug(O->counter);;Debug(", stop=");Debugln(O->stop);
     return(1);
}

// verify ID, verify FFS, Safe data, increase counter*/
uint8_t Save_Data(struct EEData *O)
{
  uint8_t b;
  Delete_File();
  fp = fopen(file_test_name.c_str(), "w");
  if(fp != NULL) {
     O->counter++;
     uint8_t* ptr = (uint8_t*) O;       // make pointer to structure O
     fwrite(ptr, 1, sizeof(EEData), fp);  fclose(fp); 
     Debug(" * Saved Object to File system with ID ");Debug(O->identity);Debug(", size ");Debug(sizeof(EEData));Debug(", filesize ");Debugln(Check_Filesize());
     return(1);
  }
  else Debugln(" *! File not open for writing");
 }


/************ Callable functions from Game  *********************/


 void Read_Highscore(int* HS, int * HL, long *T, int *K)
 {
 Mount_Qspi();
 Read_Data(&IV_Object);
 *HS=IV_Object.highscore;*HL=IV_Object.highlevel;*T=IV_Object.time;*K=IV_Object.counter;
 }

  void Write_Highscore(int HS, int HL, long T, int K)
 {
 Mount_Qspi();
 IV_Object.highscore=HS;IV_Object.highlevel=HL;IV_Object.time=T;IV_Object.counter=K;
 Save_Data(&IV_Object);
 }
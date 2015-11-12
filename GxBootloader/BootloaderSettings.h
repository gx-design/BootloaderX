/******************************************************************************
*       Description: 
*
*       Author: 
*         Date: 12 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _BOOTLOADERSETTINGS_H_
#define _BOOTLOADERSETTINGS_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

enum class SettingId
{
    BootloaderVersion,
    FirmwareVersion,    
    NumSettings
};

#endif

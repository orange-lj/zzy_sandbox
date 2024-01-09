#include "struct.h"
#include "Version.h"
ULONG Driver_OsVersion=0;
ULONG Driver_OsBuild=0;
BOOL GetVersion()
{
    ULONG MajorVersion, MinorVersion;
    CHAR  str[64];
    const ULONG MajorVersionMin = 6;
    const ULONG MinorVersionMin = 1;
    PsGetVersion(&MajorVersion, &MinorVersion, &Driver_OsBuild, NULL);
    if (MajorVersion > MajorVersionMin ||
        (MajorVersion == MajorVersionMin
            && MinorVersion >= MinorVersionMin))
    {
        if (MajorVersion == 10) 
        { // for windows 11 its still 10
            Driver_OsVersion = DRIVER_WINDOWS_10;
        }
        else if (MajorVersion == 6) 
        {
            if (MinorVersion == 3 && Driver_OsBuild >= 9600)
            {
                Driver_OsVersion = DRIVER_WINDOWS_8;
            }
            else if (MinorVersion == 1 && Driver_OsBuild >= 7600)
            {
                Driver_OsVersion = DRIVER_WINDOWS_7;
            }
        }
        if (Driver_OsVersion)
            return TRUE;
    }
    return FALSE;
}

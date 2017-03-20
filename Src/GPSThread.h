#ifndef __GPS_H__
#define __GPS_H__

class gps_fix;

void initGPS();
void vGPSTask(void *pvParameters);

gps_fix getGPSFixData();

#endif //__GPS_H__

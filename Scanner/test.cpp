/*
Bool PUMA_Init(uint16_t wHeightCode, void* hStorage);
Bool PUMA_Done();
uint32_t PUMA_GetReturnCode();
char * PUMA_GetReturnString(uint32_t dwError);
Bool PUMA_GetExportData(uint32_t dwType, void * pData);
Bool PUMA_SetImportData(uint32_t dwType, void * pData);

Bool PUMA_XOpen(void * DIB_image, const char *identifier);
Bool PUMA_XClose(void);
Bool PUMA_XFinalRecognition(void);
Bool PUMA_XSave(const char * lpOutFileName, int32_t lnFormat, int32_t lnCode );
*/

#include <cuneiform.h>
#include <iostream>

int main()
{
    Bool res = PUMA_Init(0, NULL);
    PUMA_XOpen(0, "test.bmp");
    PUMA_XFinalRecognition();
    std::cout << PUMA_GetReturnString(0) << std::endl;
}

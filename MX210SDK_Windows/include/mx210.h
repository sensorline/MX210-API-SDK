#ifndef MX210_H
#define MX210_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#ifdef MX210_EXPORTS
#define MX210_API __declspec(dllexport)
#else
#define MX210_API __declspec(dllimport)
#endif
#else
#define MX210_API
#endif

typedef struct MX210TcpIp MX210TcpIp_t;

enum MX210_CONSTANTS
{
    //port numbers
    MX210_PORT_8000 = 8000,
    MX210_PORT_8001 = 8001,
    MX210_PORT_8002 = 8002,
    MX210_PORT_8003 = 8003,
    MX210_PORT_8004 = 8004,
    MX210_PORT_8005 = 8005,
    
    //NULL definition
	MX210_NULL = 0,
	
	//channel numbers
    MX210_EMPTY = 0,
    MX210_CH1 = 1,
    MX210_CH2 = 2,
    MX210_CH3 = 3,
    MX210_CH4 = 4,
    MX210_ALL = 5,

    //ENA command
    MX210_ENA_OFF = 0,
    MX210_ENA_ON = 1,

    //DIG command
    MX210_DIG_TRIGGER = 0,
    MX210_DIG_DT = 1,
    MX210_DIG_DIR = 2,
    MX210_DIG_REDUNDANT = 3,

    //AHL command
    MX210_AHL_ACTIVELOW = 0,
    MX210_AHL_ACTIVEHIGH = 1,

    //ANA command
    MX210_ANA_05V = 0,
    MX210_ANA_15V = 1,
    MX210_ANA_05V_SIGNED = 2,
    MX210_ANA_05V_RAW = 3,
    MX210_ANA_LH_TRIGGER = 4,

	//STR command
    MX210_STR_TRIGGER = 0,
    MX210_STR_DT = 1,
    MX210_STR_TRG_CHANGE = 2,
    MX210_STR_DT_CHANGE = 3,
    MX210_STR_DIR = 4,

    //FLT command
	MX210_FLT_OFF = 0,
    MX210_FLT_1ms = 1,
    MX210_FLT_2ms = 2,
    MX210_FLT_4ms = 3,
    MX210_FLT_8ms = 4,
    MX210_FLT_16ms = 5,
    MX210_FLT_32ms = 6,
    MX210_FLT_64ms = 7,

    //ARST command
	MX210_ARST_OFF = 0,
    MX210_ARST_1MIN = 1,
    MX210_ARST_3MIN = 2,
    MX210_ARST_15MIN = 3,
    MX210_ARST_AFTER1MIN = 4,

	//ACAL command
	MX210_ACAL_OFF = 0,
	MX210_ACAL_ON = 1,
	
	//AMP command
	MX210_AMP_AUTO = 0,
    MX210_AMP_AMP1 = 1, 
	MX210_AMP_AMP2 = 2,
	MX210_AMP_AMP3 = 3,
	MX210_AMP_AMP4 = 4,

    //DRV command
	MX210_DRV_PULSE = 0,
    MX210_DRV_DC = 1,

    //DGC command   
	MX210_DGC_OFF = 0,
	MX210_DGC_ON = 1,
	
    //USB command
	MX210_USB_OFF = 0,
    MX210_USB_ANALOG = 1,
    MX210_USB_TRIGGER = 2,
    MX210_USB_DT = 3,
    MX210_USB_DIR = 4,

    //BWR commands
	MX210_BWR_OFF = 0,
	MX210_BWR_2 = 1,
    MX210_BWR_4 = 2,
	MX210_BWR_8 = 3,
	MX210_BWR_16 = 4,
	MX210_BWR_32 = 5
        
};

/*************** 
TCP/IP Functions
****************/

MX210_API MX210TcpIp_t* MX210TcpIP_init(
    const char* ip);

MX210_API int MX210TcpIP_setTimeout(
    MX210TcpIp_t* device,
    int timeOut);

MX210_API int MX210TcpIP_connect(
    MX210TcpIp_t* device,
    int port);

MX210_API int MX210TcpIP_disconnect(
    MX210TcpIp_t* device,
    int port);

MX210_API void MX210TcpIP_deinit(
    MX210TcpIp_t* device);

MX210_API int MX210TcpIP_send(
    MX210TcpIp_t* device,
    int port,
    const void* data,
    int size);

MX210_API int MX210TcpIP_receive(
    MX210TcpIp_t* device,
    int port);
	
MX210_API int MX210TcpIP_available(
    MX210TcpIp_t* device,
    int port);

MX210_API int MX210TcpIP_isConnected(
    MX210TcpIp_t* device,
    int port);

MX210_API int MX210TcpIP_readCircularBuffer(
    MX210TcpIp_t* device,
    int port,
    char* buffer,
    int bufferSize);

MX210_API int MX210TcpIP_readLine(
    char* buffer,
    int bufferSize,
    char* line,
    int lineSize);

MX210_API int MX210TcpIP_sendCommand(
    MX210TcpIp_t* device,
    const char* command,
    int channelNr,
    int value,
    char* answerBuffer,
    int answerBufferSize);

MX210_API int MX210TcpIP_getAnswer(
    MX210TcpIp_t* device,
    char* answerBuffer,
    int answerBufferSize);

MX210_API int MX210TcpIP_clearReceiveBuffer(
	MX210TcpIp_t* device,
	int port);

MX210_API int MX210TcpIP_readReceivedData(
    MX210TcpIp_t* device,
    int port,
    char* buffer,
    int bufferSize);

/*************** 
Serial Functions
****************/

typedef void* MX210SerialPort_t;

typedef struct {
    char** availableports;
    size_t count;
}MX210SerialPortsList;

/// <summary>
/// Establish Serial port
/// </summary>
/// <param name=""></param>
/// <returns></returns>
MX210_API MX210SerialPort_t MX210SerialPort_Create(void);

/// <summary>
/// Serail Port Destroy
/// </summary>
/// <param name="handle"></param>
/// <returns></returns>
MX210_API void MX210SerialPort_Destroy(MX210SerialPort_t handle);

/// <summary>
/// Find available serial ports
/// </summary>
/// <param name="buffer"></param>
/// <param name="bufferSize"></param>
/// <returns></returns>
MX210_API MX210SerialPortsList MX210SerialPort_GetPorts(void);

/// <summary>
/// Free storage allocated to retieve serial ports to avoid leak
/// </summary>
MX210_API void MX210SerialPort_FreePorts(MX210SerialPortsList* availablelist);

/// <summary>
/// Open serial port
/// </summary>
/// <param name="h"></param>
/// <param name="port"></param>
/// <param name="baudrate"></param>
/// <returns></returns>
MX210_API int MX210SerialPort_Open(MX210SerialPort_t h, const char* port, uint32_t baudrate);

/// <summary>
/// Send serial data
/// </summary>
/// <param name="h"></param>
/// <param name="data"></param>
/// <param name="len"></param>
/// <returns></returns>
MX210_API int MX210SerialPort_Write(MX210SerialPort_t h, const char* data, size_t len);

/// <summary>
/// Receive serial data
/// </summary>
/// <param name="h"></param>
/// <param name="buf"></param>
/// <param name="size"></param>
/// <param name="timeout"></param>
/// <returns></returns>
MX210_API size_t MX210SerialPort_Read(MX210SerialPort_t h, char* buf, size_t size, int timeout);

/// <summary>
/// Close serial port
/// </summary>
/// <param name="h"></param>
/// <returns></returns>
MX210_API void MX210SerialPort_Close(MX210SerialPort_t h);

/// <summary>
/// Check connection
/// </summary>
/// <param name="h"></param>
/// <returns></returns>
MX210_API int MX210SerialPort_IsOpen(MX210SerialPort_t h);

#ifdef __cplusplus
}
#endif

#endif
/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: serialport.cpp
*Purpose: Class for handling and using serialport bound to bluetooth.
*
*
*Code produced in part from qt example, which can be found. 
*  https://qt.gitorious.org/qt/qtserialport/source/c82120dd86881e964bf5104b608932d9e2ec7da4:examples/serialport/creaderasync/serialportreader.cpp
*
********************************************************/
#include "serialport.h"

#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <QChar>

QT_USE_NAMESPACE

/*
 *	Constructor of SerialPort. Connection of relevant slots.
 */

SerialPort::SerialPort(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , port(serialPort)
    , m_standardOutput(stdout)
{
    connect(port, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(port, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
}

SerialPort::~SerialPort()
{
}

/* Write exactly 27 bytes of data to port. Our char arrays are not nullterminated. Will cause issue if not for 27 bytes max.
 * Wait for a maximum och 0.6 seconds for successful transmission.
 */


void SerialPort::sendArray(const char inArray[27]){
    port->flush();
    port->write(inArray,27);
    port->waitForBytesWritten(600);
}


/*handleReadyRead(). tempData is the data from potential previous transmission. if the current transmission added with the previous
 * transmission has a length of less than 27 it is saved in tempData and we leave the readyread to accept more data.
 * On 27 or more bytes 27 bytes of data are sent to handleData, excess is stored in tempData for future transmissiondata.
 * This function is not fully tested yet.
 */

void SerialPort::handleReadyRead() {
    
    QByteArray inData;
    
    int inBytes = port->bytesAvailable();
    int tempBytes = tempData.length();
    
    int checkBytes = inBytes + tempBytes;
    
    if(checkBytes == 27){
        inData = port->readAll();
        tempData.append(inData);
        handleData(tempData);
        inData.clear();
        tempData.clear();
        return;
    }
    if(checkBytes < 27){                //too little data to handle, save data and leave rdyread.
        inData = port->readAll();
        tempData.append(inData);
        inData.clear();
        return;
    }
    if(checkBytes >27){
        inData = port->read(27-tempBytes); //Read bytes to fill to exact 27 bytes
        tempData.append(inData);           //put data together.
        handleData(tempData);               // handle the data and clear the variables.
        tempData.clear();
        inData.clear();
        return;
    }
}


/*Function for conversion and deepcopy from QByteArray to char array.
 */

char* SerialPort::QByteToArray(QByteArray inArray)
{
    int size = inArray.size();
    char *outData = new char[size];
    memcpy(outData, inArray.data(), size);
    return outData;
}

/*Pointer to gui neccessary for handleData.
 */

void SerialPort::setGui(Gui* ptr){
    GUI = ptr;
}

/*Errorhandling of serialport. Currently only informs. Improvement would be to have it reconnect.
 */

void SerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        //QCoreApplication::exit(1);
    }
}

/*Passing and preformatting data that arrives from serialport and handleReadyRead.
 */

void SerialPort::handleData(QByteArray inData){
    if(inData[1] == 'M'){
        int row = (int)QByteToArray(inData)[2];
        std::cout << row << std::endl;
        memcpy(GUI->mapArea[row],QByteToArray(inData),27);
        if(row == 31){
            GUI->updateMap();
        }
        return;
    }
    if(inData[1] == 't'){
        int pos = inData[2];
        int size = inData[3];
        int nrpos = size/20;

        if (pos==0){
            memcpy(GUI->pathArrayOne,QByteToArray(inData),27);
        }
        if (pos==1){
            memcpy(GUI->pathArrayTwo,QByteToArray(inData),27);
        }
        if (pos==2){
            memcpy(GUI->pathArrayThree,QByteToArray(inData),27);
        }
        if (pos==3){
            memcpy(GUI->pathArrayFour,QByteToArray(inData),27);
        }
        if(pos==nrpos){
            GUI->mergePathArray();
            GUI->insertPathToMap();
            GUI->updateMap();
        }
        return;
    }
    if(inData[1] == 'S'){

        int sen0 = 100*inData[3]+10*inData[4]+inData[5];
        int sen1 = 100*inData[6]+10*inData[7]+inData[8];
        int sen2 = 100*inData[9]+10*inData[10]+inData[11];
        int sen3 = 100*inData[12]+10*inData[13]+inData[14];
        int sen4 = 100*inData[15]+10*inData[16]+inData[17];
        int sen5 = 100*inData[18]+10*inData[19]+inData[20];
        int sen6 = 100*inData[21]+10*inData[22]+inData[23];
        int sen7 = 100*inData[24]+10*inData[25]+inData[26];

        GUI->updateSensorValues(sen0,sen1,sen2,sen3,sen4,sen5,sen6,sen7);

        //std::cout << sen0 << '\n' << sen1 << '\n' << sen2 << '\n' << sen3 << '\n' << sen4 << '\n' << sen5 << '\n' << sen6 << '\n' << sen7 << std::endl;
    }

}

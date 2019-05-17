#include "trtparser.h"
#include <QString>
#include <QFile>
#include <iostream>
#include <QVector>
#include "rangecolorsetdialog.h"

using namespace std;

float MIN_DATA;
float MAX_DATA;

float* cube = nullptr;

int X_LENGTH;
int Y_LENGTH;
int Z_LENGTH;

int X_SIZE;
int Y_SIZE;
int Z_SIZE;

float DEFAULT_FLOAT_VALUE;

void break_v_default(bool res)
{
    if(res)
    {
        return;
    }
}

template<typename T>
inline T getAverage(T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8)
{
    break_v_default(v1 == DEFAULT_FLOAT_VALUE);
    break_v_default(v2 == DEFAULT_FLOAT_VALUE);
    break_v_default(v3 == DEFAULT_FLOAT_VALUE);
    break_v_default(v4 == DEFAULT_FLOAT_VALUE);
    break_v_default(v5 == DEFAULT_FLOAT_VALUE);
    break_v_default(v6 == DEFAULT_FLOAT_VALUE);
    break_v_default(v7 == DEFAULT_FLOAT_VALUE);
    break_v_default(v8 == DEFAULT_FLOAT_VALUE);

    size_t count  = 0;
    T average = 0.0f;
    if(v1 != DEFAULT_FLOAT_VALUE)
    {
        average += v1;
        ++count;
    }

    if(v2 != DEFAULT_FLOAT_VALUE)
    {
        average += v2;
        ++count;
    }

    if(v3 != DEFAULT_FLOAT_VALUE)
    {
        average += v3;
        ++count;
    }

    if(v4 != DEFAULT_FLOAT_VALUE)
    {
        average += v4;
        ++count;
    }

    if(v5 != DEFAULT_FLOAT_VALUE)
    {
        average += v5;
        ++count;
    }

    if(v6 != DEFAULT_FLOAT_VALUE)
    {
        average += v6;
        ++count;
    }

    if(v7 != DEFAULT_FLOAT_VALUE)
    {
        average += v7;
        ++count;
    }

    if(v8 != DEFAULT_FLOAT_VALUE)
    {
        average += v8;
        ++count;
    }

    if(count == 0)
    {
        return 0.0;
    }

    return average/count;
}

template<typename T>
inline T getAverage(T v1, T v2, T v3, T v4, T v5, T v6)
{
    break_v_default(v1 == DEFAULT_FLOAT_VALUE);
    break_v_default(v2 == DEFAULT_FLOAT_VALUE);
    break_v_default(v3 == DEFAULT_FLOAT_VALUE);
    break_v_default(v4 == DEFAULT_FLOAT_VALUE);
    break_v_default(v5 == DEFAULT_FLOAT_VALUE);
    break_v_default(v6 == DEFAULT_FLOAT_VALUE);

    size_t count  = 0;
    T average = 0.0f;
    if(v1 != DEFAULT_FLOAT_VALUE  )
    {
        average += v1;
        ++count;
    }

    if(v2 != DEFAULT_FLOAT_VALUE )
    {
        average += v2;
        ++count;
    }

    if(v3 != DEFAULT_FLOAT_VALUE )
    {
        average += v3;
        ++count;
    }

    if(v4 != DEFAULT_FLOAT_VALUE )
    {
        average += v4;
        ++count;
    }

    if(v5 != DEFAULT_FLOAT_VALUE )
    {
        average += v5;
        ++count;
    }

    if(v6 != DEFAULT_FLOAT_VALUE )
    {
        average += v6;
        ++count;
    }

    if(count == 0)
    {
        return 0.0;
    }

    return average/count;
}

template<typename T>
inline T getAverage(T v1, T v2, T v3, T v4)
{
    break_v_default(v1 == DEFAULT_FLOAT_VALUE);
    break_v_default(v2 == DEFAULT_FLOAT_VALUE);
    break_v_default(v3 == DEFAULT_FLOAT_VALUE);
    break_v_default(v4 == DEFAULT_FLOAT_VALUE);

    size_t count  = 0;
    T average = 0.0f;
    if(v1 != DEFAULT_FLOAT_VALUE )
    {
        average += v1;
        ++count;
    }

    if(v2 != DEFAULT_FLOAT_VALUE )
    {
        average += v2;
        ++count;
    }

    if(v3 != DEFAULT_FLOAT_VALUE )
    {
        average += v3;
        ++count;
    }
    else
    {
        cout << "Error";
    }
    if(v4 != DEFAULT_FLOAT_VALUE )
    {
        average += v4;
        ++count;
    }

    if(count == 0)
    {
        return 0.0;
    }

    return average/count;
}

template<typename T>
inline T getAverage(T v1, T v2 )
{
    break_v_default(v1 == DEFAULT_FLOAT_VALUE);
    break_v_default(v2 == DEFAULT_FLOAT_VALUE);

    size_t count  = 0;
    T average = 0.0f;
    if(v1 != DEFAULT_FLOAT_VALUE )
    {
        average += v1;
        ++count;
    }

    if(v2 != DEFAULT_FLOAT_VALUE )
    {
        average += v2;
        ++count;
    }

    if(count == 0)
    {
        return 0.0;
    }

    return average/count;
}

float& arrayByIndex(int x, int y, int z)
{
    return *(cube + x*Y_SIZE*Z_SIZE + y*Z_SIZE + z);
}

float& arrayByIndexY(int y, int x, int z)
{
    return *(cube + y*X_SIZE*Z_SIZE + x*Z_SIZE + z);
}

void calcCubeDepth(unsigned int rate = 2)   // a point cloud dense algrithm
{
    for(size_t x = 0; x < X_SIZE; x += rate/2 )
    {
        if(x%rate == rate/2)
        {
            for(size_t y = 0; y < Y_SIZE; y  += rate/2 )
            {
                if(y%rate == rate/2)
                {
                    for(size_t z = 0; z < Z_SIZE; z += rate/2)
                    {
                        if(z%rate == rate/2)
                        {
                            arrayByIndex(x,y,z) = getAverage( arrayByIndex(x-rate/2, y-rate/2, z-rate/2), arrayByIndex(x+rate/2,y+rate/2, z+rate/2),     \
                                    arrayByIndex(x+rate/2,y-rate/2,z-rate/2),  arrayByIndex(x-rate/2,y+rate/2,z-rate/2),  arrayByIndex(x-rate/2,y-rate/2,z+rate/2),    \
                                    arrayByIndex(x-rate/2,y+rate/2,z+rate/2),  arrayByIndex(x+rate/2,y-rate/2,z+rate/2),  arrayByIndex(x+rate/2,y+rate/2,z-rate/2) );
                        }
                    }
                }
            }
        }
    }

    for(size_t x = 0; x < X_SIZE; x += rate/2 )
    {
        if(x%rate == rate/2)
        {
            for(size_t y = 0; y <Y_SIZE; y += rate/2 )
            {
                if(y%rate == rate/2)
                {
                    for(size_t z = 0; z <Z_SIZE; z += rate/2)
                    {
                        if( z%rate == 0 )
                        {
                            if(z != 0 && z != Z_SIZE - 1 )
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x,y,z-rate/2),  arrayByIndex(x,y,z+rate/2),     \
                                    arrayByIndex(x+rate/2,y+rate/2,z),  arrayByIndex(x+rate/2,y-rate/2,z),  \
                                    arrayByIndex(x-rate/2,y+rate/2,z),  arrayByIndex(x-rate/2,y+rate/2,z));
                            }
                            else
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x+rate/2,y+rate/2,z),  arrayByIndex(x+rate/2,y-rate/2,z),  \
                                    arrayByIndex(x-rate/2,y+rate/2,z),  arrayByIndex(x-rate/2,y+rate/2,z));
                            }
                        }
                    }
                }
            }
        }
    }

    for(size_t x = 0; x <X_SIZE; x += rate/2 )
    {
        if(x%rate == rate/2)
        {
            for(size_t z = 0; z <Z_SIZE; z += rate/2)
            {
                if(z%rate == rate/2)
                {
                    for(size_t y = 0; y <Y_SIZE; y  += rate/2)
                    {
                        if( y%rate == 0 )
                        {
                            if(y != 0 && y != Y_SIZE-1)
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x,y+rate/2,z), arrayByIndex(x,y-rate/2,z),    \
                                    arrayByIndex(x+rate/2,y,z+rate/2),  arrayByIndex(x+rate/2,y,z-rate/2),  \
                                    arrayByIndex(x-rate/2,y,z+rate/2), arrayByIndex(x-rate/2,y,z-rate/2));
                            }
                            else
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x+rate/2,y,z+rate/2),  arrayByIndex(x+rate/2,y,z-rate/2),  \
                                    arrayByIndex(x-rate/2,y,z+rate/2), arrayByIndex(x-rate/2,y,z-rate/2));
                            }
                        }
                    }
                }


            }
        }
    }

    for(size_t z = 0; z <Z_SIZE; z += rate/2 )
    {
        if(z%rate == rate/2)
        {
            for(size_t y = 0; y <Y_SIZE; y += rate/2 )
            {
                if(y%rate == rate/2)
                {
                    for(size_t x = 0; x <X_SIZE; x += rate/2)
                    {
                        if( x%rate== 0 )
                        {
                            if( x != 0 && x != X_SIZE-1)
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x+rate/2,y,z), arrayByIndex(x-rate/2,y,z),    \
                                        arrayByIndex(x,y+rate/2,z+rate/2), arrayByIndex(x,y+rate/2,z-rate/2), \
                                        arrayByIndex(x,y-rate/2,z+rate/2), arrayByIndex(x,y-rate/2,z-rate/2));
                            }
                            else
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x,y+rate/2,z+rate/2), arrayByIndex(x,y+rate/2,z-rate/2), \
                                        arrayByIndex(x,y-rate/2,z+rate/2), arrayByIndex(x,y-rate/2,z-rate/2));
                            }
                        }
                    }
                }
            }
        }
    }

    for(size_t x = 0; x <X_SIZE; x += rate/2 )
    {
        if(x%rate == 0)
        {
            for(size_t z = 0; z <Z_SIZE; z += rate/2)
            {
                if( z%rate == 0 )
                {
                    for(size_t y = 0; y <Y_SIZE; y  += rate/2)
                    {
                        if( y%rate == rate/2 )
                        {
                            if( x!=0 && x != X_SIZE-1 && z!=0 && z != Z_SIZE-1)
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x,y+rate/2,z), arrayByIndex(x,y-rate/2,z),   \
                                    arrayByIndex(x-rate/2,y,z), arrayByIndex(x+rate/2,y,z), \
                                    arrayByIndex(x,y,z+rate/2), arrayByIndex(x,y,z-rate/2));
                            }
                            else
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x,y+rate/2,z), arrayByIndex(x,y-rate/2,z));
                            }
                        }
                    }
                }
            }
        }
    }

    for(size_t x = 0; x <X_SIZE; x += rate/2 )
    {
        if(x%rate == 0)
        {
            for(size_t y = 0; y <Y_SIZE; y += rate/2 )
            {
                if( y%rate == 0 )
                {
                    for(size_t z = 0; z <Z_SIZE; z += rate/2)
                    {
                        if( z%rate == rate/2 )
                        {
                            if( x!=0 && x != X_SIZE-1 && y!=0 && y != Y_SIZE-1 )
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x,y,z+rate/2), arrayByIndex(x,y,z-rate/2),    \
                                    arrayByIndex(x-rate/2,y,z), arrayByIndex(x+rate/2,y,z),   \
                                    arrayByIndex(x,y-rate/2,z), arrayByIndex(x,y+rate/2,z));
                            }
                            else
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x,y,z+rate/2), arrayByIndex(x,y,z-rate/2));
                            }
                        }
                    }
                }
            }
        }
    }

    for(size_t z = 0; z <Z_SIZE; z += rate/2 )
    {
        if(z%rate == 0)
        {
            for(size_t y = 0; y <Y_SIZE; y += rate/2 )
            {
                if( y%rate == 0 )
                {
                    for(size_t x = 0; x <X_SIZE; x += rate/2)
                    {
                        if( x%rate == rate/2 )
                        {
                            if( y!=0 && y != Y_SIZE-1  && z!=0 && z != Z_SIZE-1 )
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x-rate/2,y,z), arrayByIndex(x+rate/2,y,z),     \
                                        arrayByIndex(x,y,z-rate/2), arrayByIndex(x,y,z+rate/2), \
                                        arrayByIndex(x,y+rate/2,z), arrayByIndex(x,y-rate/2,z));
                            }
                            else
                            {
                                arrayByIndex(x,y,z) = getAverage(arrayByIndex(x-rate/2,y,z), arrayByIndex(x+rate/2,y,z));
                            }
                        }
                    }
                }
            }
        }
    }
}

QString readTrtLine(QFile& strFile)
{
    QString str;
    char buf;
    while( 1==strFile.read( &buf, 1) )
    {
        str.append(buf);
        if( buf ==  '\r')
        {
            return str.simplified();
        }
    }

    return str.simplified();
}

bool readTrtFile(const QString& fileName, float* data, int xLength, int yLength, int zLength, float& min, float& max)
{
    memset(&DEFAULT_FLOAT_VALUE, 1, sizeof(DEFAULT_FLOAT_VALUE));

    QFile srtFile(fileName);
    if( false == srtFile.open(QIODevice::OpenModeFlag::ReadOnly) )
    {
        std::cout << __FILE__ << __LINE__  << "Error: openfile failed! srtFile = "<< fileName.toStdString() << std::endl;
        return false;
    }

    QString headLine = readTrtLine(srtFile);
    QStringList headList = headLine.split(" ");
    if(headList.size() < 3)
    {
        std::cout << __FILE__ << __LINE__  << "Error: head lien error! headLine = "<< headLine.toStdString() << std::endl;
        return false;
    }

    X_LENGTH = headList[0].toInt();
    Y_LENGTH = headList[1].toInt();
    Z_LENGTH = headList[2].toInt();

    xLength = X_LENGTH;
    yLength = Y_LENGTH;
    zLength = Z_LENGTH;

    X_SIZE = (X_LENGTH-1)*DENSITY_MULTIPLE+1;
    Y_SIZE = (Y_LENGTH-1)*DENSITY_MULTIPLE+1;
    Z_SIZE = (Z_LENGTH-1)*DENSITY_MULTIPLE+1;

    if(cube)
    {
        delete[] cube;
    }

    cube = new float[((X_SIZE)*(Y_SIZE)*(Z_SIZE))];

    for(int i = 0; i < X_LENGTH*Y_LENGTH*Z_LENGTH; ++i)
    {
        QString line = readTrtLine(srtFile);
        QStringList lineList = line.split(" ");
        if(lineList.size() < 4)
        {
            return false;
        }

        float value = lineList[3].toFloat();
        if(i == 0)
        {
            min = value;
            max = value;
        }
        if(min > value)
        {
            min = value;
        }
        else if(max < value)
        {
            max = value;
        }

        arrayByIndex(static_cast<uint32_t>(lineList[0].toFloat()+20)*DENSITY_MULTIPLE/2,static_cast<uint32_t>(lineList[1].toFloat()*DENSITY_MULTIPLE/2),static_cast<uint32_t>(lineList[2].toFloat()+20)*DENSITY_MULTIPLE/2) = value;
    }

    MIN_DATA = min;
    MAX_DATA = max;

    unsigned int cubeRate = DENSITY_MULTIPLE;
    while(cubeRate >= 2)
    {
        calcCubeDepth(cubeRate);
        cubeRate /= 2;
    }

    data = cube;

    return true;
}

bool readTxtTypeFile(const QString& fileName, float* data, int xLength, int yLength, int zLength, float& min, float& max)
{
    QFile txtFile(fileName);
    if( false == txtFile.open(QIODevice::OpenModeFlag::ReadOnly) )
    {
        std::cout << __FILE__ << __LINE__  << "Error: openfile failed! srtFile = "<< fileName.toStdString() << std::endl;
        return false;
    }

    QString line;
    int lineNumber = 0;
    while( false == ((line = txtFile.readLine()).isEmpty()) )
    {

        if(lineNumber == 0)
        {
            auto lst = line.split("  ");
            X_LENGTH = lst.size() - 1;
            Z_LENGTH = X_LENGTH;
            min = lst.at(0).toFloat();
            max = min;
        }

        ++lineNumber;
    }

    Y_LENGTH = lineNumber/Z_LENGTH;

    xLength = X_LENGTH;
    yLength = Y_LENGTH;
    zLength = Z_LENGTH;

    X_SIZE = (X_LENGTH-1)+1;
    Y_SIZE = (Y_LENGTH-1)+1;
    Z_SIZE = (Z_LENGTH-1)+1;

    cube = new float[((X_SIZE)*(Y_SIZE)*(Z_SIZE))];

    txtFile.seek(0);

    lineNumber = 0;
    while( false == ((line = txtFile.readLine()).isEmpty()) && lineNumber <= X_SIZE*Y_SIZE)
    {
        QStringList lst = line.split(QString("  "));
        for(auto i = 0; i < lst.size() -1; ++i)
        {
            float value = lst.at(i).toFloat();
            if(min > value)
            {
                min = value;
            }
            else if(max < value)
            {
                max = value;
            }

            int x = lineNumber/Y_LENGTH;
            int y = lineNumber%Y_LENGTH;

            arrayByIndex(x, y, i) = value;
        }
        lineNumber++;
    }

    MIN_DATA = min;
    MAX_DATA = max;

    data = cube;
    return true;
}

bool genVectorUChar(QVector<uchar>& customData, float* data, int xLength, int yLength, int zLength, RangeColorSetDialog* dialog)
{
    customData.resize(xLength*yLength*zLength*4);
    for(auto x = 0; x < xLength; ++x)
    {
        for(auto y = 0; y < yLength; ++y)
        {
            for(auto z = 0; z < zLength; ++z)
            {
                double percent = (arrayByIndex(x,y,z) - MIN_DATA)/(MAX_DATA-MIN_DATA);
                QVector<uchar> vec = dialog->getColor(percent);
                //memcpy( (uchar*)(customData.data()) + ( (y*(xLength*zLength) + x*zLength + z)*4), vec.data(), 4*sizeof(uchar));
                memcpy( (uchar*)(customData.data()) + ( (y*(xLength*zLength) + x*zLength + z)*4), vec.data(), 4*sizeof(uchar));
            }
        }
    }

    return true;
}

void getTrtData(float** data, int& xLength, int& yLength, int& zLength, float& min, float& max)
{
    *data = cube;
    xLength = X_SIZE;
    yLength = Y_SIZE;
    zLength = Z_SIZE;
    min = MIN_DATA;
    max = MAX_DATA;
}

void getTrtDataRange(float& min, float& max)
{
    min = MIN_DATA;
    max = MAX_DATA;
}

#ifndef TRTPARSER_H
#define TRTPARSER_H

#include <QString>
const unsigned int  DENSITY_MULTIPLE = 4;

extern int X_SIZE;
extern int Y_SIZE;
extern int Z_SIZE;

bool readTrtFile(const QString& fileName, float* data, int xLength, int yLength, int zLength, float& min, float& max);
bool readTxtTypeFile(const QString& fileName, float* data, int xLength, int yLength, int zLength, float& min, float& max);

class RangeColorSetDialog;

bool genVectorUChar(QVector<uchar>& customData, float* data, int xLength, int yLength, int zLength, RangeColorSetDialog* dialog);

void getTrtData(float** data, int& xLength, int& yLength, int& zLength, float& min, float& max);
void getTrtDataRange(float& min, float& max);
#endif // TRTPARSER_H

#ifndef IMAGESEQUENCEBOX_H
#define IMAGESEQUENCEBOX_H
#include "animationbox.h"

class ImageSequenceBox : public AnimationBox {
public:
    ImageSequenceBox();
    void setListOfFrames(const QStringList &listOfFrames);
    BoundingBox *createNewDuplicate();
    void makeDuplicate(Property *targetBox);

    bool SWT_isImageSequenceBox() { return true; }

    void changeSourceFile();
    void writeBoundingBox(std::fstream *file);
    void readBoundingBox(std::fstream *file);
private:
    QStringList mListOfFrames;
};

#endif // IMAGESEQUENCEBOX_H

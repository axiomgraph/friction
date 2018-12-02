#ifndef QREALANIMATORVALUESLIDER_H
#define QREALANIMATORVALUESLIDER_H
#include "qdoubleslider.h"
#include "smartPointers/sharedpointerdefs.h"
class IntProperty;
class QrealAnimator;
class Property;

class QrealAnimatorValueSlider : public QDoubleSlider {
    Q_OBJECT
public:
    QrealAnimatorValueSlider(QString name, qreal minVal,
                             qreal maxVal, qreal prefferedStep,
                             QWidget *parent);
    QrealAnimatorValueSlider(qreal minVal, qreal maxVal, qreal prefferedStep,
                             QWidget *parent);
    QrealAnimatorValueSlider(qreal minVal, qreal maxVal, qreal prefferedStep,
                             QrealAnimator* animator, QWidget *parent = nullptr);
    QrealAnimatorValueSlider(QrealAnimator* animator, QWidget *parent = nullptr);
    QrealAnimatorValueSlider(QWidget *parent = nullptr);
    ~QrealAnimatorValueSlider() {}

    void paint(QPainter *p);

    void setAnimator(QrealAnimator *animator);
    void setIntAnimator(IntProperty *animator);

    bool hasTargetAnimator();

    bool isTargetDisabled();

    void openContextMenu(const QPoint &globalPos);
    void clearAnimator();
public slots:
    void setValueExternal(qreal value);

    void emitEditingStarted(qreal value);
    void emitValueChangedExternal(qreal value);
    void emitEditingFinished(qreal value);

    void nullifyAnimator();
    void setValueFromAnimator(qreal val);
protected:
    void emitValueChanged(qreal value);
private:
    bool mBlockAnimatorSignals = false;
    qptr<Property> mAnimator;
signals:
    void displayedValueChanged(qreal);
};

#endif // QREALANIMATORVALUESLIDER_H

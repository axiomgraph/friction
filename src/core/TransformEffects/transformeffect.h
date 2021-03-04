// enve - 2D animations software
// Copyright (C) 2016-2020 Maurycy Liebner

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TRANSFORMEFFECT_H
#define TRANSFORMEFFECT_H

#include "Animators/eeffect.h"

enum class TransformEffectType {
    track, followPath, followObject, followObjectRelative
};

class TransformEffect : public eEffect {
    Q_OBJECT
    e_OBJECT
protected:
    TransformEffect(const QString &name,
                    const TransformEffectType type);
public:
    virtual void applyEffect(const qreal relFrame,
                             qreal& pivotX, qreal& pivotY,
                             qreal& posX, qreal& posY,
                             qreal& rot,
                             qreal& scaleX, qreal& scaleY,
                             qreal& shearX, qreal& shearY,
                             BoundingBox* const parent) = 0;

    void prp_setupTreeViewMenu(PropertyMenu * const menu);

    void writeIdentifier(eWriteStream &dst) const;
    void writeIdentifierXEV(QDomElement& ele) const;
private:
    const TransformEffectType mType;
};

#endif // TRANSFORMEFFECT_H

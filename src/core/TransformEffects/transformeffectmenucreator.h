/*
#
# Friction - https://friction.graphics
#
# Copyright (c) Friction contributors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# See 'README.md' for more information.
#
*/

#ifndef TRANSFORMEFFECTMENUCREATOR_H
#define TRANSFORMEFFECTMENUCREATOR_H

#include "core_global.h"

#include <QString>
#include <functional>
#include "TransformEffects/followobjecteffect.h"
#include "TransformEffects/followobjectrelativeeffect.h"
#include "TransformEffects/followpatheffect.h"
#include "TransformEffects/parenteffect.h"
#include "TransformEffects/trackeffect.h"
#include "smartPointers/selfref.h"

class TransformEffect;

struct CORE_EXPORT TransformEffectMenuCreator
{
    template <typename T> using Func = std::function<T>;
    template <typename T> using Creator = Func<qsptr<T>()>;
    using EffectCreator = Creator<TransformEffect>;
    using EffectAdder = Func<void(const QString&,
                                  const EffectCreator&)>;
    static void forEveryEffect(const EffectAdder& add)
    {
        add(QObject::tr("Add Track Effect"),
            []() { return enve::make_shared<TrackEffect>(); });
        add(QObject::tr("Add Follow Path Effect"),
            []() { return enve::make_shared<FollowPathEffect>(); });
        add(QObject::tr("Add Follow Object Effect"),
            []() { return enve::make_shared<FollowObjectEffect>(); });
        add(QObject::tr("Add Follow Object Relative Effect"),
            []() { return enve::make_shared<FollowObjectRelativeEffect>(); });
        add(QObject::tr("Add Parent Effect"),
            []() { return enve::make_shared<ParentEffect>(); });
    }
};

#endif // TRANSFORMEFFECTMENUCREATOR_H

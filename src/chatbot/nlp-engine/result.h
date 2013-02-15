/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LVK_NLP_RESULT_H
#define LVK_NLP_RESULT_H

#include "nlp-engine/rule.h"

#include <QString>
#include <QList>

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace Nlp
{

/// \ingroup Lvk
/// \addtogroup Nlp
/// @{

/**
 * \brief
 */
// TODO rename this class
struct Result
{
    Result(const QString &output = "", RuleId ruleId = 0, int inputIndex = 0, float score = 0)
        : output(output), ruleId(ruleId), inputIdx(inputIndex), score(score) { }

    QString output;
    RuleId ruleId;
    int inputIdx;
    float score;

    bool operator<(const Result &other) const
    {
        return score < other.score;
    }

    bool isNull()
    {
        return output.isEmpty() && !ruleId && !inputIdx && !score;
    }
};


inline QDebug& operator<<(QDebug& dbg, const Result &r)
{
    dbg.space() << r.score << r.output << r.ruleId << r.inputIdx;

    return dbg.maybeSpace();
}


typedef QList<Result> ResultList;

/// @}

} // namespace Nlp

/// @}

} // namespace Lvk


#endif // LVK_NLP_RESULT_H

